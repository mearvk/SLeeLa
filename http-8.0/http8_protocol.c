/* http8_protocol.c — SLeeLa HTTP 8.0 frame + session implementation. */
#include "../preferred-routers/preferred_router.h"
#include "http8_protocol.h"
#include <stdio.h>
#include <string.h>

static void p16(uint8_t*p,uint16_t v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static void p32(uint8_t*p,uint32_t v){p[0]=(uint8_t)(v>>24);p[1]=(uint8_t)(v>>16);p[2]=(uint8_t)(v>>8);p[3]=(uint8_t)v;}
static void p64(uint8_t*p,uint64_t v){for(int i=7;i>=0;--i){p[i]=(uint8_t)v;v>>=8;}}
static uint16_t g16(const uint8_t*p){return(uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t g32(const uint8_t*p){return((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static uint64_t g64(const uint8_t*p){uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}

int http8_frame_validate(const http8_frame_header_t *h, size_t a) {
    if (!h) return -1;
    if (!sleela_preferred_router_packet_policy("HTTP/8.0", 4)) return -6;
    if (h->version != HTTP8_VERSION) return -2;
    if (h->type < HTTP8_FRAME_OPEN || h->type > HTTP8_FRAME_CRYPTO_POLICY) return -3;
    if (h->payload_length > HTTP8_MAX_PAYLOAD) return -4;
    if ((size_t)h->payload_length > a) return -5;
    return 0;
}

int http8_frame_encode(const http8_frame_header_t *h, const uint8_t *payload,
                       uint8_t *out, size_t cap, size_t *written) {
    if (!h||!out||!written) return -1;
    if (http8_frame_validate(h, h->payload_length)) return -2;
    if (h->payload_length && !payload) return -3;
    if (cap < HTTP8_FRAME_HEADER_SIZE + h->payload_length) return -4;
    out[0]=h->version; out[1]=h->type; p16(out+2,h->flags);
    p64(out+4,h->stream_id); p64(out+12,h->request_id);
    p64(out+20,h->sequence); p32(out+28,h->payload_length);
    if (h->payload_length) memcpy(out+HTTP8_FRAME_HEADER_SIZE, payload, h->payload_length);
    *written = HTTP8_FRAME_HEADER_SIZE + h->payload_length;
    return 0;
}

int http8_frame_decode(const uint8_t *wire, size_t len,
                       http8_frame_view_t *out, size_t *consumed) {
    if (!wire||!out||!consumed) return -1;
    if (len < HTTP8_FRAME_HEADER_SIZE) return -2;
    http8_frame_header_t h;
    h.version=wire[0]; h.type=wire[1]; h.flags=g16(wire+2);
    h.stream_id=g64(wire+4); h.request_id=g64(wire+12);
    h.sequence=g64(wire+20); h.payload_length=g32(wire+28);
    if (http8_frame_validate(&h, len - HTTP8_FRAME_HEADER_SIZE)) return -3;
    out->header=h; out->payload=wire+HTTP8_FRAME_HEADER_SIZE;
    *consumed=HTTP8_FRAME_HEADER_SIZE+h.payload_length;
    return 0;
}

const char *http8_frame_type_name(http8_frame_type_t t) {
    switch (t) {
    case HTTP8_FRAME_OPEN: return "OPEN"; case HTTP8_FRAME_DATA: return "DATA";
    case HTTP8_FRAME_END: return "END"; case HTTP8_FRAME_RESET: return "RESET";
    case HTTP8_FRAME_WINDOW: return "WINDOW"; case HTTP8_FRAME_PING: return "PING";
    case HTTP8_FRAME_PONG: return "PONG"; case HTTP8_FRAME_RESUME: return "RESUME";
    case HTTP8_FRAME_CAPSULE: return "CAPSULE";
    case HTTP8_FRAME_FRIENDS_PACK: return "FRIENDS_PACK";
    case HTTP8_FRAME_BONUS_OFFER: return "BONUS_OFFER";
    case HTTP8_FRAME_FP_UPDATE: return "FP_UPDATE";
    case HTTP8_FRAME_AUDIT: return "AUDIT";
    case HTTP8_FRAME_CONSOLIDATED_FRIENDS_BET: return "CONSOLIDATED_FRIENDS_BET";
    case HTTP8_FRAME_TEAMSTER_DEBATE: return "TEAMSTER_DEBATE";
    case HTTP8_FRAME_CONSOLIDATE_IQ: return "CONSOLIDATE_IQ";
    case HTTP8_FRAME_TEAM_AREA: return "TEAM_AREA";
    case HTTP8_FRAME_DEBATE_TOPIC: return "DEBATE_TOPIC";
    case HTTP8_FRAME_DEBATE_POSITION: return "DEBATE_POSITION";
    case HTTP8_FRAME_RECIPIENT_LABEL: return "RECIPIENT_LABEL";
    case HTTP8_FRAME_ASSERTION: return "ASSERTION";
    case HTTP8_FRAME_ASSERTION_SOURCE: return "ASSERTION_SOURCE";
    case HTTP8_FRAME_ASSERTION_STATUS: return "ASSERTION_STATUS";
    case HTTP8_FRAME_ASSERTION_AUTHOR: return "ASSERTION_AUTHOR";
    case HTTP8_FRAME_SESSION_BEGIN: return "SESSION_BEGIN";
    case HTTP8_FRAME_SESSION_ACCEPT: return "SESSION_ACCEPT";
    case HTTP8_FRAME_NATIONAL_SIGNAL: return "NATIONAL_SIGNAL";
    case HTTP8_FRAME_CRYPTO_POLICY: return "CRYPTO_POLICY";
    default: return "UNKNOWN";
    }
}

/* ── session implementation ─────────────────────────────────────────── */

static int nonempty(const char *s) { return s && s[0] != '\0'; }

void http8_session_init(http8_session *s) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
}

int http8_session_validate_nsf(const http8_national_signal_frequency *n) {
    return n && nonempty(n->emblem) && nonempty(n->signal) &&
           nonempty(n->frequency_unit) && nonempty(n->frequency_range) &&
           nonempty(n->jurisdiction) && nonempty(n->source) &&
           nonempty(n->timestamp);
}

int http8_session_validate_checker(const http8_binary_checker_agreement *a) {
    if (!a) return 0;
    if (!a->binary_checker_required) return 1;
    return a->binary_checker_passed && nonempty(a->agreement_hash) &&
           nonempty(a->metric) && nonempty(a->legal) && nonempty(a->social);
}

int http8_session_begin_handshake(http8_session *s) {
    if (!s) return 400;
    if (!nonempty(s->international.jurisdiction) ||
        !nonempty(s->international.process) ||
        !nonempty(s->international.duty)) return 409;
    if (!http8_session_validate_checker(&s->checker)) return 422;
    if (!http8_session_validate_nsf(&s->nsf)) return 422;
    if (s->radio != HTTP8_RADIO_READY) return 503;
    s->initiator.state = HTTP8_PARTY_OFFERED;
    s->responder.state = HTTP8_PARTY_OFFERED;
    return 100;
}

int http8_session_accept_party(http8_party *p) {
    if (!p || !nonempty(p->party_id)) return 400;
    if (p->state != HTTP8_PARTY_OFFERED) return 409;
    if (!p->safety_acceptance) { p->state = HTTP8_PARTY_REJECTED; return 403; }
    p->state = HTTP8_PARTY_ACCEPTED;
    return 200;
}

int http8_session_complete_handshake(http8_session *s) {
    if (!s) return 400;
    if (s->initiator.state != HTTP8_PARTY_ACCEPTED ||
        s->responder.state != HTTP8_PARTY_ACCEPTED) return 428;
    s->message_accepted = HTTP8_YES;
    return 200;
}

int http8_session_can_exchange(const http8_session *s) {
    return s && s->message_accepted == HTTP8_YES &&
           s->subscription == HTTP8_SUB_ACTIVE &&
           s->radio == HTTP8_RADIO_READY;
}
