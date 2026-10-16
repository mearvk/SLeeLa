#ifndef SLEELA_SLVM7_LOG_H
#define SLEELA_SLVM7_LOG_H

#include <stdint.h>
#include "slvm7.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLVM7_LOG_INFO = 0,
    SLVM7_LOG_NOTICE = 1,
    SLVM7_LOG_WARNING = 2,
    SLVM7_LOG_SECURITY = 3,
    SLVM7_LOG_RECOVERY = 4,
    SLVM7_LOG_FATAL = 5
} slvm7_log_level_t;

typedef struct {
    uint64_t sequence;
    uint64_t timestamp_ns;
    uint32_t level;
    const char *event;
    const char *epoch;
    const char *previous_record_hash;
    const char *record_hash;
} slvm7_log_record_t;

typedef struct {
    uint64_t next_sequence;
    uint64_t security_events;
    uint64_t recovery_events;
    uint64_t dropped_events;
    uint32_t integrity_valid;
    uint32_t sink_healthy;
} slvm7_log_manager_state_t;

int slvm7_log_validate(const slvm7_log_record_t *);
int slvm7_log_manager_validate(const slvm7_log_manager_state_t *);
int slvm7_log_manager_fault(slvm7_log_manager_state_t *);
int slvm7_log_manager_recovery_marker(slvm7_log_manager_state_t *, uint64_t checkpoint_id);

#ifdef __cplusplus
}
#endif

#endif
