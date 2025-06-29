#ifndef ANALYZE_H
#define ANALYZE_H

#include "global.h"
#include "structs.h"

/*
 * Performs analysis on a single layout, calculating statistics for monograms,
 * bigrams, trigrams, quadgrams, and skipgrams. Then uses those values for meta
 * statistics.
 *
 * Parameters:
 *   lt: A pointer to the layout to analyze.
 */
void single_analyze(layout *lt);

#endif
