#include "amazon.h"

#include <string.h>

int sl_amazon_policy_check(const char *jurisdiction,
                           sl_amazon_action action,
                           int voluntary_transition) {
    (void)jurisdiction;
    /* Employment-law review is always conservative. Termination and
       involuntary transition remain human-review actions. */
    if (action == SL_AMAZON_TERMINATE && !voluntary_transition) return 0;
    return 1;
}

int sl_amazon_evaluate(const sl_amazon_profile *p,
                       sl_amazon_action requested,
                       sl_amazon_recommendation *out) {
    if (!p || !out || !p->person_id || !p->role) return -1;

    out->action = requested;
    out->score = (p->skill_fit * 0.40) +
                 (p->performance * 0.35) +
                 (p->retention_value * 0.25);
    out->requires_human_review =
        (requested == SL_AMAZON_TERMINATE ||
         requested == SL_AMAZON_RETIRE ||
         requested == SL_AMAZON_REASSIGN);
    out->policy_ok = sl_amazon_policy_check(NULL, requested,
                                            p->voluntary_transition);

    if (requested == SL_AMAZON_TRAIN) {
        out->reason = "training path selected";
    } else if (requested == SL_AMAZON_RETIRE) {
        out->reason = "transition/retirement review selected";
    } else if (requested == SL_AMAZON_TERMINATE) {
        out->reason = "termination requires documented human review";
    } else {
        out->reason = "staffing action selected from role criteria";
    }
    return 0;
}
