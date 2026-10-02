#ifndef SLEELA_VM_LINKING_H
#define SLEELA_VM_LINKING_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint32_t vm_major, vm_minor, link_code, observe_code;
 uint32_t certificate_code, transaction_code, resolver_code, audit_code;
 uint32_t attestation_code, capability_code, provenance_code, checkpoint_code;
 uint32_t immutable_audit_code, dual_control_code, least_privilege_code, retention_code;
 uint32_t tamper_evidence_code, air_gap_code, mission_partition_code, emergency_revocation_code;
} sleela_vm_linking_plan_t;
int sleela_vm_linking_plan_validate(const sleela_vm_linking_plan_t*);
int sleela_vm_linking_version_matches(const sleela_vm_linking_plan_t*, uint32_t, uint32_t);
int sleela_vm_linking_feature_enabled(const sleela_vm_linking_plan_t*, uint32_t);
int sleela_vm_linking_observation_allowed(const sleela_vm_linking_plan_t*, uint32_t);
#ifdef __cplusplus
}
#endif
#endif
