#ifndef SLEELA_SLVM4_DELEGATION_H
#define SLEELA_SLVM4_DELEGATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint64_t delegation_id;
 uint64_t issued_at_ns;
 uint64_t expires_at_ns;
 uint64_t capability_mask;
 uint64_t object_id;
 const char *peer_identity;
 const char *issuer;
} slvm4_capability_delegation_t;

int slvm4_delegation_validate(const slvm4_capability_delegation_t*, uint64_t now_ns);
#ifdef __cplusplus
}
#endif
#endif
