/*
 * virtual-machine/vm-version/002/vm_memory.c
 * VM v002 -- memory hardening (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_memory.h"
#include "vm_report.h"

#include <stdio.h>

#if defined(VM_HAVE_MEMMGR)
#include "sleela_memmgr.h"
#endif

static double g_warn = 0.80;
static double g_crit = 0.95;
static VmMemCondition g_last = VM_MEM_NORMAL;
static VmMemCondition g_peak_reported = VM_MEM_NORMAL; /* highest already announced */

void vm_memory_set_watermarks(double warning_frac, double critical_frac) {
    if (warning_frac  < 0.0) warning_frac  = 0.0;
    if (warning_frac  > 1.0) warning_frac  = 1.0;
    if (critical_frac < 0.0) critical_frac = 0.0;
    if (critical_frac > 1.0) critical_frac = 1.0;
    if (critical_frac < warning_frac) critical_frac = warning_frac;
    g_warn = warning_frac;
    g_crit = critical_frac;
}

/* Pull (live_bytes, limit_bytes, refused) from slmm, or zeros when absent. */
static void sample(size_t *live, size_t *limit, uint64_t *refused) {
    *live = 0; *limit = 0; *refused = 0;
#if defined(VM_HAVE_MEMMGR)
    if (slmm_is_enabled()) {
        SLMMStats s; slmm_stats(&s);
        *live = s.live_bytes;
        *limit = s.limit_bytes;
        *refused = s.refused;
    }
#endif
}

double vm_memory_usage_fraction(void) {
    size_t live, limit; uint64_t refused;
    sample(&live, &limit, &refused);
    if (limit == 0) return 0.0;
    return (double)live / (double)limit;
}

static VmMemCondition classify(void) {
    size_t live, limit; uint64_t refused;
    sample(&live, &limit, &refused);
    if (refused > 0) return VM_MEM_LIMIT;
    if (limit == 0)  return VM_MEM_NORMAL;
    double f = (double)live / (double)limit;
    if (f >= g_crit) return VM_MEM_CRITICAL;
    if (f >= g_warn) return VM_MEM_PRESSURE;
    return VM_MEM_NORMAL;
}

VmMemCondition vm_memory_poll(void) {
    VmMemCondition c = classify();

    /* edge-triggered: announce only when crossing to a NEW higher condition,
     * and a single recovery notice when returning to NORMAL. */
    if (c > g_peak_reported) {
        double pct = vm_memory_usage_fraction() * 100.0;
        if (c == VM_MEM_LIMIT)
            vm_error("memory", "LIMIT", "allocation refused: hard limit reached");
        else if (c == VM_MEM_CRITICAL)
            vm_warn("memory", "CRITICAL", "usage %.1f%% -- above critical watermark", pct);
        else if (c == VM_MEM_PRESSURE)
            vm_warn("memory", "PRESSURE", "usage %.1f%% -- above warning watermark", pct);
        g_peak_reported = c;
    } else if (c == VM_MEM_NORMAL && g_peak_reported != VM_MEM_NORMAL) {
        vm_notice("memory", "pressure cleared -- usage back to normal");
        g_peak_reported = VM_MEM_NORMAL;
    }

    g_last = c;
    return c;
}

VmMemCondition vm_memory_condition(void) { return g_last; }

int vm_memory_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    size_t live, limit; uint64_t refused;
    sample(&live, &limit, &refused);
    double pct = (limit ? (double)live / (double)limit * 100.0 : 0.0);
    return snprintf(buf, cap,
                    "memory: %zu/%zu MiB (%.1f%%) condition=%s refused=%llu",
                    live / (1024u*1024u), limit / (1024u*1024u), pct,
                    vm_memory_condition_name(classify()),
                    (unsigned long long)refused);
}

const char *vm_memory_condition_name(VmMemCondition c) {
    switch (c) {
        case VM_MEM_NORMAL:   return "NORMAL";
        case VM_MEM_PRESSURE: return "PRESSURE";
        case VM_MEM_CRITICAL: return "CRITICAL";
        case VM_MEM_LIMIT:    return "LIMIT";
        default:              return "NORMAL";
    }
}
