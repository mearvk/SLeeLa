#ifndef SLEELA_SLVM4_H
#define SLEELA_SLVM4_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM4_VERSION_MAJOR 4
#define SLVM4_VERSION_MINOR 0

typedef enum { SLVM4_RUNTIME_AUTO=0, SLVM4_RUNTIME_LINUX=1, SLVM4_RUNTIME_WINDOWS=2, SLVM4_RUNTIME_MACOS=3 } slvm4_runtime_target_t;
typedef enum { SLVM4_POLICY_ALLOW=0, SLVM4_POLICY_AUDIT=1, SLVM4_POLICY_DENY=2 } slvm4_policy_decision_t;

typedef struct {
 uint32_t instance;
 slvm4_runtime_target_t runtime_target;
 uint64_t memory_limit;
 uint32_t security_profile;
 uint32_t crypto_provider;
 uint32_t link_policy;
 uint32_t certificate_policy;
 uint32_t observer_mode;
 uint32_t isolation_profile;
 uint32_t determinism_mode;
 uint32_t attestation_mode;
 uint32_t broker_mode;
 uint32_t resolver_mode;
 uint32_t compliance_profile;
} slvm4_config_t;

int slvm4_config_defaults(slvm4_config_t*);
int slvm4_config_load_file(slvm4_config_t*, const char*);
int slvm4_runtime_validate(const slvm4_config_t*);
#ifdef __cplusplus
}
#endif
#endif
