#include "sleela_wifi/wifi_manager.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static int connect_calls;
static wifi_status_t fake_scan(void *ctx, wifi_network_t *out, size_t cap, size_t *count) {
    wifi_network_t n; (void)ctx; memset(&n, 0, sizeof(n));
    (void)snprintf(n.ssid, sizeof(n.ssid), "%s", "SLeeLa-Test");
    (void)snprintf(n.bssid, sizeof(n.bssid), "%s", "02:11:22:33:44:55");
    n.channel = 36; n.signal_percent = 78; n.security = WIFI_SECURITY_WPA2_PERSONAL;
    n.protected_management_frames = 1; if (cap) out[0] = n; *count = 1u;
    return cap ? WIFI_STATUS_OK : WIFI_STATUS_BUFFER_TOO_SMALL;
}
static wifi_status_t fake_connect(void *ctx, const wifi_network_t *n, const wifi_auth_t *a) {
    (void)ctx; (void)n; (void)a; ++connect_calls; return WIFI_STATUS_OK;
}
static wifi_status_t fake_disconnect(void *ctx) { (void)ctx; return WIFI_STATUS_OK; }
static const wifi_backend_t FAKE = { "test", fake_scan, fake_connect, fake_disconnect };
int main(void) {
    wifi_manager_t m; wifi_network_t networks[1], selected, forged;
    wifi_security_policy_t policy; wifi_auth_t auth = { WIFI_AUTH_OS_MANAGED, NULL, NULL, NULL, NULL, NULL };
    size_t count = 0u; wifi_security_policy_default(&policy);
    assert(wifi_manager_init(&m, &FAKE, NULL, &policy) == WIFI_STATUS_OK);
    assert(wifi_manager_scan(&m, networks, 1u, &count) == WIFI_STATUS_OK && count == 1u);
    assert(wifi_manager_select(&m, 0u, &selected) == WIFI_STATUS_OK);
    assert(wifi_manager_connect(&m, &selected, &auth) == WIFI_STATUS_OK && connect_calls == 1);
    forged = selected; (void)snprintf(forged.bssid, sizeof(forged.bssid), "%s", "02:AA:BB:CC:DD:EE");
    assert(wifi_manager_connect(&m, &forged, &auth) == WIFI_STATUS_NETWORK_NOT_FOUND);
    assert(wifi_manager_disconnect(&m) == WIFI_STATUS_OK);
    puts("wifi manager contract tests: PASS");
    return 0;
}
