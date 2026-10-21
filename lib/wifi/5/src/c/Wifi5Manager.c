#include "Wifi5Manager.h"

const char *sleela_wifi5_generation(void) {
    return "Wi-Fi 5";
}

wifi_status_t sleela_wifi5_manager_init(wifi_manager_t *manager,
                                           const wifi_security_policy_t *policy) {
    return wifi_manager_init(manager, wifi_backend_default(), 0, policy);
}
