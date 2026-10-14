/*
 * virtual-machine/vm-version/007/vm_capability.h
 * VM v007 -- capability enforcement: turn the config mask into real decisions.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-006. v001 let a user DISABLE capabilities (net, file-io, …)
 * but the mask was only recorded, not enforced -- a known gap called out in
 * v001's VERSION.md. v007 closes it: before a native bridge acts, it asks the
 * gate, and a disabled capability yields a typed DENIED result (via v003)
 * reported through v001, counted in v004 telemetry. One choke point, uniform
 * policy, observable denials.
 *
 * Bridges adopt it with a single guard line, e.g.:
 *     VmResult r;
 *     if (!vm_cap_check(VM_CAP_NETWORK, "net", "listen", &r)) return r;
 *
 * Pure C; reads the live vm_config capability mask.
 */
#ifndef VM_CAPABILITY_H
#define VM_CAPABILITY_H

#include "vm_config.h"   /* VmCapability */
#include "vm_result.h"   /* VmResult     */

#ifdef __cplusplus
extern "C" {
#endif

/* Is `cap` permitted by the current configuration? (1/0). Unknown/no-config
 * defaults to permitted so an unconfigured VM is not accidentally crippled. */
int vm_cap_allowed(VmCapability cap);

/* Guard a bridge operation. If `cap` is allowed, returns 1 (proceed). If
 * denied, fills `*out` with a typed VM_E_DENIED result (subsystem + a message
 * naming the operation), reports it, bumps the telemetry error counter, and
 * returns 0 (the bridge should return *out). `out` may be NULL. */
int vm_cap_check(VmCapability cap, const char *subsystem, const char *operation,
                 VmResult *out);

/* Count of denials observed this run (for status/telemetry). */
unsigned long vm_cap_denials(void);

/* Reset the denial counter. */
void vm_cap_reset(void);

/* Human-readable capability name (never NULL). */
const char *vm_cap_name(VmCapability cap);

#ifdef __cplusplus
}
#endif

#endif /* VM_CAPABILITY_H */
