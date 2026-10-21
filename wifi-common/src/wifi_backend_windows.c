#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "sleela_wifi/wifi_manager.h"
#include <windows.h>
#include <wlanapi.h>
#include <stdio.h>
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
        WlanCloseHandle(h); return WIFI_STATUS_BACKEND_UNAVAILABLE;
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
    WlanFreeMemory(ifaces); WlanCloseHandle(h);
    *count = total;
    return total > cap ? WIFI_STATUS_BUFFER_TOO_SMALL : WIFI_STATUS_OK;
}
static int utf8_to_wide(const char *s, WCHAR *out, int count) {
    return (s && *s) ? MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, out, count) : 0;
}
static wifi_status_t win_connect(void *ctx, const wifi_network_t *network, const wifi_auth_t *auth) {
    HANDLE h = NULL; DWORD version = 0, rc; PWLAN_INTERFACE_INFO_LIST ifaces = NULL;
    WCHAR profile[256]; WLAN_CONNECTION_PARAMETERS p; WLAN_BSSID_LIST *bl;
    (void)ctx;
    if (!network || !auth) return WIFI_STATUS_INVALID_ARGUMENT;
    if (auth->mode == WIFI_AUTH_PASSPHRASE || auth->mode == WIFI_AUTH_ENTERPRISE_PASSWORD)
        return WIFI_STATUS_AUTH_REQUIRED; /* use Windows' protected Wi-Fi profile/authentication UI */
    if (!utf8_to_wide(auth->mode == WIFI_AUTH_SAVED_PROFILE ? auth->profile_name : network->ssid,
                      profile, (int)(sizeof(profile) / sizeof(profile[0]))))
        return WIFI_STATUS_INVALID_ARGUMENT;
    if (WlanOpenHandle(2u, NULL, &version, &h) != ERROR_SUCCESS) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    if (WlanEnumInterfaces(h, NULL, &ifaces) != ERROR_SUCCESS || !ifaces || !ifaces->dwNumberOfItems) {
        if (ifaces) WlanFreeMemory(ifaces);
        WlanCloseHandle(h); return WIFI_STATUS_BACKEND_UNAVAILABLE;
    }
    bl = (WLAN_BSSID_LIST *)calloc(1u, sizeof(WLAN_BSSID_LIST));
    if (!bl) { WlanFreeMemory(ifaces); WlanCloseHandle(h); return WIFI_STATUS_BACKEND_ERROR; }
    bl->dwTotalSize = (DWORD)sizeof(WLAN_BSSID_LIST); bl->dwNumberOfItems = 1u;
    {
        unsigned int m[6]; char tail;
        if (sscanf(network->bssid, "%x:%x:%x:%x:%x:%x%c",
                   &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &tail) != 6) {
            free(bl); WlanFreeMemory(ifaces); WlanCloseHandle(h); return WIFI_STATUS_INVALID_ARGUMENT;
        }
        for (int x = 0; x < 6; ++x) bl->BSSIDs[0].dot11BSSID[x] = (UCHAR)m[x];
    }
    memset(&p, 0, sizeof(p)); p.wlanConnectionMode = wlan_connection_mode_profile;
    p.strProfile = profile; p.pDesiredBssidList = bl;
    p.dot11BssType = dot11_BSS_type_infrastructure; p.dwFlags = 0u;
    rc = WlanConnect(h, &ifaces->InterfaceInfo[0].InterfaceGuid, &p, NULL);
    free(bl); WlanFreeMemory(ifaces); WlanCloseHandle(h);
    return rc == ERROR_SUCCESS ? WIFI_STATUS_OK : WIFI_STATUS_BACKEND_ERROR;
}
static wifi_status_t win_disconnect(void *ctx) {
    HANDLE h = NULL; DWORD version = 0, rc; PWLAN_INTERFACE_INFO_LIST ifaces = NULL;
    (void)ctx;
    if (WlanOpenHandle(2u, NULL, &version, &h) != ERROR_SUCCESS) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    rc = WlanEnumInterfaces(h, NULL, &ifaces);
    if (rc != ERROR_SUCCESS || !ifaces || !ifaces->dwNumberOfItems) {
        if (ifaces) WlanFreeMemory(ifaces);
        WlanCloseHandle(h); return WIFI_STATUS_NOT_CONNECTED;
    }
    rc = WlanDisconnect(h, &ifaces->InterfaceInfo[0].InterfaceGuid, NULL);
    WlanFreeMemory(ifaces); WlanCloseHandle(h);
    return rc == ERROR_SUCCESS ? WIFI_STATUS_OK : WIFI_STATUS_NOT_CONNECTED;
}
static const wifi_backend_t WINDOWS_BACKEND = {
    "Windows Native Wi-Fi (OS profiles)", win_scan, win_connect, win_disconnect
};
const wifi_backend_t *wifi_backend_default(void) { return &WINDOWS_BACKEND; }
#endif
