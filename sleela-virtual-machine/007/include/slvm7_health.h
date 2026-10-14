#ifndef SLEELA_SLVM7_HEALTH_H
#define SLEELA_SLVM7_HEALTH_H

#include <stdint.h>
#include "slvm7.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t heartbeat_sequence;
    uint64_t last_heartbeat_ns;
    uint64_t last_checkpoint_ns;
    uint64_t last_log_sequence;
    uint64_t memory_committed_bytes;
    uint64_t memory_limit_bytes;
    uint32_t stalled;
    uint32_t degraded;
    uint32_t watchdog_triggered;
} slvm7_health_state_t;

int slvm7_health_validate(const slvm7_health_state_t *);
int slvm7_health_heartbeat(slvm7_health_state_t *, uint64_t now_ns);
int slvm7_health_check_stall(const slvm7_health_state_t *, uint64_t now_ns, uint64_t timeout_ns);
int slvm7_health_fault(slvm7_health_state_t *);

#ifdef __cplusplus
}
#endif

#endif
