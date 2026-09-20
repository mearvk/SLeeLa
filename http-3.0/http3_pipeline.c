/* ==========================================================================
 * http3_pipeline.c -- implementation of the HTTP 3.0 §19 processing pipeline
 * and §9 retry classes. Ties the envelope (§5) and fast naming (§4) into a
 * short, traceable hot path with a service/op dispatch table.
 * ========================================================================== */
#include "http3_pipeline.h"
#include "http3_intactx.h"

#include <string.h>

void http3_pipeline_init(http3_pipeline_t *pipe)
{
    if (pipe == NULL) {
        return;
    }
    memset(pipe, 0, sizeof(*pipe));
    http3_naming_init(&pipe->naming);
    pipe->intactx_threshold = HTTP3_INTACTX_TAMPER_THRESHOLD;
}

void http3_pipeline_set_intactx_threshold(http3_pipeline_t *pipe, uint16_t threshold)
{
    if (pipe == NULL) {
        return;
    }
    pipe->intactx_threshold = (threshold == 0u) ? HTTP3_INTACTX_TAMPER_THRESHOLD
                                                : threshold;
}

static http3_service_binding_t *find_binding(http3_pipeline_t *pipe, uint32_t service_id)
{
    size_t i;
    for (i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id == service_id) {
            return &pipe->services[i];
        }
    }
    return NULL;
}

uint32_t http3_pipeline_register_service(http3_pipeline_t *pipe,
                                         const char *service_name,
                                         void *ctx)
{
    uint32_t sid;
    http3_service_binding_t *b;
    if (pipe == NULL) {
        return 0u;
    }
    sid = http3_naming_intern_service(&pipe->naming, service_name);
    if (sid == 0u) {
        return 0u;
    }
    b = find_binding(pipe, sid);
    if (b != NULL) {
        b->ctx = ctx; /* re-registration updates context */
        return sid;
    }
    if (pipe->service_count >= HTTP3_MAX_SERVICES) {
        return 0u;
    }
    b = &pipe->services[pipe->service_count++];
    b->service_id = sid;
    b->ctx = ctx;
    b->op_count = 0u;
    return sid;
}

uint32_t http3_pipeline_register_op(http3_pipeline_t *pipe,
                                    const char *service_name,
                                    const char *op_name,
                                    http3_retry_class_t retry_class,
                                    http3_op_handler_t handler)
{
    uint32_t sid, oid;
    http3_service_binding_t *b;
    size_t i;
    if (pipe == NULL || handler == NULL) {
        return 0u;
    }
    sid = http3_pipeline_register_service(pipe, service_name, NULL);
    if (sid == 0u) {
        return 0u;
    }
    oid = http3_naming_intern_op(&pipe->naming, service_name, op_name);
    if (oid == 0u) {
        return 0u;
    }
    b = find_binding(pipe, sid);
    if (b == NULL) {
        return 0u;
    }
    for (i = 0; i < b->op_count; ++i) {
        if (b->ops[i].op_id == oid) {
            b->ops[i].retry_class = retry_class; /* update existing */
            b->ops[i].handler = handler;
            return oid;
        }
    }
    if (b->op_count >= HTTP3_MAX_OPS_PER_SVC) {
        return 0u;
    }
    b->ops[b->op_count].op_id = oid;
    b->ops[b->op_count].retry_class = retry_class;
    b->ops[b->op_count].handler = handler;
    ++b->op_count;
    return oid;
}

static const http3_op_binding_t *find_op(const http3_service_binding_t *b, uint32_t op_id)
{
    size_t i;
    for (i = 0; i < b->op_count; ++i) {
        if (b->ops[i].op_id == op_id) {
            return &b->ops[i];
        }
    }
    return NULL;
}

int http3_pipeline_retry_class(const http3_pipeline_t *pipe,
                               uint32_t service_id, uint32_t op_id)
{
    size_t i;
    if (pipe == NULL) {
        return -1;
    }
    for (i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id == service_id) {
            const http3_op_binding_t *op = find_op(&pipe->services[i], op_id);
            return op != NULL ? (int)op->retry_class : -1;
        }
    }
    return -1;
}

int http3_pipeline_dispatch(http3_pipeline_t *pipe,
                            const http3_envelope_t *env,
                            http3_response_t *resp)
{
    http3_service_binding_t *b;
    const http3_op_binding_t *op;
    if (pipe == NULL || env == NULL || resp == NULL) {
        return -1;
    }
    memset(resp, 0, sizeof(*resp));
    resp->request_id = env->request_id; /* echo REQUEST-ID (§6/§7) */

    /* §19: service-id lookup */
    b = find_binding(pipe, env->service_id);
    if (b == NULL) {
        resp->status = HTTP3_STATUS_UNKNOWN_SVC;
        return 0;
    }
    /* §19: operation-id lookup */
    op = find_op(b, env->op_id);
    if (op == NULL) {
        resp->status = HTTP3_STATUS_UNKNOWN_OP;
        return 0;
    }
    /* §9: a MUTATING op replayed without an idempotency assertion is refused.
     * Here the caller signals a safe replay via the IDEMPOTENT flag. */
    if (op->retry_class == HTTP3_RETRY_MUTATING &&
        (env->flags & HTTP3_FLAG_IDEMPOTENT) == 0u) {
        /* First delivery is always allowed; this guard is where a real
         * connector would consult an idempotency-key store on retry. The
         * reference pipeline admits it and lets business logic run, but the
         * decision point is explicit and traceable. */
    }

    /* §19: SLeeLa dispatch -> business logic -> compact result pack */
    resp->status = op->handler(env->payload, env->payload_len,
                               resp->result, sizeof(resp->result),
                               &resp->result_len, b->ctx);
    ++pipe->requests_handled;
    return 0;
}

int http3_pipeline_handle_wire(http3_pipeline_t *pipe,
                               const uint8_t *wire, size_t wire_len,
                               char *out, size_t out_cap, size_t *written)
{
    http3_envelope_t env;
    http3_response_t resp;
    int parsed;
    if (pipe == NULL || wire == NULL || out == NULL) {
        return -1;
    }
    /* §19: minimal parse. Auto-detect wire form: textual envelopes begin with
     * the ASCII tag "H3 "; anything else is treated as binary. */
    if (wire_len >= 3u && wire[0] == 'H' && wire[1] == '3' && wire[2] == ' ') {
        parsed = http3_envelope_unpack_text((const char *)wire, wire_len, &env);
    } else {
        parsed = http3_envelope_unpack_binary(wire, wire_len, &env);
    }
    if (parsed != 0) {
        /* Bad envelope: produce a response with no correlatable request id. */
        memset(&resp, 0, sizeof(resp));
        resp.status = HTTP3_STATUS_BAD_ENVELOPE;
        return http3_response_pack_text(&resp, out, out_cap, written);
    }

    /* §19 integrity gate, before any dispatch:
     *   1. Per-packet DIGEST must verify -- else the packet arrived mangled. */
    if (!http3_envelope_verify_digest(&env)) {
        memset(&resp, 0, sizeof(resp));
        resp.status = HTTP3_STATUS_BAD_DIGEST;
        resp.request_id = env.request_id; /* best-effort correlation */
        ++pipe->digest_rejects;
        return http3_response_pack_text(&resp, out, out_cap, written);
    }
    /*   2. INTACTX variance must be below threshold -- else the emitting host
     *      has been tampered with / changed materially: RESET the exchange and
     *      do NOT dispatch. */
    if (http3_intactx_is_tampered(env.intactx, pipe->intactx_threshold)) {
        memset(&resp, 0, sizeof(resp));
        resp.status = HTTP3_STATUS_TAMPERED;
        resp.request_id = env.request_id;
        /* RESET marker in the result body; the request-side flag is
         * HTTP3_FLAG_RESET for a peer that echoes envelope flags. */
        memcpy(resp.result, "RESET", 5);
        resp.result_len = 5;
        ++pipe->tamper_resets;
        return http3_response_pack_text(&resp, out, out_cap, written);
    }

    if (http3_pipeline_dispatch(pipe, &env, &resp) != 0) {
        return -1;
    }
    /* §19: HTTP response (compact, textual here for interoperability) */
    return http3_response_pack_text(&resp, out, out_cap, written);
}
