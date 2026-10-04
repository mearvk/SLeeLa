/*
 * AIFF container detail -- C++ implementation. Big-endian chunk reader/writer
 * including the IEEE-754 80-bit extended ("long double"-style) sample-rate field
 * the COMM chunk uses. Exposed with C linkage for aiff.c.
 */

#include "aiff_detail.h"

#include <cstring>

namespace {

inline uint16_t rd16be(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
inline uint32_t rd32be(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}
inline void wr16be(uint8_t *p, uint16_t v) { p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)(v & 0xFF); }
inline void wr32be(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16); p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v;
}

/* Decode an 80-bit IEEE-754 extended float (big-endian) to a uint32 rate. */
uint32_t read_extended80(const uint8_t *p) {
    int exponent = ((p[0] & 0x7F) << 8) | p[1];
    uint64_t mantissa = 0;
    for (int i = 0; i < 8; ++i) mantissa = (mantissa << 8) | p[2 + i];
    if (exponent == 0 && mantissa == 0) return 0;
    exponent -= 16383;                      /* unbias */
    /* value = mantissa * 2^(exponent-63); mantissa has an explicit integer bit. */
    int shift = exponent - 63;
    double value;
    if (shift >= 0) value = (double)mantissa * (double)(1ULL << (shift < 63 ? shift : 62));
    else            value = (double)mantissa / (double)(1ULL << (-shift < 63 ? -shift : 62));
    return (uint32_t)(value + 0.5);
}

/* Encode a uint32 rate as an 80-bit IEEE-754 extended float (big-endian). */
void write_extended80(uint8_t *p, uint32_t rate) {
    std::memset(p, 0, 10);
    if (rate == 0) return;
    uint32_t v = rate;
    int exponent = 0;
    while ((v & 0x80000000u) == 0 && exponent < 63) { v <<= 1; ++exponent; }
    /* Normalize: integer bit is the top bit. mantissa = v shifted into 64 bits. */
    uint64_t mantissa = (uint64_t)v << 32;
    int biased = 16383 + (31 - exponent);
    p[0] = (uint8_t)((biased >> 8) & 0x7F);
    p[1] = (uint8_t)(biased & 0xFF);
    for (int i = 0; i < 8; ++i) p[2 + i] = (uint8_t)(mantissa >> (56 - 8 * i));
}

} // namespace

extern "C" sleela_codec_result sleela_aiff_probe_bytes(const uint8_t *data, size_t len) {
    if (!data || len < 12) return SLEELA_CODEC_ERR_FORMAT;
    if (std::memcmp(data, "FORM", 4) != 0 || std::memcmp(data + 8, "AIFF", 4) != 0)
        return SLEELA_CODEC_ERR_FORMAT;
    return SLEELA_CODEC_OK;
}

extern "C" sleela_codec_result sleela_aiff_parse(const uint8_t *data, size_t len,
                                                 sleela_aiff_format *fmt,
                                                 const uint8_t **out_pcm, size_t *out_pcm_bytes) {
    if (!data || !fmt || !out_pcm || !out_pcm_bytes) return SLEELA_CODEC_ERR_INVALID;
    if (sleela_aiff_probe_bytes(data, len) != SLEELA_CODEC_OK) return SLEELA_CODEC_ERR_FORMAT;

    bool have_comm = false, have_ssnd = false;
    size_t pos = 12;
    while (pos + 8 <= len) {
        const uint8_t *id = data + pos;
        uint32_t sz = rd32be(data + pos + 4);
        size_t body = pos + 8;
        if (body > len) break;
        size_t avail = len - body;
        if (sz > avail) sz = (uint32_t)avail;

        if (std::memcmp(id, "COMM", 4) == 0 && sz >= 18) {
            fmt->channels        = rd16be(data + body + 0);
            fmt->frames          = rd32be(data + body + 2);
            fmt->bits_per_sample = rd16be(data + body + 6);
            fmt->sample_rate     = read_extended80(data + body + 8);
            have_comm = true;
        } else if (std::memcmp(id, "SSND", 4) == 0 && sz >= 8) {
            uint32_t offset = rd32be(data + body + 0);
            size_t sample_start = body + 8 + offset;
            if (sample_start <= len) {
                *out_pcm = data + sample_start;
                *out_pcm_bytes = (sz >= 8 + offset) ? (sz - 8 - offset) : 0;
                have_ssnd = true;
            }
        }
        pos = body + sz + (sz & 1);
    }
    if (!have_comm || !have_ssnd) return SLEELA_CODEC_ERR_FORMAT;
    return SLEELA_CODEC_OK;
}

extern "C" void sleela_aiff_write_header(uint8_t *dst, uint16_t channels, uint32_t sample_rate,
                                         uint16_t bits_per_sample, uint32_t frames, size_t data_bytes) {
    uint32_t ssnd_size = (uint32_t)(data_bytes + 8);     /* offset+blocksize + samples */
    uint32_t form_size = (uint32_t)(4 + (8 + 18) + (8 + ssnd_size)); /* "AIFF" + COMM + SSND */
    std::memcpy(dst + 0, "FORM", 4);
    wr32be(dst + 4, form_size);
    std::memcpy(dst + 8, "AIFF", 4);
    /* COMM chunk */
    std::memcpy(dst + 12, "COMM", 4);
    wr32be(dst + 16, 18);
    wr16be(dst + 20, channels);
    wr32be(dst + 22, frames);
    wr16be(dst + 26, bits_per_sample);
    write_extended80(dst + 28, sample_rate);             /* 10 bytes: 28..37 */
    /* SSND chunk header */
    std::memcpy(dst + 38, "SSND", 4);
    wr32be(dst + 42, ssnd_size);
    wr32be(dst + 46, 0);                                 /* offset */
    wr32be(dst + 50, 0);                                 /* block size */
    /* samples follow at dst + 54 (SLEELA_AIFF_HEADER_SIZE) */
}
