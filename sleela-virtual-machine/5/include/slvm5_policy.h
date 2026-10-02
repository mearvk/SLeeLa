#ifndef SLEELA_SLVM5_POLICY_H
#define SLEELA_SLVM5_POLICY_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint64_t version;
 const char *hash;
 const char *issuer;
 uint64_t effective_at_ns;
 uint64_t expires_at_ns;
 uint32_t immutable;
} slvm5_policy_snapshot_t;
int slvm5_policy_validate(const slvm5_policy_snapshot_t*, uint64_t now_ns);
#ifdef __cplusplus
}
#endif
#endif
