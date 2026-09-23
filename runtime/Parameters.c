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
    p->memory_manager_grade = SLVM_MEMORY_MANAGER_DEFAULT_GRADE;
    p->system_monitor_enabled = SLVM_SYSTEM_MONITOR_DEFAULT_ENABLED;
    p->system_monitor_strict = SLVM_SYSTEM_MONITOR_DEFAULT_STRICT;
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

int slvm_parameters_memory_manager_grade(const SLVMParameters *p) {
    return p ? p->memory_manager_grade : SLVM_MEMORY_MANAGER_DEFAULT_GRADE;
}

int slvm_parameters_set_memory_manager_grade(SLVMParameters *p, int grade) {
    if (!p || grade < SLVM_MEMORY_MANAGER_GRADE_I || grade > SLVM_MEMORY_MANAGER_GRADE_III) return 0;
    p->memory_manager_grade = grade;
    return 1;
}

int slvm_parameters_system_monitor_enabled(const SLVMParameters *p) {
    return p ? p->system_monitor_enabled != 0 : SLVM_SYSTEM_MONITOR_DEFAULT_ENABLED;
}
int slvm_parameters_set_system_monitor_enabled(SLVMParameters *p, int enabled) {
    if (!p) return 0;
    p->system_monitor_enabled = enabled != 0;
    return 1;
}
int slvm_parameters_system_monitor_strict(const SLVMParameters *p) {
    return p ? p->system_monitor_strict != 0 : SLVM_SYSTEM_MONITOR_DEFAULT_STRICT;
}
int slvm_parameters_set_system_monitor_strict(SLVMParameters *p, int strict) {
    if (!p) return 0;
    p->system_monitor_strict = strict != 0;
    return 1;
}
