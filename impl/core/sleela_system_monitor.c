#include "sleela_system_monitor.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static unsigned weight(SLHSMObservation o,unsigned cost) {
    unsigned w=cost?cost:1u;
    switch(o) {
        case SL_HSM_OBS_UNSUPPORTED_CAPABILITY:return 8u+w;
        case SL_HSM_OBS_PLATFORM_MISMATCH:return 10u+w;
        case SL_HSM_OBS_UNKNOWN_OPERATION:return 24u+w;
        case SL_HSM_OBS_NATIVE_ESCAPE:return 29u+w;
        case SL_HSM_OBS_PRIVILEGED_OPERATION:return 22u+w;
        case SL_HSM_OBS_RESOURCE_BURST:return 6u+w;
        case SL_HSM_OBS_RESOURCE_IMBALANCE:return 10u+w;
        default:return 0u;
    }
}
void slhsm_init(SLSystemMonitor *m) { if(!m)return; memset(m,0,sizeof(*m)); m->enabled=1; m->review_threshold=25; m->block_threshold=50; }
void slhsm_set_enabled(SLSystemMonitor *m,int e){if(m)m->enabled=e!=0;}
void slhsm_set_strict(SLSystemMonitor *m,int s){if(m)m->strict=s!=0;}
SLHSMLevel slhsm_observe(SLSystemMonitor *m,SLHSMObservation o,SLPlatformCapability c,unsigned cost) {
    unsigned w; if(!m||!m->enabled)return SL_HSM_OK; m->observations++; m->last_observation=o;
    if(o==SL_HSM_OBS_UNKNOWN_OPERATION)m->unknown_operations++;
    if(o==SL_HSM_OBS_NATIVE_ESCAPE)m->native_escapes++;
    if(o==SL_HSM_OBS_RESOURCE_IMBALANCE)m->resource_imbalance++;
    w=weight(o,cost);
    if(c>=0&&c<SL_CAP_MAX&&!slplatform_capability_available(c)){w+=10u;o=SL_HSM_OBS_UNSUPPORTED_CAPABILITY;m->last_observation=o;}
    m->risk_score=(UINT_MAX-m->risk_score<w)?UINT_MAX:m->risk_score+w;
    if(m->risk_score>=m->block_threshold)m->blocks++;
    else if(m->risk_score>=m->review_threshold)m->reviews++;
    m->level=m->risk_score>=m->block_threshold?SL_HSM_BLOCK:m->risk_score>=m->review_threshold?SL_HSM_REVIEW:SL_HSM_NOTICE;
    return m->strict&&m->level>=SL_HSM_REVIEW?SL_HSM_BLOCK:m->level;
}
SLHSMLevel slhsm_check_capability(SLSystemMonitor *m,SLPlatformCapability c){return slplatform_capability_available(c)?SL_HSM_OK:slhsm_observe(m,SL_HSM_OBS_UNSUPPORTED_CAPABILITY,c,1);}
SLHSMLevel slhsm_unknown_operation(SLSystemMonitor *m,const char *op){(void)op;return slhsm_observe(m,SL_HSM_OBS_UNKNOWN_OPERATION,-1,1);}
SLHSMLevel slhsm_native_escape(SLSystemMonitor *m,const char *op){(void)op;return slhsm_observe(m,SL_HSM_OBS_NATIVE_ESCAPE,-1,2);}
SLHSMLevel slhsm_privileged(SLSystemMonitor *m,const char *op){(void)op;return slhsm_observe(m,SL_HSM_OBS_PRIVILEGED_OPERATION,-1,2);}
SLHSMLevel slhsm_resource_burst(SLSystemMonitor *m,unsigned cost){return slhsm_observe(m,SL_HSM_OBS_RESOURCE_BURST,-1,cost);}
void slhsm_stats(const SLSystemMonitor *m,SLHSMStats *o){if(!m||!o)return; o->enabled=m->enabled;o->strict=m->strict;o->review_threshold=m->review_threshold;o->block_threshold=m->block_threshold;o->observations=m->observations;o->reviews=m->reviews;o->blocks=m->blocks;o->unknown_operations=m->unknown_operations;o->native_escapes=m->native_escapes;o->resource_imbalance=m->resource_imbalance;o->risk_score=m->risk_score;o->level=m->level;o->last_observation=m->last_observation;}
const char *slhsm_level_name(SLHSMLevel l){switch(l){case SL_HSM_OK:return "ok";case SL_HSM_NOTICE:return "notice";case SL_HSM_REVIEW:return "review";case SL_HSM_BLOCK:return "block";default:return "unknown";}}
const char *slhsm_observation_name(SLHSMObservation o){switch(o){case SL_HSM_OBS_NORMAL:return "normal";case SL_HSM_OBS_UNSUPPORTED_CAPABILITY:return "unsupported-capability";case SL_HSM_OBS_PLATFORM_MISMATCH:return "platform-mismatch";case SL_HSM_OBS_UNKNOWN_OPERATION:return "unknown-operation";case SL_HSM_OBS_NATIVE_ESCAPE:return "native-escape";case SL_HSM_OBS_PRIVILEGED_OPERATION:return "privileged-operation";case SL_HSM_OBS_RESOURCE_BURST:return "resource-burst";case SL_HSM_OBS_RESOURCE_IMBALANCE:return "resource-imbalance";default:return "unknown";}}
int slhsm_format_report(const SLSystemMonitor *m,char *buf,size_t cap){SLHSMStats s;if(!m||!buf)return -1;slhsm_stats(m,&s);return snprintf(buf,cap,"[system-monitor] enabled=%s strict=%s level=%s score=%u\n[system-monitor] observations=%llu reviews=%llu blocks=%llu unknown=%llu native=%llu imbalance=%llu last=%s\n",s.enabled?"yes":"no",s.strict?"yes":"no",slhsm_level_name(s.level),s.risk_score,(unsigned long long)s.observations,(unsigned long long)s.reviews,(unsigned long long)s.blocks,(unsigned long long)s.unknown_operations,(unsigned long long)s.native_escapes,(unsigned long long)s.resource_imbalance,slhsm_observation_name(s.last_observation));}
