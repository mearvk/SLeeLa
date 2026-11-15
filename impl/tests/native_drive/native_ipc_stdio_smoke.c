#include "../../native-drive/native_ipc_stdio.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *pipe_file = tmpfile();
    SLIPCStdioTransport transport;
    static const uint8_t sent[] = {0x53, 0x4c, 0x49, 0x50, 0x00, 0xff, 0x7e};
    uint8_t received[sizeof(sent)];
    if (!pipe_file) {
        fputs("native IPC stdio: tmpfile failed\n", stderr);
        return 1;
    }
    if (sl_ipc_stdio_init(&transport, pipe_file, pipe_file) != 0 ||
        sl_ipc_stdio_write_exact(&transport, sent, sizeof(sent)) != 0 ||
        fseek(pipe_file, 0, SEEK_SET) != 0 ||
        sl_ipc_stdio_read_exact(&transport, received, sizeof(received)) != 0 ||
        memcmp(sent, received, sizeof(sent)) != 0) {
        fputs("native IPC stdio exact-transfer: FAIL\n", stderr);
        fclose(pipe_file);
        return 1;
    }
    if (sl_ipc_stdio_read_exact(&transport, received, 1) == 0) {
        fputs("native IPC stdio truncated-read rejection: FAIL\n", stderr);
        fclose(pipe_file);
        return 1;
    }
    fclose(pipe_file);
    puts("native IPC stdio: PASS");
    return 0;
}
