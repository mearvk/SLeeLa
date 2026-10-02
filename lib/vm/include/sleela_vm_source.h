#ifndef SLEELA_VM_SOURCE_H
#define SLEELA_VM_SOURCE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SLEELA_VM_SLVM=1, SLEELA_VM_SLJVM=2 } sleela_vm_kind_t;

typedef struct {
    sleela_vm_kind_t kind;
    uint32_t target_code;
    uint32_t architecture_code;
    uint32_t operating_system_code;
    uint32_t abi_code;
    uint32_t execution_code;
    uint32_t memory_code;
    uint32_t threading_code;
    uint32_t io_code;
    uint32_t security_code;
    uint32_t link_code;
    uint32_t package_code;
    uint64_t feature_mask_low;
    uint64_t feature_mask_high;
    uint32_t pointer_bits;
    uint64_t memory_limit;
    uint32_t cpu_limit;
    uint32_t stack_limit;
    uint32_t object_limit;
} sleela_vm_requirements_t;

int sleela_vm_requirements_validate(const sleela_vm_requirements_t*);
int sleela_vm_plan_modules(const sleela_vm_requirements_t*, uint32_t module_count);
int sleela_vm_feature_enabled(const sleela_vm_requirements_t*, uint32_t bit);
int sleela_vm_option_compatible(const sleela_vm_requirements_t*, uint32_t option_code);

#ifdef __cplusplus
}
#endif
#endif
