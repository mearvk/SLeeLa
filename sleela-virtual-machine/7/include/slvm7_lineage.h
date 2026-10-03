#ifndef SLEELA_SLVM7_LINEAGE_H
#define SLEELA_SLVM7_LINEAGE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t epoch,parent_epoch; const char *artifact_hash,*manifest_hash,*policy_hash,*capability_hash,*runtime_identity,*resolver_evidence_hash,*certificate_evidence_hash,*manager_state_hash,*recovery_state_hash; } slvm7_lineage_t;
int slvm7_lineage_validate(const slvm7_lineage_t *);
#ifdef __cplusplus
}
#endif
#endif
