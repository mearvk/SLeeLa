/*
 * virtual-machine/vm-version/006/vm_profile.h
 * VM v006 -- hot-path dispatch profiler (per-opcode-family counters + timing).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-005. The VM loop dispatches many opcode families (arithmetic,
 * control flow, calls, struct/array, the native bridges). A user tuning a
 * program has no idea where time goes. v006 adds a lightweight profiler:
 *
 *   - per-family execution counts and accumulated nanoseconds;
 *   - a cheap scoped timer (vm_profile_begin/end) the dispatcher wraps around a
 *     handler, so cost is attributed to the right family;
 *   - a "hot family" query and a ranked report, feeding v004 telemetry's STEPS
 *     counter so totals stay consistent.
 *
 * Overhead is a counter increment + two clock reads per dispatch; it can be
 * compiled out by never calling begin/end. Pure C, monotonic clock.
 */
#ifndef VM_PROFILE_H
#define VM_PROFILE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opcode families the profiler attributes time to. */
typedef enum {
    VM_OP_ARITH   = 0,  /* + - * / %, comparisons, logic         */
    VM_OP_CONTROL = 1,  /* if/while/for/return/branches          */
    VM_OP_LOADSTORE = 2,/* local/field/global get & set          */
    VM_OP_CALL    = 3,  /* method calls                          */
    VM_OP_STRUCT  = 4,  /* struct/array new/get/set/pack         */
    VM_OP_BRIDGE  = 5,  /* native-bridge built-ins (io/net/...)  */
    VM_OP_PRINT   = 6,  /* print / output                        */
    VM_OP__N      = 7
} VmOpFamily;

/* Reset all profiler state. */
void vm_profile_init(void);

/* Begin timing a dispatch of family `f`; returns an opaque start token
 * (monotonic ns). Pair with vm_profile_end. */
uint64_t vm_profile_begin(VmOpFamily f);

/* End timing: attribute (now - start) ns and one execution to family `f`, and
 * bump v004's STEPS counter. */
void vm_profile_end(VmOpFamily f, uint64_t start_token);

/* Count a dispatch without timing (when a clock read is too costly). */
void vm_profile_tick(VmOpFamily f);

uint64_t vm_profile_count(VmOpFamily f);
uint64_t vm_profile_nanos(VmOpFamily f);

/* The family with the most accumulated time (the hot path). */
VmOpFamily vm_profile_hot(void);

/* Ranked, one-block report (families by time, with count and avg ns). */
int vm_profile_report(char *buf, size_t cap);

const char *vm_op_family_name(VmOpFamily f);

#ifdef __cplusplus
}
#endif

#endif /* VM_PROFILE_H */
