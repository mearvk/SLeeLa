#ifndef SLEELA_SLVM7_RECOVERY_H
#define SLEELA_SLVM7_RECOVERY_H

#include <stdint.h>
#include "slvm7.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t checkpoint_id;
    uint64_t source_epoch;
    uint64_t attempts;
    uint64_t max_attempts;
    uint32_t checkpoint_valid;
    uint32_t policy_match;
    uint32_t lineage_valid;
    uint32_t manager_set_healthy;
    uint32_t quarantined;
} slvm7_recovery_state_t;

int slvm7_recovery_validate(const slvm7_recovery_state_t *);
int slvm7_recovery_can_attempt(const slvm7_recovery_state_t *);
int slvm7_recovery_record_failure(slvm7_recovery_state_t *);
int slvm7_recovery_mark_quarantine(slvm7_recovery_state_t *);

#ifdef __cplusplus
}
#endif

#endif
