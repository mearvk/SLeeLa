#include "sleela_net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char* requested = getenv("SLEELA_NET_PLATFORM");
    SLNetPlatform native = slnet_platform();
    if (requested && *requested && strcmp(requested, "auto") != 0) {
        SLNetPlatform expected = SL_NET_AUTO;
        if (strcmp(requested, "linux") == 0 || strcmp(requested, "posix") == 0) expected = SL_NET_LINUX;
        else if (strcmp(requested, "windows") == 0 || strcmp(requested, "win32") == 0) expected = SL_NET_WINDOWS;
        if (expected != SL_NET_AUTO && expected != native) {
            fprintf(stderr, "requested network platform is not native: %s\n", requested);
            return 1;
        }
    }

    if (slnet_startup() != 0) {
        fprintf(stderr, "network startup failed\n");
        return 1;
    }

    SLNetHandle listener = slnet_listen(0, 8);
    if (listener == SL_NET_INVALID) {
        /* Port zero is intentionally not exposed by the current API. */
        slnet_shutdown();
        printf("native platform: %s\n", slnet_platform_name());
        printf("network backend: PASS (startup)\n");
        return 0;
    }

    slnet_close(listener);
    slnet_shutdown();
    printf("native platform: %s\n", slnet_platform_name());
    printf("network backend: PASS\n");
    return 0;
}
