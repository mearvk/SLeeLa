#ifndef SLEELA_SLVM5_H
#define SLEELA_SLVM5_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM5_VERSION_MAJOR 5
#define SLVM5_VERSION_MINOR 0
typedef enum { SLVM5_RUNTIME_AUTO=0, SLVM5_RUNTIME_LINUX=1, SLVM5_RUNTIME_WINDOWS=2, SLVM5_RUNTIME_MACOS=3 } slvm5_runtime_target_t;
typedef enum { SLVM5_OK=0, SLVM5_INVALID=-1, SLVM5_DENIED=-2, SLVM5_STALE=-3, SLVM5_CONFLICT=-4, SLVM5_RECOVERY_REQUIRED=-5 } slvm5_status_t;
typedef struct {
 uint32_t instance;
 slvm5_runtime_target_t runtime_target;
 uint64_t memory_limit;
 uint32_t security_profile, crypto_provider, link_policy, certificate_policy;
 uint32_t observer_mode, isolation_profile, determinism_mode, attestation_mode;
 uint32_t broker_mode, resolver_mode, recovery_mode, compliance_profile;
 uint64_t policy_version;
 const char *policy_hash;
} slvm5_config_t;
int slvm5_config_defaults(slvm5_config_t*);
int slvm5_config_load_file(slvm5_config_t*, const char*);
int slvm5_runtime_validate(const slvm5_config_t*);
#ifdef __cplusplus
}
#endif
#endif
