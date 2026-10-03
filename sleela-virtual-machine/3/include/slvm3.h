#ifndef SLEELA_SLVM3_H
#define SLEELA_SLVM3_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM3_VERSION_MAJOR 3
#define SLVM3_VERSION_MINOR 0
typedef enum { SLVM3_RUNTIME_AUTO=0, SLVM3_RUNTIME_LINUX=1, SLVM3_RUNTIME_WINDOWS=2, SLVM3_RUNTIME_MACOS=3 } slvm3_runtime_target_t;
typedef struct {
 uint32_t instance; slvm3_runtime_target_t runtime_target; uint64_t memory_limit;
 uint32_t security_profile, crypto_provider, link_policy, certificate_policy;
 uint32_t observer_mode, isolation_profile, determinism_mode, attestation_mode, compliance_profile;
} slvm3_config_t;
int slvm3_config_defaults(slvm3_config_t*);
int slvm3_config_load_file(slvm3_config_t*, const char*);
int slvm3_runtime_validate(const slvm3_config_t*);
#ifdef __cplusplus
}
#endif
#endif
