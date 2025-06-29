#ifndef UTIL_H
#define UTIL_H

#include "global.h"
#include "structs.h"

/*
 * Error handling function: Shows the cursor, prints an error message to
 * standard error, and terminates the program.
 * Parameters:
 *   msg: The error message to be displayed.
 * Does not return (terminates program).
 */
void error(const char *msg);

/*
 * Flattens an 8D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1, row2, col2, row3, col3: Row and column indices.
 *   i: Pointer to store the flattened index.
 */
void flat_quad(int row0, int col0, int row1, int col1, int row2, int col2, int row3, int col3, int *i);

/*
 * Unflattens a 1D index into a 8D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2, row3, col3: Pointers to store the
 *                                                   row and column indices.
 */
void unflat_quad(int i, int *row0, int *col0, int *row1, int *col1, int *row2, int *col2, int *row3, int *col3);

/*
 * Flattens a 6D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1, row2, col2: Row and column indices.
 *   i: Pointer to store the flattened index.
 */
void flat_tri(int row0, int col0, int row1, int col1, int row2, int col2, int *i);

/*
 * Unflattens a 1D index into a 6D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1, row2, col2: Pointers to store the row and
 *                                       column indices.
 */
void unflat_tri(int i, int *row0, int *col0, int *row1, int *col1, int *row2, int *col2);

/*
 * Flattens a 4D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0, row1, col1: Row and column indices.
 *   i: Pointer to store the flattened index.
 */
void flat_bi(int row0, int col0, int row1, int col1, int *i);

/*
 * Unflattens a 1D index into a 4D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0, row1, col1: Pointers to store the row and column indices.
 */
void unflat_bi(int i, int *row0, int *col0, int *row1, int *col1);

/*
 * Flattens a 2D matrix coordinate into a 1D index.
 * Parameters:
 *   row0, col0: Row and column indices.
 *   i: Pointer to store the flattened index.
 */
void flat_mono(int row0, int col0, int *i);

/*
 * Unflattens a 1D index into a 2D matrix coordinate.
 * Parameters:
 *   i: The flattened index.
 *   row0, col0: Pointers to store the row and column indices.
 */
void unflat_mono(int i, int *row0, int *col0);

/*
 * Calculates the index for a monogram in a linearized array.
 * Parameters:
 *   i: The index of the character in the language array.
 * Returns: The index in the linearized monogram array.
 */
size_t index_mono(int i);

/*
 * Calculates the index for a bigram in a linearized array.
 * Parameters:
 *   i, j: The indices of the characters in the language array.
 * Returns: The index in the linearized bigram array.
 */
size_t index_bi(int i, int j);

/*
 * Calculates the index for a trigram in a linearized array.
 * Parameters:
 *   i, j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized trigram array.
 */
size_t index_tri(int i, int j, int k);

/*
 * Calculates the index for a quadgram in a linearized array.
 * Parameters:
 *   i, j, k, l: The indices of the characters in the language array.
 * Returns: The index in the linearized quadgram array.
 */
size_t index_quad(int i, int j, int k, int l);

/*
 * Calculates the index for a skipgram in a linearized array.
 * Parameters:
 *   skip_index: The skip distance (1-9).
 *   j, k: The indices of the characters in the language array.
 * Returns: The index in the linearized skipgram array.
 */
size_t index_skip(int skip_index, int j, int k);

/* Normalizes the corpus data from raw frequencies to percentages. */
void normalize_corpus();

/*
 * Allocates memory for a new layout.
 * Parameters:
 *   lt: Pointer to a layout pointer where the newly allocated layout will be stored.
 */
void alloc_layout(layout **lt);

/*
 * Frees the memory occupied by a layout.
 * Parameters:
 *   lt: Pointer to the layout to be freed.
 */
void free_layout(layout *lt);

/*
 * Calculates and assigns the overall score to a layout based on its statistics.
 * Parameters:
 *   lt: Pointer to the layout.
 */
void get_score(layout *lt);

/*
 * Copies the contents of one layout to another.
 * Parameters:
 *   lt_dest: Pointer to the destination layout.
 *   lt_src: Pointer to the source layout.
 */
void copy(layout *lt_dest, layout *lt_src);

/*
 * Copies the matrix, name, and overall score only of one layout to another.
 * Parameters:
 *   lt_dest: Pointer to the destination layout.
 *   lt_src: Pointer to the source layout.
 */
void skeleton_copy(layout *lt_dest, layout *lt_src);

/* Returns a random float between 0 and 1. */
float random_float();

#endif
