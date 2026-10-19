#include "../include/cdma2000_1x.h"
#include <stddef.h>
int cdma2000_1x_make_code(int *code, size_t spreading_factor, unsigned code_index) {
    if (!code) return -1;
    if (spreading_factor < 2 || spreading_factor > 128U) return -1;
    return cdma_walsh_code(code, spreading_factor, code_index);
}
int cdma2000_1x_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips) {
    int code[512];
    if (!chips || cdma2000_1x_make_code(code, spreading_factor, code_index) != 0) return -1;
    return cdma_spread_symbol(chips, spreading_factor, symbol, code);
}
int cdma2000_1x_recover(const int *chips, size_t spreading_factor, unsigned code_index) {
    int code[512]; long corr;
    if (!chips || cdma2000_1x_make_code(code, spreading_factor, code_index) != 0) return 0;
    corr = cdma_correlate(chips, spreading_factor, code);
    return cdma_decide_symbol(corr);
}
