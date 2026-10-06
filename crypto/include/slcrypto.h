/*
 * crypto/include/slcrypto.h
 * SLeeLa National-Grade Cryptography — public C API.
 *
 * This header declares standards-faithful reference implementations of the
 * cryptographic primitives that make up the modern "national grade" / U.S.
 * government algorithm suites (NSA CNSA 1.0 / 2.0, built from open NIST FIPS
 * and SP standards):
 *
 *   Hashing      SHA-256, SHA-384, SHA-512        FIPS 180-4
 *                SHA3-256, SHA3-512 (Keccak)       FIPS 202
 *   Symmetric    AES-128/192/256                   FIPS 197
 *                AES-GCM (AEAD)                     NIST SP 800-38D
 *   MAC          HMAC                              FIPS 198-1 / RFC 2104
 *   KDF          HKDF (extract/expand)             RFC 5869
 *   Post-quantum ML-KEM (Kyber)                    FIPS 203  (interface)
 *                ML-DSA (Dilithium)                FIPS 204  (interface)
 *
 * SECURITY NOTE (read before use):
 *   These are CORRECT, SPECIFICATION-FAITHFUL REFERENCE implementations meant
 *   for study, interoperability testing, and use inside the SLeeLa toolchain.
 *   They are NOT hardened for production: they are not guaranteed constant-time
 *   and are not side-channel resistant. For data that must actually be
 *   protected, use an audited library (BoringSSL, OpenSSL, libsodium, liboqs).
 *   This boundary is intentional and is stated again at each primitive.
 *
 * All algorithms are public, published standards. Nothing here weakens,
 * backdoors, or circumvents cryptography.
 */
#ifndef SLCRYPTO_H
#define SLCRYPTO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Return codes -------------------------------------------------------- */
typedef enum {
    SLCRYPTO_OK = 0,
    SLCRYPTO_ERR_PARAM = -1,   /* bad argument */
    SLCRYPTO_ERR_AUTH  = -2,   /* authentication / tag verification failed */
    SLCRYPTO_ERR_UNIMPL = -3   /* interface declared, implementation pending */
} slcrypto_status;

/* ========================================================================= *
 * SHA-2  (FIPS 180-4)                                                        *
 * ========================================================================= */
#define SLC_SHA256_DIGEST_LEN 32
#define SLC_SHA384_DIGEST_LEN 48
#define SLC_SHA512_DIGEST_LEN 64
#define SLC_SHA256_BLOCK      64
#define SLC_SHA512_BLOCK      128

typedef struct { uint32_t h[8]; uint64_t len; uint8_t buf[SLC_SHA256_BLOCK]; size_t n; } slc_sha256_ctx;
typedef struct { uint64_t h[8]; uint64_t len_hi, len_lo; uint8_t buf[SLC_SHA512_BLOCK]; size_t n; } slc_sha512_ctx;

void slc_sha256_init(slc_sha256_ctx *c);
void slc_sha256_update(slc_sha256_ctx *c, const void *data, size_t len);
void slc_sha256_final(slc_sha256_ctx *c, uint8_t out[SLC_SHA256_DIGEST_LEN]);
void slc_sha256(const void *data, size_t len, uint8_t out[SLC_SHA256_DIGEST_LEN]);

/* SHA-384 and SHA-512 share the 64-bit core; init selects the variant. */
void slc_sha512_init(slc_sha512_ctx *c);
void slc_sha384_init(slc_sha512_ctx *c);
void slc_sha512_update(slc_sha512_ctx *c, const void *data, size_t len);
void slc_sha512_final(slc_sha512_ctx *c, uint8_t *out /* 64 or 48 bytes */);
void slc_sha512(const void *data, size_t len, uint8_t out[SLC_SHA512_DIGEST_LEN]);
void slc_sha384(const void *data, size_t len, uint8_t out[SLC_SHA384_DIGEST_LEN]);

/* ========================================================================= *
 * SHA-3 / Keccak  (FIPS 202)                                                 *
 * ========================================================================= */
#define SLC_SHA3_256_DIGEST_LEN 32
#define SLC_SHA3_512_DIGEST_LEN 64

typedef struct {
    uint64_t state[25];
    size_t   rate;      /* bytes absorbed per permutation */
    size_t   absorbed;  /* bytes currently in the rate buffer */
    uint8_t  buf[200];
} slc_sha3_ctx;

void slc_sha3_256_init(slc_sha3_ctx *c);
void slc_sha3_512_init(slc_sha3_ctx *c);
void slc_sha3_update(slc_sha3_ctx *c, const void *data, size_t len);
void slc_sha3_final(slc_sha3_ctx *c, uint8_t *out /* 32 or 64 bytes */);
void slc_sha3_256(const void *data, size_t len, uint8_t out[SLC_SHA3_256_DIGEST_LEN]);
void slc_sha3_512(const void *data, size_t len, uint8_t out[SLC_SHA3_512_DIGEST_LEN]);

/* ========================================================================= *
 * AES  (FIPS 197) + GCM AEAD  (SP 800-38D)                                   *
 * ========================================================================= */
#define SLC_AES_BLOCK 16

typedef struct {
    uint32_t rk[60];  /* round keys (enough for AES-256) */
    int      rounds;  /* 10 / 12 / 14 */
} slc_aes_key;

/* key_bits must be 128, 192, or 256. */
slcrypto_status slc_aes_set_encrypt_key(slc_aes_key *k, const uint8_t *key, int key_bits);
slcrypto_status slc_aes_set_decrypt_key(slc_aes_key *k, const uint8_t *key, int key_bits);
void slc_aes_encrypt_block(const slc_aes_key *k, const uint8_t in[16], uint8_t out[16]);
void slc_aes_decrypt_block(const slc_aes_key *k, const uint8_t in[16], uint8_t out[16]);

/*
 * AES-GCM authenticated encryption (SP 800-38D).
 *   key_bits : 128 | 192 | 256
 *   iv       : nonce (96-bit / 12-byte recommended)
 *   aad      : additional authenticated data (not encrypted)
 *   tag_len  : 1..16 (16 recommended)
 * Encryption writes ciphertext (same length as plaintext) and the tag.
 * Decryption verifies the tag before returning plaintext; a bad tag yields
 * SLCRYPTO_ERR_AUTH and no plaintext is released.
 */
slcrypto_status slc_aes_gcm_encrypt(
    const uint8_t *key, int key_bits,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *pt, size_t pt_len,
    uint8_t *ct_out,
    uint8_t *tag_out, size_t tag_len);

slcrypto_status slc_aes_gcm_decrypt(
    const uint8_t *key, int key_bits,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *ct, size_t ct_len,
    const uint8_t *tag, size_t tag_len,
    uint8_t *pt_out);

/* ========================================================================= *
 * HMAC  (FIPS 198-1 / RFC 2104)   — parameterized by hash                    *
 * ========================================================================= */
void slc_hmac_sha256(const uint8_t *key, size_t key_len,
                     const uint8_t *msg, size_t msg_len,
                     uint8_t out[SLC_SHA256_DIGEST_LEN]);
void slc_hmac_sha512(const uint8_t *key, size_t key_len,
                     const uint8_t *msg, size_t msg_len,
                     uint8_t out[SLC_SHA512_DIGEST_LEN]);

/* ========================================================================= *
 * HKDF  (RFC 5869)  over HMAC-SHA-256                                        *
 * ========================================================================= */
void slc_hkdf_sha256(const uint8_t *salt, size_t salt_len,
                     const uint8_t *ikm, size_t ikm_len,
                     const uint8_t *info, size_t info_len,
                     uint8_t *okm, size_t okm_len);

/* ========================================================================= *
 * Post-quantum (CNSA 2.0)  — interface declared, implementation pending      *
 * ---------------------------------------------------------------------------
 * ML-KEM (FIPS 203, Kyber) and ML-DSA (FIPS 204, Dilithium) are the NIST
 * post-quantum standards named by CNSA 2.0. Their reference implementations
 * are large (lattice arithmetic, NTT, sampling) and are best vendored from
 * liboqs/pq-crystals rather than hand-rolled. These entry points declare the
 * stable surface; they currently return SLCRYPTO_ERR_UNIMPL. See
 * crypto/PQ-INTEGRATION.md for the integration plan.
 * ========================================================================= */
typedef enum { SLC_MLKEM_512, SLC_MLKEM_768, SLC_MLKEM_1024 } slc_mlkem_param;
typedef enum { SLC_MLDSA_44, SLC_MLDSA_65, SLC_MLDSA_87 }     slc_mldsa_param;

slcrypto_status slc_mlkem_keygen(slc_mlkem_param p, uint8_t *pk, uint8_t *sk);
slcrypto_status slc_mlkem_encaps(slc_mlkem_param p, const uint8_t *pk, uint8_t *ct, uint8_t *shared);
slcrypto_status slc_mlkem_decaps(slc_mlkem_param p, const uint8_t *sk, const uint8_t *ct, uint8_t *shared);

slcrypto_status slc_mldsa_keygen(slc_mldsa_param p, uint8_t *pk, uint8_t *sk);
slcrypto_status slc_mldsa_sign(slc_mldsa_param p, const uint8_t *sk,
                               const uint8_t *msg, size_t msg_len,
                               uint8_t *sig, size_t *sig_len);
slcrypto_status slc_mldsa_verify(slc_mldsa_param p, const uint8_t *pk,
                                 const uint8_t *msg, size_t msg_len,
                                 const uint8_t *sig, size_t sig_len);

/* ---- Utility ------------------------------------------------------------- */
/* Constant-time comparison; returns 1 if equal, 0 otherwise. */
int slc_ct_equal(const void *a, const void *b, size_t len);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SLCRYPTO_H */
