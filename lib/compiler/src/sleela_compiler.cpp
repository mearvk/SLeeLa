#include "../include/sleela_compiler.hpp"
bool SleelaCompilerPlan::validate() const { return sleela_compiler_plan_validate(&value) != 0; }
int SleelaCompilerPlan::stageStatus(uint32_t s) const { return sleela_compiler_stage_status(&value, s); }
bool SleelaCompilerPlan::vmReady() const { return sleela_compiler_vm_ready(&value) != 0; }
