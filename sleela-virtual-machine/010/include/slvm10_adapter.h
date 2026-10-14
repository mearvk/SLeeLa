#ifndef SLVM10_ADAPTER_H
#define SLVM10_ADAPTER_H
#include "slvm10.h"
typedef struct { slvm10_os_t os; const char *name,*filesystem_family; uint32_t api_version,capabilities; uint8_t available,verified; } slvm10_adapter_t;
int slvm10_adapter_validate(const slvm10_adapter_t *a);
#endif
