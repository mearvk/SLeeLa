#ifndef SLEELA_CODEC_AIFF_DETAIL_H
#define SLEELA_CODEC_AIFF_DETAIL_H

/* AIFF content probe (C++), declared with C linkage for aiff.c. */

#include <stddef.h>
#include <stdint.h>
#include "sleela_codec_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_codec_result sleela_aiff_probe_bytes(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_AIFF_DETAIL_H */
