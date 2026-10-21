#include "wifi_manager.h"
#include "wifi_profile.h"
const char *sleela_wifi_4_generation(void) { const wifi_profile_t *p = wifi_profile_get(); return p ? p->generation : "Wi-Fi 4"; }
