// ===========================================================================
// stp_crypto.h -- STP-0001 crypto (C side, OpenSSL 3 libcrypto backend).
//
// Mirrors the Java Crypto class byte-for-byte so the Sleela client and the
// SecureJDK 28 supervisor interoperate:
//   Ed25519 (identity/sign), X25519 (ephemeral ECDH), HKDF-SHA256,
//   ChaCha20-Poly1305 (IETF), SHA-256.
//
// Compiled only when STP_HAVE_OPENSSL is defined (link -lcrypto). When absent,
// the transition client degrades to "unreachable" and falls back to safe-trim.
// ===========================================================================
#ifndef SLEELA_STP_CRYPTO_H
#define SLEELA_STP_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// An Ed25519 or X25519 keypair holder (opaque EVP_PKEY under the hood).
typedef struct stp_key stp_key;

// ---- identity / ephemeral keys ----
stp_key* stp_ed25519_generate(void);
stp_key* stp_x25519_generate(void);
void     stp_key_free(stp_key* k);

// Export the 32-byte raw public key. Returns 0 on success.
int stp_key_raw_public(const stp_key* k, uint8_t out32[32]);

// Build a public key object from a raw 32-byte encoding (for verify/agree).
stp_key* stp_ed25519_from_raw_public(const uint8_t raw32[32]);
stp_key* stp_x25519_from_raw_public(const uint8_t raw32[32]);

// ---- Ed25519 sign / verify ----
// out_sig must be 64 bytes. Returns 0 on success.
int stp_ed25519_sign(const stp_key* priv, const uint8_t* msg, size_t mlen, uint8_t out_sig[64]);
// Returns 1 if valid, 0 otherwise.
int stp_ed25519_verify(const stp_key* pub, const uint8_t* msg, size_t mlen, const uint8_t sig[64]);

// ---- X25519 agreement (32-byte shared secret). Returns 0 on success. ----
int stp_x25519_agree(const stp_key* my_priv, const stp_key* their_pub, uint8_t out_ss[32]);

// ---- HKDF-SHA256 (RFC 5869). Returns 0 on success. ----
int stp_hkdf(const uint8_t* ikm, size_t ikm_len,
             const uint8_t* salt, size_t salt_len,
             const uint8_t* info, size_t info_len,
             uint8_t* out, size_t out_len);

// ---- HMAC-SHA256 PRF (region ack tag). out must be 32 bytes. ----
int stp_hmac_sha256(const uint8_t* key, size_t key_len,
                    const uint8_t* msg, size_t msg_len, uint8_t out32[32]);

// ---- ChaCha20-Poly1305 (IETF, 12-byte nonce) ----
// seal: out = ciphertext||tag (pt_len + 16). Returns 0 on success.
int stp_aead_seal(const uint8_t key32[32], const uint8_t nonce12[12],
                  const uint8_t* aad, size_t aad_len,
                  const uint8_t* pt, size_t pt_len, uint8_t* out, size_t* out_len);
// open: out = plaintext (ct_len - 16). Returns 0 on success, -1 on tag failure.
int stp_aead_open(const uint8_t key32[32], const uint8_t nonce12[12],
                  const uint8_t* aad, size_t aad_len,
                  const uint8_t* ct, size_t ct_len, uint8_t* out, size_t* out_len);

// ---- SHA-256 ----
int stp_sha256(const uint8_t* data, size_t len, uint8_t out32[32]);

// ---- random ----
int stp_random(uint8_t* out, size_t n);

// ---- hex helper ----
void stp_hex(const uint8_t* in, size_t n, char* out /* 2n+1 */);

#ifdef __cplusplus
}
#endif
#endif // SLEELA_STP_CRYPTO_H
