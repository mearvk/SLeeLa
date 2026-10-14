#ifndef SLVM8_POLICY_H
#define SLVM8_POLICY_H
#include "slvm8.h"
typedef struct { uint64_t version,max_memory,max_cpu,max_io,max_network; uint32_t capability_count,required_signers,valid_signers; const char *policy_hash; uint8_t immutable,valid; } slvm8_policy_t;
int slvm8_policy_validate(const slvm8_policy_t *p);
int slvm8_policy_can_admit(const slvm8_policy_t *p);
#endif
