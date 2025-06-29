/*
 * io.c - Input/output operations.
 *
 * This file handles file operations, argument parsing, and user interaction.
 * It includes functions for reading configuration files, processing
 * command-line arguments, handling corpus data, and managing layout i/o.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <getopt.h>
#include <stdarg.h>

#include "io.h"
#include "io_util.h"
#include "util.h"
#include "global.h"
#include "structs.h"

#define UNICODE_MAX 65535
#define BUFFER_SIZE 10000

/*
 * Prints a message to the standard output stream, with verbosity control.
 * The message will only be printed if the current output mode meets or
 * exceeds the required verbosity level specified by 'required_level'.
 *
 * Parameters:
 *   required_level: The minimum verbosity level required to print the message.
 *                   'q' for quiet, 'n' for normal, 'v' for verbose.
 *   format:         The format string for the message.
 *   ...:            Variable arguments for the format string.
 */
void log_print(char required_level, const wchar_t *format, ...) {
    /* Check if the current output mode meets or exceeds the required level */
    if (
        (required_level == 'q' && (output_mode == 'q' || output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'n' && (output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'v' &&  output_mode == 'v')
       )
    {
        va_list args;
        va_start(args, format);
        vwprintf(format, args);
        va_end(args);
        /* force the message to be printed immediately */
        fflush(stdout);
    }
}

/*
 * Prints a message to the standard output stream, with verbosity control.
 * The message will only be printed if the current output mode meets or
 * exceeds the required verbosity level specified by 'required_level'. The
 * contents of this message will be centered on an 80 character line, and
 * will not be cut off if it is too long.
 *
 * Parameters:
 *   required_level: The minimum verbosity level required to print the message.
 *                   'q' for quiet, 'n' for normal, 'v' for verbose.
 *   format:         The format string for the message.
 *   ...:            Variable arguments for the format string.
 */
void log_print_centered(char required_level, const wchar_t *format, ...) {
    /* Check if the current output mode meets or exceeds the required level */
    if (
        (required_level == 'q' && (output_mode == 'q' || output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'n' && (output_mode == 'n' || output_mode == 'v')) ||
        (required_level == 'v' &&  output_mode == 'v')
       )
    {
        va_list args;
        /* magic number 81 for terminal window 80 + 1 for EOS */
        wchar_t buffer [81];
        va_start(args, format);
        int len = vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
        va_end(args);

        if (len < 0) {error("Error formatting centered log message.");}
        int padding = (80 - len) / 2;
        if(padding < 0) {error("Error finding padding for centered message.");}

        wprintf(L"%*s%ls\n", padding, "", buffer);

        /* force the message to be printed immediately */
        fflush(stdout);
    }
}

/* Prints a bar of 80 ='s */
void print_bar(char required_level) {
    for (int i = 0; i < 80; i++) {
        log_print(required_level, L"=");
    }
    log_print(required_level,L"\n");
}

/*
 * Reads the configuration file to set up initial program parameters.
 * This function parses 'config.conf' to initialize various settings
 * such as pinned key positions, language, corpus, layout names,
 * weights file, run mode, number of repetitions, number of threads,
 * output mode, and backend mode.
 */
void read_config()
{
    FILE *config;

    char discard[999];
    char buff[999];

    /* config.conf assumed to be in the working directory. */
    config = fopen("config.conf", "r");
    if (config == NULL) {
        error("Required file config.conf not found.");
    }
    log_print('q',L"config.conf found... ");

    /* Read and set various parameters from the configuration file. */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read language name from config file.");
    }
    lang_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(lang_name, buff);

    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read corpus name from config file.");
    }
    corpus_name = (char *)malloc((sizeof(char) * strlen(buff)) + 1);
    strcpy(corpus_name, buff);

    /* validate and convert output mode */
    if (fscanf(config, "%s %s", discard, buff) != 2) {
        error("Failed to read output mode from config file.");
    }
    output_mode = check_output_mode(buff); /* io_util.c */

    fclose(config);
}

/*
 * Processes command line arguments to override configuration settings.
 * It parses arguments passed to the main function and updates
 * corresponding global variables such as language name, corpus name,
 * layout names, weight file, repetitions, threads, run mode, output mode,
 * and backend mode.
 */
void read_args(int argc, char **argv)
{
    int opt;
    /* Parse command line arguments. */
    while ((opt = getopt(argc, argv, "l:c:o:")) != -1) {
    switch (opt) {
        case 'l':
            free(lang_name);
            lang_name = strdup(optarg);
            break;
        case 'c':
            free(corpus_name);
            corpus_name = strdup(optarg);
            break;
        case 'o':
            /* validate and convert output mode */
            output_mode = check_output_mode(optarg); /* io_util.c */
            break;
        case '?':
            error("Improper Usage: %s -l lang_name -c corpus_name "\
                "-o output_mode");
        default:
            abort();
        }
    }
}

/*
 * Validates the current program settings to ensure they are legal.
 * Terminates the program if an invalid setting is found.
 */
void check_setup()
{
    /* Ensure necessary parameters are set and have valid values. */
    if (lang_name == NULL) {error("no lang selected");}
    if (corpus_name == NULL) {error("no corpus selected");}
    if (output_mode != 'q' && output_mode != 'n' && output_mode != 'v')
    {
        error("invalid output mode selected");
    }
}

/*
 * Reads and sets the current language's character set from a language file.
 * Sets up the 'char_table' for character code lookups. It performs checks to
 * ensure the language file is correctly formatted and only contains legal
 * characters.
 */
void read_lang()
{
    FILE *lang;
    /* Construct the path to the language file. */
    char *path = (char*)malloc(strlen("./data//.lang") + strlen(lang_name) * 2 + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/");
    strcat(path, lang_name);
    strcat(path, ".lang");
    lang = fopen(path, "r");
    free(path);
    if (lang == NULL) {
        error("Lang file not found.");
    }
    log_print('v',L"Lang found... ");

    log_print('v',L"Reading... ");
    wchar_t a;
    /* Read the language file character set into lang_arr. */
    for (int i = 0; i < 101; i++) {
        if ((a = fgetwc(lang)) == EOF || a == L'\n') {lang_arr[i] = L'@';}
        else if (a == L'@') {
            error("'@' found in lang, illegal character.");
        } else {
            lang_arr[i] = a;
        }
    }

    log_print('v',L"Checking correctness... ");

     /* Validate the format of the language file. */
    if (lang_arr[0] != L' ' || lang_arr[1] != L' ') {
        error("Lang file must begin with 2 spaces");
    }

    if (lang_arr[100] != L'@') {
        error("Lang file too long (>100 characters)");
    }

    /*
     * Check for duplicate characters this allows duplicate characters that are
     * side by side for of shifted pair
     * (and to allow the double space at the start)
     */
    if (check_duplicates(lang_arr) != -1) { /* io_util.c */
        error("Lang file contains duplicate characters.");
    }

    /* Populate the character table for code lookups. */
    for (int i = 0; i < 101; i++) {
        if (lang_arr[i] == L'@') {
            char_table[L'@'] = -1;
        } else if (lang_arr[i] < UNICODE_MAX) {
            char_table[lang_arr[i]] = i/2;
        } else {
            error("Lang file contains illegal character not caught before.");
        }
    }
}

/*
 * Attempts to read corpus data from a cache file. If the cache file exists,
 * it reads the pre-computed ngram frequencies into the global corpus arrays.
 *
 * Returns:
 *   1 if the cache file was successfully read, 0 otherwise.
 */
int read_corpus_cache()
{
    FILE *corpus;
    /* Construct the path to the corpus cache file. */
    char *path = (char*)malloc(strlen("./data//corpora/.cache") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".cache");
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        free(path);
        log_print('v',L"Cache not found... ");
        return 0;
    }
    log_print('v',L"Cache found... ");
    log_print('v',L"Reading cache... ");
    wchar_t curr;
    int i,j,k,l, value;
    /* Read cached frequencies for ngrams */
    while ((curr = fgetwc(corpus)) != WEOF) {
        switch(curr)
        {
            case 'q':
                fwscanf(corpus, L" %d %d %d %d %d ", &i, &j, &k, &l, &value);
                corpus_quad[i][j][k][l] = value;
                break;
            case 't':
                fwscanf(corpus, L" %d %d %d %d ", &i, &j, &k, &value);
                corpus_tri[i][j][k] = value;
                break;
            case 'b':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_bi[i][j] = value;
                break;
            case 'm':
                fwscanf(corpus, L" %d %d ", &i, &value);
                corpus_mono[i] = value;
                break;
            case '1':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[1][i][j] = value;
                break;
            case '2':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[2][i][j] = value;
                break;
            case '3':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[3][i][j] = value;
                break;
            case '4':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[4][i][j] = value;
                break;
            case '5':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[5][i][j] = value;
                break;
            case '6':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[6][i][j] = value;
                break;
            case '7':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[7][i][j] = value;
                break;
            case '8':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[8][i][j] = value;
                break;
            case '9':
                fwscanf(corpus, L" %d %d %d ", &i, &j, &value);
                corpus_skip[9][i][j] = value;
                break;
            default:
                break;
        }
    }
    fclose(corpus);
    free(path);
    return 1;
}

/*
 * Reads and processes a corpus text file to collect ngram frequency data. Reads
 * the corpus character by character, updating the frequency counts in the
 * global corpus arrays for monograms, bigrams, trigrams, quadgrams,
 * and skipgrams.
 */
void read_corpus()
{
    FILE *corpus;
    /* Construct the path to the corpus text file. */
    char *path = (char*)malloc(strlen("./data//corpora/.txt") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".txt");
    corpus = fopen(path, "r");
    if (corpus == NULL) {
        error("Corpus file not found, make sure the file ends in .txt, but the name in config/parameters does not");
    }
    log_print('v',L"Corpus file found... ");

    /* Memory for the last 11 seen characters */
    int mem[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    wchar_t curr;
    while ((curr = fgetwc(corpus)) != WEOF) {
        /* convert characters based on the lang file */
        mem[0] = convert_char(curr); /* io_util.c */
        /* If character is valid in the language */
        if (mem[0] > 0 && mem[0] < 51) {
            corpus_mono[mem[0]]++;

            /* If there is a previous character, record the bigram */
            if (mem[1] > 0 && mem[1] < 51) {
                corpus_bi[mem[1]][mem[0]]++;
                /* If there are two, record the trigram */
                if (mem[2] > 0 && mem[2] < 51) {
                    corpus_tri[mem[2]][mem[1]][mem[0]]++;
                    /* If there are three, record the quadgram */
                    if (mem[3] > 0 && mem[3] < 51) {
                        corpus_quad[mem[3]][mem[2]][mem[1]][mem[0]]++;
                    }
                }
            }

            /* Record skipgrams from skip-1 to skip-9 */
            for (int i = 2; i < 11; i++)
            {
                if (mem[i] > 0 && mem[i] < 51)
                {
                    corpus_skip[i-1][mem[i]][mem[0]]++;
                }
            }
        }
        /* shift over an array one index, dropping the last value */
        iterate(mem, 11); /* io_util.c */
    }

    fclose(corpus);
    free(path);
}

/*
 * Creates or updates a cache file with the current corpus frequency data.
 * This function writes the current state of the global corpus arrays to a
 * cache file, allowing for quicker initialization in future runs.
 */
void cache_corpus()
{
    FILE *corpus;
    /* Construct the path to the corpus cache file. */
    char *path = (char*)malloc(strlen("./data//corpora/.cache") +
        strlen(lang_name) + strlen(corpus_name) + 1);
    strcpy(path, "./data/");
    strcat(path, lang_name);
    strcat(path, "/corpora/");
    strcat(path, corpus_name);
    strcat(path, ".cache");
    corpus = fopen(path, "w");
    if (corpus == NULL) {
        error("Corpus cache file failed to be created.");
    }
    log_print('n',L"Created cache file... ");

    for (int i = 0; i < 51; i++) {
        for (int j = 0; j < 51; j++) {
            for (int k = 0; k < 51; k++) {
                for (int l = 0; l < 51; l++) {
                    /* Write all quadgrams to the cache file. */
                    if (corpus_quad[i][j][k][l] > 0) {
                        fprintf(corpus, "q %d %d %d %d %d\n", i, j, k, l,
                            corpus_quad[i][j][k][l]);
                    }
                }
                /* Write all trigrams to the cache file. */
                if (corpus_tri[i][j][k] > 0) {
                    fprintf(corpus, "t %d %d %d %d\n", i, j, k,
                        corpus_tri[i][j][k]);
                }
            }
            /* Write all bigrams to the cache file. */
            if (corpus_bi[i][j] > 0) {
                fprintf(corpus, "b %d %d %d\n", i, j, corpus_bi[i][j]);
            }
            /* Write all skipgrams to the cache file. */
            for (int skip = 1; skip < 10; skip++) {
                if (corpus_skip[skip][i][j] > 0) {
                    fprintf(corpus, "%d %d %d %d\n", skip, i, j,
                        corpus_skip[skip][i][j]);
                }
            }
        }
        /* Write all monograms to the cache file. */
        if (corpus_mono[i] > 0) {
            fprintf(corpus, "m %d %d\n", i, corpus_mono[i]);
        }
    }
    fclose(corpus);
    free(path);
}

/*
 * Prints the layout name and score.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void quiet_print(layout *lt)
{
    log_print('q',L"%s\n", lt->name);
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            log_print('q',L"%lc ", convert_back(lt->matrix[i][j]));
        }
        log_print('q',L"\n");
    }

    log_print('q',L"score : %f\n\n", lt->score);
    return;
}

/*
 * Prints the layout along with ngram stats.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void normal_print(layout *lt)
{
    quiet_print(lt);
    log_print('n',L"\nMONOGRAM STATS\n");
    for (int i = 0; i < MONO_LENGTH; i++)
    {
        if (!stats_mono[i].skip) {log_print('n',L"%s : %08.5f\%\n", stats_mono[i].name, lt->mono_score[i]);}
    }
    log_print('n',L"\nBIGRAM STATS\n");
    for (int i = 0; i < BI_LENGTH; i++)
    {
        if (!stats_bi[i].skip) {log_print('n',L"%s : %08.5f\%\n", stats_bi[i].name, lt->bi_score[i]);}
    }
    log_print('n',L"\nTRIGRAM STATS\n");
    for (int i = 0; i < TRI_LENGTH; i++)
    {
        if (!stats_tri[i].skip) {log_print('n',L"%s : %08.5f\%\n", stats_tri[i].name, lt->tri_score[i]);}
    }
    log_print('n',L"\nQUADGRAM STATS\n");
    for (int i = 0; i < QUAD_LENGTH; i++)
    {
        if (!stats_quad[i].skip) {log_print('n',L"%s : %08.5f\%\n", stats_quad[i].name, lt->quad_score[i]);}
    }
    log_print('n',L"\nSKIPGRAM STATS\n");
    for (int i = 0; i < SKIP_LENGTH; i++)
    {
        if (!stats_skip[i].skip)
        {
            log_print('n',L"%s :\n    |", stats_skip[i].name);
            for (int j = 1; j <= 9; j++)
            {
                log_print('n',L"%06.3f", lt->skip_score[j][i]);
                log_print('n',L"%|");
            }
            log_print('n',L"\n");
        }
    }
    log_print('n',L"\nMETA STATS\n");
    for (int i = 0; i < META_LENGTH; i++)
    {
        if (!stats_meta[i].skip) {log_print('n',L"%s : %08.5f\%\n", stats_meta[i].name, lt->meta_score[i]);}
    }
    log_print('n',L"\n");
}

/*
 * Prints detailed information, currently the same as normal_print.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void verbose_print(layout *lt)
{
    normal_print(lt);
}

/*
 * Prints a layout using the current 'output_mode' to determine the level of
 * detail to print, ranging from just the layout matrix and score to detailed
 * statistics for each ngram type.
 *
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void print_layout(layout *lt)
{
    switch(output_mode)
    {
        case 'q': quiet_print(lt);
                  break;
        case 'n': normal_print(lt);
                  break;
        default:
        case 'v': verbose_print(lt);
                  break;
    }
    return;
}
