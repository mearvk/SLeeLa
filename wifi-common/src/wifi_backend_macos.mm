#import <Foundation/Foundation.h>
#import <CoreWLAN/CoreWLAN.h>
#include "sleela_wifi/wifi_manager.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

static wifi_security_t classify_security(CWNetwork *network) {
    if (!network) return WIFI_SECURITY_UNKNOWN;
    /* Test strongest advertised mode first: an AP may support transition modes. */
    if ([network supportsSecurity:kCWSecurityWPA3Enterprise]) return WIFI_SECURITY_WPA3_ENTERPRISE;
    if ([network supportsSecurity:kCWSecurityWPA3Personal]) return WIFI_SECURITY_WPA3_PERSONAL;
    if ([network supportsSecurity:kCWSecurityOWE]) return WIFI_SECURITY_OWE;
    if ([network supportsSecurity:kCWSecurityWPA2Enterprise]) return WIFI_SECURITY_WPA2_ENTERPRISE;
    if ([network supportsSecurity:kCWSecurityWPAEnterprise]) return WIFI_SECURITY_WPA_ENTERPRISE;
    if ([network supportsSecurity:kCWSecurityWPA2Personal]) return WIFI_SECURITY_WPA2_PERSONAL;
    if ([network supportsSecurity:kCWSecurityWPAPersonalMixed] ||
        [network supportsSecurity:kCWSecurityWPAPersonal]) return WIFI_SECURITY_WPA_PERSONAL;
    if ([network supportsSecurity:kCWSecurityWEP]) return WIFI_SECURITY_WEP;
    if ([network supportsSecurity:kCWSecurityNone]) return WIFI_SECURITY_OPEN;
    return WIFI_SECURITY_UNKNOWN;
}
static CWInterface *wifi_interface(void) { return [[CWWiFiClient sharedWiFiClient] interface]; }
static wifi_status_t mac_scan(void *ctx, wifi_network_t *out, size_t cap, size_t *count) {
    @autoreleasepool {
        NSError *error = nil; CWInterface *iface = wifi_interface();
        NSSet<CWNetwork *> *found; size_t used = 0u; (void)ctx;
        if (!count || (!out && cap)) return WIFI_STATUS_INVALID_ARGUMENT;
        *count = 0u; if (!iface) return WIFI_STATUS_BACKEND_UNAVAILABLE;
        found = [iface scanForNetworksWithName:nil includeHidden:NO error:&error];
        if (!found) return WIFI_STATUS_BACKEND_ERROR;
        for (CWNetwork *ap in found) {
            NSString *ssid = ap.ssid, *bssid = ap.bssid;
            NSData *utf8; wifi_network_t n; long rssi;
            if (!ssid || !bssid) continue;
            utf8 = [ssid dataUsingEncoding:NSUTF8StringEncoding];
            if (!utf8 || [utf8 length] == 0u || [utf8 length] > WIFI_SSID_MAX_BYTES) continue;
            memset(&n, 0, sizeof(n)); memcpy(n.ssid, [utf8 bytes], [utf8 length]);
            n.ssid[[utf8 length]] = '\0';
            if (strlen([bssid UTF8String]) >= sizeof(n.bssid)) continue;
            (void)snprintf(n.bssid, sizeof(n.bssid), "%s", [bssid UTF8String]);
            n.security = classify_security(ap);
            rssi = (long)ap.rssiValue; if (rssi < -90) rssi = -90; if (rssi > -30) rssi = -30;
            n.signal_percent = (unsigned)((rssi + 90) * 100 / 60);
            n.channel = (unsigned)ap.wlanChannel.channelNumber; n.frequency_mhz = 0u;
            n.in_use = iface.ssid && [iface.ssid isEqualToString:ssid] ? 1 : 0;
            n.protected_management_frames = -1;
            if (used < cap) out[used] = n; ++used;
        }
        *count = used; return used > cap ? WIFI_STATUS_BUFFER_TOO_SMALL : WIFI_STATUS_OK;
    }
}
static CWNetwork *find_scanned_network(CWInterface *iface, const wifi_network_t *n) {
    NSError *error = nil;
    NSSet<CWNetwork *> *found = [iface scanForNetworksWithName:nil includeHidden:NO error:&error];
    if (!found) return nil;
    for (CWNetwork *ap in found)
        if (ap.ssid && ap.bssid && strcmp([ap.ssid UTF8String], n->ssid) == 0 &&
            strcasecmp([ap.bssid UTF8String], n->bssid) == 0) return ap;
    return nil;
}
static wifi_status_t mac_connect(void *ctx, const wifi_network_t *network, const wifi_auth_t *auth) {
    @autoreleasepool {
        CWInterface *iface = wifi_interface(); CWNetwork *ap; NSError *error = nil;
        NSString *password = nil; BOOL ok = NO; (void)ctx;
        if (!iface || !network || !auth) return WIFI_STATUS_BACKEND_UNAVAILABLE;
        ap = find_scanned_network(iface, network);
        if (!ap) return WIFI_STATUS_NETWORK_NOT_FOUND;
        if (network->security == WIFI_SECURITY_WPA_ENTERPRISE ||
            network->security == WIFI_SECURITY_WPA2_ENTERPRISE ||
            network->security == WIFI_SECURITY_WPA3_ENTERPRISE) {
            if (auth->mode != WIFI_AUTH_OS_MANAGED && auth->mode != WIFI_AUTH_SAVED_PROFILE)
                return WIFI_STATUS_AUTH_REQUIRED;
            ok = [iface associateToEnterpriseNetwork:ap identity:nil username:nil password:nil error:&error];
        } else {
            if (auth->mode == WIFI_AUTH_ENTERPRISE_PASSWORD) return WIFI_STATUS_AUTH_REQUIRED;
            if (auth->mode == WIFI_AUTH_PASSPHRASE && auth->password)
                password = [[NSString alloc] initWithBytes:auth->password length:strlen(auth->password)
                                                  encoding:NSUTF8StringEncoding];
            if (auth->mode == WIFI_AUTH_SAVED_PROFILE && (!auth->profile_name || !*auth->profile_name))
                return WIFI_STATUS_AUTH_REQUIRED;
            ok = [iface associateToNetwork:ap password:password error:&error];
            password = nil;
        }
        return ok ? WIFI_STATUS_OK : WIFI_STATUS_BACKEND_ERROR;
    }
}
static wifi_status_t mac_disconnect(void *ctx) {
    @autoreleasepool {
        CWInterface *iface = wifi_interface(); (void)ctx;
        if (!iface) return WIFI_STATUS_BACKEND_UNAVAILABLE;
        [iface disassociate]; return WIFI_STATUS_OK;
    }
}
static const wifi_backend_t MAC_BACKEND = {
    "Apple CoreWLAN (Keychain/managed profiles)", mac_scan, mac_connect, mac_disconnect
};
extern "C" const wifi_backend_t *wifi_backend_default(void) { return &MAC_BACKEND; }
