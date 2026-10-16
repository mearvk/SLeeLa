#ifndef SLEELA_SLVM7_MIGRATION_H
#define SLEELA_SLVM7_MIGRATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t migration_id,checkpoint_id,source_epoch,target_epoch; const char *source_runtime,*target_runtime,*artifact_hash,*policy_hash,*manager_set_hash; uint32_t checkpoint_valid,attestation_valid,compatibility_valid,post_transfer_valid; } slvm7_migration_t;
int slvm7_migration_validate(const slvm7_migration_t *);
#ifdef __cplusplus
}
#endif
#endif
