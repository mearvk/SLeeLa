/*
 * virtual-machine/vm-version/004/vm_telemetry.h
 * VM v004 -- thread-safe telemetry + bounded-handle-table saturation watch.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-003. The core owns several bounded, mutex-guarded handle
 * tables (best-of 32, synchro 64, munction 64, ai, …). A user has no view of
 * how close any of them is to exhaustion, and no VM-wide counters for the work
 * the VM is doing. v004 adds a small, lock-free (C11 atomics) telemetry layer:
 *
 *   - monotonic counters the bridges bump (steps executed, allocations,
 *     bridge calls, errors) -- safe to update from any thread with no mutex;
 *   - a per-table "gauge" (live / capacity) with a high-water mark and a
 *     saturation watch that emits a typed vm_result (via v003) once a table
 *     crosses a usage watermark -- so near-exhaustion is observable before it
 *     fails.
 *
 * Pure C11 (<stdatomic.h>); degrades to plain stores if atomics are absent.
 */
#ifndef VM_TELEMETRY_H
#define VM_TELEMETRY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* VM-wide monotonic counters (lock-free). */
typedef enum {
    VM_COUNTER_STEPS      = 0,  /* VM opcodes executed            */
    VM_COUNTER_ALLOCS     = 1,  /* accounted allocations          */
    VM_COUNTER_BRIDGE     = 2,  /* native-bridge calls            */
    VM_COUNTER_ERRORS     = 3,  /* failures reported              */
    VM_COUNTER__N         = 4
} VmCounter;

/* Bounded handle tables the VM watches for saturation. */
typedef enum {
    VM_TABLE_BESTOF   = 0,
    VM_TABLE_SYNCHRO  = 1,
    VM_TABLE_MUNCTION = 2,
    VM_TABLE_AI       = 3,
    VM_TABLE_CANVAS   = 4,
    VM_TABLE__N       = 5
} VmTable;

/* Reset all counters and gauges; set each table's capacity. Call at startup. */
void vm_telemetry_init(void);

/* Register a table's capacity (e.g. SL_BESTOF_MAX). */
void vm_telemetry_set_capacity(VmTable t, int capacity);

/* Lock-free counter bump (any thread). */
void     vm_telemetry_add(VmCounter c, uint64_t delta);
uint64_t vm_telemetry_get(VmCounter c);

/* Update a table gauge to `live` entries. Thread-safe. If usage crosses the
 * saturation watermark (default 0.80) upward, emits ONE typed vm_result
 * (EXHAUSTED-category WARN) through v003; records the high-water mark. */
void vm_telemetry_gauge(VmTable t, int live);

/* Current live / high-water / capacity for a table. */
int vm_telemetry_live(VmTable t);
int vm_telemetry_peak(VmTable t);
int vm_telemetry_capacity(VmTable t);

/* Set the saturation watermark fraction (0..1) for all tables. */
void vm_telemetry_set_watermark(double frac);

/* Render a one-block telemetry report (counters + per-table gauges) into buf
 * (snprintf semantics). */
int vm_telemetry_report(char *buf, size_t cap);

/* Table name (never NULL). */
const char *vm_table_name(VmTable t);

#ifdef __cplusplus
}
#endif

#endif /* VM_TELEMETRY_H */
