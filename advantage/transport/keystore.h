/*
 * keystore.h - Encrypted-at-rest secret/key store for the acquisition layer.
 *
 * Stores named secrets (passwords, API tokens, private key material, derived
 * session keys) in a single file that is encrypted with AES-256-GCM. The
 * file encryption key is derived from a passphrase with PBKDF2-HMAC-SHA-256
 * (a large, tunable iteration count). Every record is individually sealed
 * with its own nonce and authentication tag, so tampering is detected on
 * read.
 *
 * Security properties:
 *   - Confidentiality + integrity at rest (AES-256-GCM, per-record nonce/tag).
 *   - Passphrase never stored; only the PBKDF2 salt and parameters are.
 *   - Atomic, idempotent writes: the store is serialized to a temp file and
 *     rename(2)'d into place, so a crash never leaves a half-written store.
 *   - Secret buffers are zeroed before being freed.
 *
 * This is a thin, auditable wrapper over OpenSSL's EVP interfaces; it does
 * not invent cryptography.
 */
#ifndef ADVANTAGE_KEYSTORE_H
#define ADVANTAGE_KEYSTORE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ks_store ks_store;

typedef enum {
    KS_OK = 0,
    KS_ERR_IO = -1,       /* file open/read/write/rename failed            */
    KS_ERR_CRYPTO = -2,   /* OpenSSL primitive failed                      */
    KS_ERR_AUTH = -3,     /* wrong passphrase or tampered data (GCM fail)  */
    KS_ERR_FORMAT = -4,   /* corrupt or unrecognized store format          */
    KS_ERR_NOTFOUND = -5, /* no record with the requested name             */
    KS_ERR_ARG = -6,      /* invalid argument                              */
    KS_ERR_NOMEM = -7     /* allocation failed                             */
} ks_status;

/*
 * Open (or create, if absent) a keystore backed by `path`, unlocked with
 * `passphrase`. If the file exists it is decrypted and verified; a wrong
 * passphrase or tampering yields KS_ERR_AUTH. On success *out receives an
 * owned handle that must be released with ks_close().
 */
ks_status ks_open(const char *path, const char *passphrase, ks_store **out);

/*
 * Store (or replace) the secret named `name`. Idempotent: storing the same
 * name+value twice leaves the store in the same logical state. The change is
 * persisted atomically before returning KS_OK.
 */
ks_status ks_put(ks_store *ks, const char *name,
                 const uint8_t *value, size_t value_len);

/* Convenience for NUL-terminated string secrets. */
ks_status ks_put_str(ks_store *ks, const char *name, const char *value);

/*
 * Fetch the secret named `name`. On success *out_value is a freshly
 * allocated buffer (owned by the caller; free with ks_free_secret) and
 * *out_len its length. Returns KS_ERR_NOTFOUND if absent.
 */
ks_status ks_get(ks_store *ks, const char *name,
                 uint8_t **out_value, size_t *out_len);

/* Remove the named secret. Idempotent: removing an absent name is KS_OK. */
ks_status ks_remove(ks_store *ks, const char *name);

/* Number of records currently held. */
size_t ks_count(const ks_store *ks);

/*
 * List record names into caller-provided array `names` (each NUL-terminated,
 * caller frees each with free()). Writes up to `max` names; returns the count
 * written, or a negative ks_status on error.
 */
int ks_list(const ks_store *ks, char **names, size_t max);

/* Securely wipe and free a secret returned by ks_get. */
void ks_free_secret(uint8_t *value, size_t len);

/* Flush any pending state and release the handle. */
void ks_close(ks_store *ks);

/* Human-readable message for a status code. */
const char *ks_strerror(ks_status st);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_KEYSTORE_H */
