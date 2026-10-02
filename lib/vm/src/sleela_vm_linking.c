#include "../include/sleela_vm_linking.h"
int sleela_vm_linking_plan_validate(const sleela_vm_linking_plan_t* p) {
 if (!p || p->vm_major == 0 || p->link_code == 0 || p->observe_code == 0) return 0;
 if (p->vm_major >= 4 && p->certificate_code == 0) return 0;
 if (p->attestation_code && p->certificate_code == 0) return 0;
 if (p->provenance_code && p->audit_code == 0) return 0;
 if (p->dual_control_code && p->least_privilege_code == 0) return 0;
 if (p->tamper_evidence_code && p->immutable_audit_code == 0) return 0;
 if (p->air_gap_code && p->mission_partition_code == 0) return 0;
 if (p->emergency_revocation_code && p->certificate_code == 0) return 0;
 return 1;
}
int sleela_vm_linking_version_matches(const sleela_vm_linking_plan_t* p, uint32_t major, uint32_t minor) {
 if (!sleela_vm_linking_plan_validate(p) || major == 0) return 0;
 return p->vm_major == major && p->vm_minor == minor;
}
int sleela_vm_linking_feature_enabled(const sleela_vm_linking_plan_t* p, uint32_t f) {
 if (!p) return 0;
 switch (f) {
  case 1: return p->certificate_code != 0;
  case 2: return p->transaction_code != 0;
  case 3: return p->resolver_code != 0;
  case 4: return p->audit_code != 0;
  case 5: return p->attestation_code != 0;
  case 6: return p->capability_code != 0;
  case 7: return p->provenance_code != 0;
  case 8: return p->checkpoint_code != 0;
  case 9: return p->tamper_evidence_code != 0;
  default: return 0;
 }
}
int sleela_vm_linking_observation_allowed(const sleela_vm_linking_plan_t* p, uint32_t o) {
 if (!sleela_vm_linking_plan_validate(p)) return 0;
 switch (o) {
  case 1: return p->observe_code != 0;
  case 2: return p->certificate_code != 0;
  case 3: return p->transaction_code != 0;
  case 4: return p->audit_code != 0;
  case 5: return p->attestation_code != 0;
  case 6: return p->provenance_code != 0;
  default: return 0;
 }
}
