#include "wifi_profile.h"

#include <stdio.h>

int main(void) {
    const wifi_profile_t *profile = wifi_profile_get();
    wifi_profile_print();
    printf("\nReference check: 40 MHz channel %s by this generation profile.\n",
           wifi_profile_supports_width_mhz(40u) ? "is listed as supported" : "is not listed as supported");
    printf("Note: the connected adapter's real capabilities must be queried from the OS/driver.\n");
    return profile == NULL ? 1 : 0;
}
