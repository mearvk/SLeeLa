#ifndef SLVM10_H
#define SLVM10_H
#include <stdint.h>
#define SLVM10_OK 0
#define SLVM10_INVALID -1
#define SLVM10_DENIED -2
#define SLVM10_UNSUPPORTED -3
#define SLVM10_STALE -7
typedef enum { SLVM10_LINUX=1, SLVM10_WINDOWS=2, SLVM10_MACOS=3, SLVM10_OTHER=255 } slvm10_os_t;
typedef enum { SLVM10_NEW=0, SLVM10_ADMITTED=1, SLVM10_RUNNING=2, SLVM10_QUIESCING=3, SLVM10_DEGRADED=4, SLVM10_RECOVERING=5, SLVM10_QUARANTINED=6, SLVM10_STOPPED=7 } slvm10_phase_t;
typedef struct { slvm10_os_t os; const char *name; uint64_t epoch; uint8_t filesystem_verified,adapter_verified,identity_verified,policy_verified; } slvm10_state_t;
int slvm10_validate(const slvm10_state_t *s);
int slvm10_admit(slvm10_state_t *s);
int slvm10_start(slvm10_state_t *s);
int slvm10_quiesce(slvm10_state_t *s);
int slvm10_recover(slvm10_state_t *s);
int slvm10_quarantine(slvm10_state_t *s);
int slvm10_stop(slvm10_state_t *s);
#endif
