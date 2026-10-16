#include "../include/sleela_vm_linking.hpp"
namespace sleela { namespace vm {
bool LinkingManager::validate() const { return sleela_vm_linking_plan_validate(&plan_) != 0; }
bool LinkingManager::versionMatches(unsigned major, unsigned minor) const { return sleela_vm_linking_version_matches(&plan_, major, minor) != 0; }
bool LinkingManager::featureEnabled(unsigned feature) const { return sleela_vm_linking_feature_enabled(&plan_, feature) != 0; }
bool LinkingManager::observationAllowed(unsigned observation) const { return sleela_vm_linking_observation_allowed(&plan_, observation) != 0; }
}}
