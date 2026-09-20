/* ==========================================================================
 * http3_envelope.h -- SLeeLa HTTP 3.0 compact application envelope (spec §5)
 * and response model (spec §7).
 *
 * The envelope is the unit of data that DRIVES the HTTP 3.0 application
 * protocol. It carries, in a fixed logical order:
 *
 *     VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID
 *             | DIGEST | INTACTX | PAYLOAD                            (§5)
 *
 * DIGEST is a per-packet 64-bit KEYED MAC (SipHash-2-4, see http3_mac.h) over
 * the header + payload under a per-connection secret key: it detects both
 * accidental corruption and DELIBERATE tampering, because a packet cannot be
 * rewritten with a matching tag without the key. INTACTX is a system-specific
 * 64-bit host-integrity identity (see http3_intactx.h): it fingerprints the
 * emitting host and encodes how far that host has drifted from its baseline, so
 * a tampered/changed machine reports a statically larger value and the receiver
 * can RESET the exchange.
 *
 * and a response carries:
 *
 *     STATUS  | REQUEST-ID | RESULT                                  (§7)
 *
 * Per the spec, the wire form MAY be textual (interoperable) or binary
 * (negotiated, high-performance). Both are implemented here so the same
 * logical envelope can travel either way (§5, §14 capability negotiation).
 * ========================================================================== */
#ifndef HTTP3_ENVELOPE_H
#define HTTP3_ENVELOPE_H

#include <stddef.h>
#include <stdint.h>

#include "http3_mac.h" /* HTTP3_MAC_KEY_BYTES, keyed-MAC DIGEST */

#ifdef __cplusplus
extern "C" {
#endif

/* Protocol generation of the envelope itself (distinct from any payload
 * version). "3" names the SLeeLa HTTP 3.0 protocol generation, NOT the IETF
 * HTTP/3 transport (spec §2). */
#define HTTP3_ENVELOPE_VERSION 3u

/* Maximum payload an envelope will carry in this reference implementation.
 * A real deployment negotiates this (§14 max envelope size). */
#define HTTP3_ENVELOPE_MAX_PAYLOAD 4096u

/* FLAGS bitfield (§5). Flags are advisory hints about the request; the
 * authoritative meaning of an operation stays with the service. */
typedef enum {
    HTTP3_FLAG_NONE        = 0x00,
    HTTP3_FLAG_BINARY      = 0x01, /* wire form is binary, not textual        */
    HTTP3_FLAG_COMPRESSED  = 0x02, /* payload is compressed (§11)             */
    HTTP3_FLAG_STREAM      = 0x04, /* part of a streaming exchange (§10)      */
    HTTP3_FLAG_IDEMPOTENT  = 0x08, /* caller asserts idempotency (§9)         */
    HTTP3_FLAG_RESET       = 0x10  /* packet reset: host tampered/untrusted   */
} http3_envelope_flag_t;

/*
 * The compact application envelope (§5). SERVICE-ID and OP-ID are
 * connection/service-local compact identifiers produced by fast naming (§4);
 * they MUST NOT be assumed globally stable. REQUEST-ID correlates a response
 * with its originating request without relying on ordering (§6).
 */
typedef struct {
    uint8_t  version;                              /* VERSION    */
    uint8_t  flags;                                /* FLAGS      */
    uint32_t service_id;                           /* SERVICE-ID */
    uint32_t op_id;                                /* OP-ID      */
    uint64_t request_id;                           /* REQUEST-ID */
    uint64_t digest;                               /* DIGEST     */
    uint64_t intactx;                              /* INTACTX    */
    uint8_t  payload[HTTP3_ENVELOPE_MAX_PAYLOAD];  /* PAYLOAD    */
    size_t   payload_len;
} http3_envelope_t;

/* Application execution status (§7). Distinguishes transport acceptance from
 * application execution: transport OK never implies business success. */
typedef enum {
    HTTP3_STATUS_OK            = 0, /* application executed successfully       */
    HTTP3_STATUS_APP_ERROR     = 1, /* transport fine, business logic failed   */
    HTTP3_STATUS_UNKNOWN_SVC   = 2, /* service id not resolvable               */
    HTTP3_STATUS_UNKNOWN_OP    = 3, /* operation id not resolvable             */
    HTTP3_STATUS_BAD_ENVELOPE  = 4, /* envelope failed to parse/validate       */
    HTTP3_STATUS_TOO_LARGE     = 5, /* payload/envelope exceeded a limit (§13) */
    HTTP3_STATUS_RETRY_DENIED  = 6, /* unsafe retry refused (§9)               */
    HTTP3_STATUS_BAD_DIGEST    = 7, /* per-packet DIGEST did not verify        */
    HTTP3_STATUS_TAMPERED      = 8  /* INTACTX variance exceeded threshold     */
} http3_status_t;

/* The response model (§7): STATUS | REQUEST-ID | RESULT. */
typedef struct {
    http3_status_t status;
    uint64_t       request_id;                        /* echoes the request (§6) */
    uint8_t        result[HTTP3_ENVELOPE_MAX_PAYLOAD]; /* RESULT                  */
    size_t         result_len;
} http3_response_t;

/* ---- Construction --------------------------------------------------------- */

/* Initialize an envelope with ids, INTACTX host identity, and payload. The
 * per-packet DIGEST (keyed MAC) is computed and stored automatically over the
 * finished header + payload under `key` (a 16-byte per-connection secret).
 * Returns 0 on success, -1 if the payload is too large or an argument is NULL. */
int http3_envelope_init(http3_envelope_t *env,
                        uint32_t service_id,
                        uint32_t op_id,
                        uint64_t request_id,
                        uint8_t flags,
                        uint64_t intactx,
                        const uint8_t key[HTTP3_MAC_KEY_BYTES],
                        const uint8_t *payload,
                        size_t payload_len);

/* ---- Integrity: per-packet DIGEST (keyed MAC) ----------------------------- */

/* Compute the 64-bit keyed-MAC DIGEST over an envelope's header fields
 * (excluding the digest itself) and payload, under the 16-byte `key`.
 * Deterministic and independent of wire form. Returns 0 on a NULL argument. */
uint64_t http3_envelope_compute_digest(const http3_envelope_t *env,
                                       const uint8_t key[HTTP3_MAC_KEY_BYTES]);

/* Recompute the MAC under `key` and compare against env->digest. Returns 1 if
 * the stored digest matches (packet authentic + intact), 0 otherwise. */
int http3_envelope_verify_digest(const http3_envelope_t *env,
                                 const uint8_t key[HTTP3_MAC_KEY_BYTES]);

/* ---- Textual wire form (§5: textual for interoperability) -----------------
 * Line form (single line, newline-terminated):
 *   H3 <version> <flags> <service_id> <op_id> <request_id> <digest> <intactx> <payload_len>:<payload-bytes>
 * The payload is length-prefixed so it is binary-safe and space-safe.
 */
int http3_envelope_pack_text(const http3_envelope_t *env, char *out, size_t out_cap, size_t *written);
int http3_envelope_unpack_text(const char *in, size_t in_len, http3_envelope_t *env);

/* ---- Binary wire form (§5: binary for negotiated high performance) --------
 * Fixed header, big-endian, then raw payload:
 *   [ver:1][flags:1][service_id:4][op_id:4][request_id:8][digest:8][intactx:8][payload_len:4][payload:N]
 */
#define HTTP3_ENVELOPE_BIN_HEADER 38u
int http3_envelope_pack_binary(const http3_envelope_t *env, uint8_t *out, size_t out_cap, size_t *written);
int http3_envelope_unpack_binary(const uint8_t *in, size_t in_len, http3_envelope_t *env);

/* ---- Response textual form (§7) -------------------------------------------
 *   H3R <status> <request_id> <result_len>:<result-bytes>
 */
int http3_response_pack_text(const http3_response_t *resp, char *out, size_t out_cap, size_t *written);
int http3_response_unpack_text(const char *in, size_t in_len, http3_response_t *resp);

/* Human-readable status name (for logs/§17 admin). */
const char *http3_status_name(http3_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_ENVELOPE_H */
