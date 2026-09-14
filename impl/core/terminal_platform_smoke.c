#include "sleela_terminal.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    SLTerminalHandle terminal = 0;
    const char* command = "printf terminal-ok";
    char buffer[128];
    SLTerminalCount n;
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
    memset(buffer, 0, sizeof(buffer));
    n = slterminal_read(terminal, buffer, sizeof(buffer) - 1);
    if (n <= 0 || strstr(buffer, "terminal-ok") == NULL) {
        fprintf(stderr, "terminal read failed: n=%td output=%s\n", n, buffer);
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
