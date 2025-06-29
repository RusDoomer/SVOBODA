/*
 * stats/mono.c - Monogram statistic definitions.
 *
 * This file contains the implementation for initializing, converting, and
 * freeing monogram statistics. Monogram statistics track the frequency and
 * positioning of single character sequences.
 *
 * Adding new stats:
 *     1. Incease MONO_LENGTH by as many stats as you are adding.
 *     2. Define its name, keep it a reasonable length.
 *     3. Set its weight to -INFINITY, and skip to 0 (to be changed later).
 *     4. Set its length to 0, then loop through the DIM1 (36^1) sequences.
 *       4a. Use unflat_mono() to convert the 1D index to a 2D coordinate.
 *       4b. Check if the ngram falls under the stat.
 *       4c. If it does, add it to the ngrams array and increment length.
 *       4d. Otherwise set the ngram array element to -1.
 *     5. Iterate the index.
 *     6. Add the statistic to the weights files in data/weights/.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "mono.h"
#include "util.h"
#include "stats_util.h"
#include "global.h"
#include "structs.h"

/*
 * Initializes the array of monogram statistics. The function allocates memory
 * for the stat array and sets default values, including a negative infinity
 * weight which will be later overwritten.
 */
void initialize_mono_stats()
{
    MONO_LENGTH = 53;
    stats_mono = (mono_stat *)malloc(sizeof(mono_stat) * MONO_LENGTH);
    int row0, col0;
    int index = 0;

    strcpy(stats_mono[index].name, "Heatmap 0 00");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        /* convert a 1D index into a 2D matrix coordinate */
        unflat_mono(i, &row0, &col0); /* util.c */
        if (row0 == 0 && col0 == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 01");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 1)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 02");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 2)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 03");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 3)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 04");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 4)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 05");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 5)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 06");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 6)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 07");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 7)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 08");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 8)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 09");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 9)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 10");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 10)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 0 11");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0 && col0 == 11)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 00");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 01");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 1)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 02");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 2)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 03");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 3)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 04");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 4)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 05");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 5)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 06");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 6)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 07");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 7)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 08");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 8)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 09");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 9)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 10");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 10)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 1 11");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1 && col0 == 11)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 00");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 01");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 1)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 02");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 2)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 03");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 3)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 04");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 4)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 05");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 5)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 06");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 6)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 07");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 7)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 08");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 8)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 09");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 9)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 10");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 10)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Heatmap 2 11");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2 && col0 == 11)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    /* Initialize a new stats for column/finger usage. */
    strcpy(stats_mono[index].name, "Left Outer Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Left Pinky Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Left Ring Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 1)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Left Middle Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 2)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Left Index Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 3)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Left Inner Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 5)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Inner Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 6)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Index Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 4)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Middle Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 5)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Ring Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 6)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Pinky Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (finger(row0, col0) == 7)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Outer Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (col0 == 11)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    /* Allocate and initialize a new stats for hand usage. */
    strcpy(stats_mono[index].name, "Left Hand Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'l')
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Right Hand Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (hand(row0, col0) == 'r')
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    /* Allocate and initialize a new stats for row usage. */
    strcpy(stats_mono[index].name, "Top Row Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 0)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Home Row Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 1)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    strcpy(stats_mono[index].name, "Bottom Row Usage");
    stats_mono[index].weight = -INFINITY;
    stats_mono[index].length = 0;
    stats_mono[index].skip = 0;
    for (int i = 0; i < DIM1; i++)
    {
        unflat_mono(i, &row0, &col0);
        if (row0 == 2)
        {
            stats_mono[index].ngrams[i] = i;
            stats_mono[index].length++;
        }
        else
        {
            stats_mono[index].ngrams[i] = -1;
        }
    }
    index++;

    if (index != MONO_LENGTH) {error("MONO_LENGTH incorrect for number of mono stats");}
}

/*
 * Trims the ngrams in the array to move unused entries to the end.
 * This process ensures memory efficiency by eliminating gaps in the array.
 */
void trim_mono_stats()
{
    for (int i = 0; i < MONO_LENGTH; i++)
    {
        if (stats_mono[i].length != 0)
        {
            int left = 0;
            int right = DIM1 - 1;

            /* Use two pointers to partition the array */
            while (left < right) {
                /* Find the next -1 from the left */
                while (left < right && stats_mono[i].ngrams[left] != -1) {
                    left++;
                }

                /* Find the next non -1 from the right */
                while (left < right && stats_mono[i].ngrams[right] == -1) {
                    right--;
                }

                /* Swap the elements to move -1 to the back and non -1 to the front */
                if (left < right) {
                    int temp = stats_mono[i].ngrams[left];
                    stats_mono[i].ngrams[left] = stats_mono[i].ngrams[right];
                    stats_mono[i].ngrams[right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

/* Frees the memory allocated for the monogram statistics array. */
void free_mono_stats()
{
    free(stats_mono);
}
