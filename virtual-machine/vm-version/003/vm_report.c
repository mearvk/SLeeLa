/*
 * virtual-machine/vm-version/001/vm_report.c
 * VM v001 ground floor -- uniform diagnostics (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_report.h"
#include "vm_config.h"

#include <stdarg.h>

static FILE *g_stream = NULL;   /* lazily defaults to stderr */

void vm_report_set_stream(FILE *stream) { g_stream = stream; }

const char *vm_severity_name(VmSeverity s) {
    switch (s) {
        case VM_SEV_DEBUG:  return "DEBUG";
        case VM_SEV_NOTICE: return "NOTICE";
        case VM_SEV_WARN:   return "WARN";
        case VM_SEV_ERROR:  return "ERROR";
        default:            return "INFO";
    }
}

/* Map the active verbosity to the lowest severity that will be shown. */
static VmSeverity min_shown(void) {
    const VmConfig *c = vm_config_current();
    VmVerbosity v = c ? c->verbosity : VM_VERBOSITY_NORMAL;
    switch (v) {
        case VM_VERBOSITY_QUIET:   return VM_SEV_ERROR;   /* errors only       */
        case VM_VERBOSITY_NORMAL:  return VM_SEV_WARN;    /* warn + error      */
        case VM_VERBOSITY_VERBOSE: return VM_SEV_NOTICE;  /* notice..error     */
        case VM_VERBOSITY_DEBUG:   return VM_SEV_DEBUG;   /* everything        */
        default:                   return VM_SEV_WARN;
    }
}

static int vreport(VmSeverity sev, const char *subsystem, const char *code,
                   const char *fmt, va_list ap) {
    if (sev < min_shown()) return 0;
    FILE *out = g_stream ? g_stream : stderr;
    int n = 0;
    n += fprintf(out, "[idea-vm] %s (%s", vm_severity_name(sev),
                 subsystem ? subsystem : "vm");
    if (code && code[0]) n += fprintf(out, "/%s", code);
    n += fprintf(out, "): ");
    n += vfprintf(out, fmt ? fmt : "", ap);
    n += fprintf(out, "\n");
    return n;
}

int vm_report(VmSeverity sev, const char *subsystem, const char *code,
              const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = vreport(sev, subsystem, code, fmt, ap);
    va_end(ap);
    return n;
}

int vm_error(const char *subsystem, const char *code, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = vreport(VM_SEV_ERROR, subsystem, code, fmt, ap);
    va_end(ap);
    return n;
}
int vm_warn(const char *subsystem, const char *code, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = vreport(VM_SEV_WARN, subsystem, code, fmt, ap);
    va_end(ap);
    return n;
}
int vm_notice(const char *subsystem, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = vreport(VM_SEV_NOTICE, subsystem, NULL, fmt, ap);
    va_end(ap);
    return n;
}
int vm_debug(const char *subsystem, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    int n = vreport(VM_SEV_DEBUG, subsystem, NULL, fmt, ap);
    va_end(ap);
    return n;
}
