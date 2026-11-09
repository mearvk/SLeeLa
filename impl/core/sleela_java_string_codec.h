#ifndef SLEELA_JAVA_STRING_CODEC_H
#define SLEELA_JAVA_STRING_CODEC_H

#include "sleela_java_string.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Portable standalone Java String record: "SLJS", version/reserved bytes,
 * uint32 little-endian UTF-16-unit count, then little-endian uint16 units.
 * This codec is deliberately separate from legacy .sleela artifact v2. */
#define SL_JAVA_STRING_RECORD_HEADER_SIZE 12u
int sljava_string_encode_record(const SLJavaString *value, uint8_t **out_bytes,
                                size_t *out_size);
int sljava_string_decode_record(SLJavaString *out, const uint8_t *bytes,
                                size_t size);

#ifdef __cplusplus
}
#endif
#endif
