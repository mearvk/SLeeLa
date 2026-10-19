#include <stdio.h>
#include "../include/cdma_common.h"
int main(void) {
    int a[4], b[4], x[4], y[4], mix[4];
    if (cdma_walsh_code(a, 4, 0) || cdma_walsh_code(b, 4, 1)) return 1;
    if (cdma_spread_symbol(x, 4, 1, a) || cdma_spread_symbol(y, 4, -1, b)) return 2;
    cdma_channel_clear(mix, 4);
    if (cdma_channel_add(mix, 4, x) || cdma_channel_add(mix, 4, y)) return 3;
    if (cdma_decide_symbol(cdma_correlate(mix, 4, a)) != 1) return 4;
    if (cdma_decide_symbol(cdma_correlate(mix, 4, b)) != -1) return 5;
    puts("CDMA common: multi-user recovery PASS (+1, -1)");
    return 0;
}
