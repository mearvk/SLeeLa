#ifndef SLEELA_SLVM4_RESOLVER_H
#define SLEELA_SLVM4_RESOLVER_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLVM4_RESOLVE_DNS_TO_IP=1, SLVM4_RESOLVE_IP_TO_DNS=2, SLVM4_RESOLVE_PATH=3 } slvm4_resolve_kind_t;
typedef struct {
 slvm4_resolve_kind_t kind;
 const char *input;
 const char *resolved;
 const char *source;
 uint64_t observed_at_ns;
 int authenticated;
} slvm4_resolution_result_t;
int slvm4_resolution_validate(const slvm4_resolution_result_t*);
#ifdef __cplusplus
}
#endif
#endif
