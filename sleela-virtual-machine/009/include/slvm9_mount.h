#ifndef SLVM9_MOUNT_H
#define SLVM9_MOUNT_H
#include "slvm9.h"
typedef struct { uint64_t mount_id,filesystem_id,generation; const char *path,*namespace_id; uint8_t active,verified,read_only; } slvm9_mount_t;
int slvm9_mount_validate(const slvm9_mount_t*); int slvm9_mount_is_current(const slvm9_mount_t*,uint64_t,uint64_t);
#endif
