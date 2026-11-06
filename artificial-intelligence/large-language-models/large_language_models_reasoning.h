#ifndef SLEELA_AI_LLM_REASONING_H
#define SLEELA_AI_LLM_REASONING_H
/*
 * SLeeLa Reasoning Language Model (RLM) layer.
 *
 * A neuro-symbolic reasoning controller that turns a plain next-token LLM
 * (sllm_backend) into a model that reasons: it decomposes a question, proposes
 * intermediate steps as an explicit chain of thought, REFLECTS on and critiques
 * each step, VERIFIES steps through a pluggable checker, BACKTRACKS when a path
 * fails, and only then commits an answer.
 *
 * This matches the current research consensus the SLeeLa project adopted
 * (see REASONING.md): chain-of-thought is a token-level behavior; the gains of
 * a reasoning model come from the control procedure (test-time compute,
 * self-verification, backtracking) plus an external verifier, NOT from changing
 * the vocabulary. The LLM supplies PROPOSALS and plausibility; the verifier
 * supplies SOUNDNESS. For deductive subjects a symbolic verifier (e.g. a bridge
 * to lib/deliberation) should be supplied; for open subjects a weaker critic is
 * used and the engine reports lower confidence rather than claiming proof.
 *
 * The layer bundles NO model: it drives a caller-supplied sllm_backend and an
 * optional caller-supplied verifier. It is an integration contract.
 *
 * Design goals: sophistication, nuance, and reflection. The engine is tuned for
 * subjects that are mainly philosophical, mathematical, or religious, where a
 * single confident answer is often wrong and the honest output is a reasoned,
 * self-aware position with its assumptions, objections, and confidence exposed.
 */
#include "large_language_models.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMR_API __declspec(dllexport)
# else
#  define SLLMR_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMR_API __attribute__((visibility("default")))
#else
# define SLLMR_API
#endif

#define SLLMR_ABI_VERSION 1u

typedef struct sllmr_engine sllmr_engine;

typedef enum sllmr_status {
 SLLMR_OK=0, SLLMR_INVALID_ARGUMENT=1, SLLMR_OUT_OF_MEMORY=2,
 SLLMR_BACKEND_ERROR=3, SLLMR_NOT_LOADED=4, SLLMR_CANCELLED=5,
 SLLMR_NO_SOUND_PATH=6, SLLMR_UNSUPPORTED=7
} sllmr_status;

/* Subject mode steers reflection depth, how strongly the verifier is trusted,
   and how confidence is reported. These are NOT separate models or vocabularies
   -- they are controller policies over the same backend. */
typedef enum sllmr_subject {
 SLLMR_SUBJECT_GENERAL=0,     /* balanced default */
 SLLMR_SUBJECT_MATH=1,        /* expects a symbolic/numeric verifier; seeks proof */
 SLLMR_SUBJECT_PHILOSOPHY=2,  /* dialectic: surface assumptions, steelman objections */
 SLLMR_SUBJECT_RELIGION=3     /* interpretive: attribute claims to traditions, no proof-claims */
} sllmr_subject;

/* The role a step plays in the chain of thought. */
typedef enum sllmr_step_kind {
 SLLMR_STEP_DECOMPOSE=0,  /* split the problem / name sub-questions */
 SLLMR_STEP_ASSUMPTION=1, /* an explicit premise the path depends on */
 SLLMR_STEP_DEDUCTION=2,  /* a claimed logical/mathematical consequence */
 SLLMR_STEP_INDUCTION=3,  /* a generalization from cases / evidence */
 SLLMR_STEP_OBJECTION=4,  /* a counterargument / steelman against the path */
 SLLMR_STEP_REFLECTION=5, /* self-critique of the path so far */
 SLLMR_STEP_REVISION=6,   /* a correction after reflection/verification */
 SLLMR_STEP_CONCLUSION=7  /* the committed position for this path */
} sllmr_step_kind;

/* A verifier's judgement of one step. A verifier NEVER fabricates support: when
   it cannot decide, it returns UNKNOWN, and the engine treats UNKNOWN honestly
   (it does not promote an unknown step to "verified"). */
typedef enum sllmr_verdict {
 SLLMR_VERDICT_UNKNOWN=0,   /* cannot determine (open/interpretive, or no checker) */
 SLLMR_VERDICT_SUPPORTED=1, /* consistent / entailed given the stated assumptions */
 SLLMR_VERDICT_REFUTED=2    /* contradicts assumptions or a known fact -> backtrack */
} sllmr_verdict;

/* One reasoning step, borrowed for the lifetime of the callback only. */
typedef struct sllmr_step {
 uint32_t index;            /* 0-based position within the current path */
 uint32_t depth;            /* backtrack depth; 0 = first attempt */
 sllmr_step_kind kind;
 const char *text;          /* the step's natural-language content (UTF-8) */
 sllmr_verdict verdict;     /* the verifier's judgement of this step */
 double confidence;         /* engine confidence in this step, [0,1] */
} sllmr_step;

/* ---- Verifier contract (the symbolic / critic half) ----
   Given the question, the accumulated prior steps, and a candidate step, decide
   whether the candidate is supported. 'note_out'/'note_cap' let the verifier
   return a short rationale (optional; may be left empty). Returning
   SLLMR_VERDICT_UNKNOWN is always acceptable and never penalized as a failure.
   The verifier and its user_data are borrowed and must outlive the engine. */
typedef struct sllmr_verifier {
 uint32_t abi_version;
 const char *name;
 void *user_data;
 sllmr_verdict (*check)(void *user_data,
                        sllmr_subject subject,
                        const char *question,
                        const sllmr_step *prior, size_t prior_count,
                        const sllmr_step *candidate,
                        char *note_out, size_t note_cap);
} sllmr_verifier;

/* Controller policy. Zero-initialize then sllmr_options_init for defaults. */
typedef struct sllmr_options {
 uint32_t abi_version;
 sllmr_subject subject;
 uint32_t max_steps;        /* hard cap on steps per path (test-time compute budget) */
 uint32_t max_backtracks;   /* how many times a refuted path may be abandoned and retried */
 uint32_t reflection_every; /* insert a REFLECTION step every N steps (0 = only at end) */
 double   min_confidence;   /* a CONCLUSION below this is reported as tentative */
 int      require_verified; /* if nonzero, a CONCLUSION must rest only on non-refuted,
                               and (for MATH) on SUPPORTED, steps or the path fails */
 uint64_t seed;             /* forwarded to the backend for deterministic decoding */
} sllmr_options;

/* Streamed observation of the reasoning process. Any callback may be NULL.
   Return 0 from on_step to request cancellation (yields SLLMR_CANCELLED). */
typedef struct sllmr_trace {
 void *user_data;
 int  (*on_step)(void *user_data, const sllmr_step *step);     /* each step as it is formed */
 void (*on_backtrack)(void *user_data, uint32_t from_depth, const char *reason);
} sllmr_trace;

/* The committed outcome. Strings are owned by the result and freed by
   sllmr_result_free. 'steps' is the final, accepted path. */
typedef struct sllmr_result {
 char *answer;              /* the committed position (UTF-8), may be tentative */
 char *summary;             /* a short reflective summary of how it was reached */
 sllmr_step *steps;         /* the accepted reasoning path */
 size_t step_count;
 double confidence;         /* overall confidence in [0,1] */
 int    verified;           /* nonzero if every load-bearing step is non-refuted
                               (and SUPPORTED where the subject demands proof) */
 int    tentative;          /* nonzero if confidence < min_confidence or subject is open */
 uint32_t backtracks;       /* how many paths were abandoned */
} sllmr_result;

SLLMR_API const char *sllmr_status_string(sllmr_status status);
SLLMR_API const char *sllmr_subject_name(sllmr_subject subject);
SLLMR_API const char *sllmr_step_kind_name(sllmr_step_kind kind);
SLLMR_API const char *sllmr_verdict_name(sllmr_verdict verdict);

SLLMR_API sllmr_status sllmr_options_init(sllmr_options *options);

/* Create a reasoning engine over a loaded LLM engine and an OPTIONAL verifier
   (pass NULL for a critic-free, LLM-only run that can still reflect/backtrack
   on self-critique but reports lower confidence). Both are borrowed. */
SLLMR_API sllmr_status sllmr_engine_create(sllm_engine *llm,
                                           const sllmr_verifier *verifier,
                                           sllmr_engine **out_engine);
SLLMR_API void sllmr_engine_destroy(sllmr_engine *engine);
SLLMR_API const char *sllmr_engine_verifier_name(const sllmr_engine *engine);

/* Reason about 'question'. On SLLMR_OK, *out_result is filled and must be
   released with sllmr_result_free. On SLLMR_NO_SOUND_PATH the engine exhausted
   its backtracks without a path meeting require_verified; *out_result still
   holds the best (tentative) path for inspection. */
SLLMR_API sllmr_status sllmr_reason(sllmr_engine *engine,
                                    const char *question,
                                    const sllmr_options *options,
                                    const sllmr_trace *trace,
                                    sllmr_result *out_result);

SLLMR_API void sllmr_result_free(sllmr_result *result);

#ifdef __cplusplus
}
#endif
#endif
