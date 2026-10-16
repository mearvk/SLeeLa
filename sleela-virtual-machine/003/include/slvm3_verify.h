#ifndef SLEELA_SLVM3_VERIFY_H
#define SLEELA_SLVM3_VERIFY_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint32_t schema, vm_major; const char *artifact_id, *artifact_version, *content_hash, *manifest_hash, *signature, *abi, *capabilities; } slvm3_artifact_manifest_t;
typedef struct { int hash_valid, signature_valid, abi_valid, capability_declarations_valid, policy_valid; } slvm3_verification_result_t;
int slvm3_verify_manifest(const slvm3_artifact_manifest_t*, int, slvm3_verification_result_t*);
int slvm3_verify_result_is_valid(const slvm3_verification_result_t*);
#ifdef __cplusplus
}
#endif
#endif
