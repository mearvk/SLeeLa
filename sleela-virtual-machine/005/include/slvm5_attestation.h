#ifndef SLEELA_SLVM5_ATTESTATION_H
#define SLEELA_SLVM5_ATTESTATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 const char *vm_identity;
 const char *artifact_hash;
 const char *manifest_hash;
 const char *build_identity;
 const char *policy_hash;
 const char *runtime_identity;
 const char *crypto_provider;
 uint64_t policy_version;
 uint64_t generated_at_ns;
} slvm5_attestation_t;
int slvm5_attestation_validate(const slvm5_attestation_t*);
#ifdef __cplusplus
}
#endif
#endif
