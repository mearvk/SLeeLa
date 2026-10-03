#ifndef SLEELA_SLVM2_CRYPTO_H
#define SLEELA_SLVM2_CRYPTO_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct slvm2_crypto_provider slvm2_crypto_provider_t;
typedef struct {
 int (*digest)(slvm2_crypto_provider_t*, const void*, size_t, void*, size_t*);
 int (*verify_signature)(slvm2_crypto_provider_t*, const void*, size_t, const void*, size_t, const void*, size_t);
} slvm2_crypto_provider_api_t;
struct slvm2_crypto_provider { const char *name; uint32_t version; const slvm2_crypto_provider_api_t *api; void *context; };
int slvm2_crypto_validate_provider(const slvm2_crypto_provider_t*);
#ifdef __cplusplus
}
#endif
#endif
