#include "../include/sleela_vm_management.hpp"
namespace sleela::vm {
bool MemoryManager::validate() const { return sleela_vm_memory_plan_validate(&plan_) == 0; }
bool MemoryManager::featureEnabled(unsigned f) const { return sleela_vm_memory_feature_enabled(&plan_, f) != 0; }
bool MemoryManager::secureCheckpointReady() const { return validate() && plan_.checkpoint_code && plan_.integrity_code; }
bool MemoryManager::secureMigrationReady() const { return secureCheckpointReady() && plan_.migration_code && plan_.encryption_code; }
bool SecurityManager::validate() const { return sleela_vm_security_plan_validate(&plan_) == 0; }
bool SecurityManager::featureEnabled(unsigned f) const { return sleela_vm_security_feature_enabled(&plan_, f) != 0; }
bool SecurityManager::authenticatedChannelReady() const { return validate() && plan_.crypto_code && plan_.certificate_code && plan_.replay_code; }
bool SecurityManager::attestationReady() const { return authenticatedChannelReady() && plan_.attestation_code && plan_.audit_code; }
}
