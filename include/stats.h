#ifndef STATS_H
#define STATS_H

/*
 * Initializes all statistic data structures. This involves
 * initializing arrays for each type of n-gram statistic as well as
 * meta-statistics. The function delegates the initialization of each statistic
 * type to its respective module.
 */
void initialize_stats();

/*
 * Frees the memory allocated for all statistics data structures. This function
 * deallocates the memory used by the statistics arrays for each n-gram
 * category. It ensures that all dynamically allocated memory for statistics is
 * properly released.
 */
void free_stats();

#endif
