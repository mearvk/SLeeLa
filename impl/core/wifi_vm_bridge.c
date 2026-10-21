#include "wifi_vm_bridge.h"
#include "../../wifi-common/include/sleela_wifi/wifi_manager.h"
#include <string.h>
static wifi_manager_t g_manager;
static wifi_network_t g_networks[WIFI_MAX_SCAN_RESULTS];
static size_t g_count;
static int g_initialized;
static char g_ssid[WIFI_SSID_MAX_BYTES + 1u];
static char g_bssid[WIFI_BSSID_TEXT_SIZE];
static int ensure_manager(void) {
    wifi_status_t status;
    if (g_initialized) return WIFI_STATUS_OK;
    status = wifi_manager_init(&g_manager, wifi_backend_default(), NULL, NULL);
    if (status == WIFI_STATUS_OK) g_initialized = 1;
    return (int)status;
}
int sleela_wifi_vm_scan(void) {
    size_t count = 0u;
    int init = ensure_manager();
    wifi_status_t status;
    if (init != WIFI_STATUS_OK) return init;
    status = wifi_manager_scan(&g_manager, g_networks, WIFI_MAX_SCAN_RESULTS, &count);
    if (status == WIFI_STATUS_OK || status == WIFI_STATUS_BUFFER_TOO_SMALL)
        g_count = count > WIFI_MAX_SCAN_RESULTS ? WIFI_MAX_SCAN_RESULTS : count;
    else g_count = 0u;
    return (int)status;
}
int sleela_wifi_vm_network_count(void) { return (int)g_count; }
static const wifi_network_t *network_at(int index) {
    return index >= 0 && (size_t)index < g_count ? &g_networks[index] : NULL;
}
const char *sleela_wifi_vm_ssid(int index) {
    const wifi_network_t *n = network_at(index);
    if (!n) return "";
    memcpy(g_ssid, n->ssid, sizeof(g_ssid)); g_ssid[sizeof(g_ssid)-1u] = '\0';
    return g_ssid;
}
const char *sleela_wifi_vm_bssid(int index) {
    const wifi_network_t *n = network_at(index);
    if (!n) return "";
    memcpy(g_bssid, n->bssid, sizeof(g_bssid)); g_bssid[sizeof(g_bssid)-1u] = '\0';
    return g_bssid;
}
int sleela_wifi_vm_signal(int index) {
    const wifi_network_t *n = network_at(index);
    return n ? (int)n->signal_percent : -1;
}
const char *sleela_wifi_vm_security(int index) {
    const wifi_network_t *n = network_at(index);
    return n ? wifi_security_name(n->security) : "";
}
int sleela_wifi_vm_connect(int index, const char *profile_name) {
    wifi_network_t selected;
    wifi_auth_t auth;
    int init = ensure_manager();
    if (init != WIFI_STATUS_OK) return init;
    if (!profile_name || !*profile_name) return WIFI_STATUS_AUTH_REQUIRED;
    if (index < 0 || wifi_manager_select(&g_manager, (size_t)index, &selected) != WIFI_STATUS_OK)
        return WIFI_STATUS_NETWORK_NOT_FOUND;
    memset(&auth, 0, sizeof(auth));
    auth.mode = WIFI_AUTH_SAVED_PROFILE;
    auth.profile_name = profile_name;
    return (int)wifi_manager_connect(&g_manager, &selected, &auth);
}
int sleela_wifi_vm_disconnect(void) {
    int init = ensure_manager();
    return init == WIFI_STATUS_OK ? (int)wifi_manager_disconnect(&g_manager) : init;
}
const char *sleela_wifi_vm_status_name(int status) {
    return wifi_status_name((wifi_status_t)status);
}
