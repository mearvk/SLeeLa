#ifndef SLVM10_OPERATION_H
#define SLVM10_OPERATION_H
#include "slvm10.h"
typedef enum { SLVM10_OPEN=1,SLVM10_READ_OP=2,SLVM10_WRITE_OP=3,SLVM10_SYNC_OP=4,SLVM10_RENAME_OP=5,SLVM10_DELETE_OP=6,SLVM10_CHECKPOINT_OP=7,SLVM10_PROBE_OP=8,SLVM10_REVALIDATE_OP=9 } slvm10_operation_kind_t;
typedef struct { slvm10_operation_kind_t kind; uint64_t object_id,generation,bytes; uint32_t required_capabilities; uint8_t admitted,verified,completed,durable; } slvm10_operation_t;
int slvm10_operation_validate(const slvm10_operation_t *op);
#endif
