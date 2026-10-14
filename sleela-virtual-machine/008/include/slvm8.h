#ifndef SLVM8_H
#define SLVM8_H
#include <stdint.h>
#include <stddef.h>
typedef enum { SLVM8_OK=0, SLVM8_INVALID=-1, SLVM8_DENIED=-2, SLVM8_RESOURCE_PRESSURE=-3, SLVM8_RECOVERY_REQUIRED=-4, SLVM8_QUARANTINED=-5, SLVM8_TRANSACTION_ABORTED=-6, SLVM8_POLICY_FAILURE=-7 } slvm8_status_t;
typedef enum { SLVM8_NORMAL=0, SLVM8_ADMITTED=1, SLVM8_RUNNING=2, SLVM8_DEGRADED=3, SLVM8_CHECKPOINTING=4, SLVM8_RECOVERING=5, SLVM8_QUIESCING=6, SLVM8_QUARANTINED_STATE=7, SLVM8_STOPPED=8 } slvm8_phase_t;
typedef struct { uint32_t instance, policy_version; uint64_t memory_limit, resource_epoch, capability_epoch, audit_sequence, execution_epoch; const char *policy_hash, *artifact_hash, *manager_set_hash; } slvm8_config_t;
int slvm8_config_defaults(slvm8_config_t *c);
int slvm8_config_load_file(slvm8_config_t *c,const char *path);
int slvm8_config_validate(const slvm8_config_t *c);
int slvm8_phase_valid(slvm8_phase_t from,slvm8_phase_t to);
const char *slvm8_phase_name(slvm8_phase_t phase);
#endif
