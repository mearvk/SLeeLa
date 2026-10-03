#ifndef SLEELA_SLVM7_H
#define SLEELA_SLVM7_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLVM7_VERSION_MAJOR 7
#define SLVM7_VERSION_MINOR 0

typedef enum {
    SLVM7_RUNTIME_AUTO = 0,
    SLVM7_RUNTIME_LINUX = 1,
    SLVM7_RUNTIME_WINDOWS = 2,
    SLVM7_RUNTIME_MACOS = 3
} slvm7_runtime_target_t;

typedef enum {
    SLVM7_OK = 0,
    SLVM7_INVALID = -1,
    SLVM7_DENIED = -2,
    SLVM7_STALE = -3,
    SLVM7_CONFLICT = -4,
    SLVM7_RECOVERY_REQUIRED = -5,
    SLVM7_INCOMPATIBLE = -6,
    SLVM7_RESOURCE_PRESSURE = -7,
    SLVM7_QUARANTINED = -8,
    SLVM7_MANAGER_FAILURE = -9,
    SLVM7_INTEGRITY_FAILURE = -10
} slvm7_status_t;

typedef struct {
    uint32_t instance;
    slvm7_runtime_target_t runtime_target;
    uint64_t memory_limit;
    uint32_t security_profile;
    uint32_t observer_mode;
    uint32_t isolation_profile;
    uint32_t determinism_mode;
    uint32_t attestation_mode;
    uint32_t broker_mode;
    uint32_t resolver_mode;
    uint32_t recovery_mode;
    uint32_t migration_mode;
    uint32_t log_manager_mode;
    uint32_t memory_manager_mode;
    uint32_t health_manager_mode;
    uint32_t manager_registry_mode;
    uint64_t policy_version;
    const char *policy_hash;
} slvm7_config_t;

int slvm7_config_defaults(slvm7_config_t *);
int slvm7_config_load_file(slvm7_config_t *, const char *);
int slvm7_runtime_validate(const slvm7_config_t *);
int slvm7_runtime_fault(slvm7_config_t *, slvm7_status_t);

#ifdef __cplusplus
}
#endif

#endif
