#ifndef SLEELA_NATIVE_IPC_STDIO_H
#define SLEELA_NATIVE_IPC_STDIO_H

#include "native_ipc_client.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Borrowed stdio streams. The caller owns their lifetime and must ensure that
 * input/output are connected to the authorized worker's stdout/stdin pipes.
 * These callbacks are blocking; use an OS-level supervisor for deadlines,
 * cancellation, child termination, and reaping. */
typedef struct {
    FILE *input;
    FILE *output;
} SLIPCStdioTransport;

int sl_ipc_stdio_init(SLIPCStdioTransport *transport, FILE *input,
                      FILE *output);
int sl_ipc_stdio_read_exact(void *context, uint8_t *data, size_t length);
int sl_ipc_stdio_write_exact(void *context, const uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif
#endif
