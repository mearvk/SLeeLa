#ifndef SLEELA_VM_MANAGEMENT_HPP
#define SLEELA_VM_MANAGEMENT_HPP
#include "sleela_vm_management.h"
namespace sleela::vm {
class MemoryManager {
public:
    explicit MemoryManager(const sleela_vm_memory_plan_t& plan): plan_(plan) {}
    bool validate() const;
    bool featureEnabled(unsigned feature) const;
    bool secureCheckpointReady() const;
    bool secureMigrationReady() const;
private:
    sleela_vm_memory_plan_t plan_{};
};
class SecurityManager {
public:
    explicit SecurityManager(const sleela_vm_security_plan_t& plan): plan_(plan) {}
    bool validate() const;
    bool featureEnabled(unsigned feature) const;
    bool authenticatedChannelReady() const;
    bool attestationReady() const;
private:
    sleela_vm_security_plan_t plan_{};
};
}
#endif
