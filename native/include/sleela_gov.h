/*
 * native/include/sleela_gov.h
 * SLeeLa Native Opcode-Governance Bridge — stable C ABI.
 *
 * This header is the explicit VM/OS bridge below the SLeeLa
 * lib/opcodes/governance classes (SLOpcodeRegistrar, SLOpcodeListener,
 * SLOpcodeEventObserver, SLGovernedExecution). The governance *policy* lives in
 * SLeeLa source; this bridge lets both SLeeLa and the VM know to listen for
 * ordering at the BEFORE / DURING / AFTER phases, and reports the live
 * instruction pointer the Listener hears during execution.
 *
 * The authoritative execution remains /impl/core. Governance never executes an
 * opcode itself; it only announces phases and observes the live ip.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#ifndef SLEELA_GOV_H
#define SLEELA_GOV_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Governance phases — must match lib/opcodes/governance/SLGovPhase.sleela. */
#define SLEELA_GOV_PHASE_BEFORE 1
#define SLEELA_GOV_PHASE_DURING 2
#define SLEELA_GOV_PHASE_AFTER  3

/*
 * Announce that a governance phase is beginning. Both the SLeeLa layer and the
 * VM registry are notified so they listen for ordering at this phase. Returns 1
 * when the phase is a recognised phase, 0 otherwise.
 */
int sleela_gov_announce_phase(int32_t phase);

/*
 * Report the live instruction pointer the VM is at for the opcode at
 * `expected_index` in the admitted sequence. The reference bridge tracks a
 * monotonic ip keyed by frame; /impl/core owns the authoritative pointer.
 * Returns the ip (>= 0) or -1 for an invalid frame.
 */
int32_t sleela_gov_live_ip(int32_t frame, int32_t expected_index);

/* The most recently announced phase (0 if none). */
int sleela_gov_current_phase(void);

/* How many times each phase has been announced since process start. */
int32_t sleela_gov_phase_count(int32_t phase);

/* Last human-readable governance note (never NULL). */
const char *sleela_gov_last_note(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_GOV_H */
