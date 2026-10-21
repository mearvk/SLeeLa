#include "sleela_wifi/wifi_manager.h"
#include <ctype.h>
#include <string.h>

static int bssid_equal_ci(const char *a, const char *b) {
    size_t i;
    if (!a || !b || strlen(a) != strlen(b)) return 0;
    for (i = 0u; a[i]; ++i)
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) return 0;
    return 1;
}
static int is_enterprise(wifi_security_t s) {
    return s == WIFI_SECURITY_WPA_ENTERPRISE || s == WIFI_SECURITY_WPA2_ENTERPRISE ||
           s == WIFI_SECURITY_WPA3_ENTERPRISE;
}
static int valid_ssid(const char *s) {
    size_t i, n;
    if (!s) return 0;
    n = strlen(s);
    if (n == 0u || n > WIFI_SSID_MAX_BYTES) return 0;
    for (i = 0u; i < n; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c < 0x20u || c == 0x7fu) return 0;
    }
    return 1;
}
static int valid_bssid(const char *s) {
    size_t i;
    if (!s || strlen(s) != 17u) return 0;
    for (i = 0u; i < 17u; ++i) {
        if (i == 2u || i == 5u || i == 8u || i == 11u || i == 14u) {
            if (s[i] != ':') return 0;
        } else if (!isxdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}
static int same_network(const wifi_network_t *a, const wifi_network_t *b) {
    return a && b && strcmp(a->ssid, b->ssid) == 0 &&
           bssid_equal_ci(a->bssid, b->bssid) && a->security == b->security;
}
static int valid_network(const wifi_network_t *n) {
    return n && valid_ssid(n->ssid) && valid_bssid(n->bssid) &&
           n->signal_percent <= 100u && n->security != WIFI_SECURITY_UNKNOWN;
}

void wifi_security_policy_default(wifi_security_policy_t *p) {
    if (!p) return;
    p->minimum_security_rank = 4;
    p->allow_open_networks = 0;
    p->allow_legacy_wep = 0;
    p->require_enterprise_tls_validation = 1;
    p->require_protected_management_frames = 0;
}

wifi_status_t wifi_manager_init(wifi_manager_t *m, const wifi_backend_t *b,
                                void *context, const wifi_security_policy_t *policy) {
    wifi_security_policy_t defaults;
    if (!m || !b || !b->scan || !b->connect || !b->disconnect)
        return WIFI_STATUS_INVALID_ARGUMENT;
    memset(m, 0, sizeof(*m));
    wifi_security_policy_default(&defaults);
    m->backend = b; m->backend_context = context;
    m->policy = policy ? *policy : defaults;
    if (m->policy.minimum_security_rank < 0 || m->policy.minimum_security_rank > 5)
        return WIFI_STATUS_INVALID_ARGUMENT;
    m->initialized = 1;
    return WIFI_STATUS_OK;
}

wifi_status_t wifi_manager_scan(wifi_manager_t *m, wifi_network_t *out,
                                size_t capacity, size_t *out_count) {
    size_t n = 0u, i, kept = 0u, copied;
    wifi_status_t s;
    if (!m || !m->initialized || !out_count || (!out && capacity))
        return WIFI_STATUS_INVALID_ARGUMENT;
    *out_count = 0u;
    memset(m->last_scan, 0, sizeof(m->last_scan));
    m->last_scan_count = 0u;
    s = m->backend->scan(m->backend_context, m->last_scan, WIFI_MAX_SCAN_RESULTS, &n);
    if (s != WIFI_STATUS_OK && s != WIFI_STATUS_BUFFER_TOO_SMALL) return s;
    /* A provider can discover more APs than the bounded OOD cache can retain. */
    if (n > WIFI_MAX_SCAN_RESULTS) n = WIFI_MAX_SCAN_RESULTS;
    for (i = 0u; i < n; ++i) {
        if (!valid_network(&m->last_scan[i])) continue;
        if (kept != i) m->last_scan[kept] = m->last_scan[i];
        m->last_scan[kept].ssid[WIFI_SSID_MAX_BYTES] = '\0';
        m->last_scan[kept].bssid[WIFI_BSSID_TEXT_SIZE - 1u] = '\0';
        if (m->last_scan[kept].protected_management_frames < -1 ||
            m->last_scan[kept].protected_management_frames > 1)
            m->last_scan[kept].protected_management_frames = -1;
        ++kept;
    }
    m->last_scan_count = kept;
    ++m->scan_generation;
    copied = capacity < kept ? capacity : kept;
    if (copied && out) memcpy(out, m->last_scan, copied * sizeof(*out));
    *out_count = kept;
    return capacity < kept ? WIFI_STATUS_BUFFER_TOO_SMALL : WIFI_STATUS_OK;
}

wifi_status_t wifi_manager_select(const wifi_manager_t *m, size_t index, wifi_network_t *out) {
    if (!m || !m->initialized || !out) return WIFI_STATUS_INVALID_ARGUMENT;
    if (index >= m->last_scan_count) return WIFI_STATUS_NETWORK_NOT_FOUND;
    *out = m->last_scan[index];
    return WIFI_STATUS_OK;
}

int wifi_security_rank(wifi_security_t s) {
    switch (s) {
        case WIFI_SECURITY_OPEN: return 0;
        case WIFI_SECURITY_WEP: return 1;
        case WIFI_SECURITY_WPA_PERSONAL:
        case WIFI_SECURITY_WPA_ENTERPRISE: return 3;
        case WIFI_SECURITY_WPA2_PERSONAL:
        case WIFI_SECURITY_WPA2_ENTERPRISE:
        case WIFI_SECURITY_OWE: return 4;
        case WIFI_SECURITY_WPA3_PERSONAL:
        case WIFI_SECURITY_WPA3_ENTERPRISE: return 5;
        default: return -1;
    }
}

wifi_status_t wifi_manager_connect(wifi_manager_t *m, const wifi_network_t *selected,
                                   const wifi_auth_t *auth) {
    size_t i;
    int found = 0, rank;
    const wifi_auth_t os_auth = { WIFI_AUTH_OS_MANAGED, NULL, NULL, NULL, NULL, NULL };
    const wifi_auth_t *a = auth ? auth : &os_auth;
    if (!m || !m->initialized || !selected || !valid_network(selected))
        return WIFI_STATUS_INVALID_ARGUMENT;
    for (i = 0u; i < m->last_scan_count; ++i)
        if (same_network(selected, &m->last_scan[i])) { found = 1; break; }
    if (!found) return WIFI_STATUS_NETWORK_NOT_FOUND;

    rank = wifi_security_rank(selected->security);
    if (rank < 0 || rank < m->policy.minimum_security_rank)
        return WIFI_STATUS_POLICY_BLOCKED;
    if (selected->security == WIFI_SECURITY_OPEN && !m->policy.allow_open_networks)
        return WIFI_STATUS_POLICY_BLOCKED;
    if (selected->security == WIFI_SECURITY_WEP && !m->policy.allow_legacy_wep)
        return WIFI_STATUS_POLICY_BLOCKED;
    if (m->policy.require_protected_management_frames &&
        selected->protected_management_frames != 1) return WIFI_STATUS_POLICY_BLOCKED;

    if (a->mode == WIFI_AUTH_PASSPHRASE) {
        size_t len, j;
        int hex64;
        if (!a->password) return WIFI_STATUS_AUTH_REQUIRED;
        len = strlen(a->password); hex64 = len == 64u;
        for (j = 0u; hex64 && j < len; ++j)
            if (!isxdigit((unsigned char)a->password[j])) hex64 = 0;
        if (selected->security == WIFI_SECURITY_WPA3_PERSONAL && hex64)
            return WIFI_STATUS_INVALID_ARGUMENT;
        if ((len < 8u || len > 63u) && !hex64) return WIFI_STATUS_INVALID_ARGUMENT;
    } else if (a->mode == WIFI_AUTH_ENTERPRISE_PASSWORD) {
        if (!is_enterprise(selected->security) || !a->username || !*a->username ||
            !a->password || !*a->password) return WIFI_STATUS_AUTH_REQUIRED;
        if (m->policy.require_enterprise_tls_validation &&
            (!a->ca_certificate_file || !*a->ca_certificate_file ||
             !a->expected_server_name || !*a->expected_server_name))
            return WIFI_STATUS_POLICY_BLOCKED;
    } else if (a->mode == WIFI_AUTH_SAVED_PROFILE) {
        if (!a->profile_name || !*a->profile_name) return WIFI_STATUS_AUTH_REQUIRED;
    } else if (a->mode != WIFI_AUTH_OS_MANAGED) return WIFI_STATUS_INVALID_ARGUMENT;

    return m->backend->connect(m->backend_context, selected, a);
}

wifi_status_t wifi_manager_disconnect(wifi_manager_t *m) {
    if (!m || !m->initialized) return WIFI_STATUS_INVALID_ARGUMENT;
    return m->backend->disconnect(m->backend_context);
}
const char *wifi_security_name(wifi_security_t s) {
    switch (s) {
        case WIFI_SECURITY_OPEN: return "Open";
        case WIFI_SECURITY_WEP: return "WEP (legacy)";
        case WIFI_SECURITY_WPA_PERSONAL: return "WPA-Personal";
        case WIFI_SECURITY_WPA2_PERSONAL: return "WPA2-Personal";
        case WIFI_SECURITY_WPA3_PERSONAL: return "WPA3-Personal";
        case WIFI_SECURITY_OWE: return "Enhanced Open (OWE)";
        case WIFI_SECURITY_WPA_ENTERPRISE: return "WPA-Enterprise";
        case WIFI_SECURITY_WPA2_ENTERPRISE: return "WPA2-Enterprise";
        case WIFI_SECURITY_WPA3_ENTERPRISE: return "WPA3-Enterprise";
        default: return "Unknown";
    }
}
const char *wifi_status_name(wifi_status_t s) {
    switch (s) {
        case WIFI_STATUS_OK: return "ok";
        case WIFI_STATUS_INVALID_ARGUMENT: return "invalid argument";
        case WIFI_STATUS_BACKEND_UNAVAILABLE: return "native Wi-Fi backend unavailable";
        case WIFI_STATUS_BACKEND_ERROR: return "native Wi-Fi operation failed";
        case WIFI_STATUS_BUFFER_TOO_SMALL: return "output buffer too small";
        case WIFI_STATUS_NETWORK_NOT_FOUND: return "network not in latest scan";
        case WIFI_STATUS_POLICY_BLOCKED: return "blocked by Wi-Fi security policy";
        case WIFI_STATUS_AUTH_REQUIRED: return "OS-managed authentication or a valid profile is required";
        case WIFI_STATUS_NOT_CONNECTED: return "no active Wi-Fi connection";
        default: return "unknown status";
    }
}
void wifi_secure_zero(void *memory, size_t length) {
    volatile unsigned char *p = (volatile unsigned char *)memory;
    if (!p) return;
    while (length--) *p++ = 0u;
}
