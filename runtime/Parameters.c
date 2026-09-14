/* ========================================================================
 * Parameters.c -- the explicit SLVM Parameters contract implementation.
 *
 * Supplies the Normal User profile linked into the SLVM runtime service layer
 * alongside GarbageCollector and SecuritySupervisor: a software-capability
 * threshold of 141+ and concurrent handling of 8 sociological subjects. These
 * are runtime design parameters, not psychometric judgments.
 * ======================================================================== */
#include "Parameters.h"

void slvm_parameters_init(SLVMParameters *p) {
    if (!p) return;
    p->capability_threshold = SLVM_NORMAL_USER_CAPABILITY_THRESHOLD;
    p->subject_concurrency = SLVM_NORMAL_USER_SUBJECT_CONCURRENCY;
}

int slvm_parameters_capability_threshold(const SLVMParameters *p) {
    return p ? p->capability_threshold : SLVM_NORMAL_USER_CAPABILITY_THRESHOLD;
}

int slvm_parameters_subject_concurrency(const SLVMParameters *p) {
    return p ? p->subject_concurrency : SLVM_NORMAL_USER_SUBJECT_CONCURRENCY;
}

int slvm_parameters_meets_threshold(const SLVMParameters *p, int capability) {
    int threshold = slvm_parameters_capability_threshold(p);
    return capability >= threshold;
}

int slvm_parameters_within_concurrency(const SLVMParameters *p, int subjects) {
    int limit = slvm_parameters_subject_concurrency(p);
    return subjects >= 0 && subjects <= limit;
}
