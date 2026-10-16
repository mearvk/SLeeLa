#ifndef SLVM9_ADAPTER_H
#define SLVM9_ADAPTER_H
#include "slvm9.h"
typedef struct { slvm9_platform_t platform; const char *filesystem_type; uint32_t api_version; uint8_t available,verified; } slvm9_adapter_t;
int slvm9_adapter_validate(const slvm9_adapter_t*);
#endif
