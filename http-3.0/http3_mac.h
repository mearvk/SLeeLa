/* ==========================================================================
 * http3_mac.h -- SLeeLa HTTP 3.0 keyed message authentication (SipHash-2-4).
 *
 * The per-packet DIGEST (see http3_envelope.h) is a keyed MAC, not a plain
 * hash. A plain hash detects accidental corruption; a keyed MAC additionally
 * resists DELIBERATE tampering, because an attacker who rewrites a packet
 * cannot recompute a matching tag without the secret key.
 *
 * SipHash-2-4 is a fast, well-analyzed 64-bit PRF/MAC keyed by a 128-bit key.
 * It is deterministic and endianness-independent, so the C and Python
 * references produce identical tags for the same key + message, preserving the
 * cross-language wire parity the protocol relies on. This implementation is
 * pure C (libc only) and carries no OpenSSL dependency, matching the protocol
 * core's boundary.
 *
 * The key is a PER-CONNECTION secret. In a real deployment it is derived from
 * the crypto substrate's key agreement (crypto_key_agreement.*); here it is a
 * caller-supplied 16-byte value so the data-flow layer stays independent of the
 * security layer's build (spec §1-§19 vs §2/§12 boundary).
 * ========================================================================== */
#ifndef HTTP3_MAC_H
#define HTTP3_MAC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SipHash uses a 128-bit (16-byte) key. */
#define HTTP3_MAC_KEY_BYTES 16u

/*
 * Compute the 64-bit SipHash-2-4 MAC of `data` (len bytes) under the 16-byte
 * `key`. Returns the tag as a host-order uint64_t (the value is what travels in
 * the DIGEST field; its byte serialization on the wire is handled by the
 * envelope's canonical big-endian packing).
 */
uint64_t http3_mac_siphash24(const uint8_t key[HTTP3_MAC_KEY_BYTES],
                             const void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_MAC_H */
