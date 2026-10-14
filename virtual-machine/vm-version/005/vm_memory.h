/*
 * virtual-machine/vm-version/002/vm_memory.h
 * VM v002 -- memory hardening: pressure thresholds + watermark reporting.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001's ground floor. The core memory manager (sleela_memmgr)
 * accounts live/peak bytes and enforces a hard limit, but has no notion of
 * *pressure* -- a user cannot be warned as memory approaches the cap, only told
 * after an allocation is refused. v002 adds a thin pressure layer over slmm_*:
 *
 *   - WARNING and CRITICAL watermarks (fractions of the hard limit);
 *   - a condition the VM can poll (NORMAL / PRESSURE / CRITICAL / LIMIT);
 *   - edge-triggered reporting through v001's vm_report, so the user sees a
 *     single legible notice when a watermark is crossed (not per-allocation);
 *   - a one-line human-readable pressure report.
 *
 * Pure C over sleela_memmgr when VM_HAVE_MEMMGR is defined; otherwise it
 * degrades to a self-contained accounting mode so the layer builds standalone.
 */
#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_MEM_NORMAL   = 0,  /* below the warning watermark                    */
    VM_MEM_PRESSURE = 1,  /* at/above warning, below critical               */
    VM_MEM_CRITICAL = 2,  /* at/above critical, below the hard limit        */
    VM_MEM_LIMIT    = 3   /* an allocation has been refused by the limit    */
} VmMemCondition;

/* Configure the watermarks as fractions (0..1) of the hard limit. Defaults:
 * warning 0.80, critical 0.95. Clamped and ordered (warn <= crit). */
void vm_memory_set_watermarks(double warning_frac, double critical_frac);

/* Poll the current condition from live slmm stats and (edge-triggered) emit a
 * vm_report line the first time each higher watermark is crossed, and a
 * recovery notice when pressure falls back to NORMAL. Call after allocations or
 * on a timer. Returns the current condition. */
VmMemCondition vm_memory_poll(void);

/* The condition from the most recent poll, without re-sampling or reporting. */
VmMemCondition vm_memory_condition(void);

/* Live usage as a fraction of the hard limit (0..1; 0 if no limit set). */
double vm_memory_usage_fraction(void);

/* Render a one-line pressure report into buf (snprintf semantics):
 *   "memory: 1636/2048 MiB (79.9%) condition=PRESSURE refused=0" */
int vm_memory_report(char *buf, size_t cap);

/* Condition name (never NULL). */
const char *vm_memory_condition_name(VmMemCondition c);

#ifdef __cplusplus
}
#endif

#endif /* VM_MEMORY_H */
