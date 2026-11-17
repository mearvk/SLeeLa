#include "sleela_terminal.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

typedef struct {
    SLTerminalHandle terminal;
    char buffer[4096];
    size_t used;
    int found;
    int read_error;
} SLTerminalReadContext;

static int terminal_contains_marker(const char* buffer, size_t length) {
    static const char marker[] = "terminal-ok";
    size_t i;
    if (length < sizeof(marker) - 1) return 0;
    for (i = 0; i <= length - (sizeof(marker) - 1); ++i) {
        size_t j;
        for (j = 0; j < sizeof(marker) - 1; ++j) {
            if (buffer[i + j] != marker[j]) break;
        }
        if (j == sizeof(marker) - 1) return 1;
    }
    return 0;
}

/* Keep the synchronous ConPTY read off the test's control thread so a missing
 * output frame becomes a bounded, diagnosable test failure instead of a CI
 * timeout. The worker uses only the terminal API and Win32 calls. */
static DWORD WINAPI terminal_read_worker(LPVOID argument) {
    SLTerminalReadContext* context = (SLTerminalReadContext*)argument;
    unsigned empty_reads = 0;

    while (context->used < sizeof(context->buffer) - 1) {
        SLTerminalCount n = slterminal_read(
            context->terminal,
            context->buffer + context->used,
            sizeof(context->buffer) - 1 - context->used);
        if (n < 0) {
            context->read_error = 1;
            return 1;
        }
        if (n == 0) {
            if (++empty_reads >= 200) {
                context->read_error = 2;
                return 2;
            }
            Sleep(10);
            continue;
        }
        context->used += (size_t)n;
        context->buffer[context->used] = '\0';
        if (terminal_contains_marker(context->buffer, context->used)) {
            context->found = 1;
            return 0;
        }
    }
    context->read_error = 3;
    return 3;
}
#else
#include <time.h>
static void terminal_test_pause(void) {
    const struct timespec delay = { 0, 10 * 1000 * 1000 };
    (void)nanosleep(&delay, NULL);
}
#endif

int main(void) {
    SLTerminalHandle terminal = 0;
#ifdef _WIN32
    /* Explicitly exit the shell after writing its marker. */
    const char* command = "cmd.exe /D /C \"echo terminal-ok & exit 0\"";
    SLTerminalReadContext read_context;
    HANDLE reader;
    DWORD wait_result;
#else
    /* Keep the slave open briefly after writing. On Darwin, an immediately
     * exiting PTY child can race the master read and expose EOF before output
     * is observed by this smoke test. */
    const char* command = "printf 'terminal-ok\\n'; sleep 1";
    char buffer[4096];
    size_t used = 0;
    unsigned empty_reads = 0;
    int found = 0;
#endif
    int rc;

    rc = slterminal_spawn(&terminal, command, 80, 24);
    if (rc != 0 || !slterminal_valid(terminal)) {
        fprintf(stderr, "terminal spawn failed: %d\\n", rc);
        return 1;
    }
    if (slterminal_resize(terminal, 100, 30) != 0) {
        fprintf(stderr, "terminal resize failed\\n");
        slterminal_close(terminal);
        return 1;
    }

#ifdef _WIN32
    memset(&read_context, 0, sizeof(read_context));
    read_context.terminal = terminal;
    printf("terminal read: begin (10-second deadline)\\n");
    fflush(stdout);
    reader = CreateThread(NULL, 0, terminal_read_worker, &read_context, 0, NULL);
    if (!reader) {
        fprintf(stderr, "terminal reader thread creation failed: %lu\\n",
                (unsigned long)GetLastError());
        slterminal_close(terminal);
        return 1;
    }
    wait_result = WaitForSingleObject(reader, 10000);
    if (wait_result == WAIT_TIMEOUT) {
        fprintf(stderr, "terminal read timed out after 10 seconds\\n");
        fflush(stderr);
        (void)CancelSynchronousIo(reader);
        wait_result = WaitForSingleObject(reader, 2000);
    }
    if (wait_result != WAIT_OBJECT_0) {
        fprintf(stderr, "terminal reader did not stop after cancellation (wait=%lu)\\n",
                (unsigned long)wait_result);
        CloseHandle(reader);
        return 1;
    }
    CloseHandle(reader);
    if (!read_context.found) {
        fprintf(stderr, "terminal read failed: error=%d bytes=%zu output=%s\\n",
                read_context.read_error, read_context.used, read_context.buffer);
        slterminal_close(terminal);
        return 1;
    }
    printf("terminal read: marker found (%zu bytes)\\n", read_context.used);
    fflush(stdout);
#else
    /* PTYs may emit terminal initialization sequences before the command
     * output. Some Darwin PTY masters briefly report zero bytes while the
     * child is attaching its slave; retry those transient empty reads. */
    memset(buffer, 0, sizeof(buffer));
    while (used < sizeof(buffer) - 1) {
        SLTerminalCount n = slterminal_read(terminal, buffer + used,
                                             sizeof(buffer) - 1 - used);
        if (n < 0) break;
        if (n == 0) {
            if (++empty_reads >= 200) break;
            terminal_test_pause();
            continue;
        }
        used += (size_t)n;
        buffer[used] = '\0';
        if (strstr(buffer, "terminal-ok") != NULL) {
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "terminal read failed: bytes=%zu output=%s\\n", used, buffer);
        slterminal_close(terminal);
        return 1;
    }
#endif

    /* Leave any trailing VT/final-frame bytes unread so slterminal_close must
     * drain them concurrently with ClosePseudoConsole rather than waiting on EOF. */
    printf("terminal close: begin\\n");
    fflush(stdout);
    rc = slterminal_close(terminal);
    printf("terminal close: end (rc=%d)\\n", rc);
    fflush(stdout);
    if (rc != 0) {
        fprintf(stderr, "terminal close failed: %d\\n", rc);
        return 1;
    }
    printf("terminal platform: %s\\n", slterminal_platform_name());
    printf("terminal platform smoke: PASS\\n");
    return 0;
}
