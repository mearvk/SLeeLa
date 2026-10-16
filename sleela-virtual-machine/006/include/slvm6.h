#ifndef SLEELA_SLVM6_H
#define SLEELA_SLVM6_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM6_VERSION_MAJOR 6
#define SLVM6_VERSION_MINOR 0
typedef enum { SLVM6_RUNTIME_AUTO=0, SLVM6_RUNTIME_LINUX=1, SLVM6_RUNTIME_WINDOWS=2, SLVM6_RUNTIME_MACOS=3 } slvm6_runtime_target_t;
typedef enum { SLVM6_OK=0, SLVM6_INVALID=-1, SLVM6_DENIED=-2, SLVM6_STALE=-3, SLVM6_CONFLICT=-4, SLVM6_RECOVERY_REQUIRED=-5, SLVM6_INCOMPATIBLE=-6 } slvm6_status_t;
typedef struct { uint32_t instance; slvm6_runtime_target_t runtime_target; uint64_t memory_limit; uint32_t security_profile,crypto_provider,link_policy,certificate_policy; uint32_t observer_mode,isolation_profile,determinism_mode,attestation_mode; uint32_t broker_mode,resolver_mode,recovery_mode,migration_mode,compliance_profile; uint64_t policy_version; const char *policy_hash; } slvm6_config_t;
int slvm6_config_defaults(slvm6_config_t*); int slvm6_config_load_file(slvm6_config_t*,const char*); int slvm6_runtime_validate(const slvm6_config_t*);
#ifdef __cplusplus
}
#endif
#endif
