#ifndef SLEELA_VM_MANAGEMENT_H
#define SLEELA_VM_MANAGEMENT_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t heap_limit, stack_limit, reserved_limit; uint32_t object_limit, page_size, gc_code, allocator_code, guard_code, quarantine_code, scrub_code, checkpoint_code, migration_code, encryption_code, integrity_code; } sleela_vm_memory_plan_t;
typedef struct { uint32_t policy_code, capability_code, isolation_code, audit_code, crypto_code, certificate_code, resolver_code, replay_code, attestation_code, delegation_code, provenance_code, recovery_code; } sleela_vm_security_plan_t;
int sleela_vm_memory_plan_validate(const sleela_vm_memory_plan_t*);
int sleela_vm_memory_feature_enabled(const sleela_vm_memory_plan_t*, uint32_t);
int sleela_vm_security_plan_validate(const sleela_vm_security_plan_t*);
int sleela_vm_security_feature_enabled(const sleela_vm_security_plan_t*, uint32_t);
#ifdef __cplusplus
}
#endif
#endif