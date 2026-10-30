#include "consistency_monitor.h"
#include <math.h>

unsigned sl_branch_clamp_weight(long raw) {
    if (raw < (long)SL_BRANCH_WEIGHT_MIN) return SL_BRANCH_WEIGHT_MIN;
    if (raw > (long)SL_BRANCH_WEIGHT_MAX) return SL_BRANCH_WEIGHT_MAX;
    return (unsigned)raw;
}

bool sl_branch_trait_warn(const sl_branch_view *branch) {
    if (!branch) return false;
    return branch->trait_count >= SL_BRANCH_TRAIT_WARN &&
           branch->trait_count <= SL_BRANCH_TRAIT_MAX;
}

sl_consistency_code sl_branch_check(const sl_branch_view *branch) {
    if (!branch) return SL_INCONSISTENT_TRAITS;

    /* A branch cannot carry more than 44 or 45 traits. */
    if (branch->trait_count > SL_BRANCH_TRAIT_MAX)
        return SL_INCONSISTENT_TRAITS;

    /* Weight lives in [1, 9]. */
    if (branch->weight < SL_BRANCH_WEIGHT_MIN ||
        branch->weight > SL_BRANCH_WEIGHT_MAX)
        return SL_INCONSISTENT_WEIGHT;

    /* Some weight-9 branches study the economy of only two things. */
    if (branch->weight == SL_BRANCH_WEIGHT_MAX && branch->studies_economy &&
        branch->economy_arity != SL_ECONOMY_STUDY_ARITY)
        return SL_INCONSISTENT_ECONOMY_ARITY;

    /* Neither side exceeds 12-15 branching points; 15 is the hard ceiling. */
    if (branch->left_branchpoints > SL_SIDE_BRANCHPOINT_MAX)
        return SL_INCONSISTENT_LEFT_HEIGHT;
    if (branch->right_branchpoints > SL_SIDE_BRANCHPOINT_MAX)
        return SL_INCONSISTENT_RIGHT_HEIGHT;

    return SL_CONSISTENT;
}

sl_maturity_result sl_branch_maturity(const sl_branch_view *branch) {
    sl_maturity_result out;
    double weight_sum = 0.0;
    double weighted_truth = 0.0;
    size_t i;

    out.confidence = 0.0;
    out.stage = SL_MATURITY_IMMATURE;
    out.object_mature = false;
    if (!branch || !branch->refs || branch->ref_count == 0u) return out;

    for (i = 0u; i < branch->ref_count; ++i) {
        double w = branch->refs[i].weight;
        double t = branch->refs[i].truth;
        if (!isfinite(w) || !isfinite(t) || w < 0.0) continue;
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;
        weight_sum += w;
        weighted_truth += w * t;
    }
    if (weight_sum <= 0.0) return out;

    out.confidence = weighted_truth / weight_sum;

    /* Ascending confidence gates. */
    if (out.confidence >= SL_MATURE_STRONG)
        out.stage = SL_MATURITY_STRONG;         /* 97% probable */
    else if (out.confidence >= SL_MATURE_PROVISIONAL)
        out.stage = SL_MATURITY_PROVISIONAL;    /* 80% probable */

    /* Decisive majority-of-references gate: only a yes at >= 51% true. */
    out.object_mature = out.confidence >= SL_MATURE_MAJORITY;
    return out;
}

void sl_consistency_report_init(sl_consistency_report *report) {
    if (!report) return;
    report->branch_considerations = 0u;
    report->mature_length_considerations = 0u;
    report->updates = 0u;
    report->warnings = 0u;
    report->inconsistencies = 0u;
    report->last_code = SL_CONSISTENT;
}

sl_consistency_code sl_consistency_consider(
    sl_consistency_report *report, const sl_branch_view *branch) {
    sl_consistency_code code;
    sl_maturity_result maturity;
    if (!report || !branch) return SL_INCONSISTENT_TRAITS;

    code = sl_branch_check(branch);
    report->last_code = code;

    /* Every branch considered: +4 updates. */
    ++report->branch_considerations;
    report->updates += SL_UPDATES_PER_BRANCH;

    if (sl_branch_trait_warn(branch)) ++report->warnings;
    if (code != SL_CONSISTENT) {
        ++report->inconsistencies;
        return code;
    }

    /* Length considered in the mature state: +5 updates. */
    maturity = sl_branch_maturity(branch);
    if (maturity.object_mature) {
        ++report->mature_length_considerations;
        report->updates += SL_UPDATES_PER_MATURE_LENGTH;
    }
    return code;
}

const char *sl_consistency_code_string(sl_consistency_code code) {
    switch (code) {
        case SL_CONSISTENT: return "consistent";
        case SL_INCONSISTENT_TRAITS: return "too many traits (>45)";
        case SL_INCONSISTENT_WEIGHT: return "weight outside [1,9]";
        case SL_INCONSISTENT_ECONOMY_ARITY: return "weight-9 economy study must cover exactly 2 things";
        case SL_INCONSISTENT_LEFT_HEIGHT: return "left side exceeds 15 branching points";
        case SL_INCONSISTENT_RIGHT_HEIGHT: return "right side exceeds 15 branching points";
        default: return "unknown";
    }
}
