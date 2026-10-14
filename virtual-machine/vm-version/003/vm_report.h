/*
 * virtual-machine/vm-version/001/vm_report.h
 * VM v001 ground floor -- uniform, user-readable diagnostics.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * One consistent diagnostic format for the whole VM so a user sees legible
 * messages instead of raw status ints. Every bridge reports through this:
 *
 *     [idea-vm] ERROR (memory/LIMIT): allocation would exceed 2048 MiB
 *     [idea-vm] NOTICE (net): listener bound on 127.0.0.1:45230
 *
 * Severity is gated by the active verbosity (vm_config). Pure C, writes to a
 * caller-chosen stream (default stderr) so it is testable and embeddable.
 */
#ifndef VM_REPORT_H
#define VM_REPORT_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_SEV_DEBUG  = 0,
    VM_SEV_NOTICE = 1,
    VM_SEV_WARN   = 2,
    VM_SEV_ERROR  = 3
} VmSeverity;

/* Choose where diagnostics go (default stderr). Pass NULL to reset to stderr. */
void vm_report_set_stream(FILE *stream);

/* Emit a diagnostic. `subsystem` is a short tag (e.g. "memory", "net", "vm");
 * `code` is an optional short code (e.g. "LIMIT", "OOM", NULL for none); the
 * printf-style message follows. The line is printed only if `sev` is allowed by
 * the current vm_config verbosity. Returns the number of bytes written (0 if
 * suppressed). The format is stable:
 *     [idea-vm] <SEV> (<subsystem>[/<code>]): <message>\n
 */
int vm_report(VmSeverity sev, const char *subsystem, const char *code,
              const char *fmt, ...);

/* Convenience wrappers. */
int vm_error (const char *subsystem, const char *code, const char *fmt, ...);
int vm_warn  (const char *subsystem, const char *code, const char *fmt, ...);
int vm_notice(const char *subsystem, const char *fmt, ...);
int vm_debug (const char *subsystem, const char *fmt, ...);

/* Severity name (never NULL). */
const char *vm_severity_name(VmSeverity s);

#ifdef __cplusplus
}
#endif

#endif /* VM_REPORT_H */
