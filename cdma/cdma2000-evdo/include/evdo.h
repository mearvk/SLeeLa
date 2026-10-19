/* CDMA2000 1x EV-DO compact reference model; not a conforming modem. */
#ifndef EVDO_REFERENCE_H
#define EVDO_REFERENCE_H
#include <stddef.h>
#include "../../common/include/cdma_common.h"
#ifdef __cplusplus
extern "C" {
#endif
#define EVDO_CHIP_RATE 1228800UL
#define EVDO_SLOT_CHIPS 2048U
#define EVDO_DRC_MIN 1U
#define EVDO_DRC_MAX 12U
int evdo_make_code(int *code, size_t spreading_factor, unsigned code_index);
int evdo_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips);
int evdo_recover(const int *chips, size_t spreading_factor, unsigned code_index);
#ifdef __cplusplus
}
#endif
#endif
