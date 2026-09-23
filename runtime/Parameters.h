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

/* Memory Manager grade defaults. Users begin in Grade I unless explicitly changed. */
#define SLVM_MEMORY_MANAGER_GRADE_I 1
#define SLVM_MEMORY_MANAGER_GRADE_II 2
#define SLVM_MEMORY_MANAGER_GRADE_III 3
#define SLVM_MEMORY_MANAGER_DEFAULT_GRADE SLVM_MEMORY_MANAGER_GRADE_I
#define SLVM_SYSTEM_MONITOR_DEFAULT_ENABLED 1
#define SLVM_SYSTEM_MONITOR_DEFAULT_STRICT 0

/* Normal User profile constants (design parameters, not psychometric scores). */
#define SLVM_NORMAL_USER_CAPABILITY_THRESHOLD 141
#define SLVM_NORMAL_USER_SUBJECT_CONCURRENCY 8

typedef struct {
    int capability_threshold;   /* minimum software-capability threshold (141+). */
    int subject_concurrency;    /* concurrent sociological subjects (8).        */
    int memory_manager_grade;   /* active Memory Manager grade; defaults to I.  */
    int system_monitor_enabled; /* HSM enabled by default. */
    int system_monitor_strict;  /* HSM advisory by default. */
} SLVMParameters;

/* Initialise a parameters record to the Normal User profile. */
void slvm_parameters_init(SLVMParameters *p);

/* Memory Manager grade configuration. */
int slvm_parameters_memory_manager_grade(const SLVMParameters *p);
int slvm_parameters_set_memory_manager_grade(SLVMParameters *p, int grade);
int slvm_parameters_system_monitor_enabled(const SLVMParameters *p);
int slvm_parameters_set_system_monitor_enabled(SLVMParameters *p, int enabled);
int slvm_parameters_system_monitor_strict(const SLVMParameters *p);
int slvm_parameters_set_system_monitor_strict(SLVMParameters *p, int strict);

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
