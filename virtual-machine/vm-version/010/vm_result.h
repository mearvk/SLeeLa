/*
 * virtual-machine/vm-version/003/vm_result.h
 * VM v003 -- typed results: structured, user-legible bridge outcomes.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001/v002. Across the core, native bridges return bare status ints
 * (0/-1/-2/...) whose meaning differs per family -- opaque to a user and
 * error-prone to callers. v003 introduces one typed result the whole VM shares:
 *
 *     VmResult r = vm_ok();
 *     ...
 *     return vm_fail(VM_E_LIMIT, "memory", "would exceed %llu MiB", cap);
 *
 * A VmResult carries a stable category code, the subsystem, and a formatted
 * message. `vm_result_report()` prints it through v001's vm_report in the
 * uniform format, so every failure path is legible and consistent. Callers can
 * branch on `r.code` (stable) instead of per-family magic numbers.
 *
 * Pure C, header + small .c, no new dependencies.
 */
#ifndef VM_RESULT_H
#define VM_RESULT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Stable, VM-wide result categories (map any bridge's raw status onto these). */
typedef enum {
    VM_E_OK           = 0,   /* success                                      */
    VM_E_INVALID      = 1,   /* bad argument / null handle                   */
    VM_E_LIMIT        = 2,   /* a configured bound was exceeded              */
    VM_E_NOT_FOUND    = 3,   /* handle / name not found                      */
    VM_E_UNAVAILABLE  = 4,   /* resource/capability not available            */
    VM_E_MALFORMED    = 5,   /* input could not be parsed/decoded            */
    VM_E_EXHAUSTED    = 6,   /* table/stream exhausted                       */
    VM_E_DENIED       = 7,   /* capability denied by policy                  */
    VM_E_INTERNAL     = 8    /* unexpected internal error                    */
} VmResultCode;

#define VM_RESULT_MSG_CAP 160

typedef struct {
    VmResultCode code;
    char         subsystem[32];
    char         message[VM_RESULT_MSG_CAP];
} VmResult;

/* Construct a success result. */
VmResult vm_ok(void);

/* Construct a failure result (printf-style message). */
VmResult vm_fail(VmResultCode code, const char *subsystem, const char *fmt, ...);

/* True if the result is success. */
int vm_is_ok(const VmResult *r);

/* Report a result through vm_report at the severity implied by its code
 * (VM_E_OK -> nothing; VM_E_LIMIT/DENIED/INTERNAL -> ERROR; others -> WARN).
 * Returns bytes written by vm_report (0 if suppressed or OK). */
int vm_result_report(const VmResult *r);

/* Stable short name/code string for a result code (e.g. "LIMIT"). Never NULL. */
const char *vm_result_code_name(VmResultCode code);

/* Map a legacy bridge status (the common 0 ok / negative error convention used
 * by sleela_* ABIs, e.g. idea_canvas_status) onto a VmResultCode, so existing
 * bridges can adopt typed results without changing their return values. */
VmResultCode vm_result_from_status(int status);

#ifdef __cplusplus
}
#endif

#endif /* VM_RESULT_H */
