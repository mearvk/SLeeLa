#ifndef SLEELA_VM_MANAGEMENT_H
#define SLEELA_VM_MANAGEMENT_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t heap_limit;
    uint64_t stack_limit;
    uint64_t reserved_limit;
    uint32_t object_limit;
    uint32_t page_size;
    uint32_t gc_code;
    uint32_t allocator_code;
    uint32_t guard_code;
    uint32_t quarantine_code;
    uint32_t scrub_code;
    uint32_t checkpoint_code;
    uint32_t migration_code;
    uint32_t encryption_code;
    uint32_t integrity_code;
} sleela_vm_memory_plan_t;

typedef struct {
    uint32_t policy_code;
    uint32_t capability_code;
    uint32_t isolation_code;
    uint32_t audit_code;
    uint32_t crypto_code;
    uint32_t certificate_code;
    uint32_t resolver_code;
    uint32_t replay_code;
    uint32_t attestation_code;
    uint32_t delegation_code;
    uint32_t provenance_code;
    uint32_t recovery_code;
} sleela_vm_security_plan_t;

int sleela_vm_memory_plan_validate(const sleela_vm_memory_plan_t*);
int sleela_vm_memory_feature_enabled(const sleela_vm_memory_plan_t*, uint32_t feature);
int sleela_vm_security_plan_validate(const sleela_vm_security_plan_t*);
int sleela_vm_security_feature_enabled(const sleela_vm_security_plan_t*, uint32_t feature);

#ifdef __cplusplus
}
#endif
#endif
