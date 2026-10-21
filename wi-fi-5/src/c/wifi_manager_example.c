#include "wifi_manager.h"
#include <stdio.h>
int main(void) {
    wifi_manager_t m; wifi_network_t nets[WIFI_MAX_SCAN_RESULTS], selected;
    wifi_security_policy_t policy; wifi_auth_t auth = { WIFI_AUTH_OS_MANAGED, NULL, NULL, NULL, NULL, NULL };
    size_t count = 0u, i, chosen = 0u; wifi_status_t s;
    wifi_security_policy_default(&policy);
    s = wifi_manager_init(&m, wifi_backend_default(), NULL, &policy);
    if (s != WIFI_STATUS_OK) { fprintf(stderr, "%s\n", wifi_status_name(s)); return 1; }
    s = wifi_manager_scan(&m, nets, WIFI_MAX_SCAN_RESULTS, &count);
    if (s != WIFI_STATUS_OK && s != WIFI_STATUS_BUFFER_TOO_SMALL) { fprintf(stderr, "%s\n", wifi_status_name(s)); return 2; }
    for (i = 0u; i < count && i < WIFI_MAX_SCAN_RESULTS; ++i)
        printf("%zu) %-32s %-20s %u%% ch=%u %s\n", i, nets[i].ssid, wifi_security_name(nets[i].security),
               nets[i].signal_percent, nets[i].channel, nets[i].in_use ? "[connected]" : "");
    printf("Choose network index: "); if (scanf("%zu", &chosen) != 1 || chosen >= count) return 0;
    s = wifi_manager_select(&m, chosen, &selected); if (s != WIFI_STATUS_OK) return 3;
    s = wifi_manager_connect(&m, &selected, &auth);
    if (s != WIFI_STATUS_OK) { fprintf(stderr, "Connect: %s\n", wifi_status_name(s)); return 4; }
    puts("Connection request accepted by native Wi-Fi provider.");
    return 0;
}
