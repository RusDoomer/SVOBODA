#ifndef MODE_H
#define MODE_H

void start_server();

/*
 * Performs analysis on a single layout. This involves allocating memory for the
 * layout, reading layout data from a file, analyzing the layout, calculating
 * its score, printing the output, and freeing the allocated memory.
 */
void analysis();

#endif
