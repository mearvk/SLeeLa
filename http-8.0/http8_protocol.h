/* http8_protocol.h — SLeeLa HTTP 8.0 frame + session protocol.
 *
 * Extends HTTP 7.0's 24 frame types with 4 session/crypto/NSF frames.
 * Also provides the session, NSF, and binary-checker types from http80.c
 * as a reusable C API (separate from the demo main()).
 */
#ifndef SLEELA_HTTP8_PROTOCOL_H
#define SLEELA_HTTP8_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP8_VERSION 8u
#define HTTP8_MAX_PAYLOAD (16u * 1024u * 1024u)
#define HTTP8_MAX_TEXT 128
#define HTTP8_MAX_HASH 65

/* ── frame types (HTTP 7.0's 24 + 4 session/crypto/NSF) ────────────── */

typedef enum {
    HTTP8_FRAME_OPEN = 1, HTTP8_FRAME_DATA = 2, HTTP8_FRAME_END = 3,
    HTTP8_FRAME_RESET = 4, HTTP8_FRAME_WINDOW = 5, HTTP8_FRAME_PING = 6,
    HTTP8_FRAME_PONG = 7, HTTP8_FRAME_RESUME = 8, HTTP8_FRAME_CAPSULE = 9,
    HTTP8_FRAME_FRIENDS_PACK = 10, HTTP8_FRAME_BONUS_OFFER = 11,
    HTTP8_FRAME_FP_UPDATE = 12, HTTP8_FRAME_AUDIT = 13,
    HTTP8_FRAME_CONSOLIDATED_FRIENDS_BET = 14, HTTP8_FRAME_TEAMSTER_DEBATE = 15,
    HTTP8_FRAME_CONSOLIDATE_IQ = 16, HTTP8_FRAME_TEAM_AREA = 17,
    HTTP8_FRAME_DEBATE_TOPIC = 18, HTTP8_FRAME_DEBATE_POSITION = 19,
    HTTP8_FRAME_RECIPIENT_LABEL = 20,
    HTTP8_FRAME_ASSERTION = 21, HTTP8_FRAME_ASSERTION_SOURCE = 22,
    HTTP8_FRAME_ASSERTION_STATUS = 23, HTTP8_FRAME_ASSERTION_AUTHOR = 24,
    /* 8.0-era additions */
    HTTP8_FRAME_SESSION_BEGIN    = 25,
    HTTP8_FRAME_SESSION_ACCEPT   = 26,
    HTTP8_FRAME_NATIONAL_SIGNAL  = 27,
    HTTP8_FRAME_CRYPTO_POLICY    = 28
} http8_frame_type_t;

enum {
    HTTP8_FLAG_FIN = 0x01, HTTP8_FLAG_ACK = 0x02, HTTP8_FLAG_URGENT = 0x04,
    HTTP8_FLAG_INCREMENTAL = 0x08, HTTP8_FLAG_RESUMABLE = 0x10,
    HTTP8_FLAG_OPTIONAL = 0x20, HTTP8_FLAG_USER_AUTHORED = 0x40,
    HTTP8_FLAG_FACTUAL = 0x80, HTTP8_FLAG_CRYPTO_REQUIRED = 0x100
};

typedef struct {
    uint8_t  version;
    uint8_t  type;
    uint16_t flags;
    uint64_t stream_id;
    uint64_t request_id;
    uint64_t sequence;
    uint32_t payload_length;
} http8_frame_header_t;

typedef struct {
    http8_frame_header_t header;
    const uint8_t       *payload;
} http8_frame_view_t;

#define HTTP8_FRAME_HEADER_SIZE 32u

/* ── session types (from http80.c, made reusable) ───────────────────── */

typedef enum { HTTP8_NO = 0, HTTP8_YES = 1 } http8_bool;
typedef enum { HTTP8_PARTY_UNKNOWN = 0, HTTP8_PARTY_OFFERED, HTTP8_PARTY_ACCEPTED, HTTP8_PARTY_REJECTED } http8_party_state;
typedef enum { HTTP8_SUB_UNDECLARED = 0, HTTP8_SUB_INACTIVE, HTTP8_SUB_ACTIVE, HTTP8_SUB_SUSPENDED } http8_subscription_state;
typedef enum { HTTP8_RADIO_UNDECLARED = 0, HTTP8_RADIO_READY, HTTP8_RADIO_NOT_READY } http8_radio_state;

typedef struct {
    char emblem[HTTP8_MAX_TEXT];
    char signal[HTTP8_MAX_TEXT];
    char frequency_unit[HTTP8_MAX_TEXT];
    char frequency_range[HTTP8_MAX_TEXT];
    char jurisdiction[HTTP8_MAX_TEXT];
    char source[HTTP8_MAX_TEXT];
    char timestamp[HTTP8_MAX_TEXT];
} http8_national_signal_frequency;

typedef struct {
    char metric[HTTP8_MAX_TEXT];
    char legal[HTTP8_MAX_TEXT];
    char social[HTTP8_MAX_TEXT];
    char agreement_hash[HTTP8_MAX_HASH];
    http8_bool binary_checker_required;
    http8_bool binary_checker_passed;
} http8_binary_checker_agreement;

typedef struct { char jurisdiction[HTTP8_MAX_TEXT]; char process[HTTP8_MAX_TEXT]; char duty[HTTP8_MAX_TEXT]; } http8_international_context;
typedef struct { char party_id[HTTP8_MAX_TEXT]; http8_party_state state; http8_bool safety_acceptance; } http8_party;

typedef struct {
    http8_subscription_state subscription;
    http8_radio_state radio;
    http8_international_context international;
    http8_binary_checker_agreement checker;
    http8_national_signal_frequency nsf;
    http8_party initiator;
    http8_party responder;
    http8_bool message_accepted;
} http8_session;

/* ── frame API ──────────────────────────────────────────────────────── */

int  http8_frame_validate(const http8_frame_header_t *h, size_t available);
int  http8_frame_encode(const http8_frame_header_t *h, const uint8_t *payload,
                        uint8_t *out, size_t out_cap, size_t *written);
int  http8_frame_decode(const uint8_t *wire, size_t wire_len,
                        http8_frame_view_t *out, size_t *consumed);
const char *http8_frame_type_name(http8_frame_type_t type);

/* ── session API ────────────────────────────────────────────────────── */

void http8_session_init(http8_session *s);
int  http8_session_validate_nsf(const http8_national_signal_frequency *nsf);
int  http8_session_validate_checker(const http8_binary_checker_agreement *a);
int  http8_session_begin_handshake(http8_session *s);
int  http8_session_accept_party(http8_party *p);
int  http8_session_complete_handshake(http8_session *s);
int  http8_session_can_exchange(const http8_session *s);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_HTTP8_PROTOCOL_H */
