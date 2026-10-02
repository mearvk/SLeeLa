#ifndef SLEELA_SLVM6_MIGRATION_H
#define SLEELA_SLVM6_MIGRATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t migration_id,checkpoint_id,source_epoch,target_epoch; const char *source_runtime,*target_runtime,*artifact_hash,*policy_hash; uint32_t checkpoint_valid,attestation_valid,compatibility_valid; } slvm6_migration_t;
int slvm6_migration_validate(const slvm6_migration_t*);
#ifdef __cplusplus
}
#endif
#endif
