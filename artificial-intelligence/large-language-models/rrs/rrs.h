#ifndef SLEELA_AI_LLM_RRS_H
#define SLEELA_AI_LLM_RRS_H
/*
 * RRS — Reasonable Responsibility Solidity. A kind of LLM.
 *
 * RRS is a reasoning model built as THREE CONGRUENT MODELS chained FEED-FORWARD,
 * each operating on a simple ternary signal:
 *
 *     1 = AFFIRM   (yes; the premise stands and conjoins)
 *     0 = ELIMINATE(no; the premise is reclaimed out by a prelude)
 *     2 = POWER-ADD(binary-simple power addition: an affirmed premise is
 *                   accumulated into the conjunctive "power" — potestas built
 *                   by addition of what stands together)
 *
 * The three congruent stages (same contract, composable):
 *
 *   STAGE 1  CONJUNCTIVE  — a Conjunctive Neural Net stage. Reasonable
 *            assumptions SURFACE; each is signalled. A prelude may RECLAIM an
 *            assumption as ELIMINATION (0), pruning it. Survivors AFFIRM (1) and
 *            POWER-ADD (2). The stage conjoins: the path continues only if the
 *            surviving premises hold TOGETHER (strict AND).
 *
 *   STAGE 2  RESPONSIBILITY — mounts the survivors onto the question of
 *            Reasonable Responsibility as the PRELUDE TO CONTINUE: you may
 *            proceed only if responsibility is met. Unmet responsibility
 *            ELIMINATES (0); met responsibility AFFIRMS (1) and POWER-ADDs (2).
 *
 *   STAGE 3  SOLIDITY/FIRMAMENT — the culmination: a Reasonable argument about
 *            SOLIDITY (structural firmness) and FIRMAMENT (the fixed ground).
 *            It reads the accumulated power and renders the CULMINATION.
 *
 * Explicitly "less the Faith of Trial of Man of Reason": RRS SUBTRACTS faith.
 * A premise that rests on faith / trial-of-faith is out of scope — it is
 * surfaced and ELIMINATED (0), never affirmed. RRS argues from reason about
 * what stands firm; it makes no faith-claim and no proof-by-faith.
 *
 * The CULMINATION is "Power in the Latin sense" — potestas / potentia: a
 * warranted CAPACITY to continue/act that issues from a completed responsible
 * process, graded (none / provisional / warranted). It is capacity-authority,
 * NOT force.
 *
 * RRS ships no weights: it is a controller + verifier contract over the
 * reasoning engine (SLLMR_*). The ternary net is a real, tiny conjunctive
 * accumulator defined here; a backend LLM (optional) supplies premise text.
 */
#include "../large_language_models_reasoning.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMRRS_API __declspec(dllexport)
# else
#  define SLLMRRS_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMRRS_API __attribute__((visibility("default")))
#else
# define SLLMRRS_API
#endif

#define SLLMRRS_ABI_VERSION 1u

/* The ternary signal. The names are the contract; the values are chosen so the
   conjunctive accumulator can treat AFFIRM/POWER_ADD as contributing and
   ELIMINATE as a hard zero. */
typedef enum sllmrrs_signal {
 SLLMRRS_ELIMINATE = 0,  /* no: reclaimed out by a prelude (hard zero / AND-kill) */
 SLLMRRS_AFFIRM    = 1,  /* yes: stands and conjoins */
 SLLMRRS_POWER_ADD = 2   /* binary-simple power addition into the conjunction */
} sllmrrs_signal;

/* The three congruent stages. */
typedef enum sllmrrs_stage {
 SLLMRRS_STAGE_CONJUNCTIVE   = 0, /* surface assumptions; prelude elimination; strict AND */
 SLLMRRS_STAGE_RESPONSIBILITY = 1,/* reasonable-responsibility prelude to continue */
 SLLMRRS_STAGE_SOLIDITY      = 2, /* solidity/firmament culmination */
 SLLMRRS_STAGE_COUNT         = 3
} sllmrrs_stage;

/* The culmination verdict — potestas, graded. */
typedef enum sllmrrs_potestas {
 SLLMRRS_POTESTAS_NONE        = 0, /* no warranted capacity: the argument did not stand */
 SLLMRRS_POTESTAS_PROVISIONAL = 1, /* capacity conditional on what is still open */
 SLLMRRS_POTESTAS_WARRANTED   = 2  /* full warranted capacity-to-continue (responsible power) */
} sllmrrs_potestas;

typedef enum sllmrrs_status {
 SLLMRRS_OK=0, SLLMRRS_INVALID_ARGUMENT=1, SLLMRRS_FAITH_PREMISE=2,
 SLLMRRS_NOT_SOLID=3, SLLMRRS_RESPONSIBILITY_UNMET=4, SLLMRRS_CAPACITY=5
} sllmrrs_status;

/* One premise entering the feed-forward. 'faith_or_trial' marks a premise that
   rests on faith/trial — RRS eliminates it (subtracted, never affirmed). */
typedef struct sllmrrs_premise {
 const char *text;        /* the premise (borrowed) */
 sllmrrs_signal signal;   /* the caller/handler's proposed ternary signal */
 int faith_or_trial;      /* nonzero => a faith/trial premise (will be eliminated) */
 int responsibility_met;  /* for stage 2: nonzero => responsibility is satisfied */
} sllmrrs_premise;

/* The conjunctive accumulator — the "power addition". Affirmed/power-added
   premises add to 'power'; any ELIMINATE (or a faith premise, or unmet
   responsibility at stage 2) sets the stage's conjunction false. */
typedef struct sllmrrs_accumulator {
 uint32_t power;          /* sum of contributions from AFFIRM(+1)/POWER_ADD(+2) */
 uint32_t affirmed;       /* count of surviving (non-eliminated) premises */
 uint32_t eliminated;     /* count eliminated (incl. faith/trial) */
 int      conjunction;    /* 1 if the strict-AND still holds for this stage */
} sllmrrs_accumulator;

/* The result of running the three-stage feed-forward. */
typedef struct sllmrrs_result {
 sllmrrs_accumulator stage[SLLMRRS_STAGE_COUNT];
 uint32_t total_power;         /* culminated power across the feed-forward */
 sllmrrs_potestas potestas;    /* the graded culmination verdict */
 int solid;                    /* 1 if solidity/firmament holds (all stages conjoined) */
 uint32_t faith_eliminated;    /* how many faith/trial premises were subtracted */
} sllmrrs_result;

SLLMRRS_API const char *sllmrrs_status_string(sllmrrs_status status);
SLLMRRS_API const char *sllmrrs_signal_name(sllmrrs_signal signal);
SLLMRRS_API const char *sllmrrs_stage_name(sllmrrs_stage stage);
SLLMRRS_API const char *sllmrrs_potestas_name(sllmrrs_potestas potestas);

/* The congruent stage kernel, applied identically at every stage: given the
   premises for the stage, fold the ternary signal into the accumulator under
   strict AND. A faith/trial premise is forced to ELIMINATE (subtracted). At the
   RESPONSIBILITY stage, a premise whose responsibility_met is zero is forced to
   ELIMINATE (the prelude to continue is unmet). Returns SLLMRRS_OK and fills
   *acc; the congruence is that the SAME kernel runs for all three stages. */
SLLMRRS_API sllmrrs_status sllmrrs_fold_stage(sllmrrs_stage stage,
                                              const sllmrrs_premise *premises, size_t count,
                                              sllmrrs_accumulator *acc);

/* Run the full three-model feed-forward: STAGE 1 (conjunctive elimination) ->
   STAGE 2 (responsibility prelude) -> STAGE 3 (solidity/firmament culmination).
   The feed-forward is congruent: each stage uses sllmrrs_fold_stage, and a
   stage whose conjunction is broken stops the forward pass (fail closed). On
   SLLMRRS_OK, *out holds the accumulators, total power, solidity, and the
   potestas culmination. 'stageN'/'count' give the premises for stages 1..3
   (stage 3 may be empty: its culmination reads the prior power). */
SLLMRRS_API sllmrrs_status sllmrrs_feed_forward(
 const sllmrrs_premise *stage1, size_t n1,
 const sllmrrs_premise *stage2, size_t n2,
 const sllmrrs_premise *stage3, size_t n3,
 sllmrrs_result *out);

/* Adapt RRS as an sllmr_verifier for the reasoning engine: a candidate step is
   SUPPORTED when it AFFIRMS/POWER-ADDs and conjoins, REFUTED when it ELIMINATES
   (including any faith/trial premise), UNKNOWN when RRS cannot classify it. The
   returned verifier is self-contained (no user_data needed). */
SLLMRRS_API sllmr_verifier sllmrrs_as_reasoning_verifier(void);

#ifdef __cplusplus
}
#endif
#endif
