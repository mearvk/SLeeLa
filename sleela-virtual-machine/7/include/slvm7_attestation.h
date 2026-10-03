#ifndef SLEELA_SLVM7_ATTESTATION_H
#define SLEELA_SLVM7_ATTESTATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *vm_identity,*artifact_hash,*manifest_hash,*build_identity,*policy_hash,*runtime_identity,*manager_set_hash,*lineage_hash; uint32_t required_signers,valid_signers; uint64_t policy_version,generated_at_ns; } slvm7_attestation_t;
int slvm7_attestation_validate(const slvm7_attestation_t *);
#ifdef __cplusplus
}
#endif
#endif
