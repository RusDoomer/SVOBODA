#ifndef GLOBAL_H
#define GLOBAL_H

#include <wchar.h>
#include "structs.h"

/* Defining dimensions for the layout grid. */
#define row 3
#define col 12
#define dim1 row * col
#define dim2 dim1 * dim1
#define dim3 dim2 * dim1
#define dim4 dim3 * dim1

/* Maximum character count in language. */
extern int LANG_LENGTH;

/* Maximum length of a language definition file. */
extern int LANG_FILE_LENGTH;

/* Re-iterate the dimensions for external use. */
extern int ROW;
extern int COL;
extern int DIM1;
extern int DIM2;
extern int DIM3;
extern int DIM4;

/* Paths to data files. */
extern char *lang_name;
extern char *corpus_name;

/* Control flags for program execution. */
extern char output_mode;

/* The selected language's character set. */
extern wchar_t *lang_arr;

/* Hash table for character code lookup. */
extern int *char_table;

/* Arrays to store raw frequency counts from the corpus. */
extern int *corpus_mono;
extern int **corpus_bi;
extern int ***corpus_tri;
extern int ****corpus_quad;
extern int ***corpus_skip;

/* Arrays to store normalized frequency data (percentages). */
extern float *linear_mono;
extern float *linear_bi;
extern float *linear_tri;
extern float *linear_quad;
extern float *linear_skip;

/* total umber of statistics for each ngram type. */
extern int MONO_LENGTH;
extern int BI_LENGTH;
extern int TRI_LENGTH;
extern int QUAD_LENGTH;
extern int SKIP_LENGTH;
extern int META_LENGTH;

/* Arrays to hold all statistics after processing. */
extern mono_stat *stats_mono;
extern bi_stat *stats_bi;
extern tri_stat *stats_tri;
extern quad_stat *stats_quad;
extern skip_stat *stats_skip;
extern meta_stat *stats_meta;

#endif
