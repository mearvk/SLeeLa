/* http6_protocol.c — SLeeLa HTTP 6.0 frame encode/decode/validate.
 *
 * 20 frame types: HTTP 5.0's 13 plus CONSOLIDATED_FRIENDS_BET,
 * TEAMSTER_DEBATE, CONSOLIDATE_IQ, TEAM_AREA, DEBATE_TOPIC,
 * DEBATE_POSITION, RECIPIENT_LABEL.
 */
#include "../preferred-routers/preferred_router.h"
#include "http6_protocol.h"

#include <string.h>

/* ── big-endian helpers ─────────────────────────────────────────────── */

static void p16(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v >> 8);
    p[1] = (uint8_t)v;
}

static void p32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);  p[3] = (uint8_t)v;
}

static void p64(uint8_t *p, uint64_t v)
{
    for (int i = 7; i >= 0; --i) { p[i] = (uint8_t)v; v >>= 8; }
}

static uint16_t g16(const uint8_t *p)
{
    return (uint16_t)(((uint16_t)p[0] << 8) | p[1]);
}

static uint32_t g32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  | p[3];
}

static uint64_t g64(const uint8_t *p)
{
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v = (v << 8) | p[i];
    return v;
}

/* ── frame validation ───────────────────────────────────────────────── */

int http6_frame_validate(const http6_frame_header_t *h, size_t available)
{
    if (!h)
        return -1;
    if (!sleela_preferred_router_packet_policy("HTTP/6.0", 4))
        return -6;
    if (h->version != HTTP6_VERSION)
        return -2;
    if (h->type < HTTP6_FRAME_OPEN || h->type > HTTP6_FRAME_RECIPIENT_LABEL)
        return -3;
    if (h->payload_length > HTTP6_MAX_PAYLOAD)
        return -4;
    /* Bits 7..15 are reserved and must not be silently reinterpreted. */
    if ((h->flags & (uint16_t)~(HTTP6_FLAG_FIN | HTTP6_FLAG_ACK |
                                HTTP6_FLAG_URGENT | HTTP6_FLAG_INCREMENTAL |
                                HTTP6_FLAG_RESUMABLE | HTTP6_FLAG_OPTIONAL |
                                HTTP6_FLAG_USER_AUTHORED)) != 0)
        return -7;
    if ((size_t)h->payload_length > available)
        return -5;
    return 0;
}

/* ── frame encode ───────────────────────────────────────────────────── */

int http6_frame_encode(const http6_frame_header_t *h,
                       const uint8_t *payload,
                       uint8_t *out, size_t out_cap, size_t *written)
{
    if (!h || !out || !written)
        return -1;
    if (http6_frame_validate(h, h->payload_length) != 0)
        return -2;
    if (h->payload_length && !payload)
        return -3;
    if (out_cap < HTTP6_FRAME_HEADER_SIZE + h->payload_length)
        return -4;

    out[0] = h->version;
    out[1] = h->type;
    p16(out + 2,  h->flags);
    p64(out + 4,  h->stream_id);
    p64(out + 12, h->request_id);
    p64(out + 20, h->sequence);
    p32(out + 28, h->payload_length);

    if (h->payload_length)
        memcpy(out + HTTP6_FRAME_HEADER_SIZE, payload, h->payload_length);

    *written = HTTP6_FRAME_HEADER_SIZE + h->payload_length;
    return 0;
}

/* ── frame decode ───────────────────────────────────────────────────── */

int http6_frame_decode(const uint8_t *wire, size_t wire_len,
                       http6_frame_view_t *out, size_t *consumed)
{
    if (!wire || !out || !consumed)
        return -1;
    if (wire_len < HTTP6_FRAME_HEADER_SIZE)
        return -2;

    http6_frame_header_t h;
    h.version        = wire[0];
    h.type           = wire[1];
    h.flags          = g16(wire + 2);
    h.stream_id      = g64(wire + 4);
    h.request_id     = g64(wire + 12);
    h.sequence       = g64(wire + 20);
    h.payload_length = g32(wire + 28);

    if (http6_frame_validate(&h, wire_len - HTTP6_FRAME_HEADER_SIZE) != 0)
        return -3;

    out->header  = h;
    out->payload = wire + HTTP6_FRAME_HEADER_SIZE;
    *consumed    = HTTP6_FRAME_HEADER_SIZE + h.payload_length;
    return 0;
}

/* ── frame type name ────────────────────────────────────────────────── */

const char *http6_frame_type_name(http6_frame_type_t type)
{
    switch (type) {
    case HTTP6_FRAME_OPEN:                     return "OPEN";
    case HTTP6_FRAME_DATA:                     return "DATA";
    case HTTP6_FRAME_END:                      return "END";
    case HTTP6_FRAME_RESET:                    return "RESET";
    case HTTP6_FRAME_WINDOW:                   return "WINDOW";
    case HTTP6_FRAME_PING:                     return "PING";
    case HTTP6_FRAME_PONG:                     return "PONG";
    case HTTP6_FRAME_RESUME:                   return "RESUME";
    case HTTP6_FRAME_CAPSULE:                  return "CAPSULE";
    case HTTP6_FRAME_FRIENDS_PACK:             return "FRIENDS_PACK";
    case HTTP6_FRAME_BONUS_OFFER:              return "BONUS_OFFER";
    case HTTP6_FRAME_FP_UPDATE:                return "FP_UPDATE";
    case HTTP6_FRAME_AUDIT:                    return "AUDIT";
    case HTTP6_FRAME_CONSOLIDATED_FRIENDS_BET: return "CONSOLIDATED_FRIENDS_BET";
    case HTTP6_FRAME_TEAMSTER_DEBATE:          return "TEAMSTER_DEBATE";
    case HTTP6_FRAME_CONSOLIDATE_IQ:           return "CONSOLIDATE_IQ";
    case HTTP6_FRAME_TEAM_AREA:                return "TEAM_AREA";
    case HTTP6_FRAME_DEBATE_TOPIC:             return "DEBATE_TOPIC";
    case HTTP6_FRAME_DEBATE_POSITION:          return "DEBATE_POSITION";
    case HTTP6_FRAME_RECIPIENT_LABEL:          return "RECIPIENT_LABEL";
    default:                                   return "UNKNOWN";
    }
}
