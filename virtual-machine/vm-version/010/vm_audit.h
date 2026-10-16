/*
 * virtual-machine/vm-version/009/vm_audit.h
 * VM v009 -- structured, bounded audit trail.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-008. Each prior layer reports its own events to the console
 * (vm_report). v009 adds a single durable record: a bounded ring buffer of
 * structured audit events (config changes, capability denials, memory/GC
 * pressure, table saturation, platform mismatches) with a monotonic sequence
 * number and a running hash chain, so the trail is ordered and tamper-evident.
 *
 * It does not replace vm_report (live console) -- it is the queryable history a
 * user or tool inspects after the fact, and the backbone of v010's facade.
 *
 * Pure C; fixed capacity (no allocation after init); thread-safe counter via
 * the same atomic discipline as v004.
 */
#ifndef VM_AUDIT_H
#define VM_AUDIT_H

#include "vm_result.h"   /* VmResultCode */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_AUDIT_INFO    = 0,
    VM_AUDIT_CONFIG  = 1,   /* configuration applied/changed      */
    VM_AUDIT_DENIAL  = 2,   /* capability denied                  */
    VM_AUDIT_PRESSURE= 3,   /* memory / GC pressure               */
    VM_AUDIT_SATURATE= 4,   /* handle-table near capacity         */
    VM_AUDIT_PLATFORM= 5,   /* platform parity mismatch           */
    VM_AUDIT_ERROR   = 6    /* a reported error                   */
} VmAuditKind;

#define VM_AUDIT_MSG_CAP 120
#define VM_AUDIT_CAPACITY 256   /* ring size (events; oldest overwritten) */

typedef struct {
    uint64_t    seq;          /* monotonic sequence number           */
    uint64_t    hash;         /* running chain hash (prev ^ this)    */
    VmAuditKind kind;
    char        subsystem[24];
    char        message[VM_AUDIT_MSG_CAP];
} VmAuditEvent;

/* Reset the trail (clears the ring, zeroes the sequence + chain). */
void vm_audit_init(void);

/* Record an event (printf-style message). Returns its sequence number. Also
 * mirrors the event to vm_report at a severity implied by kind, so live and
 * recorded views stay consistent. */
uint64_t vm_audit_record(VmAuditKind kind, const char *subsystem,
                         const char *fmt, ...);

/* Convenience: record a typed VmResult as an audit event (maps code->kind). */
uint64_t vm_audit_result(const VmResult *r);

/* How many events have ever been recorded (may exceed capacity). */
uint64_t vm_audit_total(void);

/* How many are currently retained in the ring (<= VM_AUDIT_CAPACITY). */
int vm_audit_count(void);

/* Copy the i-th retained event (0 = oldest retained) into *out. 1 ok / 0 oob. */
int vm_audit_get(int i, VmAuditEvent *out);

/* The running chain hash after the last event (0 if none). A verifier can
 * recompute it over the events to detect reordering/truncation within the ring. */
uint64_t vm_audit_chain(void);

/* Render the most recent `max` events (newest last) into buf (snprintf). */
int vm_audit_report(char *buf, size_t cap, int max);

const char *vm_audit_kind_name(VmAuditKind k);

#ifdef __cplusplus
}
#endif

#endif /* VM_AUDIT_H */
