#ifndef SLEELA_NATIVE_IPC_CLIENT_H
#define SLEELA_NATIVE_IPC_CLIENT_H

#include "native_ipc_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exact-transfer callbacks: return 0 only after transferring every requested
 * byte; return nonzero on EOF, timeout, cancellation, or transport failure.
 * The callbacks own the OS-specific pipe/socket implementation. */
typedef int (*SLIPCReadExactFn)(void *context, uint8_t *data, size_t length);
typedef int (*SLIPCWriteExactFn)(void *context, const uint8_t *data, size_t length);

typedef struct {
    SLIPCReadExactFn read_exact;
    SLIPCWriteExactFn write_exact;
    void *context;
    uint8_t *workspace;
    size_t workspace_capacity;
    uint64_t next_request_id;
    int handshaken;
} SLIPCClient;

/* workspace is caller-sized (1..SL_IPC_MAX_PAYLOAD bytes). The client borrows it
 * until close; decoded UTF-8 response spans point into this workspace and are
 * valid only until the next call. The caller serializes calls per client. */
int sl_ipc_client_init(SLIPCClient *client, SLIPCReadExactFn read_exact,
                       SLIPCWriteExactFn write_exact, void *context,
                       uint8_t *workspace, size_t workspace_capacity);
int sl_ipc_client_call(SLIPCClient *client, const uint8_t *method,
                       uint16_t method_length, const SLNativeValue *arguments,
                       uint16_t argument_count, SLNativeValue *result,
                       char *error, size_t error_capacity);
void sl_ipc_client_close(SLIPCClient *client);

#ifdef __cplusplus
}
#endif
#endif
