#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_util.h"
#include "io_util.h"
#include "stats_util.h"
#include "io.h"

int parse_layout_from_string(layout *lt, const char *layout_str) {
    if (strlen(layout_str) != 30) {
        return 0; // Invalid length
    }

    // Initialize matrix with -1 (empty)
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            lt->matrix[i][j] = -1;
        }
    }

    // Standard 3x10 matrix mapping
    int r = 0, c = 1;
    for (int i = 0; i < 30; i++) {
        wchar_t wide_char[2] = {0};
        mbstowcs(wide_char, &layout_str[i], 1);

        int char_index = convert_char(wide_char[0]);
        if (char_index == -1) {
            // Character not found in the language definition
            return 0;
        }

        lt->matrix[r][c] = char_index;

        c++;
        if (i == 9 || i == 19) { // Move to next row
            r++;
            c = 1;
        }
    }
    return 1;
}

char *build_json_response(layout *lt, CustomWeights *weights) {
    log_print('v', L"Building JSON response...\n");

    json_object *jobj = json_object_new_object();
    json_object *j_stat_values = json_object_new_object();

    float final_score = 0.0f;

    // 1. Same Finger Bigram (sfb)
    int sfb_idx = find_stat_index("Same Finger Bigram", 'b');
    float sfb_score = (sfb_idx != -1) ? lt->bi_score[sfb_idx] : 0.0f;
    json_object_object_add(j_stat_values, "sfb", json_object_new_double(sfb_score));
    final_score += sfb_score * weights->sfb;
    log_print('v', L"  - SFB: raw=%.4f, weight=%.2f, contribution=%.4f\n", sfb_score, weights->sfb, sfb_score * weights->sfb);


    // 2. Same Finger Skipgram (sfs) - Assuming skip-1
    int sfs_idx = find_stat_index("Same Finger Skipgram", '1');
    float sfs_score = (sfs_idx != -1) ? lt->skip_score[1][sfs_idx] : 0.0f;
    json_object_object_add(j_stat_values, "sfs", json_object_new_double(sfs_score));
    final_score += sfs_score * weights->sfs;
    log_print('v', L"  - SFS: raw=%.4f, weight=%.2f, contribution=%.4f\n", sfs_score, weights->sfs, sfs_score * weights->sfs);


    // 3. Layout Stretch Bigram (lsb)
    int idx_stretch_idx = find_stat_index("Index Stretch Bigram", 'b');
    float lsb_score = 0.0f;
    if (idx_stretch_idx != -1) {
        lsb_score = lt->bi_score[idx_stretch_idx];
    }
    json_object_object_add(j_stat_values, "lsb", json_object_new_double(lsb_score));
    final_score += lsb_score * weights->lsb;
    log_print('v', L"  - LSB (Index Only): raw=%.4f, weight=%.2f, contribution=%.4f\n", lsb_score, weights->lsb, lsb_score * weights->lsb);

    // 4. Alternation (alt)
    int alt_idx = find_stat_index("Alternation", 't');
    float alt_score = (alt_idx != -1) ? lt->tri_score[alt_idx] : 0.0f;
    json_object_object_add(j_stat_values, "alt", json_object_new_double(alt_score));
    final_score += alt_score * weights->alt;
    log_print('v', L"  - ALT: raw=%.4f, weight=%.2f, contribution=%.4f\n", alt_score, weights->alt, alt_score * weights->alt);


    // 5. Rolls
    int roll_idx = find_stat_index("Roll", 't');
    float roll_score = (roll_idx != -1) ? lt->tri_score[roll_idx] : 0.0f;
    json_object_object_add(j_stat_values, "rolls", json_object_new_double(roll_score));
    final_score += roll_score * weights->rolls;
    log_print('v', L"  - ROLLS: raw=%.4f, weight=%.2f, contribution=%.4f\n", roll_score, weights->rolls, roll_score * weights->rolls);


    log_print('v', L"  - FINAL SCORE: %.4f\n", final_score);

    // Add final scores and values to the main JSON object
    json_object_object_add(jobj, "stat_values", j_stat_values);
    json_object_object_add(jobj, "score", json_object_new_double(final_score));

    const char *response_str = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY);
    char *response_copy = strdup(response_str);

    json_object_put(jobj);

    log_print('v', L"JSON response built successfully.\n");
    return response_copy;
}
