/* http7_protocol.h — SLeeLa HTTP 7.0 Reality Assertion protocol.
 *
 * Extends the frame model with assertion types carrying classified statements,
 * provenance, and verification status.  An assertion is application data — it
 * does not make a claim true merely because it is transmitted.
 */
#ifndef SLEELA_HTTP7_PROTOCOL_H
#define SLEELA_HTTP7_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP7_VERSION 7u
#define HTTP7_MAX_PAYLOAD (16u * 1024u * 1024u)
#define HTTP7_MAX_FIELD   4096u

/* ── frame types (HTTP 6.0's 20 + 4 assertion frames) ──────────────── */

typedef enum {
    HTTP7_FRAME_OPEN = 1, HTTP7_FRAME_DATA = 2, HTTP7_FRAME_END = 3,
    HTTP7_FRAME_RESET = 4, HTTP7_FRAME_WINDOW = 5, HTTP7_FRAME_PING = 6,
    HTTP7_FRAME_PONG = 7, HTTP7_FRAME_RESUME = 8, HTTP7_FRAME_CAPSULE = 9,
    HTTP7_FRAME_FRIENDS_PACK = 10, HTTP7_FRAME_BONUS_OFFER = 11,
    HTTP7_FRAME_FP_UPDATE = 12, HTTP7_FRAME_AUDIT = 13,
    HTTP7_FRAME_CONSOLIDATED_FRIENDS_BET = 14, HTTP7_FRAME_TEAMSTER_DEBATE = 15,
    HTTP7_FRAME_CONSOLIDATE_IQ = 16, HTTP7_FRAME_TEAM_AREA = 17,
    HTTP7_FRAME_DEBATE_TOPIC = 18, HTTP7_FRAME_DEBATE_POSITION = 19,
    HTTP7_FRAME_RECIPIENT_LABEL = 20,
    /* 7.0-era additions */
    HTTP7_FRAME_ASSERTION        = 21,
    HTTP7_FRAME_ASSERTION_SOURCE = 22,
    HTTP7_FRAME_ASSERTION_STATUS = 23,
    HTTP7_FRAME_ASSERTION_AUTHOR = 24
} http7_frame_type_t;

enum {
    HTTP7_FLAG_FIN = 0x01, HTTP7_FLAG_ACK = 0x02, HTTP7_FLAG_URGENT = 0x04,
    HTTP7_FLAG_INCREMENTAL = 0x08, HTTP7_FLAG_RESUMABLE = 0x10,
    HTTP7_FLAG_OPTIONAL = 0x20, HTTP7_FLAG_USER_AUTHORED = 0x40,
    HTTP7_FLAG_FACTUAL = 0x80
};

/* ── frame header (same 32-byte wire format) ────────────────────────── */

typedef struct {
    uint8_t  version;
    uint8_t  type;
    uint16_t flags;
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t sequence;
    uint32_t payload_length;
} http7_frame_header_t;

typedef struct {
    http7_frame_header_t header;
    const uint8_t       *payload;
} http7_frame_view_t;

#define HTTP7_FRAME_HEADER_SIZE 32u

/* ── assertion status classification ────────────────────────────────── */

typedef enum {
    HTTP7_ASSERT_FACTUAL_DOCUMENTED = 0,
    HTTP7_ASSERT_USER_AUTHORED      = 1,
    HTTP7_ASSERT_FICTIONAL          = 2,
    HTTP7_ASSERT_COUNTERFACTUAL     = 3,
    HTTP7_ASSERT_DISPUTED           = 4,
    HTTP7_ASSERT_UNVERIFIED         = 5
} http7_assertion_status_t;

/* ── assertion record ───────────────────────────────────────────────── */

typedef struct {
    http7_assertion_status_t status;
    char statement[HTTP7_MAX_FIELD];
    char source[HTTP7_MAX_FIELD];
    char source_date[64];
    char author[256];
    char document_reference[HTTP7_MAX_FIELD];
} http7_assertion_t;

/* ── frame API ──────────────────────────────────────────────────────── */

int  http7_frame_validate(const http7_frame_header_t *h, size_t available);
int  http7_frame_encode(const http7_frame_header_t *h, const uint8_t *payload,
                        uint8_t *out, size_t out_cap, size_t *written);
int  http7_frame_decode(const uint8_t *wire, size_t wire_len,
                        http7_frame_view_t *out, size_t *consumed);
const char *http7_frame_type_name(http7_frame_type_t type);

/* ── assertion helpers ──────────────────────────────────────────────── */

const char *http7_assertion_status_name(http7_assertion_status_t status);

/* Serialise an assertion into a textual payload:
 *   STATUS\tSTATEMENT\tSOURCE\tSOURCE_DATE\tAUTHOR\tDOCUMENT_REFERENCE\n
 * Returns bytes written, or -1 on error. */
int  http7_assertion_serialise(const http7_assertion_t *a,
                               char *buf, size_t buf_cap);

/* Parse a serialised assertion line back into the struct. Returns 0 on success. */
int  http7_assertion_parse(const char *line, size_t line_len,
                           http7_assertion_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_HTTP7_PROTOCOL_H */
