#ifndef SLVM9_FILESYSTEM_H
#define SLVM9_FILESYSTEM_H
#include "slvm9.h"
typedef struct { uint64_t filesystem_id,generation,total_bytes,free_bytes,block_size; const char *type,*uuid,*mount_identity; uint32_t feature_bits; uint8_t mounted,read_only,integrity_valid,durability_known; } slvm9_filesystem_t;
enum { SLVM9_FS_READ=1u<<0,SLVM9_FS_WRITE=1u<<1,SLVM9_FS_ATOMIC_RENAME=1u<<2,SLVM9_FS_TRANSACTIONS=1u<<3,SLVM9_FS_DURABLE_FLUSH=1u<<4,SLVM9_FS_CONTEXT_ID=1u<<5,SLVM9_FS_RECOVERY=1u<<6,SLVM9_FS_NATIVE_HANDLES=1u<<7 };
int slvm9_filesystem_validate(const slvm9_filesystem_t*); int slvm9_filesystem_can(const slvm9_filesystem_t*,uint32_t); int slvm9_filesystem_generation_valid(const slvm9_filesystem_t*,uint64_t);
#endif
