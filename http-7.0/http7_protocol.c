/* http7_protocol.c — SLeeLa HTTP 7.0 frame + assertion implementation. */
#include "../preferred-routers/preferred_router.h"
#include "http7_protocol.h"
#include <stdio.h>
#include <string.h>

/* ── big-endian helpers ─────────────────────────────────────────────── */

static void p16(uint8_t *p, uint16_t v) { p[0]=(uint8_t)(v>>8); p[1]=(uint8_t)v; }
static void p32(uint8_t *p, uint32_t v) {
    p[0]=(uint8_t)(v>>24);p[1]=(uint8_t)(v>>16);p[2]=(uint8_t)(v>>8);p[3]=(uint8_t)v; }
static void p64(uint8_t *p, uint64_t v) {
    for(int i=7;i>=0;--i){p[i]=(uint8_t)v;v>>=8;} }
static uint16_t g16(const uint8_t *p) { return (uint16_t)(((uint16_t)p[0]<<8)|p[1]); }
static uint32_t g32(const uint8_t *p) {
    return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3]; }
static uint64_t g64(const uint8_t *p) {
    uint64_t v=0; for(int i=0;i<8;++i) v=(v<<8)|p[i]; return v; }

/* ── frame validation ───────────────────────────────────────────────── */

int http7_frame_validate(const http7_frame_header_t *h, size_t available)
{
    if (!h) return -1;
    if (!sleela_preferred_router_packet_policy("HTTP/7.0", 4)) return -6;
    if (h->version != HTTP7_VERSION) return -2;
    if (h->type < HTTP7_FRAME_OPEN || h->type > HTTP7_FRAME_ASSERTION_AUTHOR) return -3;
    if (h->payload_length > HTTP7_MAX_PAYLOAD) return -4;
    if ((size_t)h->payload_length > available) return -5;
    return 0;
}

/* ── frame encode ───────────────────────────────────────────────────── */

int http7_frame_encode(const http7_frame_header_t *h, const uint8_t *payload,
                       uint8_t *out, size_t out_cap, size_t *written)
{
    if (!h || !out || !written) return -1;
    if (http7_frame_validate(h, h->payload_length) != 0) return -2;
    if (h->payload_length && !payload) return -3;
    if (out_cap < HTTP7_FRAME_HEADER_SIZE + h->payload_length) return -4;

    out[0] = h->version; out[1] = h->type;
    p16(out+2, h->flags); p64(out+4, h->stream_id);
    p64(out+12, h->request_id); p64(out+20, h->sequence);
    p32(out+28, h->payload_length);
    if (h->payload_length)
        memcpy(out + HTTP7_FRAME_HEADER_SIZE, payload, h->payload_length);
    *written = HTTP7_FRAME_HEADER_SIZE + h->payload_length;
    return 0;
}

/* ── frame decode ───────────────────────────────────────────────────── */

int http7_frame_decode(const uint8_t *wire, size_t wire_len,
                       http7_frame_view_t *out, size_t *consumed)
{
    if (!wire || !out || !consumed) return -1;
    if (wire_len < HTTP7_FRAME_HEADER_SIZE) return -2;

    http7_frame_header_t h;
    h.version = wire[0]; h.type = wire[1]; h.flags = g16(wire+2);
    h.stream_id = g64(wire+4); h.request_id = g64(wire+12);
    h.sequence = g64(wire+20); h.payload_length = g32(wire+28);

    if (http7_frame_validate(&h, wire_len - HTTP7_FRAME_HEADER_SIZE) != 0)
        return -3;

    out->header = h;
    out->payload = wire + HTTP7_FRAME_HEADER_SIZE;
    *consumed = HTTP7_FRAME_HEADER_SIZE + h.payload_length;
    return 0;
}

/* ── frame type name ────────────────────────────────────────────────── */

const char *http7_frame_type_name(http7_frame_type_t type)
{
    switch (type) {
    case HTTP7_FRAME_OPEN: return "OPEN";
    case HTTP7_FRAME_DATA: return "DATA";
    case HTTP7_FRAME_END: return "END";
    case HTTP7_FRAME_RESET: return "RESET";
    case HTTP7_FRAME_WINDOW: return "WINDOW";
    case HTTP7_FRAME_PING: return "PING";
    case HTTP7_FRAME_PONG: return "PONG";
    case HTTP7_FRAME_RESUME: return "RESUME";
    case HTTP7_FRAME_CAPSULE: return "CAPSULE";
    case HTTP7_FRAME_FRIENDS_PACK: return "FRIENDS_PACK";
    case HTTP7_FRAME_BONUS_OFFER: return "BONUS_OFFER";
    case HTTP7_FRAME_FP_UPDATE: return "FP_UPDATE";
    case HTTP7_FRAME_AUDIT: return "AUDIT";
    case HTTP7_FRAME_CONSOLIDATED_FRIENDS_BET: return "CONSOLIDATED_FRIENDS_BET";
    case HTTP7_FRAME_TEAMSTER_DEBATE: return "TEAMSTER_DEBATE";
    case HTTP7_FRAME_CONSOLIDATE_IQ: return "CONSOLIDATE_IQ";
    case HTTP7_FRAME_TEAM_AREA: return "TEAM_AREA";
    case HTTP7_FRAME_DEBATE_TOPIC: return "DEBATE_TOPIC";
    case HTTP7_FRAME_DEBATE_POSITION: return "DEBATE_POSITION";
    case HTTP7_FRAME_RECIPIENT_LABEL: return "RECIPIENT_LABEL";
    case HTTP7_FRAME_ASSERTION: return "ASSERTION";
    case HTTP7_FRAME_ASSERTION_SOURCE: return "ASSERTION_SOURCE";
    case HTTP7_FRAME_ASSERTION_STATUS: return "ASSERTION_STATUS";
    case HTTP7_FRAME_ASSERTION_AUTHOR: return "ASSERTION_AUTHOR";
    default: return "UNKNOWN";
    }
}

/* ── assertion status name ──────────────────────────────────────────── */

const char *http7_assertion_status_name(http7_assertion_status_t status)
{
    switch (status) {
    case HTTP7_ASSERT_FACTUAL_DOCUMENTED: return "FACTUAL_DOCUMENTED";
    case HTTP7_ASSERT_USER_AUTHORED:      return "USER_AUTHORED";
    case HTTP7_ASSERT_FICTIONAL:          return "FICTIONAL";
    case HTTP7_ASSERT_COUNTERFACTUAL:     return "COUNTERFACTUAL";
    case HTTP7_ASSERT_DISPUTED:           return "DISPUTED";
    case HTTP7_ASSERT_UNVERIFIED:         return "UNVERIFIED";
    default:                              return "UNKNOWN";
    }
}

/* ── assertion serialise ────────────────────────────────────────────── */

int http7_assertion_serialise(const http7_assertion_t *a,
                              char *buf, size_t buf_cap)
{
    if (!a || !buf || buf_cap < 64) return -1;
    int n = snprintf(buf, buf_cap, "%s\t%s\t%s\t%s\t%s\t%s\n",
                     http7_assertion_status_name(a->status),
                     a->statement, a->source, a->source_date,
                     a->author, a->document_reference);
    return (n > 0 && (size_t)n < buf_cap) ? n : -1;
}

/* ── assertion parse ────────────────────────────────────────────────── */

static http7_assertion_status_t parse_status_name(const char *s)
{
    if (strcmp(s, "FACTUAL_DOCUMENTED") == 0) return HTTP7_ASSERT_FACTUAL_DOCUMENTED;
    if (strcmp(s, "USER_AUTHORED") == 0)      return HTTP7_ASSERT_USER_AUTHORED;
    if (strcmp(s, "FICTIONAL") == 0)          return HTTP7_ASSERT_FICTIONAL;
    if (strcmp(s, "COUNTERFACTUAL") == 0)     return HTTP7_ASSERT_COUNTERFACTUAL;
    if (strcmp(s, "DISPUTED") == 0)           return HTTP7_ASSERT_DISPUTED;
    return HTTP7_ASSERT_UNVERIFIED;
}

int http7_assertion_parse(const char *line, size_t line_len,
                          http7_assertion_t *out)
{
    if (!line || !out) return -1;
    memset(out, 0, sizeof(*out));

    char copy[HTTP7_MAX_FIELD * 2];
    size_t cl = line_len < sizeof(copy) - 1 ? line_len : sizeof(copy) - 1;
    memcpy(copy, line, cl);
    copy[cl] = '\0';
    /* strip trailing newline */
    if (cl > 0 && copy[cl - 1] == '\n') copy[--cl] = '\0';

    char *fields[6];
    int count = 0;
    char *tok = strtok(copy, "\t");
    while (tok && count < 6) {
        fields[count++] = tok;
        tok = strtok(NULL, "\t");
    }
    if (count < 2) return -1; /* at least status + statement */

    out->status = parse_status_name(fields[0]);
    snprintf(out->statement, sizeof(out->statement), "%s", fields[1]);
    if (count > 2) snprintf(out->source, sizeof(out->source), "%s", fields[2]);
    if (count > 3) snprintf(out->source_date, sizeof(out->source_date), "%s", fields[3]);
    if (count > 4) snprintf(out->author, sizeof(out->author), "%s", fields[4]);
    if (count > 5) snprintf(out->document_reference, sizeof(out->document_reference), "%s", fields[5]);
    return 0;
}
