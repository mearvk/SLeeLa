#include "grt_claim.h"
#include <assert.h>
#include <string.h>

int main(void) {
    grt_claim claim;
    memset(&claim, 0, sizeof(claim));
    claim.version = 1;
    claim.kind = GRT_KIND_PROJECT_IDENTITY;
    strcpy(claim.subject_id, "project:example");
    strcpy(claim.issuer_id, "issuer:example");
    strcpy(claim.component, "os-level-support");
    strcpy(claim.platform_scope, "linux,macos,windows");
    strcpy(claim.limitations, "identity and provenance not cryptographically verified");
    strcpy(claim.policy_version, "concept-1");
    claim.issued_at = 100;
    claim.expires_at = 200;

    assert(grt_claim_validate(&claim, 150) == GRT_CLAIM_WELL_FORMED);
    assert(strcmp(grt_claim_status_name(GRT_CLAIM_WELL_FORMED), "well-formed-unverified") == 0);
    assert(grt_claim_validate(&claim, 200) == GRT_CLAIM_EXPIRED);
    claim.expires_at = 99;
    assert(grt_claim_validate(&claim, 50) == GRT_CLAIM_INVALID);
    return 0;
}
