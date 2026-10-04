#ifndef SLEELA_CODEC_G711_DETAIL_H
#define SLEELA_CODEC_G711_DETAIL_H

/*
 * ITU-T G.711 companding math (mu-law and A-law), shared by the g711_mulaw and
 * g711_alaw codecs. Implemented in g711_detail.cpp (C++), declared with C
 * linkage for the plugins' C files. Each function maps one byte <-> one 16-bit
 * PCM sample.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t sleela_g711_mulaw_encode(int16_t pcm);
int16_t sleela_g711_mulaw_decode(uint8_t code);
uint8_t sleela_g711_alaw_encode(int16_t pcm);
int16_t sleela_g711_alaw_decode(uint8_t code);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_G711_DETAIL_H */
