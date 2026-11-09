#include "native_ipc_stdio.h"

int sl_ipc_stdio_init(SLIPCStdioTransport *transport, FILE *input,
                      FILE *output) {
    if (!transport || !input || !output) return -1;
    transport->input = input;
    transport->output = output;
    return 0;
}

int sl_ipc_stdio_read_exact(void *context, uint8_t *data, size_t length) {
    SLIPCStdioTransport *transport = (SLIPCStdioTransport *)context;
    size_t offset = 0;
    if (!transport || !transport->input || (!data && length)) return -1;
    while (offset < length) {
        size_t count = fread(data + offset, 1, length - offset,
                             transport->input);
        if (count == 0) return -1;
        offset += count;
    }
    return 0;
}

int sl_ipc_stdio_write_exact(void *context, const uint8_t *data,
                             size_t length) {
    SLIPCStdioTransport *transport = (SLIPCStdioTransport *)context;
    size_t offset = 0;
    if (!transport || !transport->output || (!data && length)) return -1;
    while (offset < length) {
        size_t count = fwrite(data + offset, 1, length - offset,
                              transport->output);
        if (count == 0) return -1;
        offset += count;
    }
    return fflush(transport->output) == 0 ? 0 : -1;
}
