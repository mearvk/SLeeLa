#ifndef SLEELA_CODEC_MP3_DETAIL_H
#define SLEELA_CODEC_MP3_DETAIL_H

/* MPEG Layer III content probe (C++), declared with C linkage for mp3.c. */

#include <stddef.h>
#include <stdint.h>
#include "sleela_codec_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_codec_result sleela_mp3_probe_bytes(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_MP3_DETAIL_H */
