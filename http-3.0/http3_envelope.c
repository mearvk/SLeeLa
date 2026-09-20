/* ==========================================================================
 * http3_envelope.c -- implementation of the HTTP 3.0 compact envelope (§5)
 * and response model (§7). Pure data: no I/O, no crypto, no allocation.
 * ========================================================================== */
#include "http3_envelope.h"
#include "http3_mac.h"
#include "http3_basket.h"

#include <stdio.h>
#include <string.h>

/* Canonical serialized header size the DIGEST is computed over (big-endian,
 * fixed-width fields; excludes the DIGEST itself, includes the NONCE). The
 * basket block and payload are appended after this header when MACing. */
#define ENV_DIGEST_HDR 38u

/* Big-endian field writers (defined fully in the binary-wire section below). */
static void put_u32(uint8_t *b, uint32_t v);
static void put_u64(uint8_t *b, uint64_t v);

/* ---- Per-packet DIGEST (keyed MAC: SipHash-2-4 over header + payload) ------
 * The DIGEST is a KEYED message authentication code, not a plain hash: it
 * resists deliberate forgery, because an attacker who rewrites a packet cannot
 * recompute a matching tag without the per-connection key. The MAC message is a
 * canonical, endianness-independent serialization (each header field big-endian
 * and fixed width, excluding the DIGEST) followed by the raw payload, so the
 * textual and binary wire forms of the same logical envelope share a tag and
 * the Python reference reproduces it byte-for-byte. */
uint64_t http3_envelope_compute_digest(const http3_envelope_t *env,
                                       const uint8_t key[HTTP3_MAC_KEY_BYTES])
{
    uint8_t msg[ENV_DIGEST_HDR + HTTP3_BASKET_BLOCK_SIZE + HTTP3_ENVELOPE_MAX_PAYLOAD];
    uint8_t logical_flags;
    size_t pos;
    if (env == NULL || key == NULL || env->payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return 0;
    }
    /* HTTP3_FLAG_BINARY is a transport-form marker set only on the binary wire;
     * exclude it so the tag is identical for the textual and binary forms. */
    logical_flags = (uint8_t)(env->flags & ~(uint8_t)HTTP3_FLAG_BINARY);

    msg[0] = env->version;
    msg[1] = logical_flags;
    put_u32(msg + 2, env->service_id);
    put_u32(msg + 6, env->op_id);
    put_u64(msg + 10, env->request_id);
    /* NONCE is inside the MAC so it cannot be altered without detection; the
     * receiver's high-water-mark check then defeats replay. */
    put_u64(msg + 18, env->nonce);
    put_u64(msg + 26, env->intactx);
    /* payload length as a fixed 32-bit big-endian field (payloads are capped at
     * HTTP3_ENVELOPE_MAX_PAYLOAD, well within 32 bits). */
    put_u32(msg + 34, (uint32_t)env->payload_len);
    pos = ENV_DIGEST_HDR;
    /* BASKET travels on every packet and is covered by the MAC. */
    memcpy(msg + pos, env->basket, HTTP3_BASKET_BLOCK_SIZE);
    pos += HTTP3_BASKET_BLOCK_SIZE;
    if (env->payload_len > 0U) {
        memcpy(msg + pos, env->payload, env->payload_len);
    }
    pos += env->payload_len;
    return http3_mac_siphash24(key, msg, pos);
}

int http3_envelope_verify_digest(const http3_envelope_t *env,
                                 const uint8_t key[HTTP3_MAC_KEY_BYTES])
{
    if (env == NULL || key == NULL) {
        return 0;
    }
    return http3_envelope_compute_digest(env, key) == env->digest ? 1 : 0;
}

int http3_envelope_init(http3_envelope_t *env,
                        uint32_t service_id,
                        uint32_t op_id,
                        uint64_t request_id,
                        uint8_t flags,
                        uint64_t nonce,
                        uint64_t intactx,
                        const uint8_t key[HTTP3_MAC_KEY_BYTES],
                        const uint8_t *payload,
                        size_t payload_len)
{
    if (env == NULL || key == NULL || payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    memset(env, 0, sizeof(*env));
    env->version = HTTP3_ENVELOPE_VERSION;
    env->flags = flags;
    env->service_id = service_id;
    env->op_id = op_id;
    env->request_id = request_id;
    env->nonce = nonce;
    env->intactx = intactx;
    /* The fixed basket of goods & services rides on every packet. */
    (void)http3_basket_serialize(env->basket, sizeof(env->basket));
    if (payload != NULL && payload_len > 0U) {
        memcpy(env->payload, payload, payload_len);
    }
    env->payload_len = payload_len;
    /* Seal the packet with its keyed MAC over the finished contents. */
    env->digest = http3_envelope_compute_digest(env, key);
    return 0;
}

/* ---- Textual envelope ----------------------------------------------------- */

static const char k_hex[] = "0123456789abcdef";

static void hex_encode(const uint8_t *in, size_t len, char *out)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        out[2 * i]     = k_hex[(in[i] >> 4) & 0xF];
        out[2 * i + 1] = k_hex[in[i] & 0xF];
    }
}

static int hex_val(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int hex_decode(const char *in, size_t hexlen, uint8_t *out, size_t out_cap)
{
    size_t i;
    if (hexlen % 2u != 0u || hexlen / 2u > out_cap) {
        return -1;
    }
    for (i = 0; i < hexlen / 2u; ++i) {
        int hi = hex_val(in[2 * i]);
        int lo = hex_val(in[2 * i + 1]);
        if (hi < 0 || lo < 0) {
            return -1;
        }
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    return 0;
}

int http3_envelope_pack_text(const http3_envelope_t *env, char *out, size_t out_cap, size_t *written)
{
    int n;
    size_t pos;
    char basket_hex[2 * HTTP3_BASKET_BLOCK_SIZE + 1];
    if (env == NULL || out == NULL || env->payload_len > HTTP3_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    hex_encode(env->basket, HTTP3_BASKET_BLOCK_SIZE, basket_hex);
    basket_hex[2 * HTTP3_BASKET_BLOCK_SIZE] = '\0';
    n = snprintf(out, out_cap, "H3 %u %u %u %u %llu %llu %llu %llu %s %zu:",
                 (unsigned)env->version, (unsigned)env->flags,
                 (unsigned)env->service_id, (unsigned)env->op_id,
                 (unsigned long long)env->request_id,
                 (unsigned long long)env->nonce,
                 (unsigned long long)env->digest,
                 (unsigned long long)env->intactx, basket_hex, env->payload_len);
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
    unsigned long long request_id, nonce, digest, intactx;
    unsigned long payload_len;
    int consumed = 0;
    char basket_hex[2 * HTTP3_BASKET_BLOCK_SIZE + 1];
    const char *p;
    (void)in_len;

    if (in == NULL || env == NULL) {
        return -1;
    }
    /* Parse the fixed prefix, the fixed-width basket-hex token, then the ':'
     * after payload_len. The %Ns width guards the basket_hex buffer. */
    {
        char fmt[96];
        (void)snprintf(fmt, sizeof(fmt),
                       "H3 %%u %%u %%u %%u %%llu %%llu %%llu %%llu %%%us %%lu:%%n",
                       (unsigned)(2 * HTTP3_BASKET_BLOCK_SIZE));
        if (sscanf(in, fmt, &version, &flags, &service_id, &op_id, &request_id,
                   &nonce, &digest, &intactx, basket_hex, &payload_len, &consumed) != 10 ||
            consumed <= 0) {
            return -1;
        }
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
    env->nonce = (uint64_t)nonce;
    env->digest = (uint64_t)digest;
    env->intactx = (uint64_t)intactx;
    if (hex_decode(basket_hex, strlen(basket_hex), env->basket, sizeof(env->basket)) != 0) {
        return -1;
    }
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
    put_u64(out + 18, env->nonce);
    put_u64(out + 26, env->digest);
    put_u64(out + 34, env->intactx);
    memcpy(out + 42, env->basket, HTTP3_BASKET_BLOCK_SIZE);
    put_u32(out + 42 + HTTP3_BASKET_BLOCK_SIZE, (uint32_t)env->payload_len);
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
    payload_len = get_u32(in + 42 + HTTP3_BASKET_BLOCK_SIZE);
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
    env->nonce = get_u64(in + 18);
    env->digest = get_u64(in + 26);
    env->intactx = get_u64(in + 34);
    memcpy(env->basket, in + 42, HTTP3_BASKET_BLOCK_SIZE);
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
        case HTTP3_STATUS_REPLAYED:     return "REPLAYED";
        default:                        return "UNKNOWN";
    }
}
