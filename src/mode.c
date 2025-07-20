/*
 * mode.c - Modes of operation.
 *
 * This file implements the API server itself
 */

#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>
#include <json-c/json.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include "mode.h"
#include "util.h"
#include "io.h"
#include "analyze.h"
#include "global.h"
#include "structs.h"
#include "api_util.h"

#define PORT 8888

volatile sig_atomic_t global_shutdown_flag = 0;

void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        global_shutdown_flag = 1;
    }
}


typedef struct {
    json_object *layout_data;
    char **response_data;
} AnalysisTask;

typedef struct {
    pthread_t *threads;
    AnalysisTask *tasks;
    int num_threads;
    int task_count;
    int tasks_assigned;
    int tasks_completed;
    int shutdown;
    pthread_mutex_t mutex;
    pthread_cond_t task_cond;
    pthread_cond_t batch_done_cond;
} ThreadPool;

ThreadPool *pool = NULL;

void *worker_thread(void *arg);

void create_thread_pool() {
    if (pool) return;

    pool = calloc(1, sizeof(ThreadPool));
    if (!pool) {
        error("Failed to allocate memory for thread pool.");
    }

    pool->num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    pool->threads = calloc(pool->num_threads, sizeof(pthread_t));
    if (!pool->threads) {
        error("Failed to allocate memory for threads.");
    }

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->task_cond, NULL);
    pthread_cond_init(&pool->batch_done_cond, NULL);
    pool->shutdown = 0;

    for (int i = 0; i < pool->num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, &worker_thread, NULL);
    }
}

void destroy_thread_pool() {
    if (!pool) return;

    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->task_cond);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->task_cond);
    pthread_cond_destroy(&pool->batch_done_cond);
    free(pool);
    pool = NULL;
}

void process_single_layout_analysis(json_object *layout_data, char **response_data) {
    json_object *j_layout_str, *j_weights;
    if (!json_object_object_get_ex(layout_data, "layout", &j_layout_str) ||
        !json_object_object_get_ex(layout_data, "weights", &j_weights)) {
        *response_data = strdup("{\"error\": \"Invalid JSON payload: missing layout or weights.\"}");
        return;
    }

    const char *layout_str = json_object_get_string(j_layout_str);
    CustomWeights weights;
    json_object *w;
    json_object_object_get_ex(j_weights, "sfb", &w); weights.sfb = json_object_get_double(w);
    json_object_object_get_ex(j_weights, "sfs", &w); weights.sfs = json_object_get_double(w);
    json_object_object_get_ex(j_weights, "lsb", &w); weights.lsb = json_object_get_double(w);
    json_object_object_get_ex(j_weights, "alt", &w); weights.alt = json_object_get_double(w);
    json_object_object_get_ex(j_weights, "rolls", &w); weights.rolls = json_object_get_double(w);

    layout *lt;
    alloc_layout(&lt);

    if (!parse_layout_from_string(lt, layout_str)) {
        *response_data = strdup("{\"error\": \"Invalid layout string.\"}");
    } else {
        strcpy(lt->name, "api_layout");
        single_analyze(lt);
        *response_data = build_json_response(lt, &weights);
    }
    free_layout(lt);
}

void *worker_thread(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&pool->mutex);

        while (pool->tasks_assigned >= pool->task_count && !pool->shutdown) {
            pthread_cond_wait(&pool->task_cond, &pool->mutex);
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }

        AnalysisTask task = pool->tasks[pool->tasks_assigned];
        pool->tasks_assigned++;

        pthread_mutex_unlock(&pool->mutex);

        process_single_layout_analysis(task.layout_data, task.response_data);

        pthread_mutex_lock(&pool->mutex);
        pool->tasks_completed++;
        if (pool->tasks_completed >= pool->task_count) {
            pthread_cond_signal(&pool->batch_done_cond);
        }
        pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}

typedef struct {
    char *post_data;
    size_t post_data_size;
    char *response_data;
    pthread_t thread_id;
} RequestContext;

static void *analysis_thread(void *cls) {
    RequestContext *rc = (RequestContext *)cls;
    log_print('v', L"[Thread %p] Starting analysis.\n", (void*)pthread_self());

    json_object *parsed_json = json_tokener_parse(rc->post_data);

    if (!parsed_json) {
        log_print('v', L"[Thread %p] ERROR: Invalid JSON format.\n", (void*)pthread_self());
        rc->response_data = strdup("{\"error\": \"Invalid JSON format.\"}");
        return NULL;
    }

    if (json_object_get_type(parsed_json) == json_type_array) {
        size_t batch_size = json_object_array_length(parsed_json);
        log_print('v', L"Detected batch request with %zu items.\n", batch_size);

        pool->tasks = calloc(batch_size, sizeof(AnalysisTask));
        char **responses = calloc(batch_size, sizeof(char*));
        if (!pool->tasks || !responses) {
            error("Failed to allocate memory for batch processing.");
        }

        for (size_t i = 0; i < batch_size; i++) {
            pool->tasks[i].layout_data = json_object_array_get_idx(parsed_json, i);
            pool->tasks[i].response_data = &responses[i];
        }

        pthread_mutex_lock(&pool->mutex);
        pool->task_count = batch_size;
        pool->tasks_assigned = 0;
        pool->tasks_completed = 0;
        pthread_cond_broadcast(&pool->task_cond);

        while (pool->tasks_completed < batch_size) {
            pthread_cond_wait(&pool->batch_done_cond, &pool->mutex);
        }
        pthread_mutex_unlock(&pool->mutex);

        json_object *j_response_array = json_object_new_array();
        for (size_t i = 0; i < batch_size; i++) {
            if (responses[i]) {
                json_object *j_item_response = json_tokener_parse(responses[i]);
                json_object_array_add(j_response_array, j_item_response);
                free(responses[i]);
            }
        }

        rc->response_data = strdup(json_object_to_json_string_ext(j_response_array, JSON_C_TO_STRING_PRETTY));
        json_object_put(j_response_array);
        free(pool->tasks);
        free(responses);

    } else {
        process_single_layout_analysis(parsed_json, &rc->response_data);
    }

    json_object_put(parsed_json);
    log_print('v', L"[Thread %p] Analysis finished.\n", (void*)pthread_self());

    return NULL;
}

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
    rc->response_data = NULL;

    log_print('v', L"--- Request Handled ---\n");
    return MHD_YES;
}

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

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    create_thread_pool();

    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
        &request_handler, NULL,
        MHD_OPTION_NOTIFY_COMPLETED, &request_completed, NULL,
        MHD_OPTION_END
    );

    if (NULL == daemon) {
        destroy_thread_pool();
        error("Failed to start microhttpd daemon.");
        return;
    }

    log_print('q', L"Server is running. Send SIGINT (Ctrl+C) or SIGTERM (kill) to shut down.\n");
    while (!global_shutdown_flag) {
        sleep(1);
    }

    log_print('q', L"\nShutdown signal received. Stopping server...\n");

    MHD_stop_daemon(daemon);
    destroy_thread_pool();
    log_print('q', L"Server stopped.\n");
}
