#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "sleela_wifi/wifi_manager.h"
#include <windows.h>
#include <wlanapi.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>

static wifi_security_t classify_auth(DOT11_AUTH_ALGORITHM a, DOT11_CIPHER_ALGORITHM c) {
    if (c == DOT11_CIPHER_ALGO_WEP || c == DOT11_CIPHER_ALGO_WEP104) return WIFI_SECURITY_WEP;
    switch (a) {
        case DOT11_AUTH_ALGO_80211_OPEN: return WIFI_SECURITY_OPEN;
        case DOT11_AUTH_ALGO_WPA: return WIFI_SECURITY_WPA_ENTERPRISE;
        case DOT11_AUTH_ALGO_WPA_PSK: return WIFI_SECURITY_WPA_PERSONAL;
        case DOT11_AUTH_ALGO_RSNA: return WIFI_SECURITY_WPA2_ENTERPRISE;
        case DOT11_AUTH_ALGO_RSNA_PSK: return WIFI_SECURITY_WPA2_PERSONAL;
        default: return WIFI_SECURITY_UNKNOWN;
    }
}
static int utf8_from_bytes(const UCHAR *bytes, ULONG length, char *out, size_t cap) {
    if (!bytes || !out || cap == 0u || length == 0u ||
        length > WIFI_SSID_MAX_BYTES || length >= cap) return 0;
    memcpy(out, bytes, length); out[length] = '\0'; return 1;
}
static wifi_status_t win_scan(void *ctx, wifi_network_t *out, size_t cap, size_t *count) {
    HANDLE h = NULL; DWORD version = 0, rc; PWLAN_INTERFACE_INFO_LIST ifaces = NULL;
    size_t total = 0u;
    (void)ctx;
    if (!count || (!out && cap)) return WIFI_STATUS_INVALID_ARGUMENT;
    *count = 0u;
    rc = WlanOpenHandle(2u, NULL, &version, &h);
    if (rc != ERROR_SUCCESS) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    rc = WlanEnumInterfaces(h, NULL, &ifaces);
    if (rc != ERROR_SUCCESS || !ifaces || !ifaces->dwNumberOfItems) {
        if (ifaces) WlanFreeMemory(ifaces);
        WlanCloseHandle(h, NULL); return WIFI_STATUS_BACKEND_UNAVAILABLE;
    }
    for (DWORD k = 0u; k < ifaces->dwNumberOfItems; ++k) {
        PWLAN_AVAILABLE_NETWORK_LIST nets = NULL;
        PWLAN_BSS_LIST bss = NULL;
        WLAN_INTERFACE_INFO *in = &ifaces->InterfaceInfo[k];
        (void)WlanScan(h, &in->InterfaceGuid, NULL, NULL, NULL);
        Sleep(900);
        rc = WlanGetAvailableNetworkList(h, &in->InterfaceGuid, 0u, NULL, &nets);
        if (rc != ERROR_SUCCESS || !nets) continue;
        (void)WlanGetNetworkBssList(h, &in->InterfaceGuid, NULL, dot11_BSS_type_any, FALSE, NULL, &bss);
        for (DWORD j = 0u; j < nets->dwNumberOfItems; ++j) {
            WLAN_AVAILABLE_NETWORK *a = &nets->Network[j];
            wifi_network_t n;
            if (!a->dot11Ssid.uSSIDLength || a->dot11Ssid.uSSIDLength > WIFI_SSID_MAX_BYTES) continue;
            memset(&n, 0, sizeof(n));
            if (!utf8_from_bytes(a->dot11Ssid.ucSSID, a->dot11Ssid.uSSIDLength, n.ssid, sizeof(n.ssid))) continue;
            n.security = classify_auth(a->dot11DefaultAuthAlgorithm, a->dot11DefaultCipherAlgorithm);
            n.signal_percent = a->wlanSignalQuality > 100u ? 100u : a->wlanSignalQuality;
            n.in_use = (a->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) ? 1 : 0;
            n.protected_management_frames = -1;
            if (bss) {
                for (DWORD b = 0u; b < bss->dwNumberOfItems; ++b) {
                    WLAN_BSS_ENTRY *be = &bss->wlanBssEntries[b];
                    if (be->dot11Ssid.uSSIDLength == a->dot11Ssid.uSSIDLength &&
                        memcmp(be->dot11Ssid.ucSSID, a->dot11Ssid.ucSSID, a->dot11Ssid.uSSIDLength) == 0) {
                        (void)snprintf(n.bssid, sizeof(n.bssid), "%02X:%02X:%02X:%02X:%02X:%02X",
                            be->dot11Bssid[0], be->dot11Bssid[1], be->dot11Bssid[2],
                            be->dot11Bssid[3], be->dot11Bssid[4], be->dot11Bssid[5]);
                        n.frequency_mhz = be->ulChCenterFrequency / 1000u;
                        break;
                    }
                }
            }
            if (n.bssid[0]) { if (total < cap) out[total] = n; ++total; }
        }
        if (bss) WlanFreeMemory(bss);
        WlanFreeMemory(nets);
    }
    WlanFreeMemory(ifaces); WlanCloseHandle(h, NULL);
    *count = total;
    return total > cap ? WIFI_STATUS_BUFFER_TOO_SMALL : WIFI_STATUS_OK;
}
static int utf8_to_wide(const char *s, WCHAR *out, int count) {
    return (s && *s) ? MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, out, count) : 0;
}

static int ssid_to_hex_wide(const char *ssid, WCHAR *out, size_t capacity) {
    static const WCHAR digits[] = L"0123456789ABCDEF";
    size_t i, n;
    if (!ssid || !out) return 0;
    n = strlen(ssid);
    if (n == 0u || n > WIFI_SSID_MAX_BYTES || capacity < n * 2u + 1u) return 0;
    for (i = 0u; i < n; ++i) {
        unsigned char byte = (unsigned char)ssid[i];
        out[i * 2u] = digits[byte >> 4u];
        out[i * 2u + 1u] = digits[byte & 0x0fu];
    }
    out[n * 2u] = L'\0';
    return 1;
}

static int is_hex_psk64(const char *s) {
    size_t i;
    if (!s || strlen(s) != 64u) return 0;
    for (i = 0u; i < 64u; ++i)
        if (!isxdigit((unsigned char)s[i])) return 0;
    return 1;
}

static int xml_escape_wide(const WCHAR *source, WCHAR *out, size_t capacity) {
    size_t used = 0u;
    const WCHAR *p;
    if (!source || !out || capacity == 0u) return 0;
    for (p = source; *p; ++p) {
        const WCHAR *replacement = NULL;
        size_t n;
        switch (*p) {
            case L'&': replacement = L"&amp;"; break;
            case L'<': replacement = L"&lt;"; break;
            case L'>': replacement = L"&gt;"; break;
            case L'"': replacement = L"&quot;"; break;
            case L'\'': replacement = L"&apos;"; break;
            default: break;
        }
        if (replacement) {
            n = wcslen(replacement);
            if (used + n >= capacity) return 0;
            memcpy(out + used, replacement, n * sizeof(WCHAR));
            used += n;
        } else {
            if (used + 1u >= capacity) return 0;
            out[used++] = *p;
        }
    }
    out[used] = L'\0';
    return 1;
}

/* Temporary profile means credentials are not placed in process arguments or
   persisted in the user's saved-profile list. WlanConnect parses this buffer;
   its containing stack buffer is securely wiped immediately after the call. */
static int build_temporary_profile(const wifi_network_t *network, const wifi_auth_t *auth,
                                   WCHAR *xml, size_t xml_capacity) {
    WCHAR ssid_hex[WIFI_SSID_MAX_BYTES * 2u + 1u];
    WCHAR key_wide[128] = {0};
    WCHAR key_escaped[640] = {0};
    WCHAR shared_key[800] = {0};
    const WCHAR *authentication = NULL, *encryption = NULL;
    const WCHAR *key_type = L"passPhrase";
    int needs_key, converted, written = -1;

    if (!network || !auth || !xml ||
        !ssid_to_hex_wide(network->ssid, ssid_hex, sizeof(ssid_hex) / sizeof(ssid_hex[0])))
        return 0;

    switch (network->security) {
        case WIFI_SECURITY_OPEN:
            authentication = L"open"; encryption = L"none"; break;
        case WIFI_SECURITY_WPA_PERSONAL:
            authentication = L"WPAPSK"; encryption = L"AES"; break;
        case WIFI_SECURITY_WPA2_PERSONAL:
            authentication = L"WPA2PSK"; encryption = L"AES"; break;
        case WIFI_SECURITY_WPA3_PERSONAL:
            authentication = L"WPA3SAE"; encryption = L"AES"; break;
        default:
            return 0; /* Enterprise and legacy modes require a managed OS profile. */
    }

    needs_key = auth->mode == WIFI_AUTH_PASSPHRASE;
    if (needs_key) {
        if (network->security == WIFI_SECURITY_OPEN || !auth->password) return 0;
        if (is_hex_psk64(auth->password)) key_type = L"networkKey";
        converted = utf8_to_wide(auth->password, key_wide,
                                 (int)(sizeof(key_wide) / sizeof(key_wide[0])));
        if (!converted || !xml_escape_wide(key_wide, key_escaped,
                                           sizeof(key_escaped) / sizeof(key_escaped[0]))) {
            SecureZeroMemory(key_wide, sizeof(key_wide));
            SecureZeroMemory(key_escaped, sizeof(key_escaped));
            return 0;
        }
        written = swprintf(shared_key, sizeof(shared_key) / sizeof(shared_key[0]),
            L"<sharedKey><keyType>%ls</keyType><protected>false</protected>"
            L"<keyMaterial>%ls</keyMaterial></sharedKey>", key_type, key_escaped);
        SecureZeroMemory(key_wide, sizeof(key_wide));
        SecureZeroMemory(key_escaped, sizeof(key_escaped));
        if (written < 0 || (size_t)written >= sizeof(shared_key) / sizeof(shared_key[0])) {
            SecureZeroMemory(shared_key, sizeof(shared_key));
            return 0;
        }
    } else if (auth->mode != WIFI_AUTH_OS_MANAGED) {
        return 0;
    }

    written = swprintf(xml, xml_capacity,
        L"<?xml version=\"1.0\"?>"
        L"<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">"
        L"<name>SLeeLa-Temporary</name>"
        L"<SSIDConfig><SSID><hex>%ls</hex></SSID></SSIDConfig>"
        L"<connectionType>ESS</connectionType><connectionMode>manual</connectionMode>"
        L"<MSM><security><authEncryption><authentication>%ls</authentication>"
        L"<encryption>%ls</encryption><useOneX>false</useOneX></authEncryption>"
        L"%ls</security></MSM></WLANProfile>",
        ssid_hex, authentication, encryption, shared_key);
    SecureZeroMemory(shared_key, sizeof(shared_key));
    return written >= 0 && (size_t)written < xml_capacity;
}

static int parse_bssid(const char *text, UCHAR address[6]) {
    unsigned int bytes[6];
    char tail;
    int i;
    if (!text || !address ||
        sscanf(text, "%x:%x:%x:%x:%x:%x%c",
               &bytes[0], &bytes[1], &bytes[2], &bytes[3], &bytes[4], &bytes[5], &tail) != 6)
        return 0;
    for (i = 0; i < 6; ++i) {
        if (bytes[i] > 255u) return 0;
        address[i] = (UCHAR)bytes[i];
    }
    return 1;
}

static wifi_status_t win_connect(void *ctx, const wifi_network_t *network, const wifi_auth_t *auth) {
    HANDLE h = NULL;
    DWORD version = 0, rc;
    PWLAN_INTERFACE_INFO_LIST ifaces = NULL;
    WLAN_CONNECTION_PARAMETERS p;
    PDOT11_BSSID_LIST bssid_list = NULL;
    WCHAR profile_name[256] = {0};
    WCHAR profile_xml[2048] = {0};
    LPCWSTR selected_profile = NULL;
    int temporary_profile = 0;
    wifi_status_t result = WIFI_STATUS_BACKEND_ERROR;
    (void)ctx;

    if (!network || !auth) return WIFI_STATUS_INVALID_ARGUMENT;
    if (auth->mode == WIFI_AUTH_ENTERPRISE_PASSWORD)
        return WIFI_STATUS_AUTH_REQUIRED; /* use a trusted Windows 802.1X profile */

    if (auth->mode == WIFI_AUTH_PASSPHRASE) {
        if (!build_temporary_profile(network, auth, profile_xml,
                                     sizeof(profile_xml) / sizeof(profile_xml[0]))) {
            SecureZeroMemory(profile_xml, sizeof(profile_xml));
            return WIFI_STATUS_INVALID_ARGUMENT;
        }
        temporary_profile = 1;
        selected_profile = profile_xml;
    } else if (auth->mode == WIFI_AUTH_OS_MANAGED &&
               (network->security == WIFI_SECURITY_OPEN ||
                network->security == WIFI_SECURITY_WPA_PERSONAL ||
                network->security == WIFI_SECURITY_WPA2_PERSONAL ||
                network->security == WIFI_SECURITY_WPA3_PERSONAL)) {
        if (!build_temporary_profile(network, auth, profile_xml,
                                     sizeof(profile_xml) / sizeof(profile_xml[0]))) {
            SecureZeroMemory(profile_xml, sizeof(profile_xml));
            return WIFI_STATUS_BACKEND_ERROR;
        }
        temporary_profile = 1; /* Windows may display a credential prompt */
        selected_profile = profile_xml;
    } else {
        const char *requested = auth->mode == WIFI_AUTH_SAVED_PROFILE
                              ? auth->profile_name : network->ssid;
        if (!utf8_to_wide(requested, profile_name,
                          (int)(sizeof(profile_name) / sizeof(profile_name[0]))))
            return WIFI_STATUS_INVALID_ARGUMENT;
        selected_profile = profile_name;
    }

    rc = WlanOpenHandle(2u, NULL, &version, &h);
    if (rc != ERROR_SUCCESS) {
        SecureZeroMemory(profile_xml, sizeof(profile_xml));
        return WIFI_STATUS_BACKEND_UNAVAILABLE;
    }
    rc = WlanEnumInterfaces(h, NULL, &ifaces);
    if (rc != ERROR_SUCCESS || !ifaces || !ifaces->dwNumberOfItems) {
        result = WIFI_STATUS_BACKEND_UNAVAILABLE;
        goto cleanup;
    }

    bssid_list = (PDOT11_BSSID_LIST)calloc(1u, sizeof(DOT11_BSSID_LIST));
    if (!bssid_list) { result = WIFI_STATUS_BACKEND_ERROR; goto cleanup; }
    bssid_list->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    bssid_list->Header.Revision = DOT11_BSSID_LIST_REVISION_1;
    bssid_list->Header.Size = (UCHAR)sizeof(DOT11_BSSID_LIST);
    bssid_list->uNumOfEntries = 1u;
    bssid_list->uTotalNumOfEntries = 1u;
    if (!parse_bssid(network->bssid, bssid_list->BSSIDs[0])) {
        result = WIFI_STATUS_INVALID_ARGUMENT;
        goto cleanup;
    }

    memset(&p, 0, sizeof(p));
    p.wlanConnectionMode = temporary_profile
                         ? wlan_connection_mode_temporary_profile
                         : wlan_connection_mode_profile;
    p.strProfile = selected_profile;
    p.pDesiredBssidList = bssid_list;
    p.dot11BssType = dot11_BSS_type_infrastructure;
    p.dwFlags = 0u;
    rc = WlanConnect(h, &ifaces->InterfaceInfo[0].InterfaceGuid, &p, NULL);
    result = rc == ERROR_SUCCESS ? WIFI_STATUS_OK : WIFI_STATUS_BACKEND_ERROR;

cleanup:
    SecureZeroMemory(profile_xml, sizeof(profile_xml));
    if (bssid_list) free(bssid_list);
    if (ifaces) WlanFreeMemory(ifaces);
    if (h) WlanCloseHandle(h, NULL);
    return result;
}

static wifi_status_t win_disconnect(void *ctx) {
    HANDLE h = NULL; DWORD version = 0, rc; PWLAN_INTERFACE_INFO_LIST ifaces = NULL;
    (void)ctx;
    if (WlanOpenHandle(2u, NULL, &version, &h) != ERROR_SUCCESS) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    rc = WlanEnumInterfaces(h, NULL, &ifaces);
    if (rc != ERROR_SUCCESS || !ifaces || !ifaces->dwNumberOfItems) {
        if (ifaces) WlanFreeMemory(ifaces);
        WlanCloseHandle(h, NULL); return WIFI_STATUS_NOT_CONNECTED;
    }
    rc = WlanDisconnect(h, &ifaces->InterfaceInfo[0].InterfaceGuid, NULL);
    WlanFreeMemory(ifaces); WlanCloseHandle(h, NULL);
    return rc == ERROR_SUCCESS ? WIFI_STATUS_OK : WIFI_STATUS_NOT_CONNECTED;
}
static const wifi_backend_t WINDOWS_BACKEND = {
    "Windows Native Wi-Fi (OS profiles)", win_scan, win_connect, win_disconnect
};
const wifi_backend_t *wifi_backend_default(void) { return &WINDOWS_BACKEND; }
#endif
