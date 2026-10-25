/* SPDX-License-Identifier: MIT */
#include "../include/wcdma.h"
int wcdma_make_code(int *code, size_t sf, unsigned index) {
    if (!code || sf < 2 || sf > 512U) return -1;
    return cdma_ovsf_code(code, sf, index);
}
int wcdma_spread(int symbol, size_t sf, unsigned index, int *chips) {
    int code[512];
    if (!chips || wcdma_make_code(code, sf, index) != 0) return -1;
    return cdma_spread_symbol(chips, sf, symbol, code);
}
int wcdma_recover(const int *chips, size_t sf, unsigned index) {
    int code[512];
    if (!chips || wcdma_make_code(code, sf, index) != 0) return 0;
    return cdma_decide_symbol(cdma_correlate(chips, sf, code));
}
