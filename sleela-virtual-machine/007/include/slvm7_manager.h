#ifndef SLEELA_SLVM7_MANAGER_H
#define SLEELA_SLVM7_MANAGER_H

#include <stdint.h>
#include "slvm7.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLVM7_MANAGER_LOG = 1,
    SLVM7_MANAGER_MEMORY = 2,
    SLVM7_MANAGER_HEALTH = 3,
    SLVM7_MANAGER_RECOVERY = 4,
    SLVM7_MANAGER_CHECKPOINT = 5,
    SLVM7_MANAGER_RESOURCE = 6,
    SLVM7_MANAGER_ATTESTATION = 7,
    SLVM7_MANAGER_LINEAGE = 8
} slvm7_manager_kind_t;

typedef struct {
    uint32_t kind;
    uint32_t required;
    uint32_t healthy;
    uint32_t dependencies_ready;
    uint64_t last_heartbeat_ns;
    const char *identity;
} slvm7_manager_status_t;

typedef struct {
    uint32_t manager_count;
    uint32_t required_failures;
    uint32_t dependency_failures;
    uint32_t quarantined;
} slvm7_manager_registry_state_t;

int slvm7_manager_validate(const slvm7_manager_status_t *);
int slvm7_manager_registry_validate(const slvm7_manager_registry_state_t *);
int slvm7_manager_registry_can_start(const slvm7_manager_registry_state_t *);
int slvm7_manager_registry_quarantine(slvm7_manager_registry_state_t *, uint32_t kind);

#ifdef __cplusplus
}
#endif

#endif
