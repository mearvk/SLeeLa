#include "../include/sleela_compiler.h"

int sleela_compiler_plan_validate(const sleela_compiler_plan_t *p) {
    if (!p || p->profile_code == 0 || p->required_stage_count == 0) return 0;
    if (p->completed_stage_count > p->required_stage_count) return 0;
    if (p->profile_code == SLEELA_COMPILER_ADVANCED_TOTAL &&
        (!p->capability_check_present || !p->security_check_present)) return 0;
    return 1;
}

int sleela_compiler_stage_status(const sleela_compiler_plan_t *p, uint32_t s) {
    if (!sleela_compiler_plan_validate(p) || s == 0 || s > 9) return SLEELA_COMPILER_INVALID;
    uint32_t present = 0;
    switch (s) {
        case 1: present=p->source_present; break;
        case 2: present=p->frontend_present; break;
        case 3: present=p->semantics_present; break;
        case 4: present=p->ir_present; break;
        case 5: present=p->lowering_present; break;
        case 6: present=p->codegen_present; break;
        case 7: present=p->vm_target_present; break;
        case 8: present=p->capability_check_present; break;
        case 9: present=p->security_check_present; break;
    }
    return present ? SLEELA_COMPILER_FINE : SLEELA_COMPILER_MISSING;
}

int sleela_compiler_vm_ready(const sleela_compiler_plan_t *p) {
    if (!sleela_compiler_plan_validate(p)) return 0;
    for (uint32_t s=1; s<=7; ++s)
        if (sleela_compiler_stage_status(p, s) != SLEELA_COMPILER_FINE) return 0;
    if (p->profile_code == SLEELA_COMPILER_ADVANCED_TOTAL &&
        (!p->provenance_check_present || !p->certificate_check_present)) return 0;
    return 1;
}

const char *sleela_compiler_status_name(int s) {
    switch (s) {
        case SLEELA_COMPILER_FINE: return "FINE";
        case SLEELA_COMPILER_MISSING: return "MISSING";
        case SLEELA_COMPILER_INVALID: return "INVALID";
        case SLEELA_COMPILER_DEPENDENCY: return "REQUIRES";
        default: return "UNKNOWN";
    }
}
