#include "../runtime/java_interop_protocol.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t incoming[64];
    size_t incoming_length;
    size_t incoming_offset;
    uint8_t outgoing[64];
    size_t outgoing_length;
} MemoryPipe;

static int memory_read(void *opaque, uint8_t *buffer, size_t length) {
    MemoryPipe *pipe = (MemoryPipe *)opaque;
    if (length > pipe->incoming_length - pipe->incoming_offset) return -1;
    memcpy(buffer, pipe->incoming + pipe->incoming_offset, length);
    pipe->incoming_offset += length;
    return 0;
}
static int memory_write(void *opaque, const uint8_t *buffer, size_t length) {
    MemoryPipe *pipe = (MemoryPipe *)opaque;
    if (length > sizeof(pipe->outgoing)) return -1;
    memcpy(pipe->outgoing, buffer, length);
    pipe->outgoing_length = length;
    return 0;
}

int main(void) {
    uint8_t bytes[64], payload[] = {0x00, 0x7f, 0x80, 0xff};
    SleelaJavaInteropFrame frame;
    size_t length = sleela_jlip_encode(bytes, sizeof(bytes), 0x12345678u,
                                       5u, payload, sizeof(payload));
    assert(length == SLEELA_SLJIP_HEADER_BYTES + sizeof(payload));
    assert(sleela_jlip_decode(bytes, length, &frame));
    assert(frame.request_id == 0x12345678u);
    assert(frame.opcode == 5u);
    assert(frame.payload_length == sizeof(payload));
    assert(memcmp(frame.payload, payload, sizeof(payload)) == 0);
    assert(!sleela_jlip_decode(bytes, length - 1, &frame));
    bytes[0] = 'X';
    assert(!sleela_jlip_decode(bytes, length, &frame));
    assert(sleela_jlip_encode(bytes, 4, 1u, 1u, payload, sizeof(payload)) == 0);
    assert(sleela_jlip_encode(bytes, sizeof(bytes), 1u, 1u, NULL, 1u) == 0);
    {
        MemoryPipe pipe = {{0}, 0, 0, {0}, 0};
        uint8_t response_payload[] = {0, 0};
        uint8_t response[64];
        size_t response_length = 0;
        length = sleela_jlip_encode(bytes, sizeof(bytes), 0x12345678u, 5u, payload, sizeof(payload));
        pipe.incoming_length = sleela_jlip_encode(pipe.incoming, sizeof(pipe.incoming),
                                                  0x12345678u, (uint8_t)(5u | 0x80u),
                                                  response_payload, sizeof(response_payload));
        assert(sleela_jlip_exchange(memory_read, memory_write, &pipe,
                                    bytes, length, response, sizeof(response),
                                    &response_length));
        assert(pipe.outgoing_length == length);
        assert(memcmp(pipe.outgoing, bytes, length) == 0);
        assert(response_length == pipe.incoming_length);
    }
    puts("SLJIP native frame codec and exchange: PASS");
    return 0;
}
