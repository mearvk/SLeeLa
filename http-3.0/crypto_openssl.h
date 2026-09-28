#ifndef SLEELA_HTTP3_CRYPTO_OPENSSL_H
#define SLEELA_HTTP3_CRYPTO_OPENSSL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_OPENSSL_X25519_KEY_SIZE 32U
#define HTTP3_OPENSSL_AES_KEY_SIZE 32U
#define HTTP3_OPENSSL_GCM_NONCE_SIZE 12U
#define HTTP3_OPENSSL_GCM_TAG_SIZE 16U
#define HTTP3_OPENSSL_HKDF_SIZE 32U

/* OpenSSL 3 EVP provider-backed cryptographic operations. */
int http3_openssl_x25519_generate(uint8_t private_key[32],
                                  uint8_t public_key[32]);

int http3_openssl_x25519_derive(const uint8_t private_key[32],
                                const uint8_t peer_public_key[32],
                                uint8_t shared_secret[32]);

int http3_openssl_hkdf_sha256(const uint8_t *secret,
                              size_t secret_len,
                              const uint8_t *salt,
                              size_t salt_len,
                              const uint8_t *info,
                              size_t info_len,
                              uint8_t output[32]);

int http3_openssl_random(uint8_t *output, size_t output_len);

int http3_openssl_aes256gcm_encrypt(const uint8_t key[32],
                                    const uint8_t nonce[12],
                                    const uint8_t *aad,
                                    size_t aad_len,
                                    const uint8_t *plaintext,
                                    size_t plaintext_len,
                                    uint8_t *ciphertext,
                                    uint8_t tag[16]);

int http3_openssl_aes256gcm_decrypt(const uint8_t key[32],
                                    const uint8_t nonce[12],
                                    const uint8_t *aad,
                                    size_t aad_len,
                                    const uint8_t *ciphertext,
                                    size_t ciphertext_len,
                                    const uint8_t tag[16],
                                    uint8_t *plaintext);

#ifdef __cplusplus
}
#endif

#endif
