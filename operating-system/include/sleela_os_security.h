/*
 * operating-system/include/sleela_os_security.h
 * SLeeLa OS security review -- the C backing of the SECURITY-REVIEW capability
 * of lib/os (and the repo-wide SHA-256 verify-before-execute discipline).
 *
 * A fetched OS tree must be reviewed before it is built or run. This header
 * provides the native primitives for that review:
 *   - a self-contained SHA-256 (no external crypto dependency), so a digest can
 *     be computed over a buffer or a file;
 *   - a verifier that checks a digest against an expected hex digest (the
 *     verify-before-execution contract used across the repo);
 *   - the driver-admission policy surface (re-exposed from operating-system/base_drivers.h)
 *     so a security pass can decide whether an in-tree/vendor/unknown/foreign
 *     driver may load.
 *
 * SHA-256 here is a review/integrity tool, not a secrecy tool: it answers "is
 * this exactly the reviewed artifact?", matching tools/verify-before-execution.py.
 */
#ifndef SLEELA_OS_SECURITY_H
#define SLEELA_OS_SECURITY_H

#include <stddef.h>
#include <stdint.h>
#include "base_drivers.h" /* driver-admission policy, re-exposed for reviews */

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_OS_SHA256_BYTES 32          /* raw digest length */
#define SLEELA_OS_SHA256_HEX  (64 + 1)     /* lower-case hex + NUL */

/* Compute the SHA-256 of `len` bytes at `data` into the 32-byte `out`. */
void sleela_os_sha256(const void *data, size_t len, uint8_t out[SLEELA_OS_SHA256_BYTES]);

/* Format a 32-byte digest as lower-case hex into `hex` (must hold at least
 * SLEELA_OS_SHA256_HEX bytes). */
void sleela_os_sha256_hex(const uint8_t digest[SLEELA_OS_SHA256_BYTES],
                          char hex[SLEELA_OS_SHA256_HEX]);

/* Compute the SHA-256 hex of a file's contents into `hex`. Returns 0 on
 * success, -1 if the file cannot be opened/read. */
int sleela_os_sha256_file(const char *path, char hex[SLEELA_OS_SHA256_HEX]);

/* Verify that `data`/`len` hashes to `expected_hex` (case-insensitive compare).
 * Returns 1 on match, 0 on mismatch, -1 on a bad argument. This is the
 * verify-before-execute gate in one call. */
int sleela_os_verify_buffer(const void *data, size_t len, const char *expected_hex);

/* Verify a file against an expected hex digest. 1 match, 0 mismatch, -1 on a
 * read error or bad argument. */
int sleela_os_verify_file(const char *path, const char *expected_hex);

/* ---- Driver-admission review (re-exposed from base_drivers.h) ----------- */
/* Decide whether a driver of a given origin may load under the current policy.
 * A thin, documented alias over sleela_driver_admit_check so a security pass
 * has a single security-namespaced entry point. */
sleela_driver_admit sleela_os_review_driver(sleela_driver_origin origin,
                                            const char *foreign_family,
                                            const char *device_class);

/* 1 if an admission result means the driver loads (clean, tainted, or bridged);
 * 0 if it is refused or unsupported. */
int sleela_os_admit_is_load(sleela_driver_admit admit);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_SECURITY_H */
