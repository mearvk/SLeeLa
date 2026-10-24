/*
 * ballum_demo.c -- a tiny runnable driver for the Ballum manager.
 *
 * Purpose: give `ballum/` an actual LINKED executable (not just the static
 * archive), so the per-OS display libraries (Linux -lX11 -lXrandr, macOS
 * frameworks, Windows -lgdi32 -luser32) are exercised by a real link at build
 * time -- the verification the .a-only build cannot do. Build via the Makefile:
 *   make demo        (links build/libsleela_ballum.a + $(BACKEND_LIBS))
 *
 * It initializes the manager over the compiled-in OS backend (NULL selects
 * ballum_backend_default()), enumerates displays, and prints what it found.
 * On a headless/unsupported host it reports the backend status honestly rather
 * than faking a display -- the same contract the rest of Ballum keeps.
 */
#include "sleela_ballum/ballum_manager.h"
#include <stdio.h>

int main(void) {
    printf("=== Ballum demo (backend supported=%d) ===\n",
           ballum_backend_supported());

    ballum_manager_t manager;
    ballum_status_t st = ballum_manager_init(&manager, NULL, NULL);
    if (st != BALLUM_STATUS_OK) {
        printf("manager init: status %d (backend unavailable or not initialized)\n", (int)st);
        return 0; /* honest: nothing to drive on this host, not an error */
    }

    ballum_display_t displays[8];
    size_t count = 0;
    st = ballum_manager_enumerate(&manager, displays, 8, &count);
    if (st != BALLUM_STATUS_OK) {
        printf("enumerate: status %d\n", (int)st);
        return 0;
    }

    printf("displays found: %zu\n", count);
    for (size_t i = 0; i < count; i++) {
        printf("  display[%zu]: id=%u %ux%u\n",
               i, displays[i].id, displays[i].width, displays[i].height);
    }
    printf("done\n");
    return 0;
}
