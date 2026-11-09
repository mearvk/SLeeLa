#include "native_ipc_client.h"

#include <stdio.h>
#include <string.h>

static int write_frame(SLIPCClient *c, uint16_t kind, uint64_t request_id,
                       int32_t status, const uint8_t *payload, uint32_t length) {
    uint8_t header[SL_IPC_HEADER_SIZE];
    SLIPCHeader h;
    if (!c || !c->write_exact || length > SL_IPC_MAX_PAYLOAD ||
        (length && !payload)) return -1;
    h.version = SL_IPC_VERSION;
    h.kind = kind;
    h.request_id = request_id;
    h.payload_length = length;
    h.status = status;
    if (sl_ipc_encode_header(&h, header, sizeof(header)) < 0) return -1;
    if (c->write_exact(c->context, header, sizeof(header)) != 0) return -1;
    if (length && c->write_exact(c->context, payload, length) != 0) return -1;
    return 0;
}

static int read_frame(SLIPCClient *c, SLIPCHeader *h) {
    uint8_t header[SL_IPC_HEADER_SIZE];
    if (!c || !c->read_exact || !h ||
        c->read_exact(c->context, header, sizeof(header)) != 0 ||
        sl_ipc_decode_header(header, sizeof(header), h) < 0) return -1;
    if (h->payload_length > c->workspace_capacity) return -1;
    if (h->payload_length &&
        c->read_exact(c->context, c->workspace, h->payload_length) != 0) return -1;
    return 0;
}

int sl_ipc_client_init(SLIPCClient *c, SLIPCReadExactFn read_exact,
                       SLIPCWriteExactFn write_exact, void *context,
                       uint8_t *workspace, size_t workspace_capacity) {
    static const uint8_t hello[] = "sleela-ipc/1";
    SLIPCHeader response;
    if (!c || !read_exact || !write_exact || !workspace ||
        workspace_capacity == 0 || workspace_capacity > SL_IPC_MAX_PAYLOAD) return -1;
    memset(c, 0, sizeof(*c));
    c->read_exact = read_exact;
    c->write_exact = write_exact;
    c->context = context;
    c->workspace = workspace;
    c->workspace_capacity = workspace_capacity;
    if (write_frame(c, SL_IPC_HELLO, 0, 0, hello,
                    (uint32_t)(sizeof(hello) - 1u)) != 0 ||
        read_frame(c, &response) != 0 ||
        response.kind != SL_IPC_HELLO_ACK || response.request_id != 0 ||
        response.status != 0 ||
        response.payload_length != sizeof(hello) - 1u ||
        memcmp(c->workspace, hello, sizeof(hello) - 1u) != 0) {
        memset(c, 0, sizeof(*c));
        return -1;
    }
    c->handshaken = 1;
    c->next_request_id = 1;
    return 0;
}

int sl_ipc_client_call(SLIPCClient *c, const uint8_t *method,
                       uint16_t method_length, const SLNativeValue *arguments,
                       uint16_t argument_count, SLNativeValue *result,
                       char *error, size_t error_capacity) {
    size_t request_length = 0;
    SLIPCHeader response;
    uint64_t request_id;
    if (error && error_capacity) error[0] = '\0';
    if (!c || !c->handshaken || !result || !c->workspace ||
        c->next_request_id == 0 || c->next_request_id == UINT64_MAX) return -1;
    if (sl_ipc_encode_request(method, method_length, arguments, argument_count,
                              c->workspace, c->workspace_capacity,
                              &request_length) != 0) return -1;
    request_id = c->next_request_id++;
    if (write_frame(c, SL_IPC_REQUEST, request_id, 0, c->workspace,
                    (uint32_t)request_length) != 0 ||
        read_frame(c, &response) != 0 ||
        response.kind != SL_IPC_RESPONSE || response.request_id != request_id)
        return -1;
    if (response.status != 0) {
        if (error && error_capacity) {
            size_t n = response.payload_length;
            if (n >= error_capacity) n = error_capacity - 1u;
            if (n) memcpy(error, c->workspace, n);
            error[n] = '\0';
        }
        return -2;
    }
    {
        SLIPCCursor cursor;
        cursor.data = c->workspace;
        cursor.length = response.payload_length;
        cursor.offset = 0;
        if (sl_ipc_decode_value(&cursor, result) != 0 ||
            cursor.offset != cursor.length) return -1;
    }
    return 0;
}

void sl_ipc_client_close(SLIPCClient *c) {
    if (c) memset(c, 0, sizeof(*c));
}
