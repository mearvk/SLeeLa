#ifndef SLEELAVIRIN_NATIVE_H
#define SLEELAVIRIN_NATIVE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Compute SHA-256 for a file without third-party crypto dependencies. */
int sleelavirin_sha256_file(const char *path, uint8_t digest[32]);

/* Compute SHA-256 over an in-memory buffer. Returns 0 on success, -1 on bad
 * arguments. 'data' may be NULL only when 'length' is 0. */
int sleelavirin_sha256_buffer(const uint8_t *data, size_t length,
                              uint8_t digest[32]);

/* Scan one file against a text signature database.
 * Database rows: 64 hexadecimal SHA-256 characters, optionally followed by a name.
 * Returns 1 for a signature match, 0 for no match, and -1 for an error.
 * A no-match result is not a proof that the file is safe.
 */
int sleelavirin_native_scan_file(const char *path, const char *database,
                                 char *matched_name, size_t matched_name_size);

#ifdef __cplusplus
}
#endif
#endif
