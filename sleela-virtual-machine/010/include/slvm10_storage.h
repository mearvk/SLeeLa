#ifndef SLVM10_STORAGE_H
#define SLVM10_STORAGE_H
#include "slvm10.h"
typedef struct { uint64_t filesystem_id,generation,capacity,free_bytes,block_size; const char *type, *identity; uint32_t capabilities; uint8_t mounted,read_only,integrity,durable_known; } slvm10_storage_t;
enum { SLVM10_READ=1u<<0,SLVM10_WRITE=1u<<1,SLVM10_SYNC=1u<<2,SLVM10_ATOMIC=1u<<3,SLVM10_RECOVERY=1u<<4,SLVM10_CONTEXT=1u<<5,SLVM10_NATIVE=1u<<6 };
int slvm10_storage_validate(const slvm10_storage_t *s);
int slvm10_storage_has(const slvm10_storage_t *s,uint32_t capabilities);
int slvm10_storage_generation(const slvm10_storage_t *s,uint64_t generation);
#endif
