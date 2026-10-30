#ifndef SLEELA_PHILOSOPHY_CONSISTENCY_MONITOR_H
#define SLEELA_PHILOSOPHY_CONSISTENCY_MONITOR_H
/*
 * Tree-consistency monitor for the philosophy event tree.
 *
 * This is the pure, deterministic rule layer. It does not spawn threads and
 * does not read a clock; the signal toss (signal_toss.*) drives it. Keeping
 * it side-effect free makes every rule unit-testable in isolation.
 *
 * Structural invariants enforced per branch and per sweep:
 *   - A branch carries at most SL_BRANCH_TRAIT_MAX (45) traits; 44 is the
 *     soft-warn threshold (SL_BRANCH_TRAIT_WARN).
 *   - A branch weight is bounded to [SL_BRANCH_WEIGHT_MIN, SL_BRANCH_WEIGHT_MAX]
 *     = [1, 9].
 *   - A weight-9 branch that studies the economy studies exactly
 *     SL_ECONOMY_STUDY_ARITY (2) things.
 *   - Neither side of the tree exceeds SL_SIDE_BRANCHPOINT_MAX (15) branching
 *     points; both sides are considered together for the total arrangement.
 *
 * Per-consideration update accounting (the "signal toss" cadence feeds this):
 *   - +SL_UPDATES_PER_BRANCH (4) each time a branch is considered.
 *   - +SL_UPDATES_PER_MATURE_LENGTH (5) each time a branch's length is
 *     considered while that branch is object-mature.
 *
 * Maturity is probabilistic by reference weighting, not a fixed age. Three
 * ascending gates on reference-weighted confidence:
 *   - 0.80 : provisional maturity (candidate)
 *   - 0.97 : strong maturity (confirmed)
 *   - 0.51 : the decisive majority-of-references gate; "object is Mature" is
 *            declared yes only when the reference-weighted truth is >= 0.51.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Structural bounds ------------------------------------------------- */
#define SL_BRANCH_TRAIT_WARN       44u
#define SL_BRANCH_TRAIT_MAX        45u
#define SL_BRANCH_WEIGHT_MIN        1u
#define SL_BRANCH_WEIGHT_MAX        9u
#define SL_ECONOMY_STUDY_ARITY      2u
#define SL_SIDE_BRANCHPOINT_MAX    15u

/* ---- Update accounting ------------------------------------------------- */
#define SL_UPDATES_PER_BRANCH          4u
#define SL_UPDATES_PER_MATURE_LENGTH   5u

/* ---- Maturity gates (reference-weighted confidence) -------------------- */
#define SL_MATURE_PROVISIONAL   0.80
#define SL_MATURE_STRONG        0.97
#define SL_MATURE_MAJORITY      0.51

typedef enum sl_maturity_stage {
    SL_MATURITY_IMMATURE = 0,   /* below the majority gate */
    SL_MATURITY_PROVISIONAL,    /* >= 0.80 confidence */
    SL_MATURITY_STRONG          /* >= 0.97 confidence */
} sl_maturity_stage;

/* One weighted reference contributing to a maturity judgement. */
typedef struct sl_reference {
    double weight;   /* relative importance of this reference, >= 0 */
    double truth;    /* this reference's support for maturity, [0, 1] */
} sl_reference;

typedef struct sl_maturity_result {
    double confidence;          /* reference-weighted truth in [0, 1] */
    sl_maturity_stage stage;    /* highest confidence gate reached */
    bool object_mature;         /* decisive: confidence >= 0.51 */
} sl_maturity_result;

/* A branch as seen by the consistency monitor. */
typedef struct sl_branch_view {
    unsigned trait_count;       /* number of traits attached to this branch */
    unsigned weight;            /* branch weight; clamped to [1, 9] on check */
    bool studies_economy;       /* branch studies the economy dimension */
    unsigned economy_arity;     /* number of things studied if economic */
    unsigned left_branchpoints; /* branching points on the left side */
    unsigned right_branchpoints;/* branching points on the right side */
    const sl_reference *refs;   /* reference weighting for maturity */
    size_t ref_count;
} sl_branch_view;

typedef enum sl_consistency_code {
    SL_CONSISTENT = 0,
    SL_INCONSISTENT_TRAITS,          /* trait_count > 45 */
    SL_INCONSISTENT_WEIGHT,          /* weight outside [1, 9] */
    SL_INCONSISTENT_ECONOMY_ARITY,   /* weight-9 economic branch not studying 2 */
    SL_INCONSISTENT_LEFT_HEIGHT,     /* left branching points > 15 */
    SL_INCONSISTENT_RIGHT_HEIGHT     /* right branching points > 15 */
} sl_consistency_code;

/* Running tally produced by the toss; all monotonic counters. */
typedef struct sl_consistency_report {
    uint64_t branch_considerations;      /* branches considered */
    uint64_t mature_length_considerations;/* mature-branch length considerations */
    uint64_t updates;                    /* 4*branches + 5*mature-lengths */
    uint64_t warnings;                   /* soft trait warnings (>=44) */
    uint64_t inconsistencies;            /* hard invariant failures */
    sl_consistency_code last_code;       /* most recent code observed */
} sl_consistency_report;

/* Clamp a raw weight into the legal [1, 9] band. */
unsigned sl_branch_clamp_weight(long raw);

/* Pure structural check of a single branch. Does not mutate the branch. */
sl_consistency_code sl_branch_check(const sl_branch_view *branch);

/* True when trait_count has reached the soft-warn threshold (44) but is still
 * within the hard maximum (45). */
bool sl_branch_trait_warn(const sl_branch_view *branch);

/* Reference-weighted maturity judgement with the 0.80 / 0.97 / 0.51 gates. */
sl_maturity_result sl_branch_maturity(const sl_branch_view *branch);

/* Fold one branch into a report: validates structure, accrues +4 for the
 * branch consideration, and +5 for the length consideration when the branch
 * is object-mature. Returns the branch's consistency code. */
sl_consistency_code sl_consistency_consider(
    sl_consistency_report *report, const sl_branch_view *branch);

void sl_consistency_report_init(sl_consistency_report *report);
const char *sl_consistency_code_string(sl_consistency_code code);

#ifdef __cplusplus
}
#endif
#endif
