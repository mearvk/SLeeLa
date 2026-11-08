/*
 * kex.h - Diffie-Hellman / ECDH key exchange with HKDF session-key derivation.
 *
 * Wraps OpenSSL's EVP_PKEY key-agreement interface to perform a classic
 * ephemeral key exchange between two parties and derive a symmetric session
 * key from the shared secret via HKDF-SHA-256 (RFC 5869).
 *
 * Supported groups:
 *   KEX_X25519   - Curve25519 ECDH (modern default, fast, safe)
 *   KEX_P256     - NIST P-256 ECDH (prime256v1)
 *   KEX_FFDHE2048/3072 - finite-field DH (RFC 7919 named groups)
 *
 * Typical flow (each side):
 *   kex_keypair *kp; kex_generate(KEX_X25519, &kp);
 *   kex_export_public(kp, pub, &pub_len);        // send pub to peer
 *   ... receive peer_pub ...
 *   kex_derive(kp, peer_pub, peer_pub_len,
 *              "advantage/transport session", session_key, 32);
 *
 * Both sides, given each other's public keys, derive the identical session
 * key. The private key never leaves the process; shared secrets are zeroed
 * after derivation.
 */
#ifndef ADVANTAGE_KEX_H
#define ADVANTAGE_KEX_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KEX_X25519 = 0,
    KEX_P256 = 1,
    KEX_FFDHE2048 = 2,
    KEX_FFDHE3072 = 3
} kex_group;

typedef enum {
    KEX_OK = 0,
    KEX_ERR_ARG = -1,
    KEX_ERR_CRYPTO = -2,
    KEX_ERR_BUFFER = -3   /* output buffer too small */
} kex_status;

typedef struct kex_keypair kex_keypair;

/* Generate an ephemeral keypair for the chosen group. */
kex_status kex_generate(kex_group group, kex_keypair **out);

/*
 * Export this party's public key in the standard wire form for the group
 * (raw point for X25519, SubjectPublicKeyInfo DER for P-256 / FFDHE).
 * On entry *len is the capacity of `pub`; on return it is the bytes written.
 */
kex_status kex_export_public(const kex_keypair *kp, uint8_t *pub, size_t *len);

/*
 * Derive a session key of `key_len` bytes from the shared secret produced by
 * combining our private key with the peer's public key, mixed through
 * HKDF-SHA-256 using `info` as the context/application label. `info` may be
 * NULL. The raw shared secret is wiped internally and never exposed.
 */
kex_status kex_derive(const kex_keypair *kp,
                      const uint8_t *peer_pub, size_t peer_pub_len,
                      const char *info,
                      uint8_t *out_key, size_t key_len);

void kex_free(kex_keypair *kp);
const char *kex_strerror(kex_status st);
const char *kex_group_name(kex_group g);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_KEX_H */
