#include "slvm9.h"
#include "slvm9_filesystem.h"
#include "slvm9_file.h"
#include "slvm9_adapter.h"
#include "slvm9_mount.h"
#include "slvm9_fsop.h"
#include "slvm9_context.h"
#include "slvm9_supervisor.h"
#include <string.h>
int slvm9_platform_validate(const slvm9_platform_t*p){return p&&p->os!=SLVM9_OTHER&&p->name&&p->filesystem_access&&p->integrity&&p->handles?SLVM9_OK:SLVM9_INVALID;}
const char*slvm9_os_name(slvm9_os_t o){switch(o){case SLVM9_LINUX:return "Linux";case SLVM9_WINDOWS:return "Windows";case SLVM9_MACOS:return "macOS";default:return "Other";}}
int slvm9_filesystem_validate(const slvm9_filesystem_t*f){return f&&f->filesystem_id&&f->generation&&f->block_size&&f->type&&f->uuid&&f->mount_identity&&f->mounted&&f->integrity_valid&&f->durability_known?SLVM9_OK:SLVM9_INVALID;}
int slvm9_filesystem_can(const slvm9_filesystem_t*f,uint32_t bits){return f&&((f->feature_bits&bits)==bits)?SLVM9_OK:SLVM9_UNSUPPORTED;}
int slvm9_filesystem_generation_valid(const slvm9_filesystem_t*f,uint64_t g){return f&&f->generation==g&&f->integrity_valid?SLVM9_OK:SLVM9_STALE;}
int slvm9_file_validate(const slvm9_file_t*f){return f&&f->object_id&&f->generation&&f->name&&f->identity_valid&&f->readable?SLVM9_OK:SLVM9_INVALID;}
int slvm9_file_can_write(const slvm9_file_t*f){return f&&f->identity_valid&&f->writable?SLVM9_OK:SLVM9_DENIED;}
int slvm9_adapter_validate(const slvm9_adapter_t*a){return a&&slvm9_platform_validate(&a->platform)==SLVM9_OK&&a->filesystem_type&&a->api_version&&a->available&&a->verified?SLVM9_OK:SLVM9_INVALID;}
int slvm9_mount_validate(const slvm9_mount_t*m){return m&&m->mount_id&&m->filesystem_id&&m->generation&&m->path&&m->namespace_id&&m->active&&m->verified?SLVM9_OK:SLVM9_INVALID;}
int slvm9_mount_is_current(const slvm9_mount_t*m,uint64_t f,uint64_t g){return slvm9_mount_validate(m)==SLVM9_OK&&m->filesystem_id==f&&m->generation==g?SLVM9_OK:SLVM9_STALE;}
int slvm9_fsop_validate(const slvm9_fsop_t*o){return o&&o->kind>=SLVM9_FSOP_OPEN&&o->kind<=SLVM9_FSOP_CHECKPOINT&&o->object_id&&o->expected_generation&&o->admitted?SLVM9_OK:SLVM9_INVALID;}
int slvm9_context_validate(const slvm9_context_identity_t*c){return c&&c->file_id&&c->context_id&&c->filesystem_type&&c->mount_point&&c->identity_valid?SLVM9_OK:SLVM9_INVALID;}
int slvm9_supervisor_validate(const slvm9_supervisor_t*s){return s&&s->policy_valid&&s->filesystem_valid&&s->adapter_valid&&s->capabilities_valid&&s->integrity_valid&&s->phase!=SLVM9_QUARANTINED?SLVM9_OK:SLVM9_DENIED;}
int slvm9_supervisor_admit(slvm9_supervisor_t*s){if(!s||!s->policy_valid||!s->filesystem_valid||!s->adapter_valid||!s->capabilities_valid||!s->integrity_valid)return SLVM9_DENIED;s->phase=SLVM9_ADMITTED;return SLVM9_OK;}
int slvm9_supervisor_start(slvm9_supervisor_t*s){if(!s||s->phase!=SLVM9_ADMITTED)return SLVM9_DENIED;s->phase=SLVM9_RUNNING;return SLVM9_OK;}
int slvm9_supervisor_fault(slvm9_supervisor_t*s,int integrity){if(!s)return SLVM9_INVALID;++s->faults;if(integrity){s->integrity_valid=0;s->phase=SLVM9_QUARANTINED;return SLVM9_INTEGRITY;}s->phase=SLVM9_DEGRADED;return SLVM9_RECOVERY;}
int slvm9_supervisor_quarantine(slvm9_supervisor_t*s){if(!s)return SLVM9_INVALID;s->integrity_valid=0;s->phase=SLVM9_QUARANTINED;return SLVM9_INTEGRITY;}
int slvm9_supervisor_stop(slvm9_supervisor_t*s){if(!s)return SLVM9_INVALID;if(s->phase==SLVM9_QUARANTINED||s->phase==SLVM9_RUNNING||s->phase==SLVM9_DEGRADED){s->phase=SLVM9_STOPPED;return SLVM9_OK;}return SLVM9_DENIED;}
