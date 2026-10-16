/*
 * virtual-machine/vm-version/009/vm_audit.c
 * VM v009 -- audit trail (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A fixed-capacity ring of structured events. Each event carries a monotonic
 * sequence number and a running FNV-1a chain hash folded over (prev_hash, seq,
 * kind, subsystem, message), so a verifier recomputing the chain can detect
 * reordering or truncation of the retained window.
 */
#include "vm_audit.h"
#include "vm_report.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static VmAuditEvent g_ring[VM_AUDIT_CAPACITY];
static uint64_t     g_total = 0;    /* ever recorded                 */
static int          g_count = 0;    /* retained in ring              */
static int          g_head  = 0;    /* next write index              */
static uint64_t     g_chain = 0;    /* running chain hash            */

static uint64_t fnv1a(uint64_t h, const void *data, size_t n) {
    const unsigned char *p = (const unsigned char *)data;
    if (h == 0) h = 1469598103934665603ULL;
    for (size_t i = 0; i < n; ++i) { h ^= p[i]; h *= 1099511628211ULL; }
    return h;
}

void vm_audit_init(void) {
    memset(g_ring, 0, sizeof g_ring);
    g_total = 0; g_count = 0; g_head = 0; g_chain = 0;
}

static void copy_bounded(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    size_t n = src ? strlen(src) : 0;
    if (n >= cap) n = cap - 1;
    if (n) memcpy(dst, src, n);
    dst[n] = '\0';
}

static VmSeverity sev_for(VmAuditKind k) {
    switch (k) {
        case VM_AUDIT_DENIAL:
        case VM_AUDIT_ERROR:    return VM_SEV_ERROR;
        case VM_AUDIT_PRESSURE:
        case VM_AUDIT_SATURATE:
        case VM_AUDIT_PLATFORM: return VM_SEV_WARN;
        case VM_AUDIT_CONFIG:   return VM_SEV_NOTICE;
        default:                return VM_SEV_DEBUG;
    }
}

uint64_t vm_audit_record(VmAuditKind kind, const char *subsystem,
                         const char *fmt, ...) {
    VmAuditEvent e;
    e.seq = ++g_total;
    e.kind = kind;
    copy_bounded(e.subsystem, sizeof e.subsystem, subsystem ? subsystem : "vm");
    va_list ap; va_start(ap, fmt);
    vsnprintf(e.message, sizeof e.message, fmt ? fmt : "", ap);
    va_end(ap);

    /* fold into the chain hash */
    uint64_t h = g_chain;
    h = fnv1a(h, &e.seq, sizeof e.seq);
    h = fnv1a(h, &e.kind, sizeof e.kind);
    h = fnv1a(h, e.subsystem, strlen(e.subsystem));
    h = fnv1a(h, e.message, strlen(e.message));
    g_chain = h;
    e.hash = h;

    g_ring[g_head] = e;
    g_head = (g_head + 1) % VM_AUDIT_CAPACITY;
    if (g_count < VM_AUDIT_CAPACITY) g_count++;

    /* mirror to the live console */
    vm_report(sev_for(kind), e.subsystem, vm_audit_kind_name(kind), "%s", e.message);
    return e.seq;
}

uint64_t vm_audit_result(const VmResult *r) {
    if (!r) return g_total;
    VmAuditKind k;
    switch (r->code) {
        case VM_E_DENIED:    k = VM_AUDIT_DENIAL;   break;
        case VM_E_EXHAUSTED: k = VM_AUDIT_SATURATE; break;
        case VM_E_LIMIT:     k = VM_AUDIT_PRESSURE; break;
        case VM_E_UNAVAILABLE: k = VM_AUDIT_PLATFORM; break;
        case VM_E_OK:        k = VM_AUDIT_INFO;     break;
        default:             k = VM_AUDIT_ERROR;    break;
    }
    return vm_audit_record(k, r->subsystem, "%s", r->message);
}

uint64_t vm_audit_total(void) { return g_total; }
int      vm_audit_count(void) { return g_count; }
uint64_t vm_audit_chain(void) { return g_chain; }

int vm_audit_get(int i, VmAuditEvent *out) {
    if (i < 0 || i >= g_count || !out) return 0;
    /* oldest retained is at (g_head - g_count) mod capacity */
    int start = (g_head - g_count + VM_AUDIT_CAPACITY) % VM_AUDIT_CAPACITY;
    *out = g_ring[(start + i) % VM_AUDIT_CAPACITY];
    return 1;
}

int vm_audit_report(char *buf, size_t cap, int max) {
    if (!buf || cap == 0) return 0;
    int total = g_count;
    int first = (max > 0 && max < total) ? total - max : 0;
    int n = snprintf(buf, cap, "audit: %llu events (retained %d, chain %016llx)\n",
                     (unsigned long long)g_total, g_count,
                     (unsigned long long)g_chain);
    for (int i = first; i < total && n > 0 && (size_t)n < cap; ++i) {
        VmAuditEvent e;
        if (!vm_audit_get(i, &e)) break;
        n += snprintf(buf + n, cap - (size_t)n, "  #%llu %-9s %s: %s\n",
                      (unsigned long long)e.seq, vm_audit_kind_name(e.kind),
                      e.subsystem, e.message);
    }
    return n;
}

const char *vm_audit_kind_name(VmAuditKind k) {
    switch (k) {
        case VM_AUDIT_INFO:     return "INFO";
        case VM_AUDIT_CONFIG:   return "CONFIG";
        case VM_AUDIT_DENIAL:   return "DENIAL";
        case VM_AUDIT_PRESSURE: return "PRESSURE";
        case VM_AUDIT_SATURATE: return "SATURATE";
        case VM_AUDIT_PLATFORM: return "PLATFORM";
        case VM_AUDIT_ERROR:    return "ERROR";
        default:                return "INFO";
    }
}
