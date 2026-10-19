/* IS-95 cdmaOne compact reference model; not a conforming modem. */
#ifndef IS95_REFERENCE_H
#define IS95_REFERENCE_H
#include <stddef.h>
#include "../../common/include/cdma_common.h"
#ifdef __cplusplus
extern "C" {
#endif
#define IS95_CHIP_RATE 1228800UL
#define IS95_CHANNEL_WIDTH_HZ 1250000UL
#define IS95_WALSH_LENGTH 64U
#define IS95_PILOT_CHANNEL 0U
#define IS95_SYNC_CHANNEL 32U
#define IS95_PAGING_CHANNEL 1U
#define IS95_TRAFFIC_CHANNEL 2U
int is95_make_code(int *code, size_t spreading_factor, unsigned code_index);
int is95_spread(int symbol, size_t spreading_factor, unsigned code_index, int *chips);
int is95_recover(const int *chips, size_t spreading_factor, unsigned code_index);
#ifdef __cplusplus
}
#endif
#endif
