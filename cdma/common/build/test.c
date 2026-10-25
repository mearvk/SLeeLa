#include <stdio.h>
#include "../include/cdma_common.h"
int main(void) {
    int a[4], b[4], x[4], y[4], mix[4], pn[7];
    if (cdma_walsh_code(a, 4, 0) || cdma_walsh_code(b, 4, 1)) return 1;
    if (cdma_spread_symbol(x, 4, 1, a) || cdma_spread_symbol(y, 4, -1, b)) return 2;
    cdma_channel_clear(mix, 4);
    if (cdma_channel_add(mix, 4, x) || cdma_channel_add(mix, 4, y)) return 3;
    if (cdma_decide_symbol(cdma_correlate(mix, 4, a)) != 1) return 4;
    if (cdma_decide_symbol(cdma_correlate(mix, 4, b)) != -1) return 5;

    /* PN taps validation: a 3-stage register (mask 0b111) must accept valid
     * taps and reject taps referencing stages outside the register. */
    if (cdma_pn_sequence(pn, 7, 3, 0x5U, 1U) != 0) return 6;   /* taps within mask */
    if (cdma_pn_sequence(pn, 7, 3, 0x8U, 1U) != -1) return 7;  /* tap bit outside mask -> reject */

    puts("CDMA common: multi-user recovery PASS (+1, -1); PN taps validated");
    return 0;
}
