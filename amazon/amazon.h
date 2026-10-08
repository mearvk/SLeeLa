#ifndef SLEELA_AMAZON_H
#define SLEELA_AMAZON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_AMAZON_HIRE = 1,
    SL_AMAZON_RETAIN = 2,
    SL_AMAZON_REASSIGN = 3,
    SL_AMAZON_TRAIN = 4,
    SL_AMAZON_RETIRE = 5,
    SL_AMAZON_TERMINATE = 6
} sl_amazon_action;

typedef struct {
    const char *person_id;
    const char *role;
    double skill_fit;
    double performance;
    double training_need;
    double retention_value;
    int voluntary_transition;
} sl_amazon_profile;

typedef struct {
    sl_amazon_action action;
    double score;
    int requires_human_review;
    int policy_ok;
    const char *reason;
} sl_amazon_recommendation;

int sl_amazon_evaluate(const sl_amazon_profile *profile,
                       sl_amazon_action requested,
                       sl_amazon_recommendation *out);

int sl_amazon_policy_check(const char *jurisdiction,
                           sl_amazon_action action,
                           int voluntary_transition);

#ifdef __cplusplus
}
#endif
#endif
