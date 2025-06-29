/*
 * global.c - Global variable definitions and constants.
 *
 * These variables manage configuration settings, language
 * data, corpus statistics, and layout information.
 */

#include <wchar.h>

#include "global.h"
#include "structs.h"

/* Defining dimensions for the layout grid. */
#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

/* Maximum character count in language. */
int LANG_LENGTH = 51;

/* Maximum length of a language definition file. */
int LANG_FILE_LENGTH = 100;

/* Re-iterate the dimensions for external use. */
int ROW = row;
int COL = col;
int DIM1 = dim1;
int DIM2 = dim2;
int DIM3 = dim3;
int DIM4 = dim4;

/* Paths to data files. */
char *lang_name = NULL;
char *corpus_name = NULL;

/* Control flags for program execution. */
char output_mode = 'v';

/* The selected language's character set. */
wchar_t *lang_arr;

/* Hash table for character code lookup. */
int *char_table;

/* Arrays to store raw frequency counts from the corpus. */
int *corpus_mono;
int **corpus_bi;
int ***corpus_tri;
int ****corpus_quad;
int ***corpus_skip;

/* Arrays to store normalized frequency data (percentages). */
float *linear_mono;
float *linear_bi;
float *linear_tri;
float *linear_quad;
float *linear_skip;

/* total umber of statistics for each ngram type. */
int MONO_LENGTH = 0;
int BI_LENGTH = 0;
int TRI_LENGTH = 0;
int QUAD_LENGTH = 0;
int SKIP_LENGTH = 0;
int META_LENGTH = 0;

/* Arrays to hold all statistics after processing. */
mono_stat *stats_mono;
bi_stat *stats_bi;
tri_stat *stats_tri;
quad_stat *stats_quad;
skip_stat *stats_skip;
meta_stat *stats_meta;
