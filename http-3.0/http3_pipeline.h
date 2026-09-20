/* ==========================================================================
 * http3_pipeline.h -- SLeeLa HTTP 3.0 reference processing pipeline (spec §19)
 * with idempotency/retry classes (spec §9).
 *
 * The pipeline is the DATA FLOW that drives an HTTP 3.0 exchange. It makes the
 * spec's §19 hot path explicit and traceable:
 *
 *     receive -> minimal parse -> service-id lookup -> operation-id lookup
 *             -> compact unpack -> SLeeLa dispatch -> business logic
 *             -> compact result pack -> response
 *
 * A registered service exposes operations; each operation declares a retry
 * class (§9) and a handler. Dispatch is keyed on the compact SERVICE-ID/OP-ID
 * from the envelope (§4/§5), never on a re-parsed name in the hot path.
 * ========================================================================== */
#ifndef HTTP3_PIPELINE_H
#define HTTP3_PIPELINE_H

#include <stddef.h>
#include <stdint.h>

#include "http3_envelope.h"
#include "http3_naming.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Retry classes (§9). Declared per operation so a client/connector can decide
 * whether an uncertain transport failure may be safely repeated. */
typedef enum {
    HTTP3_RETRY_READ       = 0, /* safe to retry when semantics permit        */
    HTTP3_RETRY_IDEMPOTENT = 1, /* repeat produces the same intended state    */
    HTTP3_RETRY_MUTATING   = 2, /* retry needs explicit policy/idempotency key*/
    HTTP3_RETRY_STREAM     = 3  /* retry semantics are operation-specific     */
} http3_retry_class_t;

/*
 * Business-logic handler for one operation. It receives the request payload
 * and writes a result payload plus an application status (§7). The handler is
 * pure application logic; the pipeline owns parse/dispatch/pack around it.
 */
typedef http3_status_t (*http3_op_handler_t)(const uint8_t *payload,
                                             size_t payload_len,
                                             uint8_t *result,
                                             size_t result_cap,
                                             size_t *result_len,
                                             void *service_ctx);

typedef struct {
    uint32_t            op_id;
    http3_retry_class_t retry_class;
    http3_op_handler_t  handler;
} http3_op_binding_t;

typedef struct {
    uint32_t           service_id;
    void              *ctx; /* opaque per-service state passed to handlers */
    http3_op_binding_t ops[HTTP3_MAX_OPS_PER_SVC];
    size_t             op_count;
} http3_service_binding_t;

/* The pipeline: the naming dictionary plus the dispatch table. One instance
 * per connection/service host embodies the reusable state the §19 hot path
 * relies on. */
typedef struct {
    http3_naming_t          naming;
    http3_service_binding_t services[HTTP3_MAX_SERVICES];
    size_t                  service_count;
    uint64_t                requests_handled; /* observability (§17)          */
    uint16_t                intactx_threshold;/* INTACTX variance tamper limit */
    uint64_t                digest_rejects;   /* packets dropped: bad DIGEST   */
    uint64_t                tamper_resets;    /* packets reset: INTACTX tamper */
    uint64_t                replays_rejected; /* packets dropped: stale NONCE  */
    uint8_t                 mac_key[HTTP3_MAC_KEY_BYTES]; /* per-conn MAC key   */
    uint64_t                nonce_high_water; /* highest accepted NONCE so far */
} http3_pipeline_t;

/* Initialize an empty pipeline. The INTACTX tamper threshold defaults to
 * HTTP3_INTACTX_TAMPER_THRESHOLD. The per-connection MAC key starts all-zero;
 * set the real key (from key agreement) with http3_pipeline_set_mac_key before
 * handling wire packets, so the DIGEST is verified against a shared secret. */
void http3_pipeline_init(http3_pipeline_t *pipe);

/* Set the INTACTX variance threshold above which an inbound packet is treated
 * as coming from a tampered host and RESET. Pass 0 to restore the default. */
void http3_pipeline_set_intactx_threshold(http3_pipeline_t *pipe, uint16_t threshold);

/* Install the 16-byte per-connection MAC key used to verify each packet's
 * keyed-MAC DIGEST. In a deployment this is the secret from the crypto
 * substrate's key agreement; peers that share it can authenticate each other's
 * packets, and a forger without it cannot produce a valid DIGEST. */
void http3_pipeline_set_mac_key(http3_pipeline_t *pipe,
                                const uint8_t key[HTTP3_MAC_KEY_BYTES]);

/* Reset the replay window (NONCE high-water mark) to `start`. The next packet
 * accepted must carry a NONCE strictly greater than `start`. Use when a new
 * connection/session begins; a real multi-sender deployment keys the window per
 * sender identity, but the single-connection reference keeps one monotonic
 * high-water mark. */
void http3_pipeline_reset_replay_window(http3_pipeline_t *pipe, uint64_t start);

/*
 * Register a service by name, returning its compact SERVICE-ID (§4). ctx is
 * handed to every handler of this service.
 */
uint32_t http3_pipeline_register_service(http3_pipeline_t *pipe,
                                         const char *service_name,
                                         void *ctx);

/*
 * Register an operation by name under a service. Returns the compact OP-ID.
 * The retry class (§9) and handler are recorded for dispatch.
 */
uint32_t http3_pipeline_register_op(http3_pipeline_t *pipe,
                                    const char *service_name,
                                    const char *op_name,
                                    http3_retry_class_t retry_class,
                                    http3_op_handler_t handler);

/* Query the retry class of an operation (§9). Returns -1 if unknown. */
int http3_pipeline_retry_class(const http3_pipeline_t *pipe,
                               uint32_t service_id, uint32_t op_id);

/*
 * The §19 hot path over an already-parsed envelope: service-id lookup ->
 * op-id lookup -> dispatch -> business logic -> result pack into `resp`.
 * Returns 0 if a response was produced (even an error response), -1 only on a
 * NULL argument. The response status distinguishes app failure from transport.
 */
int http3_pipeline_dispatch(http3_pipeline_t *pipe,
                            const http3_envelope_t *env,
                            http3_response_t *resp);

/*
 * Full receive path (§19 from "HTTP receive"): takes a wire buffer (textual or
 * binary, auto-detected), performs minimal parse + unpack, then an integrity
 * gate (per-packet DIGEST verify, then INTACTX tamper check), then dispatch.
 * A packet whose DIGEST fails is rejected (BAD_DIGEST); a packet whose INTACTX
 * variance exceeds the threshold is RESET (TAMPERED, response carries
 * HTTP3_FLAG_RESET) and never dispatched. Writes a packed textual response into
 * out/out_cap. Returns 0 on success.
 */
int http3_pipeline_handle_wire(http3_pipeline_t *pipe,
                               const uint8_t *wire, size_t wire_len,
                               char *out, size_t out_cap, size_t *written);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_PIPELINE_H */
