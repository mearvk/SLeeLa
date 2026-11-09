#include "java_interop_protocol.h"

#include <string.h>

static uint16_t read_u16(const uint8_t *p) {
    return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
}
static uint32_t read_u32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
         | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}
static void write_u16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v;
}
static void write_u32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v;
}

int sleela_jlip_decode(const uint8_t *bytes, size_t length,
                       SleelaJavaInteropFrame *frame) {
    uint32_t payload_length;
    if (!bytes || !frame || length < SLEELA_SLJIP_HEADER_BYTES) return 0;
    if (bytes[0] != 'S' || bytes[1] != 'L' || bytes[2] != 'J' || bytes[3] != 'V')
        return 0;
    if (read_u16(bytes + 4) != SLEELA_SLJIP_VERSION) return 0;
    payload_length = read_u32(bytes + 11);
    if (payload_length > SLEELA_SLJIP_MAX_FRAME_BYTES) return 0;
    if ((size_t)payload_length != length - SLEELA_SLJIP_HEADER_BYTES) return 0;
    frame->request_id = read_u32(bytes + 6);
    frame->opcode = bytes[10];
    frame->payload = bytes + SLEELA_SLJIP_HEADER_BYTES;
    frame->payload_length = payload_length;
    return 1;
}

size_t sleela_jlip_encode(uint8_t *out, size_t capacity,
                          uint32_t request_id, uint8_t opcode,
                          const uint8_t *payload, uint32_t payload_length) {
    size_t required = SLEELA_SLJIP_HEADER_BYTES + (size_t)payload_length;
    if (!out || (payload_length && !payload)
        || payload_length > SLEELA_SLJIP_MAX_FRAME_BYTES
        || required > capacity) return 0;
    out[0] = 'S'; out[1] = 'L'; out[2] = 'J'; out[3] = 'V';
    write_u16(out + 4, SLEELA_SLJIP_VERSION);
    write_u32(out + 6, request_id);
    out[10] = opcode;
    write_u32(out + 11, payload_length);
    if (payload_length) memcpy(out + SLEELA_SLJIP_HEADER_BYTES, payload, payload_length);
    return required;
}


int sleela_jlip_exchange(SleelaJlipReadExact read_exact,
                         SleelaJlipWriteExact write_exact, void *context,
                         const uint8_t *request, size_t request_length,
                         uint8_t *response, size_t response_capacity,
                         size_t *response_length) {
    SleelaJavaInteropFrame outgoing, incoming;
    uint8_t header[SLEELA_SLJIP_HEADER_BYTES];
    uint32_t payload_length;
    size_t total;
    if (!read_exact || !write_exact || !request || !response || !response_length)
        return 0;
    *response_length = 0;
    if (!sleela_jlip_decode(request, request_length, &outgoing)
        || (outgoing.opcode & SLEELA_SLJIP_RESPONSE_MASK) != 0)
        return 0;
    if (write_exact(context, request, request_length) != 0) return 0;
    if (read_exact(context, header, sizeof(header)) != 0) return 0;
    if (header[0] != 'S' || header[1] != 'L' || header[2] != 'J' || header[3] != 'V'
        || read_u16(header + 4) != SLEELA_SLJIP_VERSION)
        return 0;
    payload_length = read_u32(header + 11);
    if (payload_length > SLEELA_SLJIP_MAX_FRAME_BYTES) return 0;
    total = SLEELA_SLJIP_HEADER_BYTES + (size_t)payload_length;
    if (total > response_capacity) return 0;
    memcpy(response, header, sizeof(header));
    if (payload_length
        && read_exact(context, response + SLEELA_SLJIP_HEADER_BYTES,
                      (size_t)payload_length) != 0)
        return 0;
    if (!sleela_jlip_decode(response, total, &incoming)) return 0;
    if (incoming.request_id != outgoing.request_id
        || incoming.opcode != (uint8_t)(outgoing.opcode | SLEELA_SLJIP_RESPONSE_MASK))
        return 0;
    *response_length = total;
    return 1;
}
