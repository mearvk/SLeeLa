#ifndef SLVM8_CAPABILITY_H
#define SLVM8_CAPABILITY_H
#include "slvm8.h"
typedef struct { uint64_t lease_id,issued_epoch,expiry_epoch; uint32_t capability_id; uint8_t active,revocable,policy_valid; } slvm8_capability_lease_t;
int slvm8_capability_validate(const slvm8_capability_lease_t *l,uint64_t epoch);
int slvm8_capability_revoke(slvm8_capability_lease_t *l);
int slvm8_capability_renew(slvm8_capability_lease_t *l,uint64_t epoch,uint64_t extension);
#endif
