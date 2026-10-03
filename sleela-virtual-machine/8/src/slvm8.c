#include "slvm8.h"
#include "slvm8_policy.h"
#include "slvm8_capability.h"
#include "slvm8_transaction.h"
#include "slvm8_supervisor.h"
#include "slvm8_audit.h"
#include "slvm8_admission.h"
#include <string.h>

int slvm8_config_defaults(slvm8_config_t*c){if(!c)return SLVM8_INVALID;memset(c,0,sizeof(*c));c->instance=8;c->policy_version=1;c->memory_limit=536870912ULL;c->resource_epoch=1;c->capability_epoch=1;c->execution_epoch=1;c->policy_hash="required";c->artifact_hash="required";c->manager_set_hash="required";return SLVM8_OK;}
int slvm8_config_load_file(slvm8_config_t*c,const char*p){if(!c||!p||!*p)return SLVM8_INVALID;return slvm8_config_defaults(c);}
int slvm8_config_validate(const slvm8_config_t*c){return c&&c->instance==8&&c->policy_version&&c->memory_limit&&c->policy_hash&&c->artifact_hash&&c->manager_set_hash?SLVM8_OK:SLVM8_INVALID;}
int slvm8_phase_valid(slvm8_phase_t f,slvm8_phase_t t){if(f==t)return 1;if(f==SLVM8_NORMAL&&t==SLVM8_ADMITTED)return 1;if(f==SLVM8_ADMITTED&&t==SLVM8_RUNNING)return 1;if(f==SLVM8_RUNNING&&(t==SLVM8_DEGRADED||t==SLVM8_CHECKPOINTING||t==SLVM8_QUIESCING||t==SLVM8_QUARANTINED_STATE))return 1;if(f==SLVM8_DEGRADED&&(t==SLVM8_CHECKPOINTING||t==SLVM8_RECOVERING||t==SLVM8_QUARANTINED_STATE))return 1;if(f==SLVM8_CHECKPOINTING&&(t==SLVM8_RUNNING||t==SLVM8_RECOVERING||t==SLVM8_QUARANTINED_STATE))return 1;if(f==SLVM8_RECOVERING&&(t==SLVM8_RUNNING||t==SLVM8_QUARANTINED_STATE))return 1;if(f==SLVM8_QUIESCING&&t==SLVM8_STOPPED)return 1;return 0;}
const char*slvm8_phase_name(slvm8_phase_t p){switch(p){case SLVM8_NORMAL:return"normal";case SLVM8_ADMITTED:return"admitted";case SLVM8_RUNNING:return"running";case SLVM8_DEGRADED:return"degraded";case SLVM8_CHECKPOINTING:return"checkpointing";case SLVM8_RECOVERING:return"recovering";case SLVM8_QUIESCING:return"quiescing";case SLVM8_QUARANTINED_STATE:return"quarantined";case SLVM8_STOPPED:return"stopped";default:return"unknown";}}
int slvm8_policy_validate(const slvm8_policy_t*p){return p&&p->version&&p->max_memory&&p->max_cpu&&p->max_io&&p->max_network&&p->policy_hash&&p->immutable&&p->valid&&p->valid_signers>=p->required_signers?SLVM8_OK:SLVM8_POLICY_FAILURE;}
int slvm8_policy_can_admit(const slvm8_policy_t*p){return slvm8_policy_validate(p);}
int slvm8_capability_validate(const slvm8_capability_lease_t*l,uint64_t e){return l&&l->lease_id&&l->issued_epoch<=e&&e<l->expiry_epoch&&l->active&&l->policy_valid?SLVM8_OK:SLVM8_DENIED;}
int slvm8_capability_revoke(slvm8_capability_lease_t*l){if(!l)return SLVM8_INVALID;l->active=0;return SLVM8_OK;}
int slvm8_capability_renew(slvm8_capability_lease_t*l,uint64_t e,uint64_t x){if(!l||!l->active||!l->revocable||e>=l->expiry_epoch||!x)return SLVM8_DENIED;l->expiry_epoch+=x;return SLVM8_OK;}
int slvm8_transaction_begin(slvm8_transaction_t*t,uint64_t id,uint64_t e){if(!t||!id||!e)return SLVM8_INVALID;memset(t,0,sizeof(*t));t->id=id;t->execution_epoch=e;t->state=SLVM8_TXN_ACTIVE;t->integrity_valid=1;t->admission_valid=1;return SLVM8_OK;}
int slvm8_transaction_record(slvm8_transaction_t*t){if(!t||t->state!=SLVM8_TXN_ACTIVE||!t->integrity_valid||!t->admission_valid)return SLVM8_TRANSACTION_ABORTED;++t->action_count;return SLVM8_OK;}
int slvm8_transaction_commit(slvm8_transaction_t*t){if(!t||t->state!=SLVM8_TXN_ACTIVE||!t->integrity_valid||!t->admission_valid)return SLVM8_TRANSACTION_ABORTED;t->committed_actions=t->action_count;t->state=SLVM8_TXN_COMMITTED;return SLVM8_OK;}
int slvm8_transaction_abort(slvm8_transaction_t*t){if(!t)return SLVM8_INVALID;t->state=SLVM8_TXN_ABORTED;return SLVM8_TRANSACTION_ABORTED;}
uint64_t slvm8_audit_hash(uint64_t s,uint64_t e,uint64_t v,uint64_t p){uint64_t h=1469598103934665603ULL;h^=s;h*=1099511628211ULL;h^=e;h*=1099511628211ULL;h^=v;h*=1099511628211ULL;h^=p;h*=1099511628211ULL;return h;}
int slvm8_audit_validate(const slvm8_audit_record_t*r){return r&&r->sequence&&r->record_hash==slvm8_audit_hash(r->sequence,r->epoch,r->event,r->previous_hash)?SLVM8_OK:SLVM8_INVALID;}
int slvm8_admission_validate(const slvm8_admission_t*a){return a&&a->artifact_valid&&a->policy_valid&&a->managers_valid&&a->resources_valid&&a->capabilities_valid&&a->attestation_valid&&a->lineage_valid&&a->admitted?SLVM8_OK:SLVM8_DENIED;}
int slvm8_admission_evaluate(slvm8_admission_t*a){if(!a)return SLVM8_INVALID;a->admitted=(uint8_t)(a->artifact_valid&&a->policy_valid&&a->managers_valid&&a->resources_valid&&a->capabilities_valid&&a->attestation_valid&&a->lineage_valid);return a->admitted?SLVM8_OK:SLVM8_DENIED;}
int slvm8_supervisor_validate(const slvm8_supervisor_t*s){return s&&s->integrity_valid&&s->admission_valid&&s->policy_valid&&s->capabilities_valid&&s->managers_valid&&s->phase!=SLVM8_QUARANTINED_STATE?SLVM8_OK:SLVM8_DENIED;}
int slvm8_supervisor_admit(slvm8_supervisor_t*s){if(!s||!s->integrity_valid||!s->admission_valid||!s->policy_valid||!s->capabilities_valid||!s->managers_valid)return SLVM8_DENIED;s->phase=SLVM8_ADMITTED;return SLVM8_OK;}
int slvm8_supervisor_start(slvm8_supervisor_t*s){if(!s||s->phase!=SLVM8_ADMITTED)return SLVM8_DENIED;s->phase=SLVM8_RUNNING;return SLVM8_OK;}
int slvm8_supervisor_fault(slvm8_supervisor_t*s,int integrity_fault){if(!s)return SLVM8_INVALID;++s->fault_count;if(integrity_fault){s->integrity_valid=0;s->phase=SLVM8_QUARANTINED_STATE;return SLVM8_QUARANTINED;}if(s->phase==SLVM8_RUNNING)s->phase=SLVM8_DEGRADED;return SLVM8_RECOVERY_REQUIRED;}
int slvm8_supervisor_checkpoint(slvm8_supervisor_t*s){if(!s||!s->integrity_valid)return SLVM8_QUARANTINED;s->phase=SLVM8_CHECKPOINTING;++s->checkpoint_count;s->phase=SLVM8_RUNNING;return SLVM8_OK;}
int slvm8_supervisor_recover(slvm8_supervisor_t*s){if(!s||!s->integrity_valid)return SLVM8_QUARANTINED;if(s->phase!=SLVM8_DEGRADED&&s->phase!=SLVM8_CHECKPOINTING)return SLVM8_RECOVERY_REQUIRED;s->phase=SLVM8_RECOVERING;++s->recovery_count;s->phase=SLVM8_RUNNING;return SLVM8_OK;}
int slvm8_supervisor_quarantine(slvm8_supervisor_t*s){if(!s)return SLVM8_INVALID;s->phase=SLVM8_QUARANTINED_STATE;s->integrity_valid=0;return SLVM8_QUARANTINED;}
int slvm8_supervisor_stop(slvm8_supervisor_t*s){if(!s)return SLVM8_INVALID;if(s->phase==SLVM8_QUARANTINED_STATE){s->phase=SLVM8_STOPPED;return SLVM8_OK;}if(s->phase!=SLVM8_RUNNING&&s->phase!=SLVM8_DEGRADED)return SLVM8_DENIED;s->phase=SLVM8_QUIESCING;s->phase=SLVM8_STOPPED;return SLVM8_OK;}
