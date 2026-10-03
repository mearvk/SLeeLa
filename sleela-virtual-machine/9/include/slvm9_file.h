#ifndef SLVM9_FILE_H
#define SLVM9_FILE_H
#include "slvm9.h"
typedef struct { uint64_t object_id,parent_id,generation,size; const char *name; uint32_t flags; uint8_t identity_valid,readable,writable; } slvm9_file_t;
int slvm9_file_validate(const slvm9_file_t*); int slvm9_file_can_write(const slvm9_file_t*);
#endif
