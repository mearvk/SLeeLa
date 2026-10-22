#include "../../http-1.0/http1_server.h"
#include <stddef.h>
#include <stdint.h>

/* Fuzz the native request-header parser and resumable-download metadata parser.
   Inputs are bounded and the harness performs no filesystem or network I/O. */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size > 2047) return 0;
    char input[2048];
    for (size_t i = 0; i < size; ++i) input[i] = (char)data[i];
    input[size] = 0;

    http1_request request;
    size_t header_bytes = 0;
    (void)http1_request_parse_headers(input, size, &request, &header_bytes);

    http1_download_state state;
    if (http1_download_parse(input, &state) == 0) {
        char output[512];
        (void)http1_download_serialise(&state, output, sizeof(output));
    }
    return 0;
}
