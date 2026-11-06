#include "grt_claim.h"

#include <ctype.h>

static int bounded_string(const char *value, size_t capacity, int required) {
    size_t i;
    if (!value || capacity == 0) return 0;
    for (i = 0; i < capacity; ++i) {
        if (value[i] == '\0') return !required || i != 0;
    }
    return 0;
}

static int valid_digest(const char *digest, size_t capacity) {
    size_t i, n = 0;
    if (!bounded_string(digest, capacity, 1)) return 0;
    while (n < capacity && digest[n]) ++n;
    /* Require SHA-256-sized-or-longer hexadecimal syntax. This checks format,
     * not digest correctness or the declared algorithm. */
    if (n < 64 || (n & 1u)) return 0;
    for (i = 0; i < n; ++i) {
        if (!isxdigit((unsigned char)digest[i])) return 0;
    }
    return 1;
}

grt_claim_status grt_claim_validate(const grt_claim *claim, int64_t now) {
    if (!claim || claim->version != 1 ||
        claim->kind < GRT_KIND_PROJECT_IDENTITY || claim->kind > GRT_KIND_INDEPENDENT_ASSESSMENT ||
        !bounded_string(claim->subject_id, sizeof(claim->subject_id), 1) ||
        !bounded_string(claim->issuer_id, sizeof(claim->issuer_id), 1) ||
        !bounded_string(claim->component, sizeof(claim->component), 1) ||
        !bounded_string(claim->platform_scope, sizeof(claim->platform_scope), 1) ||
        !bounded_string(claim->policy_version, sizeof(claim->policy_version), 1) ||
        !bounded_string(claim->limitations, sizeof(claim->limitations), 1) ||
        claim->issued_at < 0 || claim->expires_at <= claim->issued_at) {
        return GRT_CLAIM_INVALID;
    }

    if (claim->kind == GRT_KIND_ARTIFACT_PROVENANCE &&
        (!bounded_string(claim->source_revision, sizeof(claim->source_revision), 1) ||
         !bounded_string(claim->artifact_digest_algorithm, sizeof(claim->artifact_digest_algorithm), 1) ||
         !valid_digest(claim->artifact_digest, sizeof(claim->artifact_digest)))) {
        return GRT_CLAIM_INVALID;
    }
    if (claim->kind == GRT_KIND_INDEPENDENT_ASSESSMENT &&
        !bounded_string(claim->evidence_reference, sizeof(claim->evidence_reference), 1)) {
        return GRT_CLAIM_INVALID;
    }
    if (claim->expires_at <= now) return GRT_CLAIM_EXPIRED;
    return GRT_CLAIM_WELL_FORMED;
}

const char *grt_claim_status_name(grt_claim_status status) {
    switch (status) {
        case GRT_CLAIM_WELL_FORMED: return "well-formed-unverified";
        case GRT_CLAIM_INVALID: return "invalid";
        case GRT_CLAIM_EXPIRED: return "expired-unverified";
        case GRT_CLAIM_UNSUPPORTED: return "unsupported";
        default: return "unknown";
    }
}
