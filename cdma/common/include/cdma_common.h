/* SLeeLa CDMA common DS-CDMA primitives.
 * Reference model only; not a production baseband.
 */
#ifndef SLEELA_CDMA_COMMON_H
#define SLEELA_CDMA_COMMON_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
int cdma_walsh_code(int *code, size_t length, unsigned index);
int cdma_ovsf_code(int *code, size_t sf, unsigned index);
int cdma_pn_sequence(int *out, size_t length, unsigned stages, unsigned taps, unsigned seed);
int cdma_spread_symbol(int *chips, size_t length, int symbol, const int *code);
long cdma_correlate(const int *rx, size_t length, const int *code);
int cdma_decide_symbol(long correlation);
int cdma_channel_add(int *channel, size_t length, const int *user);
void cdma_channel_clear(int *channel, size_t length);
#ifdef __cplusplus
}
#endif
#endif
