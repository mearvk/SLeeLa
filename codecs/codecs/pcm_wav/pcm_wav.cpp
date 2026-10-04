/*
 * PCM/WAV byte-level detail -- C++ implementation.
 *
 * RIFF/WAVE chunk scanning and canonical header emission. Kept in C++ for the
 * small amount of structure (a chunk walker), exposed with C linkage so the
 * plugin's C file uses it directly.
 */

#include "pcm_wav_detail.h"

#include <cstring>

namespace {

inline uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }
inline uint32_t rd32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
inline void wr16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)(v & 0xFF); p[1] = (uint8_t)(v >> 8); }
inline void wr32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v & 0xFF); p[1] = (uint8_t)((v >> 8) & 0xFF);
    p[2] = (uint8_t)((v >> 16) & 0xFF); p[3] = (uint8_t)((v >> 24) & 0xFF);
}

} // namespace

extern "C" sleela_codec_result sleela_wav_parse(const uint8_t *data, size_t len,
                                                sleela_wav_format *fmt,
                                                const uint8_t **out_pcm, size_t *out_pcm_bytes) {
    if (!data || !fmt || !out_pcm || !out_pcm_bytes) return SLEELA_CODEC_ERR_INVALID;
    if (len < 12 || std::memcmp(data, "RIFF", 4) != 0 || std::memcmp(data + 8, "WAVE", 4) != 0)
        return SLEELA_CODEC_ERR_FORMAT;

    bool have_fmt = false, have_data = false;
    size_t pos = 12;                       /* after "RIFF"<size>"WAVE" */
    while (pos + 8 <= len) {
        const uint8_t *chunk_id = data + pos;
        uint32_t chunk_size = rd32(data + pos + 4);
        size_t body = pos + 8;
        if (body > len) break;
        size_t avail = len - body;
        if (chunk_size > avail) chunk_size = (uint32_t)avail;   /* tolerate truncation */

        if (std::memcmp(chunk_id, "fmt ", 4) == 0 && chunk_size >= 16) {
            fmt->audio_format    = rd16(data + body + 0);
            fmt->channels        = rd16(data + body + 2);
            fmt->sample_rate     = rd32(data + body + 4);
            fmt->bits_per_sample = rd16(data + body + 14);
            have_fmt = true;
        } else if (std::memcmp(chunk_id, "data", 4) == 0) {
            *out_pcm = data + body;
            *out_pcm_bytes = chunk_size;
            have_data = true;
        }
        pos = body + chunk_size + (chunk_size & 1);  /* chunks are word-aligned */
    }
    if (!have_fmt || !have_data) return SLEELA_CODEC_ERR_FORMAT;
    if (fmt->audio_format != 1) return SLEELA_CODEC_ERR_UNSUPPORTED;  /* 1 = PCM */
    return SLEELA_CODEC_OK;
}

extern "C" void sleela_wav_write_header(uint8_t *dst, uint16_t channels, uint32_t sample_rate,
                                        uint16_t bits_per_sample, size_t data_bytes) {
    uint32_t byte_rate = sample_rate * channels * (bits_per_sample / 8);
    uint16_t block_align = (uint16_t)(channels * (bits_per_sample / 8));
    std::memcpy(dst + 0, "RIFF", 4);
    wr32(dst + 4, (uint32_t)(36 + data_bytes));
    std::memcpy(dst + 8, "WAVE", 4);
    std::memcpy(dst + 12, "fmt ", 4);
    wr32(dst + 16, 16);                /* PCM fmt chunk size */
    wr16(dst + 20, 1);                 /* audio format = PCM */
    wr16(dst + 22, channels);
    wr32(dst + 24, sample_rate);
    wr32(dst + 28, byte_rate);
    wr16(dst + 32, block_align);
    wr16(dst + 34, bits_per_sample);
    std::memcpy(dst + 36, "data", 4);
    wr32(dst + 40, (uint32_t)data_bytes);
}
