#include "consistency_monitor.h"
#include "signal_toss.h"
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* ---- Structural invariant checks --------------------------------------- */
static void test_structure(void) {
    sl_branch_view b;
    b.trait_count = 10u;
    b.weight = 5u;
    b.studies_economy = false;
    b.economy_arity = 0u;
    b.left_branchpoints = 10u;
    b.right_branchpoints = 12u;
    b.refs = NULL;
    b.ref_count = 0u;
    assert(sl_branch_check(&b) == SL_CONSISTENT);

    /* 45 traits is the hard maximum (ok); 46 is inconsistent. */
    b.trait_count = SL_BRANCH_TRAIT_MAX;
    assert(sl_branch_check(&b) == SL_CONSISTENT);
    assert(sl_branch_trait_warn(&b));           /* 45 >= warn 44 */
    b.trait_count = 44u;
    assert(sl_branch_trait_warn(&b));
    b.trait_count = 43u;
    assert(!sl_branch_trait_warn(&b));
    b.trait_count = 46u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_TRAITS);
    b.trait_count = 10u;

    /* Weight band [1,9]. */
    b.weight = 0u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_WEIGHT);
    b.weight = 10u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_WEIGHT);
    assert(sl_branch_clamp_weight(0) == 1u);
    assert(sl_branch_clamp_weight(100) == 9u);
    assert(sl_branch_clamp_weight(4) == 4u);
    b.weight = 5u;

    /* A weight-9 economic branch studies exactly two things. */
    b.weight = 9u;
    b.studies_economy = true;
    b.economy_arity = 2u;
    assert(sl_branch_check(&b) == SL_CONSISTENT);
    b.economy_arity = 3u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_ECONOMY_ARITY);
    b.economy_arity = 1u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_ECONOMY_ARITY);
    /* The two-things rule only binds weight-9 economic branches. */
    b.weight = 8u;
    assert(sl_branch_check(&b) == SL_CONSISTENT);
    b.weight = 9u;
    b.economy_arity = 2u;

    /* Neither side exceeds 15 branching points. */
    b.left_branchpoints = 15u;
    b.right_branchpoints = 15u;
    assert(sl_branch_check(&b) == SL_CONSISTENT);
    b.left_branchpoints = 16u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_LEFT_HEIGHT);
    b.left_branchpoints = 10u;
    b.right_branchpoints = 16u;
    assert(sl_branch_check(&b) == SL_INCONSISTENT_RIGHT_HEIGHT);
}

/* ---- Maturity by reference weighting ----------------------------------- */
static void test_maturity(void) {
    sl_branch_view b;
    sl_maturity_result m;
    /* High-confidence references: all strongly support maturity. */
    sl_reference strong[] = {{3.0, 0.99}, {2.0, 0.98}, {1.0, 0.95}};
    /* Split references: weighted truth sits between the gates. */
    sl_reference mid[] = {{1.0, 1.0}, {1.0, 0.65}};   /* mean 0.825 */
    /* Minority support: below the decisive 0.51 gate. */
    sl_reference weak[] = {{1.0, 0.40}, {1.0, 0.30}}; /* mean 0.35 */
    /* Exactly on the majority boundary. */
    sl_reference edge[] = {{1.0, 0.51}};

    b.trait_count = 1u; b.weight = 1u; b.studies_economy = false;
    b.economy_arity = 0u; b.left_branchpoints = 0u; b.right_branchpoints = 0u;

    b.refs = strong; b.ref_count = 3u;
    m = sl_branch_maturity(&b);
    assert(m.stage == SL_MATURITY_STRONG);     /* >= 0.97 */
    assert(m.object_mature);
    assert(m.confidence >= SL_MATURE_STRONG);

    b.refs = mid; b.ref_count = 2u;
    m = sl_branch_maturity(&b);
    assert(m.stage == SL_MATURITY_PROVISIONAL);/* >= 0.80, < 0.97 */
    assert(m.object_mature);                   /* > 0.51 */

    b.refs = weak; b.ref_count = 2u;
    m = sl_branch_maturity(&b);
    assert(m.stage == SL_MATURITY_IMMATURE);
    assert(!m.object_mature);                  /* below 0.51 */

    b.refs = edge; b.ref_count = 1u;
    m = sl_branch_maturity(&b);
    assert(m.object_mature);                   /* exactly 0.51 counts as yes */
    assert(m.stage == SL_MATURITY_IMMATURE);   /* 0.51 < 0.80 */

    /* No references => not mature, confidence 0. */
    b.refs = NULL; b.ref_count = 0u;
    m = sl_branch_maturity(&b);
    assert(!m.object_mature && m.confidence == 0.0);
}

/* ---- Update accounting: +4 per branch, +5 per mature length ------------ */
static void test_accounting(void) {
    sl_consistency_report r;
    sl_branch_view mature, immature;
    sl_reference high[] = {{1.0, 0.99}};
    sl_reference low[] = {{1.0, 0.10}};

    sl_consistency_report_init(&r);

    mature.trait_count = 2u; mature.weight = 3u; mature.studies_economy = false;
    mature.economy_arity = 0u; mature.left_branchpoints = 1u;
    mature.right_branchpoints = 1u; mature.refs = high; mature.ref_count = 1u;

    immature = mature;
    immature.refs = low; immature.ref_count = 1u;

    /* One mature branch: +4 (branch) +5 (mature length) = 9. */
    assert(sl_consistency_consider(&r, &mature) == SL_CONSISTENT);
    assert(r.branch_considerations == 1u);
    assert(r.mature_length_considerations == 1u);
    assert(r.updates == SL_UPDATES_PER_BRANCH + SL_UPDATES_PER_MATURE_LENGTH);

    /* One immature branch: +4 only. */
    assert(sl_consistency_consider(&r, &immature) == SL_CONSISTENT);
    assert(r.branch_considerations == 2u);
    assert(r.mature_length_considerations == 1u);
    assert(r.updates == 2u * SL_UPDATES_PER_BRANCH + SL_UPDATES_PER_MATURE_LENGTH);

    /* An inconsistent branch still counts as considered (+4) but not mature. */
    mature.trait_count = 99u; /* too many traits */
    assert(sl_consistency_consider(&r, &mature) == SL_INCONSISTENT_TRAITS);
    assert(r.inconsistencies == 1u);
    assert(r.branch_considerations == 3u);
    assert(r.mature_length_considerations == 1u);
}

/* ---- Signal toss cadence & 800-1200/hr throttle ------------------------ */
static bool ever_mature_sampler(sl_branch_view *out, void *ctx) {
    static sl_reference refs[] = {{1.0, 0.99}};
    unsigned *remaining = (unsigned *)ctx;
    out->trait_count = 3u;
    out->weight = 9u;
    out->studies_economy = true;
    out->economy_arity = 2u;
    out->left_branchpoints = 12u;
    out->right_branchpoints = 13u;
    out->refs = refs;
    out->ref_count = 1u;
    if (*remaining == 0u) return false;
    --*remaining;
    return true;
}

static void test_toss_rates(void) {
    unsigned remaining;
    sl_signal_toss *toss;
    sl_consistency_report snap;
    uint64_t t_ms;
    unsigned i;

    /* Sample interval is ~22 ms => ~44 Hz. */
    assert(sl_signal_toss_sample_interval_ms() == 1000u / SL_TOSS_SAMPLE_HZ);

    /* Rate clamping into the 800-1200/hr band. */
    remaining = 1u;
    toss = sl_signal_toss_create(ever_mature_sampler, &remaining, 500u);
    assert(toss);
    /* 500 clamps to 800/hr => 4500 ms interval. */
    assert(sl_signal_toss_commit_interval_ms(toss) == 3600000u / 800u);
    sl_signal_toss_destroy(toss);

    remaining = 1u;
    toss = sl_signal_toss_create(ever_mature_sampler, &remaining, 5000u);
    assert(sl_signal_toss_commit_interval_ms(toss) == 3600000u / 1200u);
    sl_signal_toss_destroy(toss);

    remaining = 1u;
    toss = sl_signal_toss_create(ever_mature_sampler, &remaining, 0u); /* midpoint 1000 */
    assert(sl_signal_toss_commit_interval_ms(toss) == 3600000u / 1000u);

    /* Drive the step loop over a simulated hour at 44 Hz and confirm the
     * committed-update count lands in [800, 1200]. Each mature consistent
     * sample accrues +9 updates but commits are throttled by the interval. */
    remaining = 0xFFFFFFFFu; /* never asks to stop via remaining */
    t_ms = 0u;
    {
        const uint32_t step = sl_signal_toss_sample_interval_ms();
        const uint64_t hour = 3600000u;
        for (t_ms = 0u; t_ms < hour; t_ms += step)
            (void)sl_signal_toss_step(toss, t_ms);
    }
    snap = sl_signal_toss_snapshot(toss);
    {
        uint64_t commits = sl_signal_toss_commits(toss);
        printf("  simulated hour: samples=%" PRIu64 " updates=%" PRIu64
               " commits=%" PRIu64 "\n",
               snap.branch_considerations, snap.updates, commits);
        assert(commits >= SL_TOSS_COMMIT_MIN_PER_HOUR);
        assert(commits <= SL_TOSS_COMMIT_MAX_PER_HOUR + 1u); /* +1 boundary */
        /* ~44 Hz across an hour ~= 162k samples; each mature branch => +9. */
        assert(snap.branch_considerations > 100000u);
        assert(snap.updates == snap.branch_considerations * 9u);
    }
    (void)i;
    sl_signal_toss_destroy(toss);
}

/* ---- Threaded smoke test ----------------------------------------------- */
static void test_toss_thread(void) {
    unsigned remaining = 200u;
    sl_signal_toss *toss = sl_signal_toss_create(ever_mature_sampler, &remaining, 1000u);
    sl_consistency_report snap;
    assert(toss);
    assert(sl_signal_toss_start(toss) == 0);
    /* Let the worker run briefly, then stop and join. */
    sl_sleep_ms(250u);
    sl_signal_toss_stop(toss);
    snap = sl_signal_toss_snapshot(toss);
    assert(snap.branch_considerations > 0u);
    assert(snap.inconsistencies == 0u);
    sl_signal_toss_destroy(toss);
}

int main(void) {
    printf("test_structure...\n");    test_structure();
    printf("test_maturity...\n");     test_maturity();
    printf("test_accounting...\n");   test_accounting();
    printf("test_toss_rates...\n");   test_toss_rates();
    printf("test_toss_thread...\n");  test_toss_thread();
    printf("all consistency tests passed\n");
    return 0;
}
