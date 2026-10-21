#include "sleela_wifi/wifi_manager.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static int connect_calls;
static wifi_security_t fake_security = WIFI_SECURITY_WPA2_PERSONAL;
static wifi_status_t fake_scan(void *ctx, wifi_network_t *out, size_t cap, size_t *count) {
    wifi_network_t n; (void)ctx; memset(&n, 0, sizeof(n));
    (void)snprintf(n.ssid, sizeof(n.ssid), "%s", "SLeeLa-Test");
    (void)snprintf(n.bssid, sizeof(n.bssid), "%s", "02:11:22:33:44:55");
    n.channel = 36; n.signal_percent = 78; n.security = fake_security;
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

    {
        wifi_manager_t open_manager;
        wifi_security_policy_t open_policy;
        wifi_network_t open_network;
        wifi_auth_t bad_auth = { WIFI_AUTH_PASSPHRASE, NULL, NULL, "goodpassphrase", NULL, NULL };
        fake_security = WIFI_SECURITY_OPEN;
        wifi_security_policy_default(&open_policy);
        open_policy.minimum_security_rank = 0;
        open_policy.allow_open_networks = 1;
        assert(wifi_manager_init(&open_manager, &FAKE, NULL, &open_policy) == WIFI_STATUS_OK);
        assert(wifi_manager_scan(&open_manager, out, 1u, &count) == WIFI_STATUS_OK);
        assert(wifi_manager_select(&open_manager, 0u, &open_network) == WIFI_STATUS_OK);
        assert(wifi_manager_connect(&open_manager, &open_network, &bad_auth) == WIFI_STATUS_INVALID_ARGUMENT);
        fake_security = WIFI_SECURITY_WPA2_PERSONAL;
    }
    puts("wifi manager contract tests: PASS");
    return 0;
}
