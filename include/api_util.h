#ifndef API_UTIL_H
#define API_UTIL_H

#include "global.h"
#include "structs.h"
#include <json-c/json.h>

// Holds the custom weights provided in a single API request.
typedef struct CustomWeights {
    double sfb;
    double sfs;
    double lsb;
    double alt;
    double rolls;
} CustomWeights;

// Parses a 30-character layout string into the layout matrix.
// Assumes the layout string contains characters present in the loaded language.
int parse_layout_from_string(layout *lt, const char *layout_str);

// Builds the final JSON response string.
// This function calculates the final score using custom weights.
char *build_json_response(layout *lt, CustomWeights *weights);

#endif
