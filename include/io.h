#ifndef IO_H
#define IO_H

#include <wchar.h>
#include "global.h"
#include "structs.h"

/*
 * Prints a message to the standard output stream, with verbosity control.
 * The message will only be printed if the current output mode meets or
 * exceeds the required verbosity level specified by 'required_level'.
 *
 * Parameters:
 *   required_level: The minimum verbosity level required to print the message.
 *                   'q' for quiet, 'n' for normal, 'v' for verbose.
 *   format:         The format string for the message, compatible with vwprintf.
 *   ...:            Variable arguments for the format string.
 */
void log_print(char required_level, const wchar_t *format, ...);

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
void log_print_centered(char required_level, const wchar_t *format, ...);

/* Prints a bar of 80 ='s */
void print_bar(char required_level);

/*
 * Reads the configuration file to set up initial program parameters.
 * This function parses 'config.conf' to initialize various settings
 * such as pinned key positions, language, corpus, layout names,
 * weights file, run mode, number of repetitions, number of threads,
 * output mode, and backend mode.
 */
void read_config();

/*
 * Processes command line arguments to override configuration settings.
 * It parses arguments passed to the main function and updates
 * corresponding global variables such as language name, corpus name,
 * layout names, weight file, repetitions, threads, run mode, output mode,
 * and backend mode.
 *
 * Parameters:
 *   argc: The number of command line arguments.
 *   argv: An array of C-style strings containing the command line arguments.
 */
void read_args(int argc, char **argv);

/*
 * Validates the current program settings to ensure they are legal.
 * Terminates the program if an invalid setting is found.
 */
void check_setup();

/*
 * Reads and sets the current language's character set from a language file.
 * Sets up the 'char_table' for character code lookups. It performs checks to
 * ensure the language file is correctly formatted and only contains legal
 * characters.
 */
void read_lang();

/*
 * Attempts to read corpus data from a cache file. If the cache file exists,
 * it reads the pre-computed ngram frequencies into the global corpus arrays.
 *
 * Returns:
 *   1 if the cache file was successfully read, 0 otherwise.
 */
int read_corpus_cache();

/*
 * Reads and processes a corpus text file to collect ngram frequency data. Reads
 * the corpus character by character, updating the frequency counts in the
 * global corpus arrays for monograms, bigrams, trigrams, quadgrams,
 * and skipgrams.
 */
void read_corpus();

/*
 * Creates or updates a cache file with the current corpus frequency data.
 * This function writes the current state of the global corpus arrays to a
 * cache file, allowing for quicker initialization in future runs.
 */
void cache_corpus();

/*
 * Prints the layout name and score.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void quiet_print(layout *lt);

/*
 * Prints the layout along with ngram stats.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void normal_print(layout *lt);

/*
 * Prints detailed information, currently the same as normal_print.
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void verbose_print(layout *lt);

/*
 * Prints a layout using the current 'output_mode' to determine the level of
 * detail to print, ranging from just the layout matrix and score to detailed
 * statistics for each ngram type.
 *
 * Parameters:
 *   lt: A pointer to the layout structure to be printed.
 */
void print_layout(layout *lt);

#endif
