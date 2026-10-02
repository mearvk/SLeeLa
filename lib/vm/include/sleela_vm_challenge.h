#ifndef SLEELA_VM_CHALLENGE_H
#define SLEELA_VM_CHALLENGE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint32_t condition_code, listener_code, local_code, remote_code; uint32_t binary_result_code, audit_code, resolver_code, capability_code; uint32_t attestation_code, rollback_code; } sleela_vm_challenge_plan_t;
typedef struct { uint32_t condition_code, result_code, source_code, target_code; uint64_t sequence; } sleela_vm_condition_observed_t;
int sleela_vm_challenge_plan_validate(const sleela_vm_challenge_plan_t*);
int sleela_vm_challenge_target_allowed(const sleela_vm_challenge_plan_t*, uint32_t);
int sleela_vm_challenge_condition_matches(const sleela_vm_condition_observed_t*, uint32_t);
#ifdef __cplusplus
}
#endif
#endif
