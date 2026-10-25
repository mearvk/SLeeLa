/* SPDX-License-Identifier: MIT */
#include <stdio.h>
#include "../include/wcdma.h"
int main(void) {
    int c0[16], c1[16], u0[16], u1[16], mix[16];
    size_t i;
    if (wcdma_make_code(c0, 16, 0U) != 0 || wcdma_make_code(c1, 16, 1U) != 0) return 1;
    if (cdma_spread_symbol(u0, 16, 1, c0) != 0 || cdma_spread_symbol(u1, 16, -1, c1) != 0) return 2;
    cdma_channel_clear(mix, 16);
    if (cdma_channel_add(mix, 16, u0) != 0 || cdma_channel_add(mix, 16, u1) != 0) return 3;
    if (cdma_decide_symbol(cdma_correlate(mix, 16, c0)) != 1 || cdma_decide_symbol(cdma_correlate(mix, 16, c1)) != -1) return 4;
    for (i = 0; i < 16; ++i) if (mix[i] != c0[i] - c1[i]) return 5;
    puts("W-CDMA / UMTS: shared-channel recovery PASS (+1, -1)");
    return 0;
}
