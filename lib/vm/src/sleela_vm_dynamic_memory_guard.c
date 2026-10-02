#include "../include/sleela_vm_dynamic_memory_guard.h"

static uint64_t max_u64(uint64_t a, uint64_t b) {
    return a > b ? a : b;
}

int sleela_vm_memory_guard_validate(
    const sleela_vm_memory_guard_config_t *c,
    uint64_t physical_limit) {
    if (!c || !physical_limit) return -1;
    if (!c->initial_limit || !c->hard_limit || !c->maximum_limit) return -2;
    if (c->initial_limit > c->hard_limit) return -3;
    if (c->hard_limit > c->maximum_limit) return -4;
    if (c->maximum_limit > physical_limit) return -5;
    if (c->mode < SLEELA_VM_MEMORY_GUARD_HARD ||
        c->mode > SLEELA_VM_MEMORY_GUARD_AGGRESSIVE) return -6;
    if (c->mode != SLEELA_VM_MEMORY_GUARD_HARD && !c->growth_step) return -7;
    if (c->gc_pressure_percent > 100) return -8;
    if (c->mode == SLEELA_VM_MEMORY_GUARD_HARD && c->allow_growth) return -9;
    return 0;
}

uint64_t sleela_vm_memory_guard_next_limit(
    const sleela_vm_memory_guard_config_t *c,
    const sleela_vm_memory_guard_state_t *s,
    uint64_t required_bytes) {
    uint64_t required;
    uint64_t step;
    uint64_t candidate;

    if (!c || !s || !required_bytes) return 0;
    required = s->current_usage + required_bytes;
    if (required < s->current_usage) return 0;
    if (required <= s->current_limit) return s->current_limit;
    if (c->mode == SLEELA_VM_MEMORY_GUARD_HARD) return 0;
    if (!c->allow_growth || required > c->maximum_limit) return 0;

    step = c->growth_step;
    if (c->mode == SLEELA_VM_MEMORY_GUARD_AGGRESSIVE) {
        candidate = max_u64(required, s->current_limit + step);
    } else {
        candidate = s->current_limit + step;
        if (candidate < s->current_limit) return 0;
        if (candidate < required) {
            uint64_t deficit = required - candidate;
            uint64_t extra_steps = (deficit + step - 1) / step;
            candidate += extra_steps * step;
        }
    }

    if (candidate < required || candidate > c->maximum_limit) return 0;
    return candidate;
}

int sleela_vm_memory_guard_request(
    const sleela_vm_memory_guard_config_t *c,
    sleela_vm_memory_guard_state_t *s,
    uint64_t required_bytes) {
    uint64_t next;

    if (!c || !s || !required_bytes) return SLEELA_VM_MEMORY_INVALID_REQUEST;
    if (s->current_usage + required_bytes < s->current_usage)
        return SLEELA_VM_MEMORY_INVALID_REQUEST;
    if (s->current_usage + required_bytes <= s->current_limit)
        return SLEELA_VM_MEMORY_WITHIN_LIMIT;

    if (c->mode == SLEELA_VM_MEMORY_GUARD_HARD ||
        !c->allow_growth ||
        s->current_usage + required_bytes > c->maximum_limit)
        return SLEELA_VM_MEMORY_LIMIT_REACHED;

    if (c->mode == SLEELA_VM_MEMORY_GUARD_SLOW_CAREFUL &&
        c->growth_delay != 0) {
        s->deferred_requests++;
        return SLEELA_VM_MEMORY_GROW_DEFERRED;
    }

    next = sleela_vm_memory_guard_next_limit(c, s, required_bytes);
    if (!next) return SLEELA_VM_MEMORY_LIMIT_REACHED;

    s->current_limit = next;
    s->growth_events++;
    return SLEELA_VM_MEMORY_GROW_ALLOWED;
}

void sleela_vm_memory_guard_observe(
    sleela_vm_memory_guard_state_t *s,
    uint64_t current_usage) {
    if (!s) return;
    s->current_usage = current_usage;
    if (current_usage > s->peak_usage) s->peak_usage = current_usage;
}
