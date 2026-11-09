#ifndef SLEELA_JAVA_INTEROP_PROTOCOL_H
#define SLEELA_JAVA_INTEROP_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_JLIP_VERSION 1u
#define SLEELA_JLIP_HEADER_BYTES 15u
#define SLEELA_JLIP_MAX_FRAME_BYTES 1048576u
#define SLEELA_JLIP_RESPONSE_MASK 0x80u

typedef struct {
    uint32_t request_id;
    uint8_t opcode;
    const uint8_t *payload;
    uint32_t payload_length;
} SleelaJavaInteropFrame;

/* Validates the complete frame header and payload bounds; payload remains borrowed. */
int sleela_jlip_decode(const uint8_t *bytes, size_t length,
                       SleelaJavaInteropFrame *frame);

/* Writes one complete frame. Returns required bytes; 0 means invalid input.
   If capacity is too small, no output bytes are written. */
size_t sleela_jlip_encode(uint8_t *out, size_t capacity,
                          uint32_t request_id, uint8_t opcode,
                          const uint8_t *payload, uint32_t payload_length);

#ifdef __cplusplus
}
#endif
#endif
