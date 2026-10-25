/* SPDX-License-Identifier: MIT */
#include "../include/is95.h"
#include <stddef.h>
int is95_make_code(int *code, size_t spreading_factor, unsigned code_index) {
    if (!code) return -1;
    /* IS-95 channelization is Walsh-64; cap at the documented IS95_WALSH_LENGTH. */
    if (spreading_factor < 2 || spreading_factor > IS95_WALSH_LENGTH) return -1;
    return cdma_walsh_code(code, spreading_factor, code_index);
}
int is95_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips) {
    int code[512];
    if (!chips || is95_make_code(code, spreading_factor, code_index) != 0) return -1;
    return cdma_spread_symbol(chips, spreading_factor, symbol, code);
}
int is95_recover(const int *chips, size_t spreading_factor, unsigned code_index) {
    int code[512]; long corr;
    if (!chips || is95_make_code(code, spreading_factor, code_index) != 0) return 0;
    corr = cdma_correlate(chips, spreading_factor, code);
    return cdma_decide_symbol(corr);
}
