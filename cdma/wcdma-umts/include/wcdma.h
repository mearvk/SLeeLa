/* SPDX-License-Identifier: MIT */
/* W-CDMA / UMTS compact reference model; not a conforming modem. */
#ifndef WCDMA_REFERENCE_H
#define WCDMA_REFERENCE_H
#include <stddef.h>
#include "../../common/include/cdma_common.h"
#ifdef __cplusplus
extern "C" {
#endif
#define WCDMA_CHIP_RATE 3840000UL
#define WCDMA_MIN_SF 4U
#define WCDMA_MAX_SF 512U
int wcdma_make_code(int *code, size_t spreading_factor, unsigned code_index);
int wcdma_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips);
int wcdma_recover(const int *chips, size_t spreading_factor, unsigned code_index);
#ifdef __cplusplus
}
#endif
#endif
