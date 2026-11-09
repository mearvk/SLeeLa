#include "../runtime/java_interop_protocol.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    uint8_t bytes[64], payload[] = {0x00, 0x7f, 0x80, 0xff};
    SleelaJavaInteropFrame frame;
    size_t length = sleela_jlip_encode(bytes, sizeof(bytes), 0x12345678u,
                                       5u, payload, sizeof(payload));
    assert(length == SLEELA_JLIP_HEADER_BYTES + sizeof(payload));
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
    puts("SLJIP native frame codec: PASS");
    return 0;
}
