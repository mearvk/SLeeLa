#ifndef SLEELA_WI_FI_4_WIFI_PROFILE_H
#define SLEELA_WI_FI_4_WIFI_PROFILE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIFI_BAND_2_4_GHZ = 1u,
    WIFI_BAND_5_GHZ = 2u,
    WIFI_BAND_6_GHZ = 4u
} wifi_band_t;

typedef struct {
    const char *generation;
    const char *ieee_standard;
    const char *bands;
    unsigned supported_band_mask;
    unsigned supported_width_mask; /* bit 0=20 MHz, bit 1=40, bit 2=80, bit 3=160 */
    unsigned max_spatial_streams;
    unsigned max_qam_order;
    unsigned theoretical_peak_mbps;
    const char *const *features;
    size_t feature_count;
} wifi_profile_t;

/* Static reference data only: this does not query or configure a radio. */
const wifi_profile_t *wifi_profile_get(void);
int wifi_profile_supports_band(wifi_band_t band);
int wifi_profile_supports_width_mhz(unsigned width_mhz);
void wifi_profile_print(void);

#ifdef __cplusplus
}
#endif
#endif
