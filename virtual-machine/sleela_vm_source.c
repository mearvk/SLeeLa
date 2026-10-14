#include "../include/sleela_vm_source.h"

int sleela_vm_requirements_validate(const sleela_vm_requirements_t *r) {
    if (!r) return -1;
    if (r->kind != SLEELA_VM_SLVM && r->kind != SLEELA_VM_SLJVM) return -2;
    if (!r->pointer_bits) return -3;
    if (!r->memory_limit) return -4;
    if (!r->cpu_limit) return -5;
    if (!r->stack_limit) return -6;
    if (r->kind == SLEELA_VM_SLJVM && r->abi_code != 32) return -7;
    if (r->kind == SLEELA_VM_SLVM && r->abi_code != 30 && r->abi_code != 31) return -8;
    return 0;
}

int sleela_vm_plan_modules(const sleela_vm_requirements_t *r, uint32_t n) {
    return sleela_vm_requirements_validate(r) == 0 && n > 0 ? 0 : -1;
}

int sleela_vm_feature_enabled(const sleela_vm_requirements_t *r, uint32_t bit) {
    if (!r || bit >= 128) return 0;
    if (bit < 64) return (r->feature_mask_low & (UINT64_C(1) << bit)) != 0;
    return (r->feature_mask_high & (UINT64_C(1) << (bit - 64))) != 0;
}

int sleela_vm_option_compatible(const sleela_vm_requirements_t *r, uint32_t option_code) {
    if (!r) return 0;
    if (option_code == 2) return r->kind == SLEELA_VM_SLJVM;
    if (option_code == 1) return r->kind == SLEELA_VM_SLVM;
    return 1;
}
