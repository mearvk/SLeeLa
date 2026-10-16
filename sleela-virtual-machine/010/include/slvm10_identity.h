#ifndef SLVM10_IDENTITY_H
#define SLVM10_IDENTITY_H
#include "slvm10.h"
typedef struct { uint64_t filesystem_id,generation,object_id,context_id,version,revision; const char *filesystem_type, *name, *mount_identity; uint8_t verified; } slvm10_identity_t;
int slvm10_identity_validate(const slvm10_identity_t *i);
int slvm10_identity_matches(const slvm10_identity_t *a,const slvm10_identity_t *b);
#endif
