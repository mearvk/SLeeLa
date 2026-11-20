#ifndef SLEELA_JAVA_INTEROP_PROTOCOL_H
#define SLEELA_JAVA_INTEROP_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_SLJIP_VERSION 1u
#define SLEELA_SLJIP_HEADER_BYTES 15u
#define SLEELA_SLJIP_MAX_FRAME_BYTES 1048576u
#define SLEELA_SLJIP_RESPONSE_MASK 0x80u

typedef int (*SleelaSljipReadExact)(void *context, uint8_t *buffer, size_t length);
typedef int (*SleelaSljipWriteExact)(void *context, const uint8_t *buffer, size_t length);

typedef struct {
    uint32_t request_id;
    uint8_t opcode;
    const uint8_t *payload;
    uint32_t payload_length;
} SleelaJavaInteropFrame;

/* Validates the complete frame header and payload bounds; payload remains borrowed. */
int sleela_sljip_decode(const uint8_t *bytes, size_t length,
                       SleelaJavaInteropFrame *frame);

/* Writes one complete frame. Returns required bytes; 0 means invalid input.
   If capacity is too small, no output bytes are written. */
size_t sleela_sljip_encode(uint8_t *out, size_t capacity,
                          uint32_t request_id, uint8_t opcode,
                          const uint8_t *payload, uint32_t payload_length);

/* One request/response exchange over caller-owned binary streams. Callbacks
   must transfer exactly the requested byte count or return nonzero. Returns
   1 on a matching response, 0 on protocol/transport failure. */
int sleela_sljip_exchange(SleelaSljipReadExact read_exact,
                         SleelaSljipWriteExact write_exact, void *context,
                         const uint8_t *request, size_t request_length,
                         uint8_t *response, size_t response_capacity,
                         size_t *response_length);

#ifdef __cplusplus
}
#endif
#endif
