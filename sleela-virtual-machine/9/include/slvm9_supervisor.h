#ifndef SLVM9_SUPERVISOR_H
#define SLVM9_SUPERVISOR_H
#include "slvm9.h"
typedef enum { SLVM9_NORMAL=0,SLVM9_ADMITTED=1,SLVM9_RUNNING=2,SLVM9_DEGRADED=3,SLVM9_RECOVERING=4,SLVM9_QUIESCING=5,SLVM9_QUARANTINED=6,SLVM9_STOPPED=7 } slvm9_phase_t;
typedef struct { slvm9_phase_t phase; uint64_t epoch,filesystem_generation,faults; uint8_t policy_valid,filesystem_valid,adapter_valid,capabilities_valid,integrity_valid; } slvm9_supervisor_t;
int slvm9_supervisor_validate(const slvm9_supervisor_t*); int slvm9_supervisor_admit(slvm9_supervisor_t*); int slvm9_supervisor_start(slvm9_supervisor_t*); int slvm9_supervisor_fault(slvm9_supervisor_t*,int); int slvm9_supervisor_quarantine(slvm9_supervisor_t*); int slvm9_supervisor_stop(slvm9_supervisor_t*);
#endif
