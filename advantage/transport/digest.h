/*
 * digest.h - SHA-256 helpers used for content addressing and integrity.
 *
 * Content-addressed storage keys downloaded bytes by their SHA-256, which is
 * what makes the acquisition layer idempotent: the same content always maps
 * to the same object id, so re-downloading is a no-op.
 */
#ifndef ADVANTAGE_DIGEST_H
#define ADVANTAGE_DIGEST_H

#include <stddef.h>
#include <stdint.h>
#include <string>

namespace advantage {

// Lowercase hex SHA-256 of a byte range.
std::string sha256_hex(const uint8_t *data, size_t len);

// Lowercase hex SHA-256 of a file's contents; empty string on read error.
std::string sha256_file(const std::string &path);

}  // namespace advantage

#endif  // ADVANTAGE_DIGEST_H
