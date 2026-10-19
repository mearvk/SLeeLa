#include "../include/cdma_common.h"
#include <limits.h>
int cdma_walsh_code(int *code, size_t length, unsigned index) {
    size_t i;
    if (!code || length == 0 || (length & (length - 1U)) != 0 || index >= length) return -1;
    for (i = 0; i < length; ++i) {
        unsigned v = (unsigned)i & index, parity = 0;
        while (v) { parity ^= v & 1U; v >>= 1U; }
        code[i] = parity ? -1 : 1;
    }
    return 0;
}
int cdma_ovsf_code(int *code, size_t sf, unsigned index) {
    size_t i, half;
    int lower[512];
    if (!code || sf < 2 || sf > 512 || (sf & (sf - 1U)) != 0 || index >= sf) return -1;
    if (sf == 2) { code[0] = 1; code[1] = index ? -1 : 1; return 0; }
    half = sf / 2;
    if (cdma_ovsf_code(lower, half, index % half) != 0) return -1;
    for (i = 0; i < half; ++i) {
        code[i] = lower[i];
        code[i + half] = index < half ? lower[i] : -lower[i];
    }
    return 0;
}
int cdma_pn_sequence(int *out, size_t length, unsigned stages, unsigned taps, unsigned seed) {
    size_t i; unsigned mask, state;
    if (!out || stages < 2 || stages > 30 || taps == 0) return -1;
    mask = (1U << stages) - 1U; state = seed & mask; if (!state) state = 1U;
    for (i = 0; i < length; ++i) {
        unsigned fb = 0, x = state & taps;
        while (x) { fb ^= x & 1U; x >>= 1U; }
        out[i] = (state & 1U) ? 1 : -1;
        state = ((state >> 1U) | (fb << (stages - 1U))) & mask;
    }
    return 0;
}
int cdma_spread_symbol(int *chips, size_t length, int symbol, const int *code) {
    size_t i;
    if (!chips || !code || length == 0 || (symbol != 1 && symbol != -1)) return -1;
    for (i = 0; i < length; ++i) chips[i] = symbol * code[i];
    return 0;
}
long cdma_correlate(const int *rx, size_t length, const int *code) {
    size_t i; long sum = 0;
    if (!rx || !code) return LONG_MIN;
    for (i = 0; i < length; ++i) sum += (long)rx[i] * code[i];
    return sum;
}
int cdma_decide_symbol(long correlation) { return correlation < 0 ? -1 : 1; }
int cdma_channel_add(int *channel, size_t length, const int *user) {
    size_t i;
    if (!channel || !user) return -1;
    for (i = 0; i < length; ++i) channel[i] += user[i];
    return 0;
}
void cdma_channel_clear(int *channel, size_t length) {
    size_t i;
    if (channel) for (i = 0; i < length; ++i) channel[i] = 0;
}
