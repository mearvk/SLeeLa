#ifndef SLEELA_SLVM6_LINEAGE_H
#define SLEELA_SLVM6_LINEAGE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t epoch,parent_epoch; const char *artifact_hash,*manifest_hash,*policy_hash,*capability_hash,*runtime_identity,*resolver_evidence_hash,*certificate_evidence_hash; } slvm6_lineage_t;
int slvm6_lineage_validate(const slvm6_lineage_t*);
#ifdef __cplusplus
}
#endif
#endif
