/* ==========================================================================
 * h21_pipeline.c -- HTTP 2.1 processing pipeline.
 *
 * Hot path (no integrity gate — that is HTTP 3.0's addition):
 *
 *   receive → minimal parse (auto-detect textual/binary)
 *           → service-id lookup → operation-id lookup
 *           → dispatch → handler → pack response
 * ========================================================================== */
#include "h21_pipeline.h"

#include <string.h>

/* ── init ─────────────────────────────────────────────────────────── */

void h21_pipeline_init(h21_pipeline_t *pipe)
{
    if (pipe == NULL)
        return;
    memset(pipe, 0, sizeof(*pipe));
    h21_naming_init(&pipe->naming);
}

/* ── service registration ─────────────────────────────────────────── */

uint32_t h21_pipeline_register_service(h21_pipeline_t *pipe,
                                       const char     *service_name,
                                       void           *ctx)
{
    if (!pipe || !service_name || !*service_name)
        return 0u;

    /* Intern the name → compact id. */
    uint32_t svc_id = h21_naming_intern_service(&pipe->naming, service_name);
    if (svc_id == 0)
        return 0u;

    /* Check if already registered in the pipeline. */
    for (size_t i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id == svc_id) {
            pipe->services[i].ctx = ctx; /* allow context update */
            return svc_id;
        }
    }

    /* Capacity check. */
    if (pipe->service_count >= H21_MAX_SERVICES)
        return 0u;

    /* Register. */
    h21_service_binding_t *b = &pipe->services[pipe->service_count++];
    memset(b, 0, sizeof(*b));
    b->service_id = svc_id;
    b->ctx        = ctx;

    return svc_id;
}

/* ── operation registration ───────────────────────────────────────── */

uint32_t h21_pipeline_register_op(h21_pipeline_t   *pipe,
                                  const char       *service_name,
                                  const char       *op_name,
                                  h21_retry_class_t retry_class,
                                  h21_op_handler_t  handler)
{
    if (!pipe || !service_name || !*service_name ||
        !op_name || !*op_name || !handler)
        return 0u;

    /* Ensure the service is registered. */
    uint32_t svc_id = h21_pipeline_register_service(pipe, service_name, NULL);
    if (svc_id == 0)
        return 0u;

    /* Intern the operation name. */
    uint32_t op_id = h21_naming_intern_op(&pipe->naming, service_name, op_name);
    if (op_id == 0)
        return 0u;

    /* Find the service binding. */
    h21_service_binding_t *svc = NULL;
    for (size_t i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id == svc_id) {
            svc = &pipe->services[i];
            break;
        }
    }
    if (!svc)
        return 0u;

    /* Check if already bound. */
    for (size_t i = 0; i < svc->op_count; ++i) {
        if (svc->ops[i].op_id == op_id) {
            svc->ops[i].retry_class = retry_class;
            svc->ops[i].handler     = handler;
            return op_id;
        }
    }

    /* Capacity check. */
    if (svc->op_count >= H21_MAX_OPS_PER_SVC)
        return 0u;

    /* Bind. */
    h21_op_binding_t *ob = &svc->ops[svc->op_count++];
    ob->op_id       = op_id;
    ob->retry_class = retry_class;
    ob->handler     = handler;

    return op_id;
}

/* ── retry-class query ────────────────────────────────────────────── */

int h21_pipeline_retry_class(const h21_pipeline_t *pipe,
                             uint32_t              service_id,
                             uint32_t              op_id)
{
    if (!pipe)
        return -1;

    for (size_t i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id != service_id)
            continue;
        for (size_t j = 0; j < pipe->services[i].op_count; ++j) {
            if (pipe->services[i].ops[j].op_id == op_id)
                return (int)pipe->services[i].ops[j].retry_class;
        }
        return -1; /* service found, op not found */
    }

    return -1;
}

/* ── dispatch (hot path) ──────────────────────────────────────────── */

int h21_pipeline_dispatch(h21_pipeline_t      *pipe,
                          const h21_envelope_t *env,
                          h21_response_t       *resp)
{
    if (!pipe || !env || !resp)
        return -1;

    memset(resp, 0, sizeof(*resp));
    resp->request_id = env->request_id; /* always echo REQUEST-ID */

    /* Service-ID lookup. */
    const h21_service_binding_t *svc = NULL;
    for (size_t i = 0; i < pipe->service_count; ++i) {
        if (pipe->services[i].service_id == env->service_id) {
            svc = &pipe->services[i];
            break;
        }
    }
    if (!svc) {
        resp->status = H21_STATUS_UNKNOWN_SVC;
        return 0;
    }

    /* Operation-ID lookup. */
    const h21_op_binding_t *op = NULL;
    for (size_t j = 0; j < svc->op_count; ++j) {
        if (svc->ops[j].op_id == env->op_id) {
            op = &svc->ops[j];
            break;
        }
    }
    if (!op) {
        resp->status = H21_STATUS_UNKNOWN_OP;
        return 0;
    }

    /* Dispatch to the handler. */
    resp->status = op->handler(env->payload, env->payload_len,
                               resp->result, sizeof(resp->result),
                               &resp->result_len,
                               svc->ctx);

    ++pipe->requests_handled;
    return 0;
}

/* ── full wire path: buffer → parse → dispatch → response ─────────── */

int h21_pipeline_handle_wire(h21_pipeline_t *pipe,
                             const uint8_t  *wire,
                             size_t          wire_len,
                             char           *out,
                             size_t          out_cap,
                             size_t         *written)
{
    h21_envelope_t env;
    h21_response_t resp;
    int            parsed;

    if (!pipe || !wire || !out)
        return -1;

    /* Minimal parse.  Auto-detect wire form: textual envelopes begin with
     * the ASCII tag "H21 "; anything else is treated as binary. */
    if (wire_len >= 4u &&
        wire[0] == 'H' && wire[1] == '2' && wire[2] == '1' && wire[3] == ' ') {
        parsed = h21_envelope_unpack_text((const char *)wire, wire_len, &env);
    } else {
        parsed = h21_envelope_unpack_binary(wire, wire_len, &env);
    }

    if (parsed != 0) {
        memset(&resp, 0, sizeof(resp));
        resp.status = H21_STATUS_BAD_ENVELOPE;
        return h21_response_pack_text(&resp, out, out_cap, written);
    }

    /* 2.1 has NO integrity gate — straight to dispatch. */
    if (h21_pipeline_dispatch(pipe, &env, &resp) != 0)
        return -1;

    return h21_response_pack_text(&resp, out, out_cap, written);
}
