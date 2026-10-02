#ifndef SLEELA_VM_DYNAMIC_MEMORY_GUARD_H
#define SLEELA_VM_DYNAMIC_MEMORY_GUARD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLEELA_VM_MEMORY_GUARD_HARD = 1,
    SLEELA_VM_MEMORY_GUARD_SLOW_CAREFUL = 2,
    SLEELA_VM_MEMORY_GUARD_AGGRESSIVE = 3
} sleela_vm_memory_guard_mode_t;

typedef enum {
    SLEELA_VM_MEMORY_WITHIN_LIMIT = 1,
    SLEELA_VM_MEMORY_GROW_ALLOWED = 2,
    SLEELA_VM_MEMORY_GROW_DEFERRED = 3,
    SLEELA_VM_MEMORY_LIMIT_REACHED = 4,
    SLEELA_VM_MEMORY_INVALID_REQUEST = 5
} sleela_vm_memory_guard_result_t;

typedef struct {
    uint64_t initial_limit;
    uint64_t hard_limit;
    uint64_t soft_limit;
    uint64_t maximum_limit;
    uint64_t growth_step;
    uint64_t growth_delay;
    uint32_t gc_pressure_percent;
    uint32_t mode;
    uint32_t allow_growth;
} sleela_vm_memory_guard_config_t;

typedef struct {
    uint64_t current_limit;
    uint64_t current_usage;
    uint64_t peak_usage;
    uint64_t growth_events;
    uint64_t deferred_requests;
} sleela_vm_memory_guard_state_t;

int sleela_vm_memory_guard_validate(
    const sleela_vm_memory_guard_config_t *config,
    uint64_t physical_limit);

int sleela_vm_memory_guard_request(
    const sleela_vm_memory_guard_config_t *config,
    sleela_vm_memory_guard_state_t *state,
    uint64_t required_bytes);

uint64_t sleela_vm_memory_guard_next_limit(
    const sleela_vm_memory_guard_config_t *config,
    const sleela_vm_memory_guard_state_t *state,
    uint64_t required_bytes);

void sleela_vm_memory_guard_observe(
    sleela_vm_memory_guard_state_t *state,
    uint64_t current_usage);

#ifdef __cplusplus
}
#endif

#endif
