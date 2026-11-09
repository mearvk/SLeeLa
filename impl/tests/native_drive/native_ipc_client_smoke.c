#include "../native-drive/native_ipc_client.h"

#include <stdio.h>
#include <string.h>

typedef struct { uint8_t bytes[256]; size_t length, offset; } MemoryPipe;

static int memory_read(void *context, uint8_t *data, size_t length) {
    MemoryPipe *p = (MemoryPipe *)context;
    if (!p || !data || length > p->length - p->offset) return -1;
    memcpy(data, p->bytes + p->offset, length);
    p->offset += length;
    return 0;
}

static int memory_write(void *context, const uint8_t *data, size_t length) {
    MemoryPipe *p = (MemoryPipe *)context;
    SLIPCHeader request, response;
    SLNativeValue value;
    size_t used;
    uint8_t encoded[64], header[SL_IPC_HEADER_SIZE];
    if (!p || !data) return -1;
    if (p->offset == p->length) p->offset = p->length = 0;
    if (length == SL_IPC_HEADER_SIZE) {
        if (sl_ipc_decode_header(data, length, &request) < 0) return -1;
        if (request.payload_length != 0) return -1;
        response = request;
        response.kind = request.kind == SL_IPC_HELLO ? SL_IPC_HELLO_ACK : SL_IPC_RESPONSE;
        response.payload_length = 0;
        if (request.kind == SL_IPC_HELLO) {
            static const uint8_t hello[] = "sleela-ipc/1";
            response.payload_length = (uint32_t)(sizeof(hello) - 1u);
            if (sl_ipc_encode_header(&response, header, sizeof(header)) < 0) return -1;
            memcpy(p->bytes, header, sizeof(header));
            memcpy(p->bytes + sizeof(header), hello, sizeof(hello) - 1u);
            p->length = sizeof(header) + sizeof(hello) - 1u;
            p->offset = 0;
            return 0;
        }
        memset(&value, 0, sizeof(value));
        value.tag = SL_NATIVE_VALUE_UTF8;
        value.as.utf8.data = (const uint8_t *)"pong";
        value.as.utf8.length = 4;
        if (sl_ipc_encode_value(&value, encoded, sizeof(encoded), &used) != 0) return -1;
        response.payload_length = (uint32_t)used;
        if (sl_ipc_encode_header(&response, header, sizeof(header)) < 0) return -1;
        memcpy(p->bytes, header, sizeof(header));
        memcpy(p->bytes + sizeof(header), encoded, used);
        p->length = sizeof(header) + used;
        p->offset = 0;
        return 0;
    }
    return -1;
}

int main(void) {
    MemoryPipe pipe;
    SLIPCClient client;
    SLNativeValue result;
    uint8_t workspace[SL_IPC_MAX_PAYLOAD];
    char error[128];
    memset(&pipe, 0, sizeof(pipe));
    if (sl_ipc_client_init(&client, memory_read, memory_write, &pipe,
                           workspace, sizeof(workspace)) != 0) {
        fputs("native IPC client handshake: FAIL\n", stderr);
        return 1;
    }
    if (sl_ipc_client_call(&client, (const uint8_t *)"math.ping", 9,
                           NULL, 0, &result, error, sizeof(error)) != 0 ||
        result.tag != SL_NATIVE_VALUE_UTF8 || result.as.utf8.length != 4 ||
        memcmp(result.as.utf8.data, "pong", 4) != 0) {
        fputs("native IPC client request/response: FAIL\n", stderr);
        return 1;
    }
    sl_ipc_client_close(&client);
    puts("native IPC client: PASS");
    return 0;
}
