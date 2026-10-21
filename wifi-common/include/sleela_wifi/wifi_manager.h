#ifndef SLEELA_WIFI_MANAGER_H
#define SLEELA_WIFI_MANAGER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID_MAX_BYTES 32u
#define WIFI_BSSID_TEXT_SIZE 18u
#define WIFI_MAX_SCAN_RESULTS 128u

typedef enum {
    WIFI_STATUS_OK = 0, WIFI_STATUS_INVALID_ARGUMENT,
    WIFI_STATUS_BACKEND_UNAVAILABLE, WIFI_STATUS_BACKEND_ERROR,
    WIFI_STATUS_BUFFER_TOO_SMALL, WIFI_STATUS_NETWORK_NOT_FOUND,
    WIFI_STATUS_POLICY_BLOCKED, WIFI_STATUS_AUTH_REQUIRED,
    WIFI_STATUS_NOT_CONNECTED
} wifi_status_t;

typedef enum {
    WIFI_SECURITY_UNKNOWN = 0, WIFI_SECURITY_OPEN, WIFI_SECURITY_WEP,
    WIFI_SECURITY_WPA_PERSONAL, WIFI_SECURITY_WPA2_PERSONAL,
    WIFI_SECURITY_WPA3_PERSONAL, WIFI_SECURITY_OWE,
    WIFI_SECURITY_WPA_ENTERPRISE, WIFI_SECURITY_WPA2_ENTERPRISE,
    WIFI_SECURITY_WPA3_ENTERPRISE
} wifi_security_t;

typedef enum {
    WIFI_AUTH_OS_MANAGED = 0, WIFI_AUTH_SAVED_PROFILE,
    WIFI_AUTH_PASSPHRASE, WIFI_AUTH_ENTERPRISE_PASSWORD
} wifi_auth_mode_t;

typedef struct {
    char ssid[WIFI_SSID_MAX_BYTES + 1u];
    char bssid[WIFI_BSSID_TEXT_SIZE];
    unsigned channel;
    unsigned frequency_mhz;
    unsigned signal_percent;
    wifi_security_t security;
    int in_use;
    int protected_management_frames; /* 1=yes, 0=no, -1=unknown */
} wifi_network_t;

typedef struct {
    wifi_auth_mode_t mode;
    const char *profile_name;
    const char *username;
    const char *password; /* never log; caller owns and clears it */
    const char *ca_certificate_file;
    const char *expected_server_name;
} wifi_auth_t;

typedef struct {
    int minimum_security_rank; /* default 4: WPA2/OWE or stronger */
    int allow_open_networks;
    int allow_legacy_wep;
    int require_enterprise_tls_validation;
    int require_protected_management_frames;
} wifi_security_policy_t;

typedef struct wifi_backend {
    const char *name;
    wifi_status_t (*scan)(void *context, wifi_network_t *out, size_t capacity, size_t *out_count);
    wifi_status_t (*connect)(void *context, const wifi_network_t *network, const wifi_auth_t *auth);
    wifi_status_t (*disconnect)(void *context);
} wifi_backend_t;

typedef struct wifi_manager {
    const wifi_backend_t *backend;
    void *backend_context;
    wifi_security_policy_t policy;
    wifi_network_t last_scan[WIFI_MAX_SCAN_RESULTS];
    size_t last_scan_count;
    uint64_t scan_generation;
    int initialized;
} wifi_manager_t;

const wifi_backend_t *wifi_backend_default(void);
void wifi_security_policy_default(wifi_security_policy_t *policy);
wifi_status_t wifi_manager_init(wifi_manager_t *manager, const wifi_backend_t *backend,
                                void *backend_context, const wifi_security_policy_t *policy);
wifi_status_t wifi_manager_scan(wifi_manager_t *manager, wifi_network_t *out,
                                size_t capacity, size_t *out_count);
wifi_status_t wifi_manager_select(const wifi_manager_t *manager, size_t index, wifi_network_t *out);
wifi_status_t wifi_manager_connect(wifi_manager_t *manager, const wifi_network_t *selected,
                                   const wifi_auth_t *auth);
wifi_status_t wifi_manager_disconnect(wifi_manager_t *manager);
int wifi_security_rank(wifi_security_t security);
const char *wifi_security_name(wifi_security_t security);
const char *wifi_status_name(wifi_status_t status);
void wifi_secure_zero(void *memory, size_t length);

#ifdef __cplusplus
}
#endif
#endif
