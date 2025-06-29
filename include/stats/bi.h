#ifndef BI_H
#define BI_H

/*
 * Initializes the array of bigram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_bi_stats();

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_bi_stats();

/* Frees the memory allocated for the bigram statistics array. */
void free_bi_stats();

#endif
