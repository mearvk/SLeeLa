#ifndef SLVM9_H
#define SLVM9_H
#include <stdint.h>
typedef enum { SLVM9_OK=0, SLVM9_INVALID=-1, SLVM9_DENIED=-2, SLVM9_UNSUPPORTED=-3, SLVM9_RESOURCE=-4, SLVM9_INTEGRITY=-5, SLVM9_RECOVERY=-6, SLVM9_STALE=-7 } slvm9_status_t;
typedef enum { SLVM9_LINUX=1, SLVM9_WINDOWS=2, SLVM9_MACOS=3, SLVM9_OTHER=255 } slvm9_os_t;
typedef struct { slvm9_os_t os; uint32_t major,minor; const char *name; uint8_t filesystem_access,transactions,integrity,durability,handles; } slvm9_platform_t;
int slvm9_platform_validate(const slvm9_platform_t*); const char *slvm9_os_name(slvm9_os_t);
#endif
