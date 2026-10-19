#include <stdio.h>
#include <stdlib.h>
#include "../include/cdma2000_1x.h"
int main(void) {
    int c0[64], c1[64], u0[64], u1[64], mix[64];
    size_t i;
    if (cdma2000_1x_make_code(c0, 64, 0U) != 0 || cdma2000_1x_make_code(c1, 64, 1U) != 0) return 1;
    if (cdma_spread_symbol(u0, 64, 1, c0) != 0 || cdma_spread_symbol(u1, 64, -1, c1) != 0) return 2;
    cdma_channel_clear(mix, 64);
    if (cdma_channel_add(mix, 64, u0) != 0 || cdma_channel_add(mix, 64, u1) != 0) return 3;
    if (cdma_decide_symbol(cdma_correlate(mix, 64, c0)) != 1 || cdma_decide_symbol(cdma_correlate(mix, 64, c1)) != -1) return 4;
    for (i = 0; i < 64; ++i) { if (mix[i] != c0[i] - c1[i]) return 5; }
    printf("CDMA2000 1xRTT: shared-channel recovery PASS (+1, -1)\\n");
    return 0;
}
