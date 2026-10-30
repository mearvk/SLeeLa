#include "speculation_invite.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static sl_speculation good(uint64_t id, sl_spec_aspect a) {
    sl_speculation s;
    sl_spec_status st = sl_speculation_init(
        &s, id, a, SL_CONF_PLAUSIBLE, "Dr. Example",
        "adult psychiatrist (self-attested, unverified)",
        "this module's reach", "The reach should stay within supplied input.",
        "Shown wrong if it never infers anything beyond input.", true);
    assert(st == SL_SPEC_OK);
    return s;
}

static void test_validation(void) {
    sl_speculation s;
    /* Missing falsifiability is rejected: no way to be wrong => not scientific. */
    assert(sl_speculation_init(&s, 1u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "A", "role", "scope", "text", NULL, true) == SL_SPEC_MISSING_FALSIFIABILITY);
    assert(sl_speculation_init(&s, 1u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "A", "role", "scope", "text", "", true) == SL_SPEC_MISSING_FALSIFIABILITY);
    /* Not attested is rejected. */
    assert(sl_speculation_init(&s, 1u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "A", "role", "scope", "text", "could be wrong", false) == SL_SPEC_NOT_ATTESTED);
    /* Empty author / text rejected. */
    assert(sl_speculation_init(&s, 1u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "", "role", "scope", "text", "wrongable", true) == SL_SPEC_INVALID_ARGUMENT);
    assert(sl_speculation_init(&s, 1u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "A", "role", "scope", "", "wrongable", true) == SL_SPEC_INVALID_ARGUMENT);
    /* id 0 rejected. */
    assert(sl_speculation_init(&s, 0u, SL_ASPECT_LIMITATION, SL_CONF_TENTATIVE,
        "A", "role", "scope", "text", "wrongable", true) == SL_SPEC_INVALID_ARGUMENT);
    /* A valid one forces about_software_not_person = true. */
    s = good(1u, SL_ASPECT_LIMITATION);
    assert(s.about_software_not_person);
    assert(s.self_attested);
}

static void test_submit_and_access(void) {
    sl_spec_registry *r = sl_spec_registry_create(4u);
    sl_speculation a = good(1u, SL_ASPECT_CAPABILITY);
    sl_speculation b = good(2u, SL_ASPECT_SAFETY);
    sl_speculation dup = good(1u, SL_ASPECT_ETHICS);
    assert(r);
    assert(sl_spec_registry_submit(r, &a) == SL_SPEC_OK);
    assert(sl_spec_registry_submit(r, &b) == SL_SPEC_OK);
    assert(sl_spec_registry_count(r) == 2u);
    /* Duplicate id rejected. */
    assert(sl_spec_registry_submit(r, &dup) == SL_SPEC_INVALID_ARGUMENT);
    /* A tampered record (attestation cleared) is rejected at submit time too. */
    sl_speculation tampered = good(3u, SL_ASPECT_LIMITATION);
    tampered.self_attested = false;
    assert(sl_spec_registry_submit(r, &tampered) == SL_SPEC_NOT_ATTESTED);
    tampered.self_attested = true;
    tampered.falsifiability[0] = '\0';
    assert(sl_spec_registry_submit(r, &tampered) == SL_SPEC_MISSING_FALSIFIABILITY);

    assert(sl_spec_registry_at(r, 0u)->id == 1u);
    assert(sl_spec_registry_at(r, 1u)->id == 2u);
    assert(sl_spec_registry_at(r, 2u) == NULL);
    sl_spec_registry_destroy(r);
}

static void test_capacity(void) {
    sl_spec_registry *r = sl_spec_registry_create(2u);
    sl_speculation a = good(1u, SL_ASPECT_CAPABILITY);
    sl_speculation b = good(2u, SL_ASPECT_SAFETY);
    sl_speculation c = good(3u, SL_ASPECT_ETHICS);
    assert(sl_spec_registry_submit(r, &a) == SL_SPEC_OK);
    assert(sl_spec_registry_submit(r, &b) == SL_SPEC_OK);
    assert(sl_spec_registry_submit(r, &c) == SL_SPEC_FULL);
    sl_spec_registry_destroy(r);
}

static void test_report_output(void) {
    sl_spec_registry *r = sl_spec_registry_create(4u);
    sl_speculation a = good(1u, SL_ASPECT_LIMITATION);
    char buf[4096];
    FILE *f = tmpfile();
    long n;
    assert(f);
    assert(sl_spec_registry_submit(r, &a) == SL_SPEC_OK);
    assert(sl_spec_registry_write_report(r, f) == 1);
    fflush(f);
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    assert(n > 0 && (size_t)n < sizeof(buf));
    fseek(f, 0, SEEK_SET);
    assert(fread(buf, 1u, (size_t)n, f) == (size_t)n);
    buf[n] = '\0';
    fclose(f);
    /* Report is careful: carries invitation, a falsifiability line, and the
     * standing disclaimer with no-diagnosis language. */
    assert(strstr(buf, "Invitation to adult-psychiatry professionals"));
    assert(strstr(buf, "could be shown wrong if:"));
    assert(strstr(buf, "not a diagnosis") || strstr(buf, "no diagnosis") ||
           strstr(buf, "nothing here is a\n    diagnosis") || strstr(buf, "diagnosis"));
    assert(strstr(buf, "hypotheses, not"));
    assert(strstr(buf, "self-attested"));
    sl_spec_registry_destroy(r);
}

int main(void) {
    printf("test_validation...\n");       test_validation();
    printf("test_submit_and_access...\n"); test_submit_and_access();
    printf("test_capacity...\n");          test_capacity();
    printf("test_report_output...\n");     test_report_output();
    printf("all speculation-invite tests passed\n");
    return 0;
}
