// ===========================================================================
// stp_crypto_optional.h -- a small always-available SHA-256 for the driver to
// compute a parse digest, regardless of whether the OpenSSL-backed transition
// client is compiled in. Implemented in stp_sha256_portable.c (no deps).
// ===========================================================================
#ifndef SLEELA_STP_CRYPTO_OPTIONAL_H
#define SLEELA_STP_CRYPTO_OPTIONAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Portable SHA-256 (FIPS 180-4). out must be 32 bytes.
void stp_sha256_portable(const uint8_t* data, size_t len, uint8_t out32[32]);

#ifdef __cplusplus
}
#endif
#endif // SLEELA_STP_CRYPTO_OPTIONAL_H
