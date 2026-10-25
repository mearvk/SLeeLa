/* SPDX-License-Identifier: MIT */
/* CDMA2000 1xRTT compact reference model; not a conforming modem. */
#ifndef CDMA2000_1X_REFERENCE_H
#define CDMA2000_1X_REFERENCE_H
#include <stddef.h>
#include "../../common/include/cdma_common.h"
#ifdef __cplusplus
extern "C" {
#endif
#define CDMA2000_1X_CHIP_RATE 1228800UL
#define CDMA2000_1X_CHANNEL_WIDTH_HZ 1250000UL
#define CDMA2000_1X_MIN_SF 4U
#define CDMA2000_1X_MAX_SF 128U
#define CDMA2000_1X_FUNDAMENTAL 0U
#define CDMA2000_1X_SUPPLEMENTAL 1U
int cdma2000_1x_make_code(int *code, size_t spreading_factor, unsigned code_index);
int cdma2000_1x_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips);
int cdma2000_1x_recover(const int *chips, size_t spreading_factor, unsigned code_index);
#ifdef __cplusplus
}
#endif
#endif
