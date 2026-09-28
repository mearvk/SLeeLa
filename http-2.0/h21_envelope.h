/* ==========================================================================
 * h21_envelope.h -- SLeeLa HTTP 2.1 compact application envelope (SKETCH).
 *
 * HTTP 2.1 shares the BASIC DESIGN GOALS of HTTP 3.0 -- a compact application
 * envelope, fast service/operation naming, a response model that separates
 * transport from application status, and a small traceable processing pipeline
 * -- but is an EARLIER protocol generation: it carries the clean core only and
 * omits the 3.0-era per-packet integrity additions (keyed-MAC DIGEST, INTACTX
 * host identity, replay NONCE, and the goods/services BASKET).
 *
 * The envelope carries, in a fixed logical order:
 *
 *     VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD
 *
 * and a response carries:
 *
 *     STATUS | REQUEST-ID | RESULT
 *
 * This header is a SKETCH: the types and signatures define the 2.1 shape; the
 * .c bodies are stubs to be filled in.
 * ========================================================================== */
#ifndef H21_ENVELOPE_H
#define H21_ENVELOPE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Protocol generation of the envelope itself. "2" names the SLeeLa HTTP 2.1
 * protocol generation (NOT the IETF HTTP/2 transport). */
#define H21_ENVELOPE_VERSION 2u

/* Maximum payload this reference envelope will carry. */
#define H21_ENVELOPE_MAX_PAYLOAD 4096u

/* FLAGS bitfield. Flags are advisory hints; the authoritative meaning of an
 * operation stays with the service. 2.1 keeps only the core flags. */
typedef enum {
    H21_FLAG_NONE       = 0x00,
    H21_FLAG_BINARY     = 0x01, /* wire form is binary, not textual */
    H21_FLAG_COMPRESSED = 0x02, /* payload is compressed            */
    H21_FLAG_IDEMPOTENT = 0x08  /* caller asserts idempotency       */
} h21_envelope_flag_t;

/* The compact application envelope (2.1 core). */
typedef struct {
    uint8_t  version;                            /* VERSION    */
    uint8_t  flags;                              /* FLAGS      */
    uint32_t service_id;                         /* SERVICE-ID */
    uint32_t op_id;                              /* OP-ID      */
    uint64_t request_id;                         /* REQUEST-ID */
    uint8_t  payload[H21_ENVELOPE_MAX_PAYLOAD];  /* PAYLOAD    */
    size_t   payload_len;
} h21_envelope_t;

/* Application execution status: transport acceptance never implies business
 * success. */
typedef enum {
    H21_STATUS_OK           = 0, /* application executed successfully   */
    H21_STATUS_APP_ERROR    = 1, /* transport fine, business failed     */
    H21_STATUS_UNKNOWN_SVC  = 2, /* service id not resolvable           */
    H21_STATUS_UNKNOWN_OP   = 3, /* operation id not resolvable         */
    H21_STATUS_BAD_ENVELOPE = 4, /* envelope failed to parse/validate   */
    H21_STATUS_TOO_LARGE    = 5, /* payload/envelope exceeded a limit   */
    H21_STATUS_RETRY_DENIED = 6  /* unsafe retry refused                */
} h21_status_t;

/* The response model: STATUS | REQUEST-ID | RESULT. */
typedef struct {
    h21_status_t status;
    uint64_t     request_id;                       /* echoes the request */
    uint8_t      result[H21_ENVELOPE_MAX_PAYLOAD];  /* RESULT             */
    size_t       result_len;
} h21_response_t;

/* ---- Construction (SKETCH: to be implemented) ----------------------------- */

/* Initialize an envelope with ids and payload. Returns 0 on success, -1 if the
 * payload is too large or an argument is NULL. */
int h21_envelope_init(h21_envelope_t *env,
                      uint32_t service_id,
                      uint32_t op_id,
                      uint64_t request_id,
                      uint8_t flags,
                      const uint8_t *payload,
                      size_t payload_len);

/* ---- Textual wire form (interoperable) ------------------------------------
 * Line form (single line, newline-terminated):
 *   H21 <version> <flags> <service_id> <op_id> <request_id> <payload_len>:<payload-bytes>
 * The payload is length-prefixed so it is binary-safe and space-safe.
 */
int h21_envelope_pack_text(const h21_envelope_t *env, char *out, size_t out_cap, size_t *written);
int h21_envelope_unpack_text(const char *in, size_t in_len, h21_envelope_t *env);

/* ---- Binary wire form (negotiated, high-performance) ----------------------
 * Fixed header, big-endian, then raw payload:
 *   [ver:1][flags:1][service_id:4][op_id:4][request_id:8][payload_len:4][payload:N]
 */
#define H21_ENVELOPE_BIN_HEADER 22u
int h21_envelope_pack_binary(const h21_envelope_t *env, uint8_t *out, size_t out_cap, size_t *written);
int h21_envelope_unpack_binary(const uint8_t *in, size_t in_len, h21_envelope_t *env);

/* ---- Response textual form ------------------------------------------------
 *   H21R <status> <request_id> <result_len>:<result-bytes>
 */
int h21_response_pack_text(const h21_response_t *resp, char *out, size_t out_cap, size_t *written);
int h21_response_unpack_text(const char *in, size_t in_len, h21_response_t *resp);

/* Human-readable status name (for logs/diagnostics). */
const char *h21_status_name(h21_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* H21_ENVELOPE_H */
