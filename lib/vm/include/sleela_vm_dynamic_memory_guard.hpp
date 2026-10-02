#ifndef SLEELA_VM_DYNAMIC_MEMORY_GUARD_HPP
#define SLEELA_VM_DYNAMIC_MEMORY_GUARD_HPP

#include "sleela_vm_dynamic_memory_guard.h"

namespace sleela::vm {
class DynamicMemoryGuard {
public:
    DynamicMemoryGuard(sleela_vm_memory_guard_config_t config,
                       sleela_vm_memory_guard_state_t state = {})
        : config_(config), state_(state) {}

    int validate(uint64_t physicalLimit) const {
        return sleela_vm_memory_guard_validate(&config_, physicalLimit);
    }

    int request(uint64_t requiredBytes) {
        return sleela_vm_memory_guard_request(&config_, &state_, requiredBytes);
    }

    uint64_t nextLimit(uint64_t requiredBytes) const {
        return sleela_vm_memory_guard_next_limit(&config_, &state_, requiredBytes);
    }

    void observe(uint64_t currentUsage) {
        sleela_vm_memory_guard_observe(&state_, currentUsage);
    }

    const sleela_vm_memory_guard_config_t& config() const { return config_; }
    const sleela_vm_memory_guard_state_t& state() const { return state_; }

private:
    sleela_vm_memory_guard_config_t config_;
    sleela_vm_memory_guard_state_t state_;
};
}

#endif
