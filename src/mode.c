/*
 * mode.c - Modes of operation.
 *
 * This file implements the various modes of operation: analysis,
 * comparison, ranking, generation, and improvement. It utilizes multithreading
 * to enhance performance, specifically in the layout improvement process.
 */

#include <time.h>

#include "mode.h"
#include "util.h"
#include "io.h"
#include "analyze.h"
#include "global.h"
#include "structs.h"


void start_server() {
    log_print('q',L"server started");
    log_print('q',L"\n\n");

    //language to use, corpus to use
    //weights, layout
    //shutdown command

    //no generation, single analysis should be enough for everyone else
}


/*
 * Performs analysis on a single layout. This involves allocating memory for the
 * layout, reading layout data from a file, analyzing the layout, calculating
 * its score, printing the output, and freeing the allocated memory.
 */
void analysis() {
    /* Work for timing total/real layouts/second */
    layouts_analyzed = 1;
    struct timespec compute_start, compute_end;
    clock_gettime(CLOCK_MONOTONIC, &compute_start);

    layout *lt;

    /* allocates memory for a layout structure */
    log_print('n',L"1/6: Allocating layout... ");
    alloc_layout(&lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* reads a keyboard layout from a file and initializes a layout structure */
    log_print('n',L"2/6: Reading layout... ");
    read_layout(lt, 1); /* io.c */
    log_print('n',L"Done\n\n");

    /* performs a single layout analysis */
    log_print('n',L"3/6: Analyzing layout... ");
    single_analyze(lt); /* analyze.c */
    log_print('n',L"Done\n\n");

    /* calculates and assigns the overall score to a layout based on its statistics */
    log_print('n',L"4/6: Calculating Score... ");
    get_score(lt); /* util.c */
    log_print('n',L"Done\n\n");

    /* prints the contents of a layout structure to the standard output */
    log_print('n',L"5/6: Printing Output...\n\n");
    print_layout(lt); /* io.c */
    log_print('n',L"Done\n\n");

    /* frees the memory occupied by a layout data structure */
    log_print('n',L"6/6: Freeing layout... ");
    free_layout(lt); /* util.c */
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &compute_end);
    elapsed_compute_time += (compute_end.tv_sec - compute_start.tv_sec) + (compute_end.tv_nsec - compute_start.tv_nsec) / 1e9;
    return;
}
