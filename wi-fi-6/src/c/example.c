#include "wifi_profile.h"

#include <stdio.h>

int main(void) {
    const wifi_profile_t *profile = wifi_profile_get();
    if (profile == NULL) {
        fputs("Wi-Fi reference profile unavailable.\n", stderr);
        return 1;
    }

    wifi_profile_print();
    printf("\n80 MHz profile support: %s\n",
           wifi_profile_supports_width_mhz(80u) ? "yes" : "no");
    puts("Query the operating system/driver for actual adapter capabilities.");
    return 0;
}
