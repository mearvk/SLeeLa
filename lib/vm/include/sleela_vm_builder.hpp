#ifndef SLEELA_VM_BUILDER_HPP
#define SLEELA_VM_BUILDER_HPP
#include "sleela_vm_source.h"
#include <string>
namespace sleela::vm {
class Builder { public: explicit Builder(const sleela_vm_requirements_t& r): requirements_(r) {} bool validate() const; std::string target() const; private: sleela_vm_requirements_t requirements_{}; };
}
#endif
