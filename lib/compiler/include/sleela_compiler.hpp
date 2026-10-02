#ifndef SLEELA_COMPILER_HPP
#define SLEELA_COMPILER_HPP
#include "sleela_compiler.h"
class SleelaCompilerPlan {
public:
    sleela_compiler_plan_t value{};
    bool validate() const;
    int stageStatus(uint32_t stage) const;
    bool vmReady() const;
};
#endif
