#include "http4_resume.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int token_ok(const uint8_t *t, size_t n, const http4_resume_state_t *s,
                    uint64_t now, void *u) {
    (void)s; (void)now; (void)u;
    return n == 3 && memcmp(t, "tok", 3) == 0;
}
static int chunk_ok(uint64_t off, const uint8_t *d, size_t n,
                    const uint8_t *dg, size_t dn, void *u) {
    (void)off; (void)u;
    return n == 4 && memcmp(d, "data", 4) == 0 &&
           dn == 3 && memcmp(dg, "sum", 3) == 0;
}
static int final_ok(const http4_resume_state_t *s, const uint8_t *d,
                    size_t n, void *u) {
    (void)s; (void)u;
    return n == 5 && memcmp(d, "final", 5) == 0;
}
int main(void) {
    http4_resume_state_t s;
    memset(&s, 0, sizeof(s));
    strcpy(s.transfer_id, "transfer-1");
    strcpy(s.principal_id, "principal-A");
    strcpy(s.file_version, "version-2");
    s.total_size = 4; s.expires_at_unix = 2000;
    s.digest_size = 32; s.active = 1;
    const uint8_t token[] = "tok", chunk_digest[] = "sum", final_digest[] = "final";
    assert(http4_resume_authorize(&s, "principal-A", "version-2", 4, 0,
        token, 3, 1000, token_ok, NULL) == 0);
    assert(http4_resume_authorize(&s, "principal-B", "version-2", 4, 0,
        token, 3, 1000, token_ok, NULL) != 0);
    assert(http4_resume_authorize(&s, "principal-A", "version-2", 4, 0,
        token, 3, 2001, token_ok, NULL) != 0);
    assert(http4_resume_accept_chunk(&s, 1, (const uint8_t *)"data", 4,
        chunk_digest, 3, chunk_ok, NULL) != 0);
    assert(http4_resume_accept_chunk(&s, 0, (const uint8_t *)"data", 4,
        chunk_digest, 3, chunk_ok, NULL) == 0);
    assert(http4_resume_commit(&s, final_digest, 5, final_ok, NULL) == 0);
    assert(s.committed && !s.active);
    assert(http4_resume_commit(&s, final_digest, 5, final_ok, NULL) != 0);
    puts("SLeeLa HTTP 4.0 resume-policy self-test: PASS");
    return 0;
}
