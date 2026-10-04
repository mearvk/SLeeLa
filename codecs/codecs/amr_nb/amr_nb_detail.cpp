/*
 * AMR-NB content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "amr_nb_detail.h"

extern "C" sleela_codec_result sleela_amr_nb_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 6 && data[0] == 0x23 && data[1] == 0x21 && data[2] == 0x41 && data[3] == 0x4D && data[4] == 0x52 && data[5] == 0x0A) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
