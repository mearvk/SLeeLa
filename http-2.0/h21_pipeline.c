/* ==========================================================================
 * h21_pipeline.c -- HTTP 2.1 processing pipeline (SKETCH).
 *
 * STUB bodies define the 2.1 shape. Fill them in to mirror the HTTP 3.0
 * pipeline MINUS the integrity gate: 2.1 has no per-packet MAC/INTACTX/NONCE/
 * basket, so the receive path goes straight from parse to dispatch.
 * ========================================================================== */
#include "h21_pipeline.h"

#include <string.h>

void h21_pipeline_init(h21_pipeline_t *pipe)
{
    if (pipe == NULL) {
        return;
    }
    memset(pipe, 0, sizeof(*pipe));
    h21_naming_init(&pipe->naming);
}

uint32_t h21_pipeline_register_service(h21_pipeline_t *pipe, const char *service_name, void *ctx)
{
    (void)pipe; (void)service_name; (void)ctx;
    /* TODO(2.1): intern the service name -> compact id; record ctx; return id. */
    return 0u;
}

uint32_t h21_pipeline_register_op(h21_pipeline_t *pipe, const char *service_name,
                                  const char *op_name, h21_retry_class_t retry_class,
                                  h21_op_handler_t handler)
{
    (void)pipe; (void)service_name; (void)op_name; (void)retry_class; (void)handler;
    /* TODO(2.1): intern service+op; record retry class + handler; return op id. */
    return 0u;
}

int h21_pipeline_retry_class(const h21_pipeline_t *pipe, uint32_t service_id, uint32_t op_id)
{
    (void)pipe; (void)service_id; (void)op_id;
    /* TODO(2.1): return the op's retry class, or -1 if unknown. */
    return -1;
}

int h21_pipeline_dispatch(h21_pipeline_t *pipe, const h21_envelope_t *env, h21_response_t *resp)
{
    if (pipe == NULL || env == NULL || resp == NULL) {
        return -1;
    }
    memset(resp, 0, sizeof(*resp));
    resp->request_id = env->request_id; /* echo REQUEST-ID */
    /* TODO(2.1): service-id lookup -> op-id lookup -> handler dispatch, setting
     * resp->status/result. For now, report unknown service. */
    resp->status = H21_STATUS_UNKNOWN_SVC;
    return 0;
}

int h21_pipeline_handle_wire(h21_pipeline_t *pipe, const uint8_t *wire, size_t wire_len,
                             char *out, size_t out_cap, size_t *written)
{
    h21_envelope_t env;
    h21_response_t resp;
    int parsed;
    if (pipe == NULL || wire == NULL || out == NULL) {
        return -1;
    }
    /* Minimal parse. Auto-detect wire form: textual envelopes begin with the
     * ASCII tag "H21 "; anything else is treated as binary. */
    if (wire_len >= 4u && wire[0] == 'H' && wire[1] == '2' && wire[2] == '1' && wire[3] == ' ') {
        parsed = h21_envelope_unpack_text((const char *)wire, wire_len, &env);
    } else {
        parsed = h21_envelope_unpack_binary(wire, wire_len, &env);
    }
    if (parsed != 0) {
        memset(&resp, 0, sizeof(resp));
        resp.status = H21_STATUS_BAD_ENVELOPE;
        return h21_response_pack_text(&resp, out, out_cap, written);
    }
    /* 2.1 has NO integrity gate here -- straight to dispatch. */
    if (h21_pipeline_dispatch(pipe, &env, &resp) != 0) {
        return -1;
    }
    return h21_response_pack_text(&resp, out, out_cap, written);
}
