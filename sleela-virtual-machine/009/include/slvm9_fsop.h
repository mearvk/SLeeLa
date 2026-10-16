#ifndef SLVM9_FSOP_H
#define SLVM9_FSOP_H
#include "slvm9.h"
typedef enum { SLVM9_FSOP_OPEN=1,SLVM9_FSOP_READ=2,SLVM9_FSOP_WRITE=3,SLVM9_FSOP_SYNC=4,SLVM9_FSOP_RENAME=5,SLVM9_FSOP_DELETE=6,SLVM9_FSOP_CHECKPOINT=7 } slvm9_fsop_kind_t;
typedef struct { slvm9_fsop_kind_t kind; uint64_t object_id,expected_generation,bytes; uint32_t flags; uint8_t admitted,completed,durable; } slvm9_fsop_t;
int slvm9_fsop_validate(const slvm9_fsop_t*);
#endif
