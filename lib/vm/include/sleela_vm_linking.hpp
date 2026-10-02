#ifndef SLEELA_VM_LINKING_HPP
#define SLEELA_VM_LINKING_HPP
#include "sleela_vm_linking.h"
namespace sleela { namespace vm {
class LinkingManager {
public:
 explicit LinkingManager(const sleela_vm_linking_plan_t& plan) : plan_(plan) {}
 bool validate() const;
 bool versionMatches(unsigned major, unsigned minor) const;
 bool featureEnabled(unsigned feature) const;
 bool observationAllowed(unsigned observation) const;
private:
 sleela_vm_linking_plan_t plan_;
};
}}
#endif
