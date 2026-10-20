#include "sleela_terminal.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    SLTerminalHandle terminal = 0;
#ifdef _WIN32
    /* ConPTY accepts a Windows command line. The larger buffer below allows
     * initial VT-mode sequences to arrive before the command's visible output. */
    const char* command = "cmd.exe /C echo terminal-ok";
#else
    /* Keep the slave open briefly after writing. On Darwin, an immediately
     * exiting PTY child can race the master read and expose EOF before output
     * is observed by this smoke test. */
    const char* command = "printf 'terminal-ok\\n'; sleep 1";
#endif
    char buffer[4096];
    size_t used = 0;
    int found = 0;
    int rc;

    rc = slterminal_spawn(&terminal, command, 80, 24);
    if (rc != 0 || !slterminal_valid(terminal)) {
        fprintf(stderr, "terminal spawn failed: %d\n", rc);
        return 1;
    }
    if (slterminal_resize(terminal, 100, 30) != 0) {
        fprintf(stderr, "terminal resize failed\n");
        slterminal_close(terminal);
        return 1;
    }

    /* PTYs may emit terminal initialization sequences before the command
     * output. Read incrementally until the marker appears or the child exits. */
    memset(buffer, 0, sizeof(buffer));
    while (used < sizeof(buffer) - 1) {
        SLTerminalCount n = slterminal_read(terminal, buffer + used,
                                             sizeof(buffer) - 1 - used);
        if (n <= 0) break;
        used += (size_t)n;
        buffer[used] = '\0';
        if (strstr(buffer, "terminal-ok") != NULL) {
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "terminal read failed: bytes=%zu output=%s\n", used, buffer);
        slterminal_close(terminal);
        return 1;
    }
    if (slterminal_close(terminal) != 0) {
        fprintf(stderr, "terminal close failed\n");
        return 1;
    }
    printf("terminal platform: %s\n", slterminal_platform_name());
    printf("terminal platform smoke: PASS\n");
    return 0;
}
