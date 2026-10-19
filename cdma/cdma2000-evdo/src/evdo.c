#include "../include/evdo.h"
#include <stddef.h>
int evdo_make_code(int *code, size_t spreading_factor, unsigned code_index) {
    if (!code) return -1;
    if (spreading_factor < 2 || spreading_factor > 512U) return -1;
    return cdma_walsh_code(code, spreading_factor, code_index);
}
int evdo_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips) {
    int code[512];
    if (!chips || evdo_make_code(code, spreading_factor, code_index) != 0) return -1;
    return cdma_spread_symbol(chips, spreading_factor, symbol, code);
}
int evdo_recover(const int *chips, size_t spreading_factor, unsigned code_index) {
    int code[512]; long corr;
    if (!chips || evdo_make_code(code, spreading_factor, code_index) != 0) return 0;
    corr = cdma_correlate(chips, spreading_factor, code);
    return cdma_decide_symbol(corr);
}
