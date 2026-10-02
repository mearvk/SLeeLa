#ifndef SLEELA_SLVM3_VERIFY_H
#define SLEELA_SLVM3_VERIFY_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint32_t schema;
 uint32_t vm_major;
 const char *artifact_id;
 const char *artifact_version;
 const char *content_hash;
 const char *manifest_hash;
 const char *signature;
 const char *abi;
 const char *capabilities;
} slvm3_artifact_manifest_t;

typedef struct {
 int hash_valid;
 int signature_valid;
 int abi_valid;
 int capability_declarations_valid;
 int policy_valid;
} slvm3_verification_result_t;

int slvm3_verify_manifest(const slvm3_artifact_manifest_t*, int require_signature,
                          slvm3_verification_result_t*);
int slvm3_verify_result_is_valid(const slvm3_verification_result_t*);
#ifdef __cplusplus
}
#endif
#endif
