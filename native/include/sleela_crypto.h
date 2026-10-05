/*
 * native/include/sleela_crypto.h
 * SLeeLa Native Cryptography Bridge — stable C ABI.
 *
 * This header is the explicit VM/OS bridge below the SLeeLa /lib/crypto
 * sophistication classes (SLRadix, SLCryptoBlock, SLIntermix*, SLCryptoSeries,
 * SLCryptoComparator, SLNationalRegister, ...). The SLeeLa front end declares
 * the object model; the deterministic radix/intermix/compare primitives live
 * here in C/C++ because they are value-calculation primitives the language
 * delegates across the bridge.
 *
 * SECURITY NOTE: Mirroring the reference AES2 module, these transforms are
 * deterministic, reversible mixed-radix conversions ORed with constants. They
 * are OBFUSCATION, not cryptographic protection. Do not use them to protect
 * sensitive data; use a vetted AEAD cipher (AES-256-GCM, ChaCha20-Poly1305).
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#ifndef SLEELA_CRYPTO_H
#define SLEELA_CRYPTO_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Supported radix range for the general converter. */
#define SLEELA_CRYPTO_MIN_BASE 1
#define SLEELA_CRYPTO_MAX_BASE 2055

/* Main-series ordered step bounds. */
#define SLEELA_CRYPTO_MIN_ORDER 1
#define SLEELA_CRYPTO_MAX_ORDER 255

/* Maximum number of initial plaintext fields. */
#define SLEELA_CRYPTO_MAX_FIELDS 255

/*
 * Encode a non-negative value into the given base (1..2055).
 * Base 1 produces a unary tally. Writes a NUL-terminated digit string into out
 * (capacity out_cap). Returns the number of characters written (excluding the
 * NUL), or -1 on invalid base / insufficient buffer.
 */
int sleela_crypto_radix_to_base(int base, int64_t value, char *out, size_t out_cap);

/*
 * Decode a NUL-terminated digit string in the given base back to its value.
 * Returns the decoded value, or 0 and sets *ok = 0 on error (ok may be NULL).
 */
int64_t sleela_crypto_radix_from_base(int base, const char *digits, int *ok);

/*
 * One ordered main-series step: parse the running text as an integer, convert
 * through base, OR with mask, and return the decimal string of the result.
 * Writes into out (capacity out_cap). Returns written length or -1 on error.
 * The order argument is carried for diagnostics/ordering only.
 */
int sleela_crypto_block_advance(int order, int base, int64_t mask,
                                const char *running, char *out, size_t out_cap);

/*
 * Pass-two style symmetry-row intermix at three (base,mask) row points.
 * Returns written length or -1 on error.
 */
int sleela_crypto_block_two_rows(const char *running,
                                 int base_row2, int64_t mask_row2,
                                 int base_row7, int64_t mask_row7,
                                 int base_row6, int64_t mask_row6,
                                 char *out, size_t out_cap);

/*
 * Primary intermix (positions 7,2,6,1) over a span. Returns written length or -1.
 */
int sleela_crypto_intermix_primary(const char *running, int span,
                                   int pos7, int base7, int64_t mask7,
                                   int pos2, int base2, int64_t mask2,
                                   int pos6, int base6, int64_t mask6,
                                   int pos1, int base1, int64_t mask1,
                                   char *out, size_t out_cap);

/*
 * Secondary intermix (positions 17,2,3) over a span. Returns written length or -1.
 */
int sleela_crypto_intermix_secondary(const char *running, int span,
                                     int pos17, int base17, int64_t mask17,
                                     int pos2,  int base2,  int64_t mask2,
                                     int pos3,  int base3,  int64_t mask3,
                                     char *out, size_t out_cap);

/*
 * Length-independent (non-short-circuiting) comparison of expected vs actual.
 * Returns 1 on match, 0 otherwise.
 */
int sleela_crypto_compare(const char *expected, const char *actual);

/*
 * National-register call stub. Records (registry_id, value) through the OS
 * bridge. Returns 1 on acceptance, 0 on refusal. This reference build logs the
 * submission deterministically and accepts non-empty values.
 */
int sleela_crypto_national_register(const char *registry_id, const char *value);

/* Last human-readable error from any of the above (never NULL). */
const char *sleela_crypto_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CRYPTO_H */
