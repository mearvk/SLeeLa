#ifndef SLEELA_WIFI_VM_BRIDGE_H
#define SLEELA_WIFI_VM_BRIDGE_H
#ifdef __cplusplus
extern "C" {
#endif
/* VM-safe facade over wifi-common. Returned strings are borrowed until the
 * next call to the same accessor. Connections use an OS saved profile only;
 * raw credentials are deliberately not accepted by the VM bridge. */
int sleela_wifi_vm_scan(void);
int sleela_wifi_vm_network_count(void);
const char *sleela_wifi_vm_ssid(int index);
const char *sleela_wifi_vm_bssid(int index);
int sleela_wifi_vm_signal(int index);
const char *sleela_wifi_vm_security(int index);
int sleela_wifi_vm_connect(int index, const char *profile_name);
int sleela_wifi_vm_disconnect(void);
const char *sleela_wifi_vm_status_name(int status);
#ifdef __cplusplus
}
#endif
#endif
