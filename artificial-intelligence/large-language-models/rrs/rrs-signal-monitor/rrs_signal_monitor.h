#ifndef SLEELA_AI_LLM_RRS_SIGNAL_MONITOR_H
#define SLEELA_AI_LLM_RRS_SIGNAL_MONITOR_H
/*
 * RRS signal monitor — observability over the RRS ternary 1/0/2 signal stream.
 *
 * It watches an RRS feed-forward and produces a "signal analysis": for every
 * premise in every stage it records the EFFECTIVE ternary signal actually used
 * (after RRS subtracts faith and after the responsibility prelude forces
 * elimination), the reason an elimination occurred, and the running power. It
 * then reports the per-signal / per-stage tallies and the potestas culmination.
 *
 * The monitor is read-only: it never changes an RRS outcome. It re-derives the
 * effective signal exactly as sllmrrs_fold_stage does, so the analysis matches
 * what the model actually computed. No weights, no allocation beyond a caller-
 * sized record buffer.
 *
 *   1 = AFFIRM     (+1 power)
 *   0 = ELIMINATE  (breaks the stage's strict AND)
 *   2 = POWER_ADD  (+2 power, binary-simple power addition)
 */
#include "../rrs.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMRM_API __declspec(dllexport)
# else
#  define SLLMRM_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMRM_API __attribute__((visibility("default")))
#else
# define SLLMRM_API
#endif

#define SLLMRM_ABI_VERSION 1u

/* Why a premise was eliminated (0). NONE means it was not eliminated. */
typedef enum sllmrm_reason {
 SLLMRM_REASON_NONE=0,            /* affirmed / power-added, not eliminated */
 SLLMRM_REASON_SIGNALLED_NO=1,    /* the premise itself signalled ELIMINATE */
 SLLMRM_REASON_FAITH=2,           /* faith/trial premise subtracted by RRS */
 SLLMRM_REASON_RESPONSIBILITY=3   /* responsibility prelude unmet at stage 2 */
} sllmrm_reason;

typedef enum sllmrm_status {
 SLLMRM_OK=0, SLLMRM_INVALID_ARGUMENT=1, SLLMRM_CAPACITY=2
} sllmrm_status;

/* One observed signal event: a premise, its stage, the effective signal RRS
   used, the elimination reason (if any), and the running power after it. */
typedef struct sllmrm_event {
 sllmrrs_stage  stage;
 uint32_t       index_in_stage;   /* 0-based within the stage */
 const char    *text;             /* borrowed from the premise */
 sllmrrs_signal effective_signal; /* 1/0/2 actually applied */
 sllmrm_reason  reason;           /* why eliminated, or NONE */
 uint32_t       running_power;    /* cumulative power up to and including this event */
} sllmrm_event;

/* Tallies of the full run. */
typedef struct sllmrm_tally {
 uint32_t affirm;        /* count of effective AFFIRM (1) */
 uint32_t eliminate;     /* count of effective ELIMINATE (0) */
 uint32_t power_add;     /* count of effective POWER_ADD (2) */
 uint32_t by_reason[4];  /* indexed by sllmrm_reason */
 uint32_t stage_power[SLLMRRS_STAGE_COUNT];
 uint32_t total_power;
 sllmrrs_potestas potestas;
 int solid;
} sllmrm_tally;

/* A caller-owned monitor: a fixed event buffer plus running tallies. */
typedef struct sllmrm_monitor {
 uint32_t abi_version;
 sllmrm_event *events;   /* caller-owned buffer */
 size_t        event_cap;
 size_t        event_count;
 sllmrm_tally  tally;
} sllmrm_monitor;

SLLMRM_API const char *sllmrm_reason_name(sllmrm_reason reason);

/* Initialize a monitor over a caller-owned event buffer (may be NULL/0 to tally
   only, without recording individual events). */
SLLMRM_API sllmrm_status sllmrm_monitor_init(sllmrm_monitor *monitor,
                                             sllmrm_event *event_buffer,
                                             size_t event_capacity);

/* Observe a full RRS feed-forward: runs sllmrrs_feed_forward, re-derives the
   effective per-premise signal (matching the model), records events (up to the
   buffer capacity; overflow is reported via SLLMRM_CAPACITY but tallies remain
   complete), and fills the tally. 'result_out' (optional) receives the RRS
   result. The stageN/countN inputs are the same premises passed to RRS. */
SLLMRM_API sllmrm_status sllmrm_observe(sllmrm_monitor *monitor,
                                        const sllmrrs_premise *stage1, size_t n1,
                                        const sllmrrs_premise *stage2, size_t n2,
                                        const sllmrrs_premise *stage3, size_t n3,
                                        sllmrrs_result *result_out);

/* Write a compact, human-readable signal-analysis report to 'out' (a caller
   buffer of 'cap' bytes). Returns the number of bytes that would be written
   (like snprintf); never writes past cap. Safe with out=NULL/cap=0 to size. */
SLLMRM_API int sllmrm_format_report(const sllmrm_monitor *monitor, char *out, size_t cap);

#ifdef __cplusplus
}
#endif
#endif
