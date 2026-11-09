#include "../native-drive/native_ipc_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t tx[256], rx[256];
    size_t tx_length, rx_length, rx_offset;
} MemoryPipe;

static int memory_read(void *context, uint8_t *data, size_t length) {
    MemoryPipe *p = (MemoryPipe *)context;
    if (!p || !data || p->rx_offset > p->rx_length ||
        length > p->rx_length - p->rx_offset) return -1;
    memcpy(data, p->rx + p->rx_offset, length);
    p->rx_offset += length;
    return 0;
}

static int memory_write(void *context, const uint8_t *data, size_t length) {
    MemoryPipe *p = (MemoryPipe *)context;
    SLIPCHeader request, response;
    SLNativeValue value;
    size_t used, frame_length;
    uint8_t encoded[64], header[SL_IPC_HEADER_SIZE];
    if (!p || !data || length > sizeof(p->tx) - p->tx_length) return -1;
    memcpy(p->tx + p->tx_length, data, length);
    p->tx_length += length;
    if (p->tx_length < SL_IPC_HEADER_SIZE) return 0;
    if (sl_ipc_decode_header(p->tx, p->tx_length, &request) < 0) return -1;
    frame_length = SL_IPC_HEADER_SIZE + request.payload_length;
    if (p->tx_length < frame_length) return 0;
    if (p->tx_length != frame_length) return -1;

    response = request;
    response.status = 0;
    if (request.kind == SL_IPC_HELLO) {
        static const uint8_t hello[] = "sleela-ipc/1";
        response.kind = SL_IPC_HELLO_ACK;
        response.payload_length = (uint32_t)(sizeof(hello) - 1u);
        if (sl_ipc_encode_header(&response, header, sizeof(header)) < 0) return -1;
        memcpy(p->rx, header, sizeof(header));
        memcpy(p->rx + sizeof(header), hello, sizeof(hello) - 1u);
        p->rx_length = sizeof(header) + sizeof(hello) - 1u;
    } else if (request.kind == SL_IPC_REQUEST) {
        response.kind = SL_IPC_RESPONSE;
        memset(&value, 0, sizeof(value));
        value.tag = SL_NATIVE_VALUE_UTF8;
        value.as.utf8.data = (const uint8_t *)"pong";
        value.as.utf8.length = 4;
        if (sl_ipc_encode_value(&value, encoded, sizeof(encoded), &used) != 0) return -1;
        response.payload_length = (uint32_t)used;
        if (sl_ipc_encode_header(&response, header, sizeof(header)) < 0) return -1;
        memcpy(p->rx, header, sizeof(header));
        memcpy(p->rx + sizeof(header), encoded, used);
        p->rx_length = sizeof(header) + used;
    } else return -1;
    p->rx_offset = 0;
    p->tx_length = 0;
    return 0;
}

int main(void) {
    MemoryPipe pipe;
    SLIPCClient client;
    SLNativeValue result;
    uint8_t *workspace = (uint8_t *)malloc(4096u);
    char error[128];
    memset(&pipe, 0, sizeof(pipe));
    if (!workspace) return 1;
    if (sl_ipc_client_init(&client, memory_read, memory_write, &pipe,
                           workspace, 4096u) != 0) {
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
    free(workspace);
    puts("native IPC client: PASS");
    return 0;
}
