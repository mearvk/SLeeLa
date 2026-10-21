#include "Wifi4Manager.h"

const char *sleela_wifi4_generation(void) {
    return "Wi-Fi 4";
}

wifi_status_t sleela_wifi4_manager_init(wifi_manager_t *manager,
                                           const wifi_security_policy_t *policy) {
    return wifi_manager_init(manager, wifi_backend_default(), 0, policy);
}
