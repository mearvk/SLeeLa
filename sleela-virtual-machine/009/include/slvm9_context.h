#ifndef SLVM9_CONTEXT_H
#define SLVM9_CONTEXT_H
#include "slvm9.h"
typedef struct { uint64_t file_id,context_id,parent_id,version,revision; const char *name,*filesystem_type,*mount_point; uint8_t identity_valid; } slvm9_context_identity_t;
int slvm9_context_validate(const slvm9_context_identity_t*);
#endif
