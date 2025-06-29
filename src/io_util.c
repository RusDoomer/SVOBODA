/*
 * io_util.c - Utility functions related to I/O operations.
 *
 * This file provides utility functions for handling character conversions,
 * validating run and output modes, and other I/O related operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "io_util.h"
#include "util.h"
#include "global.h"

/*
 * Converts a character to its corresponding index in the language array.
 * Parameters:
 *   c: The character to convert.
 * Returns: The index of the character in the language array, or -1 if not found.
 */
int convert_char(wchar_t c)
{
    int ret = char_table[(int) c];
    if (ret != 0) {return ret;}
    else {return -1;}
}

/*
 * Converts an index in the language array back to its corresponding character.
 * Parameters:
 *   i: The index to convert.
 * Returns: The character corresponding to the index, or L'@' if out of bounds.
 */
wchar_t convert_back(int i)
{
    if (i < 50 && i >= 0) {
        return lang_arr[i*2];
    }
    return L'@';
}

/*
 * Shifts all elements in an integer array one position to the right, discarding
 * the last element.
 * Parameters:
 *   mem: Pointer to the integer array.
 *   size: The size of the array.
 */
void iterate(int *mem, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        mem[i] = mem[i - 1];
    }
}

/*
 * Checks for duplicate characters in a wide character array, excluding adjacent
 * duplicates.
 * Parameters:
 *   arr: The wide character array to check.
 * Returns: The number of duplicate pairs found (excluding adjacent pairs), or
 *          -1 if no duplicates.
 */
int check_duplicates(wchar_t *arr)
{
    int dups = -1;
    for (int i = 0; i < 101; i++) {
        for (int j = i + 2; j < 101; j++) {
            if (arr[i] == arr[j] && arr[i] != L'@') {
                dups++;
            }
        }
    }
    return dups;
}

/*
 * Validates and converts an output mode string to its corresponding character
 * representation.
 * Parameters:
 *   optarg: The string representing the output mode.
 * Returns: The character representing the validated output mode, or 'n' if
 *          invalid.
 */
char check_output_mode(char *optarg)
{
    if (strcmp(optarg, "q") == 0 || strcmp(optarg, "quiet") == 0
        || strcmp(optarg, "SHUTUP") == 0) {
        return 'q';
    } else if (strcmp(optarg, "n") == 0
        || strcmp(optarg, "norm") == 0
        || strcmp(optarg, "normal") == 0) {
        return 'n';
    } else if (strcmp(optarg, "v") == 0
        || strcmp(optarg, "loud") == 0
        || strcmp(optarg, "verbose") == 0) {
        return 'v';
    } else {
        error("Invalid output mode in arguments.");
        return 'n';
    }
}
