#ifndef SLEELA_COMPILER_H
#define SLEELA_COMPILER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    SLEELA_COMPILER_BASIC_COMPLETE = 1,
    SLEELA_COMPILER_ADVANCED_TOTAL = 2
};

enum {
    SLEELA_COMPILER_FINE = 1,
    SLEELA_COMPILER_MISSING = 2,
    SLEELA_COMPILER_INVALID = 3,
    SLEELA_COMPILER_DEPENDENCY = 4
};

typedef struct sleela_compiler_plan {
    uint32_t profile_code;
    uint32_t required_stage_count;
    uint32_t completed_stage_count;
    uint32_t source_present;
    uint32_t frontend_present;
    uint32_t semantics_present;
    uint32_t ir_present;
    uint32_t lowering_present;
    uint32_t codegen_present;
    uint32_t vm_target_present;
    uint32_t capability_check_present;
    uint32_t security_check_present;
    uint32_t provenance_check_present;
    uint32_t certificate_check_present;
} sleela_compiler_plan_t;

int sleela_compiler_plan_validate(const sleela_compiler_plan_t *plan);
int sleela_compiler_stage_status(const sleela_compiler_plan_t *plan, uint32_t stage_code);
int sleela_compiler_vm_ready(const sleela_compiler_plan_t *plan);
const char *sleela_compiler_status_name(int status_code);

#ifdef __cplusplus
}
#endif
#endif
