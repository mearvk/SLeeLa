#ifndef SLEELA_WIFI_4_NATIVE_MANAGER_H
#define SLEELA_WIFI_4_NATIVE_MANAGER_H
#include "../../../../../wifi-common/include/sleela_wifi/wifi_manager.h"
#ifdef __cplusplus
extern "C" {
#endif
const char *sleela_wifi4_generation(void);
wifi_status_t sleela_wifi4_manager_init(wifi_manager_t *manager,
                                           const wifi_security_policy_t *policy);
#ifdef __cplusplus
}
#endif
#endif
