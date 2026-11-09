#include "sleela_java_string_codec.h"

#include <limits.h>
#include <stdlib.h>

static const uint8_t magic[4] = { 'S', 'L', 'J', 'S' };

int sljava_string_encode_record(const SLJavaString *value, uint8_t **out_bytes,
                                size_t *out_size) {
    uint8_t *bytes;
    size_t size;
    if (!value || !out_bytes || !out_size ||
        (value->length && !value->units) || value->length > INT32_MAX)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (value->length > (SIZE_MAX - SL_JAVA_STRING_RECORD_HEADER_SIZE) / 2u)
        return SL_JAVA_STRING_TOO_LARGE;
    size = SL_JAVA_STRING_RECORD_HEADER_SIZE + value->length * 2u;
    bytes = (uint8_t *)malloc(size);
    if (!bytes) return SL_JAVA_STRING_OUT_OF_MEMORY;
    bytes[0] = magic[0]; bytes[1] = magic[1]; bytes[2] = magic[2]; bytes[3] = magic[3];
    bytes[4] = 1u; bytes[5] = 0u; bytes[6] = 0u; bytes[7] = 0u;
    bytes[8] = (uint8_t)value->length;
    bytes[9] = (uint8_t)(value->length >> 8);
    bytes[10] = (uint8_t)(value->length >> 16);
    bytes[11] = (uint8_t)(value->length >> 24);
    for (size_t i = 0; i < value->length; ++i) {
        uint16_t unit = value->units[i];
        bytes[SL_JAVA_STRING_RECORD_HEADER_SIZE + i * 2u] = (uint8_t)unit;
        bytes[SL_JAVA_STRING_RECORD_HEADER_SIZE + i * 2u + 1u] = (uint8_t)(unit >> 8);
    }
    *out_bytes = bytes;
    *out_size = size;
    return SL_JAVA_STRING_OK;
}

int sljava_string_decode_record(SLJavaString *out, const uint8_t *bytes,
                                size_t size) {
    uint32_t length;
    uint16_t *units = NULL;
    int status;
    if (!out || !bytes || size < SL_JAVA_STRING_RECORD_HEADER_SIZE)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (bytes[0] != magic[0] || bytes[1] != magic[1] ||
        bytes[2] != magic[2] || bytes[3] != magic[3] ||
        bytes[4] != 1u || bytes[5] != 0u || bytes[6] != 0u || bytes[7] != 0u)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    length = (uint32_t)bytes[8] | ((uint32_t)bytes[9] << 8) |
             ((uint32_t)bytes[10] << 16) | ((uint32_t)bytes[11] << 24);
    if (length > INT32_MAX || (size_t)length > (SIZE_MAX - SL_JAVA_STRING_RECORD_HEADER_SIZE) / 2u ||
        size != SL_JAVA_STRING_RECORD_HEADER_SIZE + (size_t)length * 2u)
        return SL_JAVA_STRING_INVALID_ARGUMENT;
    if (length) {
        units = (uint16_t *)malloc((size_t)length * sizeof(*units));
        if (!units) return SL_JAVA_STRING_OUT_OF_MEMORY;
        for (uint32_t i = 0; i < length; ++i)
            units[i] = (uint16_t)bytes[SL_JAVA_STRING_RECORD_HEADER_SIZE + (size_t)i * 2u] |
                       (uint16_t)((uint16_t)bytes[SL_JAVA_STRING_RECORD_HEADER_SIZE + (size_t)i * 2u + 1u] << 8);
    }
    status = sljava_string_from_utf16(out, units, length);
    free(units);
    return status;
}
