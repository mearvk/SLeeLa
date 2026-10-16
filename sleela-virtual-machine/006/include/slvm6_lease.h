#ifndef SLEELA_SLVM6_LEASE_H
#define SLEELA_SLVM6_LEASE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t object_id,lease_id,issued_at_ns,expires_at_ns,revocation_epoch; const char *holder; uint32_t rights,revoked; } slvm6_lease_t;
int slvm6_lease_validate(const slvm6_lease_t*,uint64_t now_ns,uint64_t current_revocation_epoch);
#ifdef __cplusplus
}
#endif
#endif
