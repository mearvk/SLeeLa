#ifndef SLEELA_COMPRESSION_ZIP_DETAIL_H
#define SLEELA_COMPRESSION_ZIP_DETAIL_H

/* ZIP archive content probe (C++), declared with C linkage for zip.c. */

#include <stddef.h>
#include <stdint.h>
#include "sleela_compressor_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_compression_result sleela_zip_probe_bytes(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_ZIP_DETAIL_H */
