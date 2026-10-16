#ifndef SLVM8_SUPERVISOR_H
#define SLVM8_SUPERVISOR_H
#include "slvm8.h"
typedef struct { slvm8_phase_t phase; uint64_t epoch,fault_count,checkpoint_count,recovery_count; uint8_t admission_valid,policy_valid,capabilities_valid,managers_valid,integrity_valid; } slvm8_supervisor_t;
int slvm8_supervisor_validate(const slvm8_supervisor_t *s);
int slvm8_supervisor_admit(slvm8_supervisor_t *s);
int slvm8_supervisor_start(slvm8_supervisor_t *s);
int slvm8_supervisor_fault(slvm8_supervisor_t *s,int integrity_fault);
int slvm8_supervisor_checkpoint(slvm8_supervisor_t *s);
int slvm8_supervisor_recover(slvm8_supervisor_t *s);
int slvm8_supervisor_quarantine(slvm8_supervisor_t *s);
int slvm8_supervisor_stop(slvm8_supervisor_t *s);
#endif
