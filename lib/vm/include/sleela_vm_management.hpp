#ifndef SLEELA_VM_MANAGEMENT_HPP
#define SLEELA_VM_MANAGEMENT_HPP
#include "sleela_vm_management.h"
namespace sleela::vm { class MemoryManager{public:explicit MemoryManager(const sleela_vm_memory_plan_t&p):plan_(p){}bool validate()const;bool featureEnabled(unsigned)const;bool secureCheckpointReady()const;bool secureMigrationReady()const;private:sleela_vm_memory_plan_t plan_{};}; class SecurityManager{public:explicit SecurityManager(const sleela_vm_security_plan_t&p):plan_(p){}bool validate()const;bool featureEnabled(unsigned)const;bool authenticatedChannelReady()const;bool attestationReady()const;private:sleela_vm_security_plan_t plan_{};};}
#endif