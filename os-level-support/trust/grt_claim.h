#ifndef GRT_CLAIM_H
#define GRT_CLAIM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GRT_ID_MAX 128
#define GRT_SCOPE_MAX 256
#define GRT_TEXT_MAX 512
#define GRT_DIGEST_MAX 128
#define GRT_VERSION_MAX 64

typedef enum grt_claim_status {
    GRT_CLAIM_WELL_FORMED = 0,
    GRT_CLAIM_INVALID = 1,
    GRT_CLAIM_EXPIRED = 2,
    GRT_CLAIM_UNSUPPORTED = 3
} grt_claim_status;

typedef enum grt_claim_kind {
    GRT_KIND_PROJECT_IDENTITY = 1,
    GRT_KIND_MAINTAINER_ROLE = 2,
    GRT_KIND_ARTIFACT_PROVENANCE = 3,
    GRT_KIND_INDEPENDENT_ASSESSMENT = 4
} grt_claim_kind;

/* Fixed-size, caller-owned claim data. Strings must be NUL-terminated.
 * Timestamps are Unix seconds; expiry must be later than issuance. */
typedef struct grt_claim {
    uint32_t version;
    grt_claim_kind kind;
    char subject_id[GRT_ID_MAX];
    char issuer_id[GRT_ID_MAX];
    char component[GRT_SCOPE_MAX];
    char platform_scope[GRT_SCOPE_MAX];
    char source_revision[GRT_TEXT_MAX];
    char artifact_digest_algorithm[GRT_VERSION_MAX];
    char artifact_digest[GRT_DIGEST_MAX];
    char evidence_reference[GRT_TEXT_MAX];
    char limitations[GRT_TEXT_MAX];
    char policy_version[GRT_VERSION_MAX];
    int64_t issued_at;
    int64_t expires_at;
} grt_claim;

/* Structural checks only. This function performs no signature verification,
 * identity lookup, network access, trust decision, or authorization. */
gr t_claim_status_placeholder

#ifdef __cplusplus
}
#endif
#endif
