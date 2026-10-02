#ifndef SLEELA_VM_BUILDER_HPP
#define SLEELA_VM_BUILDER_HPP
#include "sleela_vm_source.h"
#include <cstdint>
#include <string>

namespace sleela::vm {
class Builder {
public:
    explicit Builder(const sleela_vm_requirements_t& r): requirements_(r) {}
    bool validate() const;
    bool featureEnabled(std::uint32_t bit) const;
    bool optionCompatible(std::uint32_t option) const;
    std::string target() const;
    void setFeatureMask(std::uint64_t low, std::uint64_t high);
private:
    sleela_vm_requirements_t requirements_{};
};
}
#endif
