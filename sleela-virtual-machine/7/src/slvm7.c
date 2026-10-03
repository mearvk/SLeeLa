#include "slvm7.h"
#include "slvm7_log.h"
#include "slvm7_memory.h"
#include "slvm7_manager.h"
#include "slvm7_health.h"
#include "slvm7_recovery.h"
#include "slvm7_checkpoint.h"
#include "slvm7_resources.h"
int slvm7_config_defaults(slvm7_config_t *c){if(!c)return SLVM7_INVALID;*c=(slvm7_config_t){0};c->instance=7;c->runtime_target=SLVM7_RUNTIME_AUTO;c->memory_limit=536870912ULL;c->policy_version=1;return SLVM7_OK;}
int slvm7_config_load_file(slvm7_config_t *c,const char *p){if(!c||!p||!*p)return SLVM7_INVALID;return slvm7_config_defaults(c);}
int slvm7_runtime_validate(const slvm7_config_t *c){return c&&c->instance==7&&c->memory_limit&&c->policy_hash?SLVM7_OK:SLVM7_INVALID;}
int slvm7_runtime_fault(slvm7_config_t *c,slvm7_status_t s){(void)c;return s==SLVM7_OK?SLVM7_INVALID:s;}
int slvm7_log_validate(const slvm7_log_record_t *r){return r&&r->sequence&&r->timestamp_ns&&r->event&&r->record_hash?SLVM7_OK:SLVM7_INVALID;}
int slvm7_log_manager_validate(const slvm7_log_manager_state_t *s){return s&&s->integrity_valid&&s->sink_healthy&&!s->dropped_events?SLVM7_OK:SLVM7_INTEGRITY_FAILURE;}
int slvm7_log_manager_fault(slvm7_log_manager_state_t *s){if(!s)return SLVM7_INVALID;s->sink_healthy=0;return SLVM7_MANAGER_FAILURE;}
int slvm7_log_manager_recovery_marker(slvm7_log_manager_state_t *s,uint64_t id){if(!s||!id)return SLVM7_INVALID;++s->recovery_events;return SLVM7_OK;}
int slvm7_memory_validate(const slvm7_memory_manager_state_t *s){return s&&s->limit_bytes&&s->committed_bytes<=s->limit_bytes&&s->reserved_bytes<=s->limit_bytes&&s->integrity_valid?SLVM7_OK:SLVM7_INVALID;}
int slvm7_memory_pressure(slvm7_memory_manager_state_t *s,uint64_t n){if(!s)return SLVM7_INVALID;if(n>s->limit_bytes-s->committed_bytes){s->pressure_level=3;return SLVM7_RESOURCE_PRESSURE;}s->pressure_level=0;return SLVM7_OK;}
int slvm7_memory_checkpoint_account(slvm7_memory_manager_state_t *s,uint64_t n){if(!s||n>s->limit_bytes-s->checkpoint_bytes)return SLVM7_RESOURCE_PRESSURE;s->checkpoint_bytes+=n;return SLVM7_OK;}
int slvm7_memory_fault(slvm7_memory_manager_state_t *s){if(!s)return SLVM7_INVALID;s->quarantine_required=1;s->integrity_valid=0;return SLVM7_INTEGRITY_FAILURE;}
int slvm7_manager_validate(const slvm7_manager_status_t *s){return s&&s->kind&&s->identity&&s->healthy&&s->dependencies_ready?SLVM7_OK:SLVM7_MANAGER_FAILURE;}
int slvm7_manager_registry_validate(const slvm7_manager_registry_state_t *s){return s&&s->manager_count&&!s->required_failures&&!s->dependency_failures?SLVM7_OK:SLVM7_MANAGER_FAILURE;}
int slvm7_manager_registry_can_start(const slvm7_manager_registry_state_t *s){return slvm7_manager_registry_validate(s);}
int slvm7_manager_registry_quarantine(slvm7_manager_registry_state_t *s,uint32_t k){(void)k;if(!s)return SLVM7_INVALID;s->quarantined=1;return SLVM7_QUARANTINED;}
int slvm7_health_validate(const slvm7_health_state_t *s){return s&&!s->stalled&&!s->watchdog_triggered&&s->memory_committed_bytes<=s->memory_limit_bytes?SLVM7_OK:SLVM7_MANAGER_FAILURE;}
int slvm7_health_heartbeat(slvm7_health_state_t *s,uint64_t n){if(!s||!n)return SLVM7_INVALID;++s->heartbeat_sequence;s->last_heartbeat_ns=n;return SLVM7_OK;}
int slvm7_health_check_stall(const slvm7_health_state_t *s,uint64_t n,uint64_t t){if(!s||n<s->last_heartbeat_ns)return SLVM7_INVALID;return n-s->last_heartbeat_ns>t?SLVM7_RECOVERY_REQUIRED:SLVM7_OK;}
int slvm7_health_fault(slvm7_health_state_t *s){if(!s)return SLVM7_INVALID;s->stalled=1;s->watchdog_triggered=1;return SLVM7_RECOVERY_REQUIRED;}
int slvm7_recovery_validate(const slvm7_recovery_state_t *s){return s&&s->checkpoint_valid&&s->policy_match&&s->lineage_valid&&s->manager_set_healthy&&!s->quarantined?SLVM7_OK:SLVM7_RECOVERY_REQUIRED;}
int slvm7_recovery_can_attempt(const slvm7_recovery_state_t *s){return s&&slvm7_recovery_validate(s)==SLVM7_OK&&s->attempts<s->max_attempts?SLVM7_OK:SLVM7_RECOVERY_REQUIRED;}
int slvm7_recovery_record_failure(slvm7_recovery_state_t *s){if(!s)return SLVM7_INVALID;++s->attempts;if(s->attempts>=s->max_attempts)s->quarantined=1;return s->quarantined?SLVM7_QUARANTINED:SLVM7_RECOVERY_REQUIRED;}
int slvm7_recovery_mark_quarantine(slvm7_recovery_state_t *s){if(!s)return SLVM7_INVALID;s->quarantined=1;return SLVM7_QUARANTINED;}
int slvm7_checkpoint_validate(const slvm7_checkpoint_t *c){return c&&c->checkpoint_id&&c->epoch&&c->artifact_hash&&c->policy_hash&&c->lineage_hash&&c->integrity_hash&&c->complete&&c->replay_safe?SLVM7_OK:SLVM7_INVALID;}
int slvm7_resource_validate(const slvm7_resource_state_t *r){return r&&r->cpu_used<=r->cpu_budget&&r->io_used<=r->io_budget&&r->network_used<=r->network_budget&&r->memory_used<=r->memory_budget?SLVM7_OK:SLVM7_RESOURCE_PRESSURE;}
int slvm7_resource_pressure(const slvm7_resource_state_t *r){return slvm7_resource_validate(r);}
