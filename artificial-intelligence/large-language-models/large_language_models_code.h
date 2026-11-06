#ifndef SLEELA_AI_LLM_CODE_H
#define SLEELA_AI_LLM_CODE_H
/*
 * SLeeLa Code LLM profile and verifier contract.
 *
 * A "Code LLM" (a.k.a. coding assistant / program-aided language model) is a
 * language model specialized for source code: because code is structured plain
 * text, the model handles programming syntax much like natural language, and is
 * effective at tracking the sequential logic, state changes, loops, and blocks
 * of PROCEDURAL languages (C, Fortran, Pascal, procedural Python).
 *
 * This is the SLeeLa project's supported "code-language-model" type (it matches
 * the architecture-catalog entry of that id). As with every other model here,
 * NO weights and NO model are bundled: this declares a USER-OWNED code model and
 * the contract by which its step proposals are verified. The neural half (the
 * Code LLM) proposes summaries, dry-run state, and refactorings; the symbolic
 * half (a compiler / test runner / tracer, supplied by the caller) decides
 * soundness. It composes with the capacity profile (2..16 GB) and the reasoning
 * layer's SLLMR_SUBJECT_CODE.
 */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMCX_API __declspec(dllexport)
# else
#  define SLLMCX_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMCX_API __attribute__((visibility("default")))
#else
# define SLLMCX_API
#endif

#define SLLMCX_ABI_VERSION 1u

/* Procedural languages this profile is tuned for. OTHER covers any language the
   caller supplies by name; the list names the common procedural ones. */
typedef enum sllmcx_language {
 SLLMCX_LANG_C=0, SLLMCX_LANG_FORTRAN=1, SLLMCX_LANG_PASCAL=2,
 SLLMCX_LANG_PYTHON=3, SLLMCX_LANG_SHELL=4, SLLMCX_LANG_SLEELA=5,
 SLLMCX_LANG_OTHER=6, SLLMCX_LANG_COUNT=7
} sllmcx_language;

/* Code-reasoning tasks (bit flags), matching the established Code LLM uses. */
typedef enum sllmcx_task {
 SLLMCX_TASK_SUMMARIZE  = 1u<<0,  /* explain code in plain language, line by line */
 SLLMCX_TASK_DRY_RUN    = 1u<<1,  /* mentally execute; predict variable/state changes */
 SLLMCX_TASK_REFACTOR   = 1u<<2,  /* restructure into clean, reusable procedures */
 SLLMCX_TASK_TRANSLATE  = 1u<<3,  /* port to another language/paradigm, behavior-preserving */
 SLLMCX_TASK_DEBUG      = 1u<<4,  /* locate and explain a fault */
 SLLMCX_TASK_COMPLETE   = 1u<<5   /* complete/continue code */
} sllmcx_task;

typedef enum sllmcx_status {
 SLLMCX_OK=0, SLLMCX_INVALID_ARGUMENT=1, SLLMCX_UNSUPPORTED=2
} sllmcx_status;

/* How a code step was (or could be) checked. A Code LLM's claim about behavior
   is only as sound as the check behind it. */
typedef enum sllmcx_check_mode {
 SLLMCX_CHECK_NONE=0,     /* no checker available -> step stays UNKNOWN */
 SLLMCX_CHECK_COMPILE=1,  /* syntactic/type check via a compiler */
 SLLMCX_CHECK_TEST=2,     /* run against tests / expected output */
 SLLMCX_CHECK_TRACE=3     /* execute and compare the traced state (dry-run truth) */
} sllmcx_check_mode;

/* A user-owned Code LLM profile. Strings are borrowed (may be NULL). */
typedef struct sllmcx_profile {
 uint32_t abi_version;
 const char *model_id;        /* the caller's code model, e.g. "user/code-llm-7b" */
 const char *base_family;     /* informational, e.g. "code-llama", "deepseek-coder" */
 sllmcx_language primary_language;
 const char *other_language;  /* used when primary_language==OTHER (name, borrowed) */
 uint32_t tasks;              /* OR of sllmcx_task flags the caller enables */
 sllmcx_check_mode check_mode;/* strongest verification the caller can provide */
 int   fill_in_middle;        /* nonzero if the model supports FIM infilling */
} sllmcx_profile;

SLLMCX_API const char *sllmcx_status_string(sllmcx_status status);
SLLMCX_API const char *sllmcx_language_name(sllmcx_language language);
SLLMCX_API const char *sllmcx_task_name(sllmcx_task task);
SLLMCX_API const char *sllmcx_check_mode_name(sllmcx_check_mode mode);

/* Defaults: C, summarize+dry-run+refactor+translate, trace checking, FIM off. */
SLLMCX_API sllmcx_status sllmcx_profile_init(sllmcx_profile *profile);

/* True if the profile enables a given task. */
SLLMCX_API int sllmcx_profile_has_task(const sllmcx_profile *profile, sllmcx_task task);

/* A stable, human-readable label for the language (profile->other_language when
   OTHER). Never NULL. */
SLLMCX_API const char *sllmcx_profile_language_label(const sllmcx_profile *profile);

#ifdef __cplusplus
}
#endif
#endif
