#ifndef HTTP80_CRYPTO_H
#define HTTP80_CRYPTO_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HTTP80_CRYPTO_NAME_MAX 48
typedef enum { HTTP80_CRYPTO_DIGEST=1, HTTP80_CRYPTO_MAC, HTTP80_CRYPTO_KDF, HTTP80_CRYPTO_AEAD, HTTP80_CRYPTO_BLOCK, HTTP80_CRYPTO_STREAM, HTTP80_CRYPTO_KEY_AGREEMENT, HTTP80_CRYPTO_KEM, HTTP80_CRYPTO_SIGNATURE, HTTP80_CRYPTO_PKE, HTTP80_CRYPTO_RANDOM } http80_crypto_operation;
typedef enum { HTTP80_CRYPTO_STANDARD=1, HTTP80_CRYPTO_LEGACY=2, HTTP80_CRYPTO_PROVIDER=3, HTTP80_CRYPTO_EXTERNAL=4 } http80_crypto_source;
typedef struct { const char *name; const char *standard; http80_crypto_operation operation; http80_crypto_source source; int enabled_by_default; int approved_for_new_sessions; } http80_crypto_algorithm;
typedef struct { const char *algorithm; const char *provider; int available; int approved; char reason[128]; } http80_crypto_selection;
size_t http80_crypto_catalog(const http80_crypto_algorithm **out);
const http80_crypto_algorithm *http80_crypto_find(const char *name);
int http80_crypto_is_approved(const char *name);
int http80_crypto_select(const char *name, http80_crypto_selection *selection);
int http80_crypto_validate_transition(const char *key_agreement, const char *kem, const char *kdf, const char *aead, const char *signature);
#ifdef __cplusplus
}
#endif
#endif
