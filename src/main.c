/* main.c - Main source file. */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>

#include "io.h"
#include "global.h"
#include "util.h"
#include "mode.h"
#include "stats.h"

#define UNICODE_MAX 65535

/* Performs initialization: allocates memory and seeds RNG. */
void start_up()
{
    /* Seed random number generator. */
    log_print('n',L"1/3: Seeding RNG... ");
    srand(time(NULL));
    log_print('n',L"Done\n\n");

    /* Allocate language array. */
    log_print('n',L"2/3: Allocating language array... ");
    lang_arr = (wchar_t *)calloc(101, sizeof(wchar_t));

    /* Allocate character hash table array. */
    log_print('n',L"Allocating character hashmap... ");
    char_table = (int *)calloc(UNICODE_MAX+1, sizeof(int));
    log_print('n',L"Done\n\n");

    /* Allocate arrays for ngrams directly from corpus. */
    log_print('n',L"3/3: Allocating corpus arrays...\n");
    log_print('v',L"     Monograms... Integer... ");
    corpus_mono = (int *)calloc(LANG_LENGTH, sizeof(int));
    log_print('v',L"Floating Point... ");
    linear_mono = (float *)calloc(LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Bigrams... Integer... ");
    corpus_bi = (int **)malloc(LANG_LENGTH * sizeof(int *));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_bi[i] = (int *)calloc(LANG_LENGTH, sizeof(int));
    }
    log_print('v',L"Floating Point... ");
    linear_bi = (float *)calloc(LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Trigrams... Integer... ");
    corpus_tri = (int ***)malloc(LANG_LENGTH * sizeof(int **));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_tri[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_tri[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
    }
    log_print('v',L"Floating Point... ");
    linear_tri = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Quadgrams... Integer... ");
    corpus_quad = (int ****)malloc(LANG_LENGTH * sizeof(int ***));
    for (int i = 0; i < LANG_LENGTH; i++) {
        corpus_quad[i] = (int ***)malloc(LANG_LENGTH * sizeof(int **));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_quad[i][j] = (int **)malloc(LANG_LENGTH * sizeof(int *));
            for (int k = 0; k < LANG_LENGTH; k++) {
                corpus_quad[i][j][k] = (int *)calloc(LANG_LENGTH, sizeof(int));
            }
        }
    }
    log_print('v',L"Floating Point... ");
    linear_quad = (float *)calloc(LANG_LENGTH * LANG_LENGTH * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");

    log_print('v',L"     Skipgrams...\n");
    corpus_skip = (int ***)malloc(10 * sizeof(int **));
    for (int i = 1; i <= 9; i++) {
        log_print('v',L"       Skip-%d... Integer... ", i);
        corpus_skip[i] = (int **)malloc(LANG_LENGTH * sizeof(int *));
        for (int j = 0; j < LANG_LENGTH; j++) {
            corpus_skip[i][j] = (int *)calloc(LANG_LENGTH, sizeof(int));
        }
        log_print('v',L"Done\n");
    }
    log_print('v',L"       Floating Point... ");
    linear_skip = (float *)calloc(10 * LANG_LENGTH * LANG_LENGTH, sizeof(float));
    log_print('v',L"Done\n");


    log_print('n',L"     Done\n\n");
}

/* Performs cleanup: frees allocated memory. */
void shut_down()
{
    /* Free language array. */
    log_print('n',L"Freeing lang array... ");
    free(lang_arr);

    /* Free character hash table array. */
    log_print('n',L"Freeing character map... ");
    free(char_table);
    log_print('n',L"Done\n\n");

    /* Free arrays for ngrams directly from corpus. */
    log_print('n',L"2/3: Freeing corpus arrays... ");
    log_print('v',L"\n     Monograms... ");
    free(corpus_mono);
    free(linear_mono);
    log_print('v',L"Done\n");

    log_print('v',L"     Bigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        free(corpus_bi[i]);
    }
    free(corpus_bi);
    free(linear_bi);
    log_print('v',L"Done\n");

    log_print('v',L"     Trigrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_tri[i][j]);
        }
        free(corpus_tri[i]);
    }
    free(corpus_tri);
    free(linear_tri);
    log_print('v',L"Done\n");

    log_print('v',L"     Quadgrams... ");
    for (int i = 0; i < LANG_LENGTH; i++) {
        for (int j = 0; j < LANG_LENGTH; j++) {
            for (int k = 0; k < LANG_LENGTH; k++) {
                free(corpus_quad[i][j][k]);
            }
            free(corpus_quad[i][j]);
        }
        free(corpus_quad[i]);
    }
    free(corpus_quad);
    free(linear_quad);
    log_print('v',L"Done\n");

    log_print('v',L"     Skipgrams...\n");
    for (int i = 1; i <= 9; i++) {
        log_print('v',L"       Skip-%d... ", i);
        for (int j = 0; j < LANG_LENGTH; j++) {
            free(corpus_skip[i][j]);
        }
        free(corpus_skip[i]);
        log_print('v',L"Done\n");
    }
    free(corpus_skip);
    free(linear_skip);
    log_print('v',L"       Done\n");
    log_print('n',L"     Done\n\n");

    /* frees all stats */
    log_print('n',L"3/3: Freeing stats... ");
    free_stats(); /* stats.c */
    log_print('n',L"     Done\n\n");
}

/* Program entry point. */
int main(int argc, char **argv) {
    struct timespec full_start, start, end, full_end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &full_start);
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* THIS MUST COME BEFORE ANY PRINT STATEMENTS OR UNICODE BREAKS */
    if (fwide(stdout, 1) <= 0) {error("Failed to set wide-oriented stream.");}
    const char* locale = setlocale(LC_ALL, "en_US.UTF-8");
    if (locale == NULL) {error("Failed to set locale.");}

    log_print('q',L"\n");
    print_bar('q');
    log_print_centered('q',L"Starting Up");
    log_print('q',L"\n");

    start_up(); /* main.c */

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Start Up Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    clock_gettime(CLOCK_MONOTONIC, &start);

    print_bar('q');
    log_print_centered('q',L"Setting Up");
    log_print('q',L"\n");

    log_print('q',L"1/3: Reading config... ");
    read_config(); /* io.c */
    log_print('q',L"Done\n\n");

    /* overwrites config */
    log_print('q',L"2/3: Reading command line arguments... ");
    read_args(argc, argv); /* io.c */
    log_print('q',L"Done\n\n");

    /* final check that all options are correct */
    log_print('q',L"3/3: Checking arguments... ");
    check_setup(); /* io.c */
    log_print('q',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Set Up Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    print_bar('n');
    log_print_centered('n',L"Configuration");
    log_print('n',L"\n");

    /* Maybe move reading arguments/config to before start up?
     * Could allow for some purging/skipping? and proper verboseness */
    log_print('n',L"Language         :    %s\n", lang_name);
    log_print('n',L"Corpus File      :    %s\n", corpus_name);
    log_print('n',L"Output Mode      :    %c\n", output_mode);

    log_print('n',L"\n");
    print_bar('n');
    log_print('n',L"\n");


    clock_gettime(CLOCK_MONOTONIC, &start);

    print_bar('q');
    log_print_centered('q',L"Initializing Stats");
    log_print('q',L"\n");

    log_print('n',L"1/1: Building stats... ");
    initialize_stats(); /* stats.c */
    log_print('n',L"     Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Initialization Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    clock_gettime(CLOCK_MONOTONIC, &start);

    print_bar('q');
    log_print_centered('q',L"Reading Data");
    log_print('q',L"\n");

    /* read language file and fill array */
    log_print('n',L"1/3: Reading language... ");
    read_lang(lang_name); /* io.c */
    log_print('n',L"Done\n\n");

    /* read from cache if it exists */
    log_print('n',L"2/3: Reading corpus... ");
    log_print('v',L"Finding cache... ");
    int corpus_cache = 0;
    corpus_cache = read_corpus_cache(); /* io.c */
    log_print('n',L"Done\n\n");
    if (!corpus_cache) {
        /* The next operation is slow so we want to let the user see
           what step they are stuck on. */
        /* read entire corpus file and fill arrays */
        log_print('n',L"     2.3/3: Reading raw corpus... ");
        read_corpus(); /* io.c */
        log_print('n',L"Done\n\n");

        /* create new corpus cache */
        log_print('n',L"     2.6/3: Creating corpus cache... ");
        cache_corpus(); /* io.c */
        log_print('n',L"Done\n\n");
    }

    /* take corpus arrays from raw frequencies to percentages */
    log_print('n',L"3/3: Normalize corpus... ");
    normalize_corpus(); /* util.c */
    log_print('n',L"Done\n\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Reading Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    clock_gettime(CLOCK_MONOTONIC, &start);

    print_bar('q');
    log_print_centered('q',L"Running");
    log_print('q',L"\n");

    /* all in mode.c */
    start_server();

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Run Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    clock_gettime(CLOCK_MONOTONIC, &start);
    //log_print('q',L"----- Shutting Down -----\n\n");

    print_bar('q');
    log_print_centered('q',L"Shutting Down");
    log_print('q',L"\n");

    free(lang_name);
    free(corpus_name);

    /* reverse start_up */
    shut_down();

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    log_print_centered('q',L"Shut Down Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");


    print_bar('q');

    clock_gettime(CLOCK_MONOTONIC, &full_end);
    elapsed = (full_end.tv_sec - full_start.tv_sec) + (full_end.tv_nsec - full_start.tv_nsec) / 1e9;
    log_print_centered('q',L"Program Complete : %.9lf seconds", elapsed);
    print_bar('q');
    log_print('q',L"\n");

    return 0;
}
