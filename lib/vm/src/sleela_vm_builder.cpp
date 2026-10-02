#include "../include/sleela_vm_builder.hpp"
namespace sleela::vm { bool Builder::validate() const { return sleela_vm_requirements_validate(&requirements_)==0; } std::string Builder::target() const { return requirements_.kind==SLEELA_VM_SLJVM ? "SLJVM" : "SLVM"; } }
