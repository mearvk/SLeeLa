/*
 * native/include/sleela_opcode.h
 * SLeeLa Native Opcode Bridge — stable C ABI.
 *
 * This header is the explicit VM/OS bridge below the SLeeLa /lib/opcodes
 * single-opcode classes. Those classes declare the one-opcode-per-object model
 * in SLeeLa source; the actual "advance the instruction pointer" (fetch) and
 * "execute exactly one decoded opcode" (dispatch one) steps are value/VM
 * primitives delegated here.
 *
 * The authoritative runtime remains /impl/core (impl/core/sleela_core.c), whose
 * dispatch loop is `SLInstr in = vm->code[ip++]; switch (in.op) { ... }`. This
 * bridge exposes that same discipline one step at a time so a SLeeLa opcode
 * object can request the fetch and then its single dispatch.
 *
 * Canonical ISA: 103 opcodes, codes 0..102. Codes 0..97 are the base 98
 * (OP_NOP .. OP_AUDIO_PLATFORM); codes 98..102 are the array extension.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#ifndef SLEELA_OPCODE_H
#define SLEELA_OPCODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_OPCODE_MIN_CODE 0
#define SLEELA_OPCODE_MAX_CODE 102
#define SLEELA_OPCODE_BASE_COUNT 98  /* codes 0..97 */
#define SLEELA_OPCODE_TOTAL_COUNT 103

/*
 * Fetch: carefully advance the VM frame to its next instruction and return the
 * resulting instruction pointer, or -1 on an inactive/invalid frame. This does
 * not execute anything; it is the ip++ half of the dispatch loop.
 */
int32_t sleela_opcode_vm_next(int32_t frame);

/*
 * Dispatch-one: execute exactly the single opcode `code` against the VM frame at
 * instruction pointer `ip`. Returns the next instruction pointer to continue
 * from, or -1 when the VM halts (OP_HALT) or rejects an unknown/unsupported
 * opcode. An out-of-range code is rejected (mirrors the canonical map rule:
 * unknown opcodes are never silently treated as no-ops).
 */
int32_t sleela_opcode_execute_one(int32_t frame, int32_t code, int32_t ip);

/* Canonical mnemonic for a code (0..102), or "OP_UNKNOWN". Never NULL. */
const char *sleela_opcode_mnemonic(int32_t code);

/* 1 if code is a valid canonical opcode (0..102), else 0. */
int sleela_opcode_is_valid(int32_t code);

/*
 * Running-helper signals (used by lib/opcodes/running). Observe a well-known VM
 * signal for the conditional-reactive layer. `signal` is one of the SIG_*
 * selectors below; `index` is the current stream position (passed through for
 * index-relative signals). Returns the observed value, or -1 on invalid frame.
 */
#define SLEELA_OPCODE_SIG_IP           1  /* live instruction pointer    */
#define SLEELA_OPCODE_SIG_LOCK_DEPTH   2  /* locks currently held        */
#define SLEELA_OPCODE_SIG_CALL_DEPTH   3  /* open call frames            */
#define SLEELA_OPCODE_SIG_OPEN_SOCKETS 4  /* open sockets                */
#define SLEELA_OPCODE_SIG_WARN_COUNT   5  /* governance warnings so far  */
#define SLEELA_OPCODE_SIG_INDEX        6  /* current stream index        */
int32_t sleela_opcode_signal(int32_t frame, int32_t signal, int32_t index);

/*
 * Pre-stage (warm) an opcode: ask the VM to make the opcode `code` ready to fire
 * with no cold-start cost. Returns 1 when staged, 0 when the code is invalid.
 * Warming never executes the opcode.
 */
int sleela_opcode_prestage(int32_t frame, int32_t code);

/* Last human-readable error (never NULL). */
const char *sleela_opcode_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OPCODE_H */
