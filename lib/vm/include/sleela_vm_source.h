#ifndef SLEELA_VM_SOURCE_H
#define SLEELA_VM_SOURCE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLEELA_VM_SLVM=1, SLEELA_VM_SLJVM=2 } sleela_vm_kind_t;
typedef struct { sleela_vm_kind_t kind; uint32_t pointer_bits; uint64_t memory_limit; uint32_t cpu_limit; uint32_t stack_limit; } sleela_vm_requirements_t;
int sleela_vm_requirements_validate(const sleela_vm_requirements_t*);
int sleela_vm_plan_modules(const sleela_vm_requirements_t*, uint32_t module_count);
#ifdef __cplusplus
}
#endif
#endif
