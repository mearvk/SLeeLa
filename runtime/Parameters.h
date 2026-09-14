#ifndef SLEELA_PARAMETERS_H
#define SLEELA_PARAMETERS_H
/* ========================================================================
 * Parameters.h -- the explicit SLVM Parameters contract.
 *
 * The runtime service layer links three services (see COMPILER.md sec.5 and
 * NATIVE_API.md "Runtime Services"): GarbageCollector, SecuritySupervisor, and
 * this explicit Parameters contract. Parameters supplies the Normal User
 * profile: a software-capability threshold of 141+ and concurrent handling of
 * 8 sociological subjects. These are runtime design parameters, not
 * psychometric judgments.
 * ======================================================================== */
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Normal User profile constants (design parameters, not psychometric scores). */
#define SLVM_NORMAL_USER_CAPABILITY_THRESHOLD 141
#define SLVM_NORMAL_USER_SUBJECT_CONCURRENCY 8

typedef struct {
    int capability_threshold;   /* minimum software-capability threshold (141+). */
    int subject_concurrency;    /* concurrent sociological subjects (8).        */
} SLVMParameters;

/* Initialise a parameters record to the Normal User profile. */
void slvm_parameters_init(SLVMParameters *p);

/* Accessors for the active profile values. */
int slvm_parameters_capability_threshold(const SLVMParameters *p);
int slvm_parameters_subject_concurrency(const SLVMParameters *p);

/* Contract check: does the supplied capability meet the profile threshold? */
int slvm_parameters_meets_threshold(const SLVMParameters *p, int capability);

/* Contract check: is the requested subject count within the concurrency limit? */
int slvm_parameters_within_concurrency(const SLVMParameters *p, int subjects);

#ifdef __cplusplus
}
#endif
#endif
