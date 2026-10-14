#include "../include/sleela_vm_challenge.h"
int sleela_vm_challenge_plan_validate(const sleela_vm_challenge_plan_t* p){if(!p||!p->condition_code||!p->listener_code||!p->binary_result_code)return 0;if(p->remote_code&&!p->resolver_code)return 0;if(p->attestation_code&&!p->capability_code)return 0;if(p->rollback_code&&!p->audit_code)return 0;return 1;}
int sleela_vm_challenge_target_allowed(const sleela_vm_challenge_plan_t* p,uint32_t t){if(!sleela_vm_challenge_plan_validate(p))return 0;return t==1?p->local_code!=0:t==2?p->remote_code!=0:0;}
int sleela_vm_challenge_condition_matches(const sleela_vm_condition_observed_t* e,uint32_t c){return e&&e->condition_code==c&&e->result_code!=0&&e->sequence!=0;}
