#ifndef SKYA_POLICY_H
#define SKYA_POLICY_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SKYA_CONFIG_BASIC=1, SKYA_CONFIG_INTERMEDIATE=2, SKYA_CONFIG_ADVANCED=3 } skya_config_level_t;
typedef enum { SKYA_FW_NEVER=0, SKYA_FW_AUTO=1, SKYA_FW_ALWAYS=2 } skya_firewall_mode_t;
typedef struct { skya_config_level_t level; skya_firewall_mode_t firewall; uint8_t nat_mode; uint8_t http_version; uint8_t relay; uint8_t tls_required; uint16_t port; char summary[512]; } skya_policy_t;
int skya_policy_initial(skya_policy_t *out);
const char *skya_policy_level_name(skya_config_level_t level);
const char *skya_policy_firewall_name(skya_firewall_mode_t mode);
#ifdef __cplusplus
}
#endif
#endif
