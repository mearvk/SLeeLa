#include "sleela_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char* requested = getenv("SLEELA_IO_PLATFORM");
    SLIOPlatform host = slio_platform();
    if (requested && *requested && strcmp(requested, "auto") != 0) {
        if ((strcmp(requested, "linux") == 0 || strcmp(requested, "posix") == 0) && host != SL_IO_LINUX) {
            fprintf(stderr, "SLeeLa I/O platform mismatch: requested Linux on Windows\n");
            return 2;
        }
        if ((strcmp(requested, "windows") == 0 || strcmp(requested, "win32") == 0) && host != SL_IO_WINDOWS) {
            fprintf(stderr, "SLeeLa I/O platform mismatch: requested Windows on Linux/POSIX\n");
            return 2;
        }
    }

    SLIOHandle r = SLIO_INVALID_HANDLE, w = SLIO_INVALID_HANDLE;
    if (slio_pipe(&r, &w) != 0) {
        fprintf(stderr, "pipe creation failed on %s\n", slio_platform_name());
        return 1;
    }
    if (slio_write(w, "sleela", 6) != 6) return 1;
    char buf[64];
    if (slio_read(r, buf, sizeof(buf)) != 6 || strcmp(buf, "sleela") != 0) return 1;
    slio_close(r);
    slio_close(w);

    printf("SLeeLa I/O platform: %s\n", slio_platform_name());
    printf("anonymous pipe: PASS\n");
    return 0;
}
