/*
 * virtual-machine/vm-version/010/vm_runtime.h
 * VM v010 -- the consolidated runtime facade (the eventual program).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * The think-tank's final round. v002-009 each added one user-level improvement
 * over the baseline core; they are powerful but separate. v010 wires them into
 * ONE coherent surface so an embedder uses the whole hardened VM through four
 * calls:
 *
 *     VmConfig c = vm_config_defaults();
 *     vm_runtime_init(&c);          // config -> platform -> caps -> mem -> gc
 *                                   //        -> telemetry -> profile -> audit
 *     ... run the program, periodically:
 *     vm_runtime_poll();            // sample memory/gc pressure + saturation
 *     ... on a guarded native op:
 *     if (!vm_runtime_permit(VM_CAP_NETWORK, "net", "listen", &r)) return r;
 *     ...
 *     vm_runtime_report(buf, cap);  // one consolidated status block
 *     vm_runtime_shutdown();
 *
 * The facade owns the wiring and ordering; the individual layers remain usable
 * directly. Pure C.
 */
#ifndef VM_RUNTIME_H
#define VM_RUNTIME_H

#include "vm_config.h"
#include "vm_result.h"
#include "vm_telemetry.h"   /* VmTable */
#include "vm_gc.h"          /* VmGcSnapshot */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VM_RUNTIME_VERSION "idea-vm v010 (consolidated)"

/* Initialise the whole runtime from a user configuration, in dependency order:
 *   apply config -> detect platform + parity audit -> reset capability counter
 *   -> (config already sized/enabled the memory manager) -> init gc quality
 *   -> init telemetry (+ register the real table capacities) -> init profiler
 *   -> init the audit trail (and record a CONFIG event).
 * Returns 0 on success. */
int  vm_runtime_init(const VmConfig *c);

/* Register a bounded handle-table capacity with telemetry (e.g. SL_BESTOF_MAX).
 * Call once per table after init if you want saturation watching. */
void vm_runtime_register_table(VmTable t, int capacity);

/* Periodic health sample: polls memory pressure and (given a fresh GC
 * snapshot, or NULL to skip) GC quality, feeding telemetry and the audit trail.
 * Call on a timer or every N steps. */
void vm_runtime_poll(const VmGcSnapshot *gc_snapshot);

/* Guard a native operation by capability. 1 = proceed; 0 = denied (fills *out
 * with a typed DENIED result, records it to the audit trail). */
int  vm_runtime_permit(VmCapability cap, const char *subsystem,
                       const char *operation, VmResult *out);

/* Record an arbitrary typed result into the audit trail (and live report). */
void vm_runtime_note(const VmResult *r);

/* Render ONE consolidated status block: version + config/platform + memory + gc
 * + telemetry + recent audit. snprintf semantics. */
int  vm_runtime_report(char *buf, size_t cap);

/* Tear everything down (records a final audit event). */
void vm_runtime_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* VM_RUNTIME_H */
