/* ==========================================================================
 * station_demo.c -- trace a two-party manualog through the Station so the
 * J-STA-0001 model is observable end to end: the Router witnesses conditions,
 * the start/intermediary/cancelling premise triple, agreement (CLEAR), and the
 * terminal states (UNCLEAR / HUNG-UP / DISASTROUS).
 *
 * Runs on the SIMULATION backend, so it needs no privileges and no kernel
 * module; the classifier/session logic is identical to the kernel driver.
 * ========================================================================== */
#include "station_backend.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, label) do { \
    if (cond) { printf("    [ok]   %s\n", (label)); } \
    else      { printf("    [FAIL] %s\n", (label)); ++failures; } \
} while (0)

static uint64_t clk = 1000; /* demo clock (ns), advanced per signal */

/* Send one signal and print how the Station witnessed it. */
static void step(sl_station_t *st, uint64_t sid, sl_party_t party,
                 uint8_t urg, int hop, uint8_t extra, const char *note)
{
    sl_signal_t sig = sl_make_signal(sid, party, urg, hop, extra, clk += 10);
    sl_record_t rec;
    int delivered;
    int premise_bearing = (urg > 0u) || (extra & SL_TCP_RST);
    memset(&rec, 0, sizeof(rec));
    delivered = (sl_station_signal(st, &sig, &rec) == 0);
    if (delivered && premise_bearing && rec.session_id == sid) {
        printf("    %s %-10s urg=%u hop=%+d -> cond=%-11s premise=%-12s %s | %s\n",
               party == SL_PARTY_A ? "A" : "B",
               "signals", urg, hop,
               sl_condition_name((sl_condition_t)rec.condition),
               sl_premise_name((sl_premise_t)rec.premise),
               sl_hop_verb_name((sl_hop_verb_t)rec.hop_verb), note);
    } else {
        printf("    %s %-10s urg=%u hop=%+d -> (control signal, no premise) | %s\n",
               party == SL_PARTY_A ? "A" : "B", "signals", urg, hop, note);
    }
}

static sl_state_t state_of(sl_station_t *st, uint64_t sid)
{
    sl_session_view_t v;
    memset(&v, 0, sizeof(v));
    sl_station_query(st, sid, &v);
    return (sl_state_t)v.state;
}

/* Scenario 1: a full manualog that reaches agreement (CLEAR). */
static void scenario_clear(sl_station_t *st)
{
    uint64_t sid = 0;
    sl_session_view_t v;
    printf("\n== Scenario 1: manualog reaches agreement (CLEAR) ==\n");
    CHECK(sl_station_open(st, &sid) == 0, "Station opened a witnessed session");

    /* start premise (ADVANCE opens START) */
    step(st, sid, SL_PARTY_A, 3, +1, 0, "start premise");
    step(st, sid, SL_PARTY_B, 3, +1, 0, "start premise");
    /* intermediary premise (second ADVANCE) with mutual ACK */
    step(st, sid, SL_PARTY_A, 5, +1, SL_TCP_ACK, "intermediary premise + ACK peer");
    step(st, sid, SL_PARTY_B, 5, +1, SL_TCP_ACK, "intermediary premise + ACK peer");

    memset(&v, 0, sizeof(v));
    sl_station_query(st, sid, &v);
    printf("    -> state=%s records=%u\n", sl_state_name((sl_state_t)v.state), v.record_count);
    CHECK(v.state == SL_STATE_CLEAR, "both reached matching INTERMEDIARY + ACK -> CLEAR");
    CHECK(v.record_count == 4u, "4 premise signals witnessed and stored");
    sl_station_close_session(st, sid);
}

/* Scenario 2: the cancelling premise leaves things UNCLEAR. */
static void scenario_unclear(sl_station_t *st)
{
    uint64_t sid = 0;
    printf("\n== Scenario 2: cancelling premise -> UNCLEAR ==\n");
    sl_station_open(st, &sid);
    step(st, sid, SL_PARTY_A, 4, +1, 0, "start premise");
    step(st, sid, SL_PARTY_B, 4, +1, 0, "start premise");
    step(st, sid, SL_PARTY_A, 4, -1, 0, "cancelling premise (DIST-1)");
    CHECK(state_of(st, sid) == SL_STATE_UNCLEAR, "standing cancellation -> UNCLEAR");
    sl_station_close_session(st, sid);
}

/* Scenario 3: a party FINs (goes away) -> HUNG-UP. */
static void scenario_hungup(sl_station_t *st)
{
    uint64_t sid = 0;
    printf("\n== Scenario 3: a party hangs up -> HUNG-UP ==\n");
    sl_station_open(st, &sid);
    step(st, sid, SL_PARTY_A, 3, +1, 0, "start premise");
    step(st, sid, SL_PARTY_B, 0, 0, SL_TCP_FIN, "FIN (hang up)");
    CHECK(state_of(st, sid) == SL_STATE_HUNG_UP, "FIN from a party -> HUNG-UP");
    sl_station_close_session(st, sid);
}

/* Scenario 4: RST aborts -> DISASTROUS. */
static void scenario_disastrous(sl_station_t *st)
{
    uint64_t sid = 0;
    printf("\n== Scenario 4: RST abort -> DISASTROUS ==\n");
    sl_station_open(st, &sid);
    step(st, sid, SL_PARTY_A, 3, +1, 0, "start premise");
    step(st, sid, SL_PARTY_B, 0, 0, SL_TCP_RST, "RST (abort)");
    CHECK(state_of(st, sid) == SL_STATE_DISASTROUS, "RST -> DISASTROUS");
    sl_station_close_session(st, sid);
}

/* Scenario 5: condition classification — LEGAL / WARNING / EMERGENCY. */
static void scenario_conditions(sl_station_t *st)
{
    uint64_t sid = 0;
    sl_signal_t sig; sl_record_t rec;
    printf("\n== Scenario 5: condition classification ==\n");
    sl_station_open(st, &sid);

    sig = sl_make_signal(sid, SL_PARTY_A, 8, +1, SL_TCP_ACK, clk += 10); /* URG+ACK@8 */
    sl_station_signal(st, &sig, &rec);
    CHECK(rec.condition == SL_COND_LEGAL, "URG+ACK at 8 urg -> LEGAL");

    sig = sl_make_signal(sid, SL_PARTY_B, 4, 0, SL_TCP_PSH, clk += 10); /* URG+PSH */
    sl_station_signal(st, &sig, &rec);
    CHECK(rec.condition == SL_COND_WARNING, "URG+PSH -> WARNING");

    sig = sl_make_signal(sid, SL_PARTY_A, 8, +1, SL_TCP_PSH | SL_TCP_ACK, clk += 10);
    sl_station_signal(st, &sig, &rec);
    CHECK(rec.condition == SL_COND_EMERGENCY, "URG+PSH+ACK at 8 urg -> EMERGENCY");

    sl_station_close_session(st, sid);
}

int main(void)
{
    sl_station_t st;
    printf("== SLeeLa HTTP 3.0 Station: two-party manualog (simulation backend) ==\n");
    if (sl_station_init(&st, SL_BACKEND_SIMULATION, NULL) != 0) {
        printf("station init failed\n");
        return 1;
    }

    scenario_clear(&st);
    scenario_unclear(&st);
    scenario_hungup(&st);
    scenario_disastrous(&st);
    scenario_conditions(&st);

    sl_station_shutdown(&st);

    if (failures == 0) {
        printf("\nStation manualog demo: PASS\n");
        return 0;
    }
    printf("\nStation manualog demo: FAIL (%d)\n", failures);
    return 1;
}
