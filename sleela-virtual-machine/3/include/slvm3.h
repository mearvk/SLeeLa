#ifndef SLEELA_SLVM3_H
#define SLEELA_SLVM3_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM3_VERSION_MAJOR 3
#define SLVM3_VERSION_MINOR 0

typedef enum {
 SLVM3_RUNTIME_AUTO=0, SLVM3_RUNTIME_LINUX=1,
 SLVM3_RUNTIME_WINDOWS=2, SLVM3_RUNTIME_MACOS=3
} slvm3_runtime_target_t;

typedef enum {
 SLVM3_POLICY_ALLOW=0, SLVM3_POLICY_AUDIT=1,
 SLVM3_POLICY_DENY=2
} slvm3_policy_decision_t;

typedef struct {
 uint32_t instance;
 slvm3_runtime_target_t runtime_target;
 uint64_t memory_limit;
 uint32_t security_profile;
 uint32_t crypto_provider;
 uint32_t link_policy;
 uint32_t certificate_policy;
 uint32_t observer_mode;
 uint32_t isolation_profile;
 uint32_t determinism_mode;
 uint32_t attestation_mode;
 uint32_t compliance_profile;
} slvm3_config_t;

int slvm3_config_defaults(slvm3_config_t*);
int slvm3_config_load_file(slvm3_config_t*, const char*);
int slvm3_runtime_validate(const slvm3_config_t*);
#ifdef __cplusplus
}
#endif
#endif
