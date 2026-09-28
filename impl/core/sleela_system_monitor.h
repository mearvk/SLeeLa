#ifndef SLEELA_SYSTEM_MONITOR_H
#define SLEELA_SYSTEM_MONITOR_H
#include <stddef.h>
#include <stdint.h>
#include "sleela_platform.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SL_HSM_OK=0, SL_HSM_NOTICE=1, SL_HSM_REVIEW=2, SL_HSM_BLOCK=3 } SLHSMLevel;
typedef enum { SL_HSM_OBS_NORMAL=0, SL_HSM_OBS_UNSUPPORTED_CAPABILITY, SL_HSM_OBS_PLATFORM_MISMATCH, SL_HSM_OBS_UNKNOWN_OPERATION, SL_HSM_OBS_NATIVE_ESCAPE, SL_HSM_OBS_PRIVILEGED_OPERATION, SL_HSM_OBS_RESOURCE_BURST, SL_HSM_OBS_RESOURCE_IMBALANCE } SLHSMObservation;
typedef struct { int enabled, strict; unsigned review_threshold, block_threshold; uint64_t observations,reviews,blocks,unknown_operations,native_escapes,resource_imbalance; unsigned risk_score; SLHSMLevel level; SLHSMObservation last_observation; } SLHSMStats;
typedef struct { int enabled, strict; unsigned review_threshold,block_threshold,risk_score; uint64_t observations,reviews,blocks,unknown_operations,native_escapes,resource_imbalance; SLHSMObservation last_observation; } SLSystemMonitor;
void slhsm_init(SLSystemMonitor *m);
void slhsm_set_enabled(SLSystemMonitor *m,int enabled);
void slhsm_set_strict(SLSystemMonitor *m,int strict);
SLHSMLevel slhsm_observe(SLSystemMonitor *m,SLHSMObservation observation,SLPlatformCapability capability,unsigned resource_cost);
SLHSMLevel slhsm_check_capability(SLSystemMonitor *m,SLPlatformCapability capability);
SLHSMLevel slhsm_unknown_operation(SLSystemMonitor *m,const char *operation);
SLHSMLevel slhsm_native_escape(SLSystemMonitor *m,const char *operation);
SLHSMLevel slhsm_privileged(SLSystemMonitor *m,const char *operation);
SLHSMLevel slhsm_resource_burst(SLSystemMonitor *m,unsigned cost);
void slhsm_stats(const SLSystemMonitor *m,SLHSMStats *out);
const char *slhsm_level_name(SLHSMLevel level);
const char *slhsm_observation_name(SLHSMObservation observation);
int slhsm_format_report(const SLSystemMonitor *m,char *buf,size_t cap);
#ifdef __cplusplus
}
#endif
#endif
