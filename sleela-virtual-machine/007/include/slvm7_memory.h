#ifndef SLEELA_SLVM7_MEMORY_H
#define SLEELA_SLVM7_MEMORY_H

#include <stdint.h>
#include "slvm7.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t limit_bytes;
    uint64_t committed_bytes;
    uint64_t reserved_bytes;
    uint64_t checkpoint_bytes;
    uint64_t high_watermark_bytes;
    uint32_t pressure_level;
    uint32_t integrity_valid;
    uint32_t quarantine_required;
} slvm7_memory_manager_state_t;

int slvm7_memory_validate(const slvm7_memory_manager_state_t *);
int slvm7_memory_pressure(slvm7_memory_manager_state_t *, uint64_t requested_bytes);
int slvm7_memory_checkpoint_account(slvm7_memory_manager_state_t *, uint64_t bytes);
int slvm7_memory_fault(slvm7_memory_manager_state_t *);

#ifdef __cplusplus
}
#endif

#endif
