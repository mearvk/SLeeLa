#include "Wifi6Manager.h"

const char *sleela_wifi6_generation(void) {
    return "Wi-Fi 6";
}

wifi_status_t sleela_wifi6_manager_init(wifi_manager_t *manager,
                                           const wifi_security_policy_t *policy) {
    return wifi_manager_init(manager, wifi_backend_default(), 0, policy);
}
