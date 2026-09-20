/* ==========================================================================
 * http3_pipeline_demo.c -- traces a request through the full HTTP 3.0 data
 * flow so the spec's §4/§5/§7/§9/§19 model is observable end to end.
 *
 * It registers an "orders" service with two operations, then shows:
 *   - first request by NAME -> compact SERVICE-ID/OP-ID (§4 fast naming),
 *   - the compact envelope packed both textual and binary (§5),
 *   - the §19 pipeline receiving the wire bytes and dispatching,
 *   - the response with echoed REQUEST-ID and application STATUS (§7),
 *   - retry-class reporting (§9), and posture checks (unknown svc/op).
 * ========================================================================== */
#include "http3_pipeline.h"
#include "http3_intactx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, label) do { \
    if (cond) { printf("  [ok]   %s\n", (label)); } \
    else      { printf("  [FAIL] %s\n", (label)); ++failures; } \
} while (0)

/* orders.calculate: payload "A,B" -> result "sum=<A+B>" (READ, safe to retry). */
static http3_status_t op_calculate(const uint8_t *payload, size_t payload_len,
                                   uint8_t *result, size_t result_cap,
                                   size_t *result_len, void *ctx)
{
    char buf[64];
    long a = 0, b = 0;
    int n;
    (void)ctx;
    if (payload_len >= sizeof(buf)) {
        return HTTP3_STATUS_TOO_LARGE;
    }
    memcpy(buf, payload, payload_len);
    buf[payload_len] = '\0';
    if (sscanf(buf, "%ld,%ld", &a, &b) != 2) {
        return HTTP3_STATUS_APP_ERROR;
    }
    n = snprintf((char *)result, result_cap, "sum=%ld", a + b);
    if (n < 0 || (size_t)n >= result_cap) {
        return HTTP3_STATUS_TOO_LARGE;
    }
    *result_len = (size_t)n;
    return HTTP3_STATUS_OK;
}

/* orders.place: a MUTATING op; echoes an order id (§9 mutating class). */
static http3_status_t op_place(const uint8_t *payload, size_t payload_len,
                               uint8_t *result, size_t result_cap,
                               size_t *result_len, void *ctx)
{
    unsigned *counter = (unsigned *)ctx;
    int n;
    (void)payload; (void)payload_len;
    n = snprintf((char *)result, result_cap, "order_id=%u", counter ? ++(*counter) : 0u);
    if (n < 0 || (size_t)n >= result_cap) {
        return HTTP3_STATUS_TOO_LARGE;
    }
    *result_len = (size_t)n;
    return HTTP3_STATUS_OK;
}

int main(void)
{
    http3_pipeline_t pipe;
    http3_intactx_t ix;
    uint64_t intactx;
    unsigned order_counter = 0;
    uint32_t svc, op_calc, op_place_id;
    http3_envelope_t env;
    char text[512];
    uint8_t bin[512];
    char resp_buf[512];
    size_t written = 0;
    http3_response_t resp;

    printf("== HTTP 3.0 data flow: request -> envelope -> pipeline -> response ==\n");

    http3_pipeline_init(&pipe);

    /* INTACTX: establish/load this host's integrity baseline (persisted). Every
     * packet we emit is stamped with a fresh INTACTX reading. */
    http3_intactx_init(&ix, "./.http3_intactx_baseline.demo");
    intactx = http3_intactx_compute(&ix);
    printf("\n-- INTACTX host identity --\n");
    printf("  baseline %s, intactx=%llu variance=%u\n",
           ix.loaded ? "loaded" : "established",
           (unsigned long long)intactx, (unsigned)http3_intactx_variance(intactx));

    /* §4: register by name; compact ids are assigned on first use. */
    svc = http3_pipeline_register_service(&pipe, "orders", &order_counter);
    op_calc = http3_pipeline_register_op(&pipe, "orders", "calculate",
                                         HTTP3_RETRY_READ, op_calculate);
    op_place_id = http3_pipeline_register_op(&pipe, "orders", "place",
                                             HTTP3_RETRY_MUTATING, op_place);
    printf("\n-- §4 fast naming --\n");
    printf("  service \"orders\"    -> id %u\n", svc);
    printf("  op \"calculate\"      -> id %u (retry=READ)\n", op_calc);
    printf("  op \"place\"          -> id %u (retry=MUTATING)\n", op_place_id);
    CHECK(svc == 1u && op_calc == 1u && op_place_id == 2u, "compact ids assigned in first-use order");
    CHECK(http3_naming_lookup_service(&pipe.naming, "orders") == svc, "service name re-resolves to same id (cached)");

    /* §5: build the compact envelope for orders.calculate("20,22"). The init
     * stamps the INTACTX host identity and seals the per-packet DIGEST. */
    CHECK(http3_envelope_init(&env, svc, op_calc, /*request_id*/ 1001u,
                              HTTP3_FLAG_NONE, intactx,
                              (const uint8_t *)"20,22", 5u) == 0,
          "envelope initialized");
    CHECK(http3_envelope_verify_digest(&env), "per-packet DIGEST verifies");
    printf("  DIGEST=%llu  INTACTX=%llu\n",
           (unsigned long long)env.digest, (unsigned long long)env.intactx);

    /* §5: pack textual, then binary; both carry the same logical envelope. */
    CHECK(http3_envelope_pack_text(&env, text, sizeof(text), &written) == 0, "envelope packed (textual)");
    printf("\n-- §5 compact envelope (textual wire) --\n  %.*s", (int)written, text);
    CHECK(http3_envelope_pack_binary(&env, bin, sizeof(bin), &written) == 0, "envelope packed (binary)");
    printf("-- §5 compact envelope (binary wire) --\n  %zu bytes: ", written);
    for (size_t i = 0; i < written; ++i) printf("%02x", bin[i]);
    printf("\n");

    /* §19: feed the TEXTUAL wire through the pipeline. */
    printf("\n-- §19 pipeline (textual in) --\n");
    CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)text, strlen(text),
                                     resp_buf, sizeof(resp_buf), &written) == 0,
          "textual wire handled");
    printf("  response: %.*s", (int)written, resp_buf);
    CHECK(http3_response_unpack_text(resp_buf, written, &resp) == 0, "response parsed");
    CHECK(resp.status == HTTP3_STATUS_OK, "status OK (§7)");
    CHECK(resp.request_id == 1001u, "REQUEST-ID echoed (§6/§7)");
    CHECK(resp.result_len == 6u && memcmp(resp.result, "sum=42", 6) == 0, "result sum=42");

    /* §19: feed the BINARY wire through the pipeline -> identical result. */
    printf("\n-- §19 pipeline (binary in) --\n");
    (void)http3_envelope_pack_binary(&env, bin, sizeof(bin), &written);
    CHECK(http3_pipeline_handle_wire(&pipe, bin, written,
                                     resp_buf, sizeof(resp_buf), &written) == 0,
          "binary wire handled");
    CHECK(http3_response_unpack_text(resp_buf, written, &resp) == 0 &&
          resp.status == HTTP3_STATUS_OK &&
          resp.result_len == 6u && memcmp(resp.result, "sum=42", 6) == 0,
          "binary path yields identical result (sum=42)");

    /* §9: retry-class reporting. */
    printf("\n-- §9 retry classes --\n");
    CHECK(http3_pipeline_retry_class(&pipe, svc, op_calc) == HTTP3_RETRY_READ, "calculate is READ");
    CHECK(http3_pipeline_retry_class(&pipe, svc, op_place_id) == HTTP3_RETRY_MUTATING, "place is MUTATING");

    /* §19 posture: unknown service and unknown op return typed statuses. */
    printf("\n-- §7 posture: unknown service / operation --\n");
    (void)http3_envelope_init(&env, 999u, 1u, 2002u, HTTP3_FLAG_NONE, intactx, NULL, 0u);
    (void)http3_pipeline_dispatch(&pipe, &env, &resp);
    CHECK(resp.status == HTTP3_STATUS_UNKNOWN_SVC, "unknown service -> UNKNOWN_SERVICE");
    (void)http3_envelope_init(&env, svc, 999u, 2003u, HTTP3_FLAG_NONE, intactx, NULL, 0u);
    (void)http3_pipeline_dispatch(&pipe, &env, &resp);
    CHECK(resp.status == HTTP3_STATUS_UNKNOWN_OP, "unknown op -> UNKNOWN_OPERATION");

    /* Integrity gate: a mangled packet (bad DIGEST) is rejected before dispatch. */
    printf("\n-- integrity: corrupted packet -> BAD_DIGEST --\n");
    (void)http3_envelope_init(&env, svc, op_calc, 3001u, HTTP3_FLAG_NONE,
                              intactx, (const uint8_t *)"20,22", 5u);
    (void)http3_envelope_pack_text(&env, text, sizeof(text), &written);
    /* Flip a payload byte AFTER the digest was sealed: 20,22 -> 90,22. */
    {
        char *dig = strstr(text, ":20,22");
        if (dig != NULL) { dig[1] = '9'; }
    }
    CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)text, strlen(text),
                                     resp_buf, sizeof(resp_buf), &written) == 0,
          "corrupted wire handled");
    (void)http3_response_unpack_text(resp_buf, written, &resp);
    CHECK(resp.status == HTTP3_STATUS_BAD_DIGEST, "corrupted packet -> BAD_DIGEST");

    /* Tamper reset: a packet whose INTACTX variance exceeds the threshold is
     * RESET and never dispatched. Simulate a tampered host by forcing a
     * max-variance INTACTX value on the packet. */
    printf("\n-- INTACTX: tampered host -> RESET --\n");
    {
        uint64_t tampered = ((uint64_t)HTTP3_INTACTX_VARIANCE_MASK
                                 << HTTP3_INTACTX_VARIANCE_SHIFT) | 0x1234u;
        (void)http3_envelope_init(&env, svc, op_calc, 3002u, HTTP3_FLAG_RESET,
                                  tampered, (const uint8_t *)"20,22", 5u);
        (void)http3_envelope_pack_text(&env, text, sizeof(text), &written);
        CHECK(http3_intactx_is_tampered(tampered, pipe.intactx_threshold),
              "high-variance INTACTX flagged as tampered");
        CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)text, strlen(text),
                                         resp_buf, sizeof(resp_buf), &written) == 0,
              "tampered wire handled");
        (void)http3_response_unpack_text(resp_buf, written, &resp);
        CHECK(resp.status == HTTP3_STATUS_TAMPERED &&
              resp.result_len == 5u && memcmp(resp.result, "RESET", 5) == 0,
              "tampered packet -> TAMPERED + RESET");
    }

    printf("\n-- observability (§17) --\n"
           "  requests handled: %llu\n"
           "  digest rejects:   %llu\n"
           "  tamper resets:    %llu\n",
           (unsigned long long)pipe.requests_handled,
           (unsigned long long)pipe.digest_rejects,
           (unsigned long long)pipe.tamper_resets);

    if (failures == 0) {
        printf("\nHTTP 3.0 pipeline demo: PASS\n");
        return 0;
    }
    printf("\nHTTP 3.0 pipeline demo: FAIL (%d)\n", failures);
    return 1;
}
