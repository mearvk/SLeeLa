#include "../include/sleela_vm_builder.hpp"
namespace sleela::vm {
bool Builder::validate() const { return sleela_vm_requirements_validate(&requirements_) == 0; }
bool Builder::featureEnabled(std::uint32_t bit) const { return sleela_vm_feature_enabled(&requirements_, bit) != 0; }
bool Builder::optionCompatible(std::uint32_t option) const { return sleela_vm_option_compatible(&requirements_, option) != 0; }
std::string Builder::target() const { return requirements_.kind == SLEELA_VM_SLJVM ? "SLJVM" : "SLVM"; }
void Builder::setFeatureMask(std::uint64_t low, std::uint64_t high) {
    requirements_.feature_mask_low = low;
    requirements_.feature_mask_high = high;
}
}
