/*
 * ITU-T G.711 companding -- C++ implementation shared by the mu-law and A-law
 * codecs. Standard reference algorithm (bias/segment companding), exposed with
 * C linkage.
 */

#include "g711_detail.h"

namespace {
constexpr int MULAW_BIAS = 0x84;   /* 132 */
constexpr int MULAW_CLIP = 32635;
constexpr int ALAW_CLIP  = 32635;
} // namespace

extern "C" uint8_t sleela_g711_mulaw_encode(int16_t pcm) {
    int sign = (pcm >> 8) & 0x80;
    int sample = pcm;
    if (sign) sample = -sample;
    if (sample > MULAW_CLIP) sample = MULAW_CLIP;
    sample += MULAW_BIAS;
    int exponent = 7;
    for (int mask = 0x4000; (sample & mask) == 0 && exponent > 0; mask >>= 1) --exponent;
    int mantissa = (sample >> (exponent + 3)) & 0x0F;
    uint8_t code = (uint8_t)~(sign | (exponent << 4) | mantissa);
    return code;
}

extern "C" int16_t sleela_g711_mulaw_decode(uint8_t code) {
    code = (uint8_t)~code;
    int sign = code & 0x80;
    int exponent = (code >> 4) & 0x07;
    int mantissa = code & 0x0F;
    int sample = ((mantissa << 3) + MULAW_BIAS) << exponent;
    sample -= MULAW_BIAS;
    return (int16_t)(sign ? -sample : sample);
}

extern "C" uint8_t sleela_g711_alaw_encode(int16_t pcm) {
    int sign = ((~pcm) >> 8) & 0x80;
    int sample = pcm;
    if (!sign) sample = -sample;
    if (sample > ALAW_CLIP) sample = ALAW_CLIP;
    uint8_t code;
    if (sample >= 256) {
        int exponent = 7;
        for (int mask = 0x4000; (sample & mask) == 0 && exponent > 0; mask >>= 1) --exponent;
        int mantissa = (sample >> (exponent + 3)) & 0x0F;
        code = (uint8_t)((exponent << 4) | mantissa);
    } else {
        code = (uint8_t)(sample >> 4);
    }
    return (uint8_t)(code ^ (sign | 0x55));
}

extern "C" int16_t sleela_g711_alaw_decode(uint8_t code) {
    code ^= 0x55;
    int sign = code & 0x80;
    int exponent = (code >> 4) & 0x07;
    int mantissa = code & 0x0F;
    int sample;
    if (exponent == 0) {
        sample = (mantissa << 4) + 8;
    } else {
        sample = ((mantissa << 4) + 0x108) << (exponent - 1);
    }
    return (int16_t)(sign ? sample : -sample);
}
