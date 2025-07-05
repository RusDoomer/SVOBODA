/*
 * mode.c - Modes of operation.
 *
 * This file implements the various modes of operation: analysis,
 * comparison, ranking, generation, and improvement. It utilizes multithreading
 * to enhance performance, specifically in the layout improvement process.
 */
/* src/mode.c */

#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>
#include <json-c/json.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h> // For getting client IP

#include "mode.h"
#include "util.h"
#include "io.h"
#include "analyze.h"
#include "global.h"
#include "structs.h"
#include "api_util.h"

#define PORT 8888

// Struct to hold the state for a single request
typedef struct {
    char *post_data;
    size_t post_data_size;
    char *response_data;
    pthread_t thread_id;
} RequestContext;


static void *analysis_thread(void *cls) {
    RequestContext *rc = (RequestContext *)cls;
    log_print('v', L"[Thread %p] Starting analysis.\n", (void*)pthread_self());

    log_print('v', L"[Thread %p] Parsing JSON payload: %s\n", (void*)pthread_self(), rc->post_data);

    json_object *parsed_json = json_tokener_parse(rc->post_data);

    if (!parsed_json) {
        log_print('v', L"[Thread %p] ERROR: Invalid JSON format.\n", (void*)pthread_self());
        rc->response_data = strdup("{\"error\": \"Invalid JSON format.\"}");
        return NULL;
    }

    json_object *j_layout_str, *j_weights;
    if (!json_object_object_get_ex(parsed_json, "layout", &j_layout_str) ||
        !json_object_object_get_ex(parsed_json, "weights", &j_weights)) {
        log_print('v', L"[Thread %p] ERROR: Missing 'layout' or 'weights' in JSON.\n", (void*)pthread_self());
        rc->response_data = strdup("{\"error\": \"Invalid JSON payload: missing layout or weights.\"}");
        json_object_put(parsed_json);
        return NULL;
    }

    const char *layout_str = json_object_get_string(j_layout_str);
    log_print('v', L"[Thread %p] Parsed layout string: %s\n", (void*)pthread_self(), layout_str);

    CustomWeights weights;
    json_object *w;
    log_print('v', L"[Thread %p] Parsing weights...\n", (void*)pthread_self());
    json_object_object_get_ex(j_weights, "sfb", &w); weights.sfb = json_object_get_double(w); log_print('v',L"  - sfb: %.2f\n", weights.sfb);
    json_object_object_get_ex(j_weights, "sfs", &w); weights.sfs = json_object_get_double(w); log_print('v',L"  - sfs: %.2f\n", weights.sfs);
    json_object_object_get_ex(j_weights, "lsb", &w); weights.lsb = json_object_get_double(w); log_print('v',L"  - lsb: %.2f\n", weights.lsb);
    json_object_object_get_ex(j_weights, "alt", &w); weights.alt = json_object_get_double(w); log_print('v',L"  - alt: %.2f\n", weights.alt);
    json_object_object_get_ex(j_weights, "rolls", &w); weights.rolls = json_object_get_double(w); log_print('v',L"  - rolls: %.2f\n", weights.rolls);

    layout *lt;
    alloc_layout(&lt);

    log_print('v', L"[Thread %p] Converting layout string to matrix...\n", (void*)pthread_self());
    if (!parse_layout_from_string(lt, layout_str)) {
        log_print('v', L"[Thread %p] ERROR: Invalid layout string provided.\n", (void*)pthread_self());
        rc->response_data = strdup("{\"error\": \"Invalid layout string.\"}");
        free_layout(lt);
        json_object_put(parsed_json);
        return NULL;
    }

    strcpy(lt->name, "api_layout");

    log_print('v', L"[Thread %p] Starting core analysis with single_analyze()...\n", (void*)pthread_self());
    single_analyze(lt);
    log_print('v', L"[Thread %p] Core analysis complete.\n", (void*)pthread_self());

    rc->response_data = build_json_response(lt, &weights);

    log_print('v', L"[Thread %p] Generated Response:\n%s\n", (void*)pthread_self(), rc->response_data);

    free_layout(lt);
    json_object_put(parsed_json);
    log_print('v', L"[Thread %p] Analysis finished.\n", (void*)pthread_self());

    return NULL;
}


// Main request handler for microhttpd
static enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                     const char *url, const char *method,
                                     const char *version, const char *upload_data,
                                     size_t *upload_data_size, void **con_cls) {
    (void)cls; (void)version;

    if (*con_cls == NULL) {
        RequestContext *rc = calloc(1, sizeof(RequestContext));
        if (rc == NULL) {
            log_print('v', L"ERROR: Failed to allocate memory for request context.\n");
            return MHD_NO;
        }
        *con_cls = (void *)rc;

        const union MHD_ConnectionInfo *ci = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
        struct sockaddr_in *addr = (struct sockaddr_in *)ci->client_addr;
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr->sin_addr), client_ip, INET_ADDRSTRLEN);

        log_print('v', L"\n--- New Request from %s:%d ---\nURL: %s\nMethod: %s\n",
                  client_ip, ntohs(addr->sin_port), url, method);

        return MHD_YES;
    }

    RequestContext *rc = *con_cls;

    if (strcmp(method, "POST") != 0) {
        log_print('v', L"Request rejected: Not a POST request.\n");
        const char *page = "{\"error\": \"POST requests only\"}";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Accumulate POST data
    if (*upload_data_size != 0) {
        rc->post_data = realloc(rc->post_data, rc->post_data_size + *upload_data_size + 1);
        if (!rc->post_data) {
            log_print('v', L"ERROR: Failed to realloc for POST data.\n");
            return MHD_NO;
        }
        memcpy(rc->post_data + rc->post_data_size, upload_data, *upload_data_size);
        rc->post_data_size += *upload_data_size;
        rc->post_data[rc->post_data_size] = '\0';
        *upload_data_size = 0;
        return MHD_YES;
    }

    // If we get here and upload_data_size is 0, the body is complete.
    if (rc->post_data == NULL) {
        log_print('v', L"ERROR: POST request received with no body.\n");
        const char *page = "{\"error\": \"Empty POST body\"}";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
        return ret;
    }

    log_print('v', L"POST data reception complete. Spawning analysis thread...\n");
    pthread_create(&rc->thread_id, NULL, &analysis_thread, rc);
    pthread_join(rc->thread_id, NULL);

    log_print('v', L"Thread finished. Sending response to client.\n");

    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(rc->response_data), (void *)rc->response_data, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, "Content-Type", "application/json");

    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    rc->response_data = NULL; // MHD has taken ownership and will free it

    log_print('v', L"--- Request Handled ---\n");
    return MHD_YES;
}


// Cleanup function called by MHD when a connection is closed
void request_completed(void *cls, struct MHD_Connection *connection,
                       void **con_cls, enum MHD_RequestTerminationCode toe) {
    (void)cls; (void)connection; (void)toe;
    RequestContext *rc = *con_cls;

    if (rc == NULL) return;

    if (rc->post_data) {
        free(rc->post_data);
    }
    if (rc->response_data) {
        free(rc->response_data);
    }
    free(rc);
    *con_cls = NULL;
    log_print('v', L"Request context cleaned up.\n");
}


void start_server() {
    log_print('q', L"Starting server on port %d...\n", PORT);
    log_print('q', L"Set output mode to 'v' for detailed request logging.\n");

    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
        &request_handler, NULL,
        MHD_OPTION_NOTIFY_COMPLETED, &request_completed, NULL,
        MHD_OPTION_END
    );

    if (NULL == daemon) {
        error("Failed to start microhttpd daemon.");
        return;
    }

    log_print('q', L"Server is running. Press Enter to shut down.\n");
    (void)getchar();

    MHD_stop_daemon(daemon);
    log_print('q', L"Server stopped.\n");
}
