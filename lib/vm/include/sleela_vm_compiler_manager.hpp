#ifndef SLEELA_VM_COMPILER_MANAGER_HPP
#define SLEELA_VM_COMPILER_MANAGER_HPP
#include "sleela_vm_compiler_manager.h"
#include <string>
namespace sleela { namespace vm { class CompilerManager { public: explicit CompilerManager(const sleela_vm_compile_manifest_t& m):manifest_(m){} bool validate()const; int objectCountStatus()const; int categoryStatus(unsigned,unsigned)const; std::string statusName(unsigned)const; private: sleela_vm_compile_manifest_t manifest_; }; }}
#endif
