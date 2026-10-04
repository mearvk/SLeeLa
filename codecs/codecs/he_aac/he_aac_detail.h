#ifndef SLEELA_CODEC_HE_AAC_DETAIL_H
#define SLEELA_CODEC_HE_AAC_DETAIL_H

/* HE-AAC content probe (C++), declared with C linkage for he_aac.c. */

#include <stddef.h>
#include <stdint.h>
#include "sleela_codec_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_codec_result sleela_he_aac_probe_bytes(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_HE_AAC_DETAIL_H */
