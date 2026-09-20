/* ==========================================================================
 * http3_envelope.c -- implementation of the HTTP 3.0 compact envelope (§5)
 * and response model (§7). Pure data: no I/O, no crypto, no allocation.
 * ========================================================================== */
#include "http3_envelope.h"

#include <stdio.h>
#include <string.h>

/* ---- Per-packet DIGEST (64-bit FNV-1a over header + payload) --------------- */

#define ENV_FNV64_OFFSET 14695981039346656037ULL
#define ENV_FNV64_PRIME  1099511628211ULL

static uint64_t env_fnv1a(uint64_t h, const void *data, size_t len)
{
    const unsigned char *p = (const unsigned char *)data;
    size_t i;
    for (i = 0; i < len; ++i) {
        h ^= (uint64_t)p[i];
        h *= ENV_FNV64_PRIME;
    }
    return h;
}

/* Big-endian field writers (defined fully in the binary-wire section below). */
static void put_u32(uint8_t *b, uint32_t v);
static void put_u64(uint8_t *b, uint64_t v);

uint64_t http3_envelope_compute_digest(const http3_envelope_t *env)
{
    /* Canonical, endianness-independent serialization of the header fields
     * (each big-endian, fixed width) followed by the raw payload. Defining the
     * digest over a canonical byte layout -- rather than raw struct memory --
     * lets the Python reference (http3_flow.py) reproduce it byte-for-byte. */
    uint8_t hdr[30];
    uint64_t h = ENV_FNV64_OFFSET;
    /* HTTP3_FLAG_BINARY is a transport-form marker set only on the binary wire;
     * exclude it so the digest is identical for the textual and binary forms of
     * the same logical envelope. */
    uint8_t logical_flags;
    if (env == NULL) {
        return 0;
    }
    logical_flags = (uint8_t)(env->flags & ~(uint8_t)HTTP3_FLAG_BINARY);

    hdr[0] = env->version;
    hdr[1] = logical_flags;
    put_u32(hdr + 2, env->service_id);
    put_u32(hdr + 6, env->op_id);
    put_u64(hdr + 10, env->request_id);
    put_u64(hdr + 18, env->intactx);
    /* payload length as a fixed 32-bit big-endian field (payloads are capped at
     * HTTP3_ENVELOPE_MAX_PAYLOAD, well within 32 bits). */
    put_u32(hdr + 26, (uint32_t)env->payload_len);

    h = env_fnv1a(h, hdr, sizeof(hdr));
    h = env_fnv1a(h, env->payload, env->payload_len);
    return h;
}

int http3_envelope_verify_digest(const http3_envelope_t *env)
{
    if (env == NULL) {
        return 0;
    }
    return http3_envelope_compute_digest(env) == env->digest ? 1 : 0;
}

int http3_envelope_init(http3_envelope_t *env,
                        uint32_t service_id,
                        uint32_t op_id,
                        uint64_t request_id,
                        uint8_t flags,
                        uint64_t intactx,
                        const uint8_t *payload,
                        size_t payload_len)
{
    if (env == NULL || payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    memset(env, 0, sizeof(*env));
    env->version = HTTP3_ENVELOPE_VERSION;
    env->flags = flags;
    env->service_id = service_id;
    env->op_id = op_id;
    env->request_id = request_id;
    env->intactx = intactx;
    if (payload != NULL && payload_len > 0U) {
        memcpy(env->payload, payload, payload_len);
    }
    env->payload_len = payload_len;
    /* Seal the packet with its integrity digest over the finished contents. */
    env->digest = http3_envelope_compute_digest(env);
    return 0;
}

/* ---- Textual envelope ----------------------------------------------------- */

int http3_envelope_pack_text(const http3_envelope_t *env, char *out, size_t out_cap, size_t *written)
{
    int n;
    size_t pos;
    if (env == NULL || out == NULL || env->payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    n = snprintf(out, out_cap, "H3 %u %u %u %u %llu %llu %llu %zu:",
                 (unsigned)env->version, (unsigned)env->flags,
                 (unsigned)env->service_id, (unsigned)env->op_id,
                 (unsigned long long)env->request_id,
                 (unsigned long long)env->digest,
                 (unsigned long long)env->intactx, env->payload_len);
    if (n < 0 || (size_t)n >= out_cap) {
        return -1;
    }
    pos = (size_t)n;
    if (pos + env->payload_len + 1U > out_cap) { /* +1 for trailing newline */
        return -1;
    }
    memcpy(out + pos, env->payload, env->payload_len);
    pos += env->payload_len;
    out[pos++] = '\n';
    if (written != NULL) {
        *written = pos;
    }
    return 0;
}

int http3_envelope_unpack_text(const char *in, size_t in_len, http3_envelope_t *env)
{
    unsigned version, flags, service_id, op_id;
    unsigned long long request_id, digest, intactx;
    unsigned long payload_len;
    int consumed = 0;
    const char *p;
    (void)in_len;

    if (in == NULL || env == NULL) {
        return -1;
    }
    /* Parse the fixed prefix up to and including the ':' after payload_len. */
    if (sscanf(in, "H3 %u %u %u %u %llu %llu %llu %lu:%n",
               &version, &flags, &service_id, &op_id, &request_id,
               &digest, &intactx, &payload_len, &consumed) != 8 || consumed <= 0) {
        return -1;
    }
    if (payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    p = in + consumed;
    memset(env, 0, sizeof(*env));
    env->version = (uint8_t)version;
    env->flags = (uint8_t)flags;
    env->service_id = (uint32_t)service_id;
    env->op_id = (uint32_t)op_id;
    env->request_id = (uint64_t)request_id;
    env->digest = (uint64_t)digest;
    env->intactx = (uint64_t)intactx;
    env->payload_len = (size_t)payload_len;
    if (payload_len > 0U) {
        memcpy(env->payload, p, payload_len);
    }
    return 0;
}

/* ---- Binary envelope (big-endian header) ---------------------------------- */

static void put_u32(uint8_t *b, uint32_t v)
{
    b[0] = (uint8_t)(v >> 24); b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);  b[3] = (uint8_t)v;
}
static void put_u64(uint8_t *b, uint64_t v)
{
    b[0] = (uint8_t)(v >> 56); b[1] = (uint8_t)(v >> 48);
    b[2] = (uint8_t)(v >> 40); b[3] = (uint8_t)(v >> 32);
    b[4] = (uint8_t)(v >> 24); b[5] = (uint8_t)(v >> 16);
    b[6] = (uint8_t)(v >> 8);  b[7] = (uint8_t)v;
}
static uint32_t get_u32(const uint8_t *b)
{
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) |
           ((uint32_t)b[2] << 8)  |  (uint32_t)b[3];
}
static uint64_t get_u64(const uint8_t *b)
{
    return ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) |
           ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) |
           ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) |
           ((uint64_t)b[6] << 8)  |  (uint64_t)b[7];
}

int http3_envelope_pack_binary(const http3_envelope_t *env, uint8_t *out, size_t out_cap, size_t *written)
{
    if (env == NULL || out == NULL || env->payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD ||
        out_cap < HTTP3_ENVELOPE_BIN_HEADER + env->payload_len) {
        return -1;
    }
    out[0] = env->version;
    out[1] = (uint8_t)(env->flags | HTTP3_FLAG_BINARY);
    put_u32(out + 2, env->service_id);
    put_u32(out + 6, env->op_id);
    put_u64(out + 10, env->request_id);
    put_u64(out + 18, env->digest);
    put_u64(out + 26, env->intactx);
    put_u32(out + 34, (uint32_t)env->payload_len);
    memcpy(out + HTTP3_ENVELOPE_BIN_HEADER, env->payload, env->payload_len);
    if (written != NULL) {
        *written = HTTP3_ENVELOPE_BIN_HEADER + env->payload_len;
    }
    return 0;
}

int http3_envelope_unpack_binary(const uint8_t *in, size_t in_len, http3_envelope_t *env)
{
    uint32_t payload_len;
    if (in == NULL || env == NULL || in_len < HTTP3_ENVELOPE_BIN_HEADER) {
        return -1;
    }
    payload_len = get_u32(in + 34);
    if (payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD ||
        in_len < (size_t)HTTP3_ENVELOPE_BIN_HEADER + payload_len) {
        return -1;
    }
    memset(env, 0, sizeof(*env));
    env->version = in[0];
    env->flags = in[1];
    env->service_id = get_u32(in + 2);
    env->op_id = get_u32(in + 6);
    env->request_id = get_u64(in + 10);
    env->digest = get_u64(in + 18);
    env->intactx = get_u64(in + 26);
    env->payload_len = payload_len;
    if (payload_len > 0U) {
        memcpy(env->payload, in + HTTP3_ENVELOPE_BIN_HEADER, payload_len);
    }
    return 0;
}

/* ---- Response ------------------------------------------------------------- */

int http3_response_pack_text(const http3_response_t *resp, char *out, size_t out_cap, size_t *written)
{
    int n;
    size_t pos;
    if (resp == NULL || out == NULL || resp->result_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    n = snprintf(out, out_cap, "H3R %d %llu %zu:",
                 (int)resp->status, (unsigned long long)resp->request_id, resp->result_len);
    if (n < 0 || (size_t)n >= out_cap) {
        return -1;
    }
    pos = (size_t)n;
    if (pos + resp->result_len + 1U > out_cap) {
        return -1;
    }
    memcpy(out + pos, resp->result, resp->result_len);
    pos += resp->result_len;
    out[pos++] = '\n';
    if (written != NULL) {
        *written = pos;
    }
    return 0;
}

int http3_response_unpack_text(const char *in, size_t in_len, http3_response_t *resp)
{
    int status;
    unsigned long long request_id;
    unsigned long result_len;
    int consumed = 0;
    (void)in_len;
    if (in == NULL || resp == NULL) {
        return -1;
    }
    if (sscanf(in, "H3R %d %llu %lu:%n", &status, &request_id, &result_len, &consumed) != 3 ||
        consumed <= 0 || result_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    memset(resp, 0, sizeof(*resp));
    resp->status = (http3_status_t)status;
    resp->request_id = (uint64_t)request_id;
    resp->result_len = (size_t)result_len;
    if (result_len > 0U) {
        memcpy(resp->result, in + consumed, result_len);
    }
    return 0;
}

const char *http3_status_name(http3_status_t status)
{
    switch (status) {
        case HTTP3_STATUS_OK:           return "OK";
        case HTTP3_STATUS_APP_ERROR:    return "APP_ERROR";
        case HTTP3_STATUS_UNKNOWN_SVC:  return "UNKNOWN_SERVICE";
        case HTTP3_STATUS_UNKNOWN_OP:   return "UNKNOWN_OPERATION";
        case HTTP3_STATUS_BAD_ENVELOPE: return "BAD_ENVELOPE";
        case HTTP3_STATUS_TOO_LARGE:    return "TOO_LARGE";
        case HTTP3_STATUS_RETRY_DENIED: return "RETRY_DENIED";
        case HTTP3_STATUS_BAD_DIGEST:   return "BAD_DIGEST";
        case HTTP3_STATUS_TAMPERED:     return "TAMPERED";
        default:                        return "UNKNOWN";
    }
}
