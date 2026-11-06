#ifndef SLEELA_AI_LLM_SLEELA_VERIFIER_H
#define SLEELA_AI_LLM_SLEELA_VERIFIER_H
/*
 * SLeeLa code verifier — the symbolic (soundness) half of a SLeeLa-specialized
 * Code LLM.
 *
 * A Code LLM for the SLeeLa language proposes SLeeLa source (summaries, dry-run
 * state, refactorings, completions). This verifier judges a proposed snippet
 * against SLeeLa's REAL rules, so the reasoning layer can refute bad proposals
 * and backtrack. The rules encoded here are taken from the normative spec
 * (SLEELA.syntax), the lexer keyword set, and the project's confirmed gotchas:
 *
 *   - reserved words used as identifiers (record, native, enum, class, ... ),
 *   - a no-arg close() method (RUNTIME-reserved Munction builtin collision),
 *   - comment-form errors: C-style block comments (unsupported in SLeeLa), a
 *     multi-line single '<' comment, an inline trailing '<' comment, and an
 *     apostrophe inside an angle comment,
 *   - a class/struct with more than 64 named fields (SL_MAX_STRUCT_FIELDS),
 *   - (advisory) a new class missing the Metadata Class metapattern header.
 *
 * This is a fast STATIC checker. For ground truth, a host may additionally run
 * the real compiler (impl/build/sleela check <file>); this header stays
 * dependency-free and does not shell out itself.
 *
 * The verifier plugs into the reasoning layer: sllmsl_as_reasoning_verifier()
 * yields an sllmr_verifier whose check() returns SUPPORTED when no rule is
 * violated, REFUTED (with a note) when one is, and UNKNOWN when the snippet is
 * not SLeeLa code it can judge.
 */
#include "../large_language_models_reasoning.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMSL_API __declspec(dllexport)
# else
#  define SLLMSL_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMSL_API __attribute__((visibility("default")))
#else
# define SLLMSL_API
#endif

#define SLLMSL_ABI_VERSION 1u

/* Which rule a snippet violated. SLLMSL_OK means no violation found. */
typedef enum sllmsl_rule {
 SLLMSL_OK=0,
 SLLMSL_RESERVED_IDENTIFIER=1, /* reserved word used as a method/var/param name */
 SLLMSL_CLOSE_COLLISION=2,     /* no-arg close() collides with Munction builtin */
 SLLMSL_CSTYLE_COMMENT=3,      /* C-style block comment is not valid SLeeLa */
 SLLMSL_MULTILINE_ANGLE=4,     /* multi-line single '<' comment (needs << >>) */
 SLLMSL_INLINE_ANGLE=5,        /* inline trailing '<' comment after code */
 SLLMSL_APOSTROPHE_COMMENT=6,  /* apostrophe inside an angle comment */
 SLLMSL_FIELD_CAP=7,           /* > 64 named fields in a class/struct */
 SLLMSL_MISSING_METADATA=8     /* advisory: new class without Metadata metapattern */
} sllmsl_rule;

/* The maximum named fields a class/struct may declare (SL_MAX_STRUCT_FIELDS). */
#define SLLMSL_MAX_FIELDS 64

/* Options controlling checker strictness. */
typedef struct sllmsl_options {
 uint32_t abi_version;
 int treat_missing_metadata_as_error; /* if 0, missing metadata is advisory only */
} sllmsl_options;

/* Result of a static check. 'rule' is the first violation found (SLLMSL_OK if
   none). 'line' is 1-based within the snippet (0 if not line-specific).
   'message' is a short static/formatted explanation (never NULL). */
typedef struct sllmsl_finding {
 sllmsl_rule rule;
 uint32_t line;
 char message[256];
} sllmsl_finding;

SLLMSL_API const char *sllmsl_rule_name(sllmsl_rule rule);
SLLMSL_API int sllmsl_is_reserved_word(const char *word);     /* 1 if reserved */

SLLMSL_API void sllmsl_options_init(sllmsl_options *options);

/* Statically check a SLeeLa source snippet. Returns 1 if a violation was found
   (and fills *out), 0 if clean. A NULL/empty snippet is treated as clean. */
SLLMSL_API int sllmsl_check_snippet(const char *sleela_source,
                                    const sllmsl_options *options,
                                    sllmsl_finding *out);

/* Adapt this verifier into an sllmr_verifier for the reasoning engine. The
   returned struct borrows nothing persistent; copy it or keep it in scope. The
   options pointer (may be NULL for defaults) must outlive the verifier. */
SLLMSL_API sllmr_verifier sllmsl_as_reasoning_verifier(const sllmsl_options *options);

#ifdef __cplusplus
}
#endif
#endif
