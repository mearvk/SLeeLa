#include "config_location.h"
#include <stdio.h>
#include <string.h>
int main(void) {
    SLEELA_CONFIG_LOCATION location;
    sleela_config_location_defaults(&location);
    if (!sleela_config_location_resolve(&location, "/tmp/SLeeLa", NULL, NULL)) return 1;
    if (strcmp(sleela_config_location_root(&location), "/tmp/SLeeLa/config") != 0) return 2;
    if (!sleela_config_location_resolve(&location, NULL, "/opt/sleela", NULL)) return 3;
    if (strcmp(sleela_config_location_root(&location), "/opt/sleela/Config") != 0) return 4;
    if (!sleela_config_location_resolve(&location, NULL, NULL, "/var/lib/sleela/config")) return 5;
    if (strcmp(sleela_config_location_root(&location), "/var/lib/sleela/config") != 0) return 6;
    if (sleela_config_location_resolve(&location, NULL, NULL, "relative/config")) return 7;
    puts("config location smoke: PASS");
    return 0;
}
