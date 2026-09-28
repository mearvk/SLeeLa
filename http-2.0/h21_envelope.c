/* ==========================================================================
 * h21_envelope.c -- HTTP 2.1 compact envelope + response model (SKETCH).
 * Pure data: no I/O, no crypto, no allocation. The 2.1 core mirrors HTTP 3.0's
 * design goals without the 3.0-era per-packet integrity additions.
 * ========================================================================== */
#include "h21_envelope.h"

#include <stdio.h>
#include <string.h>

int h21_envelope_init(h21_envelope_t *env,
                      uint32_t service_id,
                      uint32_t op_id,
                      uint64_t request_id,
                      uint8_t flags,
                      const uint8_t *payload,
                      size_t payload_len)
{
    if (env == NULL || payload_len > H21_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    memset(env, 0, sizeof(*env));
    env->version = H21_ENVELOPE_VERSION;
    env->flags = flags;
    env->service_id = service_id;
    env->op_id = op_id;
    env->request_id = request_id;
    if (payload != NULL && payload_len > 0U) {
        memcpy(env->payload, payload, payload_len);
    }
    env->payload_len = payload_len;
    return 0;
}

/* ---- Textual envelope ----------------------------------------------------- */

int h21_envelope_pack_text(const h21_envelope_t *env, char *out, size_t out_cap, size_t *written)
{
    int n;
    size_t pos;
    if (env == NULL || out == NULL || env->payload_len > H21_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    n = snprintf(out, out_cap, "H21 %u %u %u %u %llu %zu:",
                 (unsigned)env->version, (unsigned)env->flags,
                 (unsigned)env->service_id, (unsigned)env->op_id,
                 (unsigned long long)env->request_id, env->payload_len);
    if (n < 0 || (size_t)n >= out_cap) {
        return -1;
    }
    pos = (size_t)n;
    if (pos + env->payload_len + 1U > out_cap) {
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

int h21_envelope_unpack_text(const char *in, size_t in_len, h21_envelope_t *env)
{
    unsigned version, flags, service_id, op_id;
    unsigned long long request_id;
    unsigned long payload_len;
    int consumed = 0;
    const char *p;
    (void)in_len;

    if (in == NULL || env == NULL) {
        return -1;
    }
    if (sscanf(in, "H21 %u %u %u %u %llu %lu:%n",
               &version, &flags, &service_id, &op_id, &request_id,
               &payload_len, &consumed) != 6 || consumed <= 0) {
        return -1;
    }
    if (payload_len > H21_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    p = in + consumed;
    memset(env, 0, sizeof(*env));
    env->version = (uint8_t)version;
    env->flags = (uint8_t)flags;
    env->service_id = (uint32_t)service_id;
    env->op_id = (uint32_t)op_id;
    env->request_id = (uint64_t)request_id;
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

int h21_envelope_pack_binary(const h21_envelope_t *env, uint8_t *out, size_t out_cap, size_t *written)
{
    if (env == NULL || out == NULL || env->payload_len > H21_ENVELOPE_MAX_PAYLOAD ||
        out_cap < H21_ENVELOPE_BIN_HEADER + env->payload_len) {
        return -1;
    }
    out[0] = env->version;
    out[1] = (uint8_t)(env->flags | H21_FLAG_BINARY);
    put_u32(out + 2, env->service_id);
    put_u32(out + 6, env->op_id);
    put_u64(out + 10, env->request_id);
    put_u32(out + 18, (uint32_t)env->payload_len);
    memcpy(out + H21_ENVELOPE_BIN_HEADER, env->payload, env->payload_len);
    if (written != NULL) {
        *written = H21_ENVELOPE_BIN_HEADER + env->payload_len;
    }
    return 0;
}

int h21_envelope_unpack_binary(const uint8_t *in, size_t in_len, h21_envelope_t *env)
{
    uint32_t payload_len;
    if (in == NULL || env == NULL || in_len < H21_ENVELOPE_BIN_HEADER) {
        return -1;
    }
    payload_len = get_u32(in + 18);
    if (payload_len > H21_ENVELOPE_MAX_PAYLOAD ||
        in_len < (size_t)H21_ENVELOPE_BIN_HEADER + payload_len) {
        return -1;
    }
    memset(env, 0, sizeof(*env));
    env->version = in[0];
    env->flags = in[1];
    env->service_id = get_u32(in + 2);
    env->op_id = get_u32(in + 6);
    env->request_id = get_u64(in + 10);
    env->payload_len = payload_len;
    if (payload_len > 0U) {
        memcpy(env->payload, in + H21_ENVELOPE_BIN_HEADER, payload_len);
    }
    return 0;
}

/* ---- Response ------------------------------------------------------------- */

int h21_response_pack_text(const h21_response_t *resp, char *out, size_t out_cap, size_t *written)
{
    int n;
    size_t pos;
    if (resp == NULL || out == NULL || resp->result_len > H21_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    n = snprintf(out, out_cap, "H21R %d %llu %zu:",
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

int h21_response_unpack_text(const char *in, size_t in_len, h21_response_t *resp)
{
    int status;
    unsigned long long request_id;
    unsigned long result_len;
    int consumed = 0;
    (void)in_len;
    if (in == NULL || resp == NULL) {
        return -1;
    }
    if (sscanf(in, "H21R %d %llu %lu:%n", &status, &request_id, &result_len, &consumed) != 3 ||
        consumed <= 0 || result_len > H21_ENVELOPE_MAX_PAYLOAD) {
        return -1;
    }
    memset(resp, 0, sizeof(*resp));
    resp->status = (h21_status_t)status;
    resp->request_id = (uint64_t)request_id;
    resp->result_len = (size_t)result_len;
    if (result_len > 0U) {
        memcpy(resp->result, in + consumed, result_len);
    }
    return 0;
}

const char *h21_status_name(h21_status_t status)
{
    switch (status) {
        case H21_STATUS_OK:           return "OK";
        case H21_STATUS_APP_ERROR:    return "APP_ERROR";
        case H21_STATUS_UNKNOWN_SVC:  return "UNKNOWN_SERVICE";
        case H21_STATUS_UNKNOWN_OP:   return "UNKNOWN_OPERATION";
        case H21_STATUS_BAD_ENVELOPE: return "BAD_ENVELOPE";
        case H21_STATUS_TOO_LARGE:    return "TOO_LARGE";
        case H21_STATUS_RETRY_DENIED: return "RETRY_DENIED";
        default:                      return "UNKNOWN";
    }
}
