/* ==========================================================================
 * h21_pipeline.h -- SLeeLa HTTP 2.1 reference processing pipeline (SKETCH).
 *
 * Same design goal as HTTP 3.0's pipeline: make the hot path explicit and
 * traceable --
 *
 *   receive -> minimal parse -> service-id lookup -> operation-id lookup
 *           -> dispatch -> business logic -> pack -> response
 *
 * A registered service exposes operations; each operation declares a retry
 * class and a handler. Dispatch keys on the compact SERVICE-ID/OP-ID. HTTP 2.1
 * has NO per-packet integrity gate (no MAC/INTACTX/NONCE/basket); that is the
 * 3.0 generation's addition.
 * ========================================================================== */
#ifndef H21_PIPELINE_H
#define H21_PIPELINE_H

#include <stddef.h>
#include <stdint.h>

#include "h21_envelope.h"
#include "h21_naming.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Retry classes, declared per operation. */
typedef enum {
    H21_RETRY_READ       = 0, /* safe to retry when semantics permit         */
    H21_RETRY_IDEMPOTENT = 1, /* repeat produces the same intended state     */
    H21_RETRY_MUTATING   = 2  /* retry needs explicit policy/idempotency key */
} h21_retry_class_t;

typedef h21_status_t (*h21_op_handler_t)(const uint8_t *payload,
                                         size_t payload_len,
                                         uint8_t *result,
                                         size_t result_cap,
                                         size_t *result_len,
                                         void *service_ctx);

typedef struct {
    uint32_t          op_id;
    h21_retry_class_t retry_class;
    h21_op_handler_t  handler;
} h21_op_binding_t;

typedef struct {
    uint32_t         service_id;
    void            *ctx;
    h21_op_binding_t ops[H21_MAX_OPS_PER_SVC];
    size_t           op_count;
} h21_service_binding_t;

/* One instance per connection/service host. */
typedef struct {
    h21_naming_t          naming;
    h21_service_binding_t services[H21_MAX_SERVICES];
    size_t                service_count;
    uint64_t              requests_handled; /* observability */
} h21_pipeline_t;

void     h21_pipeline_init(h21_pipeline_t *pipe);
uint32_t h21_pipeline_register_service(h21_pipeline_t *pipe, const char *service_name, void *ctx);
uint32_t h21_pipeline_register_op(h21_pipeline_t *pipe, const char *service_name,
                                  const char *op_name, h21_retry_class_t retry_class,
                                  h21_op_handler_t handler);
int      h21_pipeline_retry_class(const h21_pipeline_t *pipe, uint32_t service_id, uint32_t op_id);

/* The hot path over an already-parsed envelope: svc-id -> op-id -> dispatch ->
 * result pack into `resp`. Returns 0 if a response was produced. */
int h21_pipeline_dispatch(h21_pipeline_t *pipe, const h21_envelope_t *env, h21_response_t *resp);

/* Full receive path: wire buffer (textual or binary, auto-detected) -> minimal
 * parse + unpack -> dispatch -> packed textual response. Returns 0 on success. */
int h21_pipeline_handle_wire(h21_pipeline_t *pipe, const uint8_t *wire, size_t wire_len,
                             char *out, size_t out_cap, size_t *written);

#ifdef __cplusplus
}
#endif

#endif /* H21_PIPELINE_H */
