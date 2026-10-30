#ifndef SLEELA_PHILOSOPHY_SPECULATION_INVITE_H
#define SLEELA_PHILOSOPHY_SPECULATION_INVITE_H
/*
 * Speculation invite (adult-psychiatry commentary on programming reach).
 *
 * This module lets a person INVITE, and a self-attested adult-psychiatry
 * professional SUBMIT, careful, attributed SPECULATIONS about what this
 * software's programming does and does not reach -- its capabilities and,
 * especially, its limits. It is a structured way to collect good scientific
 * speculation ABOUT the software, not an assessment OF any person.
 *
 * Boundaries (consistent with this folder's README):
 *   - It makes NO diagnosis of any condition and accepts none as input.
 *   - It gives NO treatment, medication, or hormone advice.
 *   - It does NOT verify credentials. A professional's role and attestation
 *     are SELF-REPORTED and labelled as such; the library cannot confirm a
 *     licence or clinical correctness.
 *   - It never presents generated text as a professional's opinion. Every
 *     speculation stores the submitter's own supplied words with attribution.
 *   - It performs no network I/O and does no background listening.
 *
 * Careful scientific output: each speculation carries a confidence level, a
 * REQUIRED falsifiability note ("how this speculation could be shown wrong"),
 * an explicit scope, and a standing disclaimer line, so the collected set
 * reads as hypotheses to be tested, not findings.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h> /* FILE */

#ifdef __cplusplus
extern "C" {
#endif

#define SL_SPEC_AUTHOR_CAP     96u
#define SL_SPEC_ROLE_CAP       96u
#define SL_SPEC_TEXT_CAP       1024u
#define SL_SPEC_FALSIFY_CAP    512u
#define SL_SPEC_SCOPE_CAP      160u
#define SL_SPEC_DEFAULT_CAPACITY 64u

typedef enum sl_spec_status {
    SL_SPEC_OK = 0,
    SL_SPEC_INVALID_ARGUMENT,
    SL_SPEC_NOT_ATTESTED,       /* submitter did not attest authorship */
    SL_SPEC_MISSING_FALSIFIABILITY, /* no "how this could be wrong" note */
    SL_SPEC_FULL,
    SL_SPEC_NOT_FOUND
} sl_spec_status;

/* Confidence expressed as a careful, bounded scientific band -- never a claim
 * of certainty. */
typedef enum sl_spec_confidence {
    SL_CONF_TENTATIVE = 0,  /* a hunch worth recording */
    SL_CONF_PLAUSIBLE,      /* some reasoning supports it */
    SL_CONF_WELL_REASONED   /* argued, but still a hypothesis */
} sl_spec_confidence;

/* Which aspect of the software's programming reach a speculation addresses. */
typedef enum sl_spec_aspect {
    SL_ASPECT_CAPABILITY = 0,   /* what the programming can reach / do */
    SL_ASPECT_LIMITATION,       /* what it cannot or should not reach */
    SL_ASPECT_SAFETY,           /* safety / harm-avoidance behavior */
    SL_ASPECT_ETHICS,           /* consent, provenance, boundaries */
    SL_ASPECT_RESEARCH_DIRECTION/* a suggested study or question */
} sl_spec_aspect;

typedef struct sl_speculation {
    uint64_t id;
    char author[SL_SPEC_AUTHOR_CAP];     /* submitter-supplied name/handle */
    char role[SL_SPEC_ROLE_CAP];         /* self-reported role, e.g. "adult psychiatrist (self-attested)" */
    char scope[SL_SPEC_SCOPE_CAP];       /* what the speculation applies to */
    char text[SL_SPEC_TEXT_CAP];         /* the speculation, submitter's own words */
    char falsifiability[SL_SPEC_FALSIFY_CAP]; /* how it could be shown wrong (required) */
    sl_spec_aspect aspect;
    sl_spec_confidence confidence;
    bool self_attested;                  /* submitter attests these are their words */
    bool about_software_not_person;      /* must be true: speculation is about the software */
} sl_speculation;

typedef struct sl_spec_registry sl_spec_registry;

/* ---- Lifecycle --------------------------------------------------------- */
sl_spec_registry *sl_spec_registry_create(size_t capacity);
void sl_spec_registry_destroy(sl_spec_registry *reg);
size_t sl_spec_registry_count(const sl_spec_registry *reg);

/* The standing invitation text shown to a professional (careful framing). */
const char *sl_spec_invitation_text(void);
/* The standing disclaimer attached to every rendered report. */
const char *sl_spec_disclaimer_text(void);

/* ---- Build and submit -------------------------------------------------- *
 * Initialize a speculation with validated, bounded fields. Requires a
 * non-empty author, text, and falsifiability note; self_attested must be true;
 * about_software_not_person is forced true. */
sl_spec_status sl_speculation_init(
    sl_speculation *out, uint64_t id, sl_spec_aspect aspect,
    sl_spec_confidence confidence, const char *author, const char *role,
    const char *scope, const char *text, const char *falsifiability,
    bool self_attested);

/* Submit a speculation into the registry. Rejects not-attested or
 * missing-falsifiability records so the collected set stays scientific. */
sl_spec_status sl_spec_registry_submit(sl_spec_registry *reg, const sl_speculation *s);

/* Access by index (0..count-1). */
const sl_speculation *sl_spec_registry_at(const sl_spec_registry *reg, size_t i);

/* ---- Careful scientific output ----------------------------------------- *
 * Render the whole registry as a plain-text report: invitation, each
 * speculation with aspect/confidence/scope/falsifiability and attribution,
 * then the standing disclaimer. Returns bytes written (>0 on success). */
int sl_spec_registry_write_report(const sl_spec_registry *reg, FILE *out);

/* ---- Stringization ----------------------------------------------------- */
const char *sl_spec_aspect_string(sl_spec_aspect);
const char *sl_spec_confidence_string(sl_spec_confidence);
const char *sl_spec_status_string(sl_spec_status);

#ifdef __cplusplus
}
#endif
#endif
