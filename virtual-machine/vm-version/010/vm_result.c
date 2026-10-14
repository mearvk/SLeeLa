/*
 * virtual-machine/vm-version/003/vm_result.c
 * VM v003 -- typed results (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_result.h"
#include "vm_report.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void copy_bounded(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    size_t n = strlen(src);
    if (n >= cap) n = cap - 1;
    memcpy(dst, src, n);
    dst[n] = '\0';
}

VmResult vm_ok(void) {
    VmResult r;
    r.code = VM_E_OK;
    r.subsystem[0] = '\0';
    r.message[0] = '\0';
    return r;
}

VmResult vm_fail(VmResultCode code, const char *subsystem, const char *fmt, ...) {
    VmResult r;
    r.code = code;
    copy_bounded(r.subsystem, sizeof r.subsystem, subsystem ? subsystem : "vm");
    va_list ap; va_start(ap, fmt);
    vsnprintf(r.message, sizeof r.message, fmt ? fmt : "", ap);
    va_end(ap);
    return r;
}

int vm_is_ok(const VmResult *r) { return r && r->code == VM_E_OK; }

int vm_result_report(const VmResult *r) {
    if (!r || r->code == VM_E_OK) return 0;
    const char *code = vm_result_code_name(r->code);
    switch (r->code) {
        case VM_E_LIMIT:
        case VM_E_DENIED:
        case VM_E_INTERNAL:
            return vm_error(r->subsystem, code, "%s", r->message);
        default:
            return vm_warn(r->subsystem, code, "%s", r->message);
    }
}

const char *vm_result_code_name(VmResultCode code) {
    switch (code) {
        case VM_E_OK:          return "OK";
        case VM_E_INVALID:     return "INVALID";
        case VM_E_LIMIT:       return "LIMIT";
        case VM_E_NOT_FOUND:   return "NOT_FOUND";
        case VM_E_UNAVAILABLE: return "UNAVAILABLE";
        case VM_E_MALFORMED:   return "MALFORMED";
        case VM_E_EXHAUSTED:   return "EXHAUSTED";
        case VM_E_DENIED:      return "DENIED";
        case VM_E_INTERNAL:    return "INTERNAL";
        default:               return "INTERNAL";
    }
}

/* Common sleela_* ABI convention: 0 == OK, negatives are per-family errors.
 * We map the widely-used codes (as in idea_canvas_status and the memmgr/bridge
 * families) onto the stable VM categories; unknown negatives -> INTERNAL. */
VmResultCode vm_result_from_status(int status) {
    if (status == 0) return VM_E_OK;
    switch (status) {
        case -1: return VM_E_INVALID;       /* INVALID            */
        case -2: return VM_E_LIMIT;          /* OOM/LIMIT family   */
        case -3: return VM_E_UNAVAILABLE;    /* NO_RENDERER / n/a  */
        case -4: return VM_E_MALFORMED;      /* malformed / GL err */
        case -5: return VM_E_MALFORMED;
        case -6: return VM_E_EXHAUSTED;
        case -7: return VM_E_INTERNAL;       /* corrupt            */
        default: return VM_E_INTERNAL;
    }
}
