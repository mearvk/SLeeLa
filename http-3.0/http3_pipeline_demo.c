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
#include "http3_handshake.h"

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
    uint64_t nonce = 0; /* monotonic per-connection packet counter (replay guard) */
    unsigned order_counter = 0;
    uint32_t svc, op_calc, op_place_id;
    http3_envelope_t env;
    char text[1024];
    uint8_t bin[1024];
    char resp_buf[512];
    size_t written = 0;
    http3_response_t resp;

    /* Per-connection MAC key (in a deployment this comes from key agreement).
     * Both the sender and the pipeline share it; a forger without it cannot
     * produce a valid per-packet DIGEST. */
    static const uint8_t mac_key[HTTP3_MAC_KEY_BYTES] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
    };

    printf("== HTTP 3.0 data flow: request -> envelope -> pipeline -> response ==\n");

    http3_pipeline_init(&pipe);
    http3_pipeline_set_mac_key(&pipe, mac_key);

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
     * stamps the INTACTX host identity + a monotonic NONCE and seals the keyed
     * DIGEST. Each fresh packet uses the next NONCE (replay guard). */
    CHECK(http3_envelope_init(&env, svc, op_calc, /*request_id*/ 1001u,
                              HTTP3_FLAG_NONE, /*nonce*/ ++nonce, intactx, mac_key,
                              (const uint8_t *)"20,22", 5u) == 0,
          "envelope initialized");
    CHECK(http3_envelope_verify_digest(&env, mac_key), "per-packet keyed DIGEST verifies");
    printf("  NONCE=%llu  DIGEST=%llu  INTACTX=%llu\n",
           (unsigned long long)env.nonce,
           (unsigned long long)env.digest, (unsigned long long)env.intactx);

    /* Basket of goods & services: the fixed 14-item basket rides on every
     * packet, atomic-bound to the US capitalism system (ISO USD per gram). */
    {
        const http3_basket_item_t *table = http3_basket_table();
        http3_basket_item_t parsed[HTTP3_BASKET_ITEMS];
        uint16_t iso = 0;
        size_t count, i;
        printf("\n-- basket of goods & services (%u items, ISO %s/%u per gram) --\n",
               (unsigned)HTTP3_BASKET_ITEMS, HTTP3_BASKET_ISO_CURRENCY,
               (unsigned)HTTP3_BASKET_ISO_NUMERIC);
        for (i = 0; i < HTTP3_BASKET_ITEMS; ++i) {
            printf("  #%2u  %-28s %12llu uUSD/g\n",
                   (unsigned)table[i].atomic_number, table[i].name,
                   (unsigned long long)table[i].value_ugram);
        }
        printf("  basket block: %u bytes; total %llu uUSD/g\n",
               (unsigned)HTTP3_BASKET_BLOCK_SIZE,
               (unsigned long long)http3_basket_total_ugram());
        /* The basket in the packet parses back to the fixed table. */
        count = http3_basket_parse(env.basket, sizeof(env.basket),
                                   parsed, HTTP3_BASKET_ITEMS, &iso);
        CHECK(count == HTTP3_BASKET_ITEMS && iso == HTTP3_BASKET_ISO_NUMERIC,
              "packet basket parses to 14 items (ISO USD)");
        CHECK(parsed[0].atomic_number == table[0].atomic_number &&
              parsed[0].value_ugram == table[0].value_ugram &&
              parsed[HTTP3_BASKET_ITEMS - 1].value_ugram ==
                  table[HTTP3_BASKET_ITEMS - 1].value_ugram,
              "packet basket values match the fixed table");
    }

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

    /* §19: feed the BINARY wire through the pipeline -> identical result. Use a
     * fresh NONCE so this is a new packet, not a replay of the textual one. */
    printf("\n-- §19 pipeline (binary in) --\n");
    (void)http3_envelope_init(&env, svc, op_calc, 1002u, HTTP3_FLAG_NONE,
                              /*nonce*/ ++nonce, intactx, mac_key,
                              (const uint8_t *)"20,22", 5u);
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
    (void)http3_envelope_init(&env, 999u, 1u, 2002u, HTTP3_FLAG_NONE, ++nonce, intactx, mac_key, NULL, 0u);
    (void)http3_pipeline_dispatch(&pipe, &env, &resp);
    CHECK(resp.status == HTTP3_STATUS_UNKNOWN_SVC, "unknown service -> UNKNOWN_SERVICE");
    (void)http3_envelope_init(&env, svc, 999u, 2003u, HTTP3_FLAG_NONE, ++nonce, intactx, mac_key, NULL, 0u);
    (void)http3_pipeline_dispatch(&pipe, &env, &resp);
    CHECK(resp.status == HTTP3_STATUS_UNKNOWN_OP, "unknown op -> UNKNOWN_OPERATION");

    /* Integrity gate: a mangled packet (bad MAC) is rejected before dispatch. */
    printf("\n-- integrity: corrupted packet -> BAD_DIGEST --\n");
    (void)http3_envelope_init(&env, svc, op_calc, 3001u, HTTP3_FLAG_NONE,
                              ++nonce, intactx, mac_key, (const uint8_t *)"20,22", 5u);
    (void)http3_envelope_pack_text(&env, text, sizeof(text), &written);
    /* Flip a payload byte AFTER the MAC was sealed: 20,22 -> 90,22. */
    {
        char *dig = strstr(text, ":20,22");
        if (dig != NULL) { dig[1] = '9'; }
    }
    CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)text, strlen(text),
                                     resp_buf, sizeof(resp_buf), &written) == 0,
          "corrupted wire handled");
    (void)http3_response_unpack_text(resp_buf, written, &resp);
    CHECK(resp.status == HTTP3_STATUS_BAD_DIGEST, "corrupted packet -> BAD_DIGEST");

    /* Forgery: a well-formed packet whose MAC was computed with the WRONG key
     * (an attacker who lacks the shared secret) is rejected. This is what a
     * keyed MAC buys over a plain hash -- the attacker can recompute a hash but
     * not a valid tag. */
    printf("\n-- integrity: forged packet (wrong key) -> BAD_DIGEST --\n");
    {
        static const uint8_t wrong_key[HTTP3_MAC_KEY_BYTES] = {
            0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
            0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00
        };
        (void)http3_envelope_init(&env, svc, op_calc, 3003u, HTTP3_FLAG_NONE,
                                  ++nonce, intactx, wrong_key, (const uint8_t *)"20,22", 5u);
        (void)http3_envelope_pack_text(&env, text, sizeof(text), &written);
        CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)text, strlen(text),
                                         resp_buf, sizeof(resp_buf), &written) == 0,
              "forged wire handled");
        (void)http3_response_unpack_text(resp_buf, written, &resp);
        CHECK(resp.status == HTTP3_STATUS_BAD_DIGEST,
              "forged packet (wrong MAC key) -> BAD_DIGEST");
    }

    /* Tamper reset: a packet whose INTACTX variance exceeds the threshold is
     * RESET and never dispatched. Simulate a tampered host by forcing a
     * max-variance INTACTX value on the packet. */
    printf("\n-- INTACTX: tampered host -> RESET --\n");
    {
        uint64_t tampered = ((uint64_t)HTTP3_INTACTX_VARIANCE_MASK
                                 << HTTP3_INTACTX_VARIANCE_SHIFT) | 0x1234u;
        (void)http3_envelope_init(&env, svc, op_calc, 3002u, HTTP3_FLAG_RESET,
                                  ++nonce, tampered, mac_key, (const uint8_t *)"20,22", 5u);
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

    /* Replay guard: a fresh packet is accepted; re-sending the SAME wire bytes
     * (same NONCE, valid MAC) is rejected as a replay. Because the NONCE is
     * inside the MAC, an attacker cannot edit it to dodge the check. */
    printf("\n-- replay: same packet resent -> REPLAYED --\n");
    {
        size_t replay_len;
        char replay_wire[1024];
        (void)http3_envelope_init(&env, svc, op_calc, 4001u, HTTP3_FLAG_NONE,
                                  ++nonce, intactx, mac_key,
                                  (const uint8_t *)"1,2", 3u);
        (void)http3_envelope_pack_text(&env, replay_wire, sizeof(replay_wire), &replay_len);
        /* First delivery: accepted, advances the high-water mark. */
        CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)replay_wire, replay_len,
                                         resp_buf, sizeof(resp_buf), &written) == 0,
              "fresh packet handled");
        (void)http3_response_unpack_text(resp_buf, written, &resp);
        CHECK(resp.status == HTTP3_STATUS_OK && resp.result_len == 5u &&
              memcmp(resp.result, "sum=3", 5) == 0, "fresh packet accepted (sum=3)");
        /* Replay: identical bytes resent -> rejected without dispatch. */
        CHECK(http3_pipeline_handle_wire(&pipe, (const uint8_t *)replay_wire, replay_len,
                                         resp_buf, sizeof(resp_buf), &written) == 0,
              "replayed packet handled");
        (void)http3_response_unpack_text(resp_buf, written, &resp);
        CHECK(resp.status == HTTP3_STATUS_REPLAYED, "replayed packet -> REPLAYED");
    }

    /* Connection-level timing (advisory): max speed, on time, balance, and a
     * running carrier-certainty estimate. Times are nanoseconds from a caller
     * clock; nothing here rejects a packet. */
    printf("\n-- timing: max speed / on time / balance / carrier certainty --\n");
    {
        /* A well-paced stream: ~10ms apart, each within its deadline. */
        uint64_t base = 1000000000ull; /* 1s */
        uint64_t step = 10000000ull;   /* 10ms */
        int k;
        unsigned f;
        http3_pipeline_set_timing(&pipe, 1000ull /*min gap 1us*/,
                                  50000000ull /*50ms grace*/, 10000000ull /*10ms band*/);
        uint64_t last_at = base; /* track the most recent arrival we fed in */
        for (k = 0; k < 6; ++k) {
            uint64_t at = base + (uint64_t)k * step;
            uint64_t due = at; /* on time: arrives exactly when due */
            (void)http3_pipeline_observe_timing(&pipe, at, due);
            last_at = at;
        }
        CHECK(pipe.late_packets == 0u && pipe.over_rate_packets == 0u,
              "well-paced stream: no late / over-rate flags");
        /* A burst arriving only 100ns after the previous packet -> OVER_RATE
         * (below the 1us minimum gap). */
        last_at = last_at + 100ull;
        f = http3_pipeline_observe_timing(&pipe, last_at, 0ull);
        CHECK((f & HTTP3_TIMING_OVER_RATE) != 0u, "burst arrival -> OVER_RATE flagged");
        /* A packet arriving well past its deadline -> LATE. */
        f = http3_pipeline_observe_timing(&pipe, last_at + 100ull * step,
                                          last_at /*due long ago*/);
        CHECK((f & HTTP3_TIMING_LATE) != 0u, "past-deadline arrival -> LATE flagged");
        printf("  carrier certainty = %.3f (1.0 = fully dependable)\n",
               http3_pipeline_carrier_certainty(&pipe));
    }

    /* Capability handshake: two peers advertise their supported levels; the
     * negotiation picks the highest common one, with a baseline fallback so an
     * older router runs L1 until its software is updated. MAC-backed offers. */
    printf("\n-- handshake: capability negotiation (highest common level) --\n");
    {
        http3_cap_offer_t full, older, l3;
        uint32_t agreed;
        (void)http3_handshake_make_offer(&full, HTTP3_CAP_ALL, mac_key);
        (void)http3_handshake_make_offer(&older, HTTP3_CAP_L1_BASE, mac_key);
        (void)http3_handshake_make_offer(&l3, HTTP3_CAP_L1_BASE | HTTP3_CAP_L2_PERLEG |
                                              HTTP3_CAP_L3_PACING, mac_key);
        CHECK(http3_handshake_verify_offer(&full, mac_key), "offer MAC verifies");
        (void)http3_handshake_resolve(&full, &older, mac_key, &agreed);
        printf("  full <-> older router -> %s\n", http3_handshake_level_name(agreed));
        CHECK(agreed == HTTP3_CAP_L1_BASE, "older router caps the pair at L1 baseline");
        (void)http3_handshake_resolve(&full, &l3, mac_key, &agreed);
        printf("  full <-> L3 peer      -> %s\n", http3_handshake_level_name(agreed));
        CHECK(agreed == HTTP3_CAP_L3_PACING, "highest common level is L3");
        (void)http3_handshake_resolve(&full, &full, mac_key, &agreed);
        printf("  full <-> full         -> %s\n", http3_handshake_level_name(agreed));
        CHECK(agreed == HTTP3_CAP_L4_ECHO, "two current peers reach L4");
    }

    printf("\n-- observability (§17) --\n"
           "  requests handled: %llu\n"
           "  digest rejects:   %llu\n"
           "  tamper resets:    %llu\n"
           "  replays rejected: %llu\n"
           "  late packets:     %llu\n"
           "  over-rate packets:%llu\n"
           "  unbalanced pkts:  %llu\n"
           "  carrier certainty:%.3f\n",
           (unsigned long long)pipe.requests_handled,
           (unsigned long long)pipe.digest_rejects,
           (unsigned long long)pipe.tamper_resets,
           (unsigned long long)pipe.replays_rejected,
           (unsigned long long)pipe.late_packets,
           (unsigned long long)pipe.over_rate_packets,
           (unsigned long long)pipe.unbalanced_packets,
           http3_pipeline_carrier_certainty(&pipe));

    if (failures == 0) {
        printf("\nHTTP 3.0 pipeline demo: PASS\n");
        return 0;
    }
    printf("\nHTTP 3.0 pipeline demo: FAIL (%d)\n", failures);
    return 1;
}
