#ifndef SLEELA_SLVM6_ATTESTATION_H
#define SLEELA_SLVM6_ATTESTATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *vm_identity,*artifact_hash,*manifest_hash,*build_identity,*policy_hash,*runtime_identity,*crypto_provider,*lineage_hash; uint32_t required_signers,valid_signers; uint64_t policy_version,generated_at_ns; } slvm6_attestation_t;
int slvm6_attestation_validate(const slvm6_attestation_t*);
#ifdef __cplusplus
}
#endif
#endif
