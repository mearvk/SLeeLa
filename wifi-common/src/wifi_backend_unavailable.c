#include "sleela_wifi/wifi_manager.h"
static wifi_status_t no_scan(void *c, wifi_network_t *o, size_t z, size_t *n) {
    (void)c; (void)o; (void)z; if (n) *n = 0u; return WIFI_STATUS_BACKEND_UNAVAILABLE;
}
static wifi_status_t no_connect(void *c, const wifi_network_t *n, const wifi_auth_t *a) {
    (void)c; (void)n; (void)a; return WIFI_STATUS_BACKEND_UNAVAILABLE;
}
static wifi_status_t no_disconnect(void *c) { (void)c; return WIFI_STATUS_BACKEND_UNAVAILABLE; }
static const wifi_backend_t UNAVAILABLE = {
    "no native Wi-Fi provider compiled", no_scan, no_connect, no_disconnect
};
const wifi_backend_t *wifi_backend_default(void) { return &UNAVAILABLE; }
