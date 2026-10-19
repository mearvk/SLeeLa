#include "wifi_profile.h"

#include <stdio.h>

static const char *const FEATURES[] = {
    "Wide channels",
    "256-QAM",
    "Downlink MU-MIMO",
    "Beamforming",
    "A-MPDU/A-MSDU aggregation"
};

static const wifi_profile_t PROFILE = {
    "Wi-Fi 5", "IEEE 802.11ac", "Primarily 5 GHz",
    2u, 15u, 8u, 256u, 6933u,
    FEATURES, sizeof(FEATURES) / sizeof(FEATURES[0])
};

const wifi_profile_t *wifi_profile_get(void) { return &PROFILE; }

int wifi_profile_supports_band(wifi_band_t band) {
    return (PROFILE.supported_band_mask & (unsigned)band) != 0u;
}

int wifi_profile_supports_width_mhz(unsigned width_mhz) {
    unsigned bit;
    switch (width_mhz) {
        case 20u: bit = 1u; break;
        case 40u: bit = 2u; break;
        case 80u: bit = 4u; break;
        case 160u: bit = 8u; break;
        default: return 0;
    }
    return (PROFILE.supported_width_mask & bit) != 0u;
}

void wifi_profile_print(void) {
    size_t i;
    printf("%s (%s)\n", PROFILE.generation, PROFILE.ieee_standard);
    printf("Bands: %s\n", PROFILE.bands);
    printf("Maximum theoretical PHY rate: %u Mbit/s (not application throughput)\n", PROFILE.theoretical_peak_mbps);
    printf("Maximum standard spatial streams: %u; highest listed modulation: %u-QAM\n", PROFILE.max_spatial_streams, PROFILE.max_qam_order);
    printf("Reference features:\n");
    for (i = 0; i < PROFILE.feature_count; ++i) printf("  - %s\n", PROFILE.features[i]);
}
