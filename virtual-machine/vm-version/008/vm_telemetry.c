/*
 * virtual-machine/vm-version/004/vm_telemetry.c
 * VM v004 -- thread-safe telemetry + saturation watch (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Counters are lock-free C11 atomics. The per-table gauge uses an atomic live
 * count + an atomic high-water mark updated with a compare-exchange loop, and
 * an atomic "already-warned" flag so the saturation notice is edge-triggered
 * exactly once per table even under concurrent updates.
 */
#include "vm_telemetry.h"
#include "vm_result.h"
#include "vm_report.h"

#include <stdio.h>
#include <string.h>

#if defined(__STDC_NO_ATOMICS__)
#  define VM_NO_ATOMICS 1
#endif

#if !defined(VM_NO_ATOMICS)
#  include <stdatomic.h>
typedef _Atomic uint64_t atomic_u64;
typedef _Atomic int      atomic_i;
#else
typedef uint64_t atomic_u64;
typedef int      atomic_i;
#  define atomic_fetch_add(p, v) (*(p) += (v))
#  define atomic_load(p)         (*(p))
#  define atomic_store(p, v)     (*(p) = (v))
#endif

static atomic_u64 g_counters[VM_COUNTER__N];

typedef struct {
    atomic_i live;
    atomic_i peak;
    atomic_i capacity;
    atomic_i warned;     /* 0/1, edge-trigger latch */
} Gauge;

static Gauge  g_tables[VM_TABLE__N];
static double g_watermark = 0.80;

void vm_telemetry_init(void) {
    for (int i = 0; i < VM_COUNTER__N; ++i) atomic_store(&g_counters[i], 0);
    for (int t = 0; t < VM_TABLE__N; ++t) {
        atomic_store(&g_tables[t].live, 0);
        atomic_store(&g_tables[t].peak, 0);
        atomic_store(&g_tables[t].capacity, 0);
        atomic_store(&g_tables[t].warned, 0);
    }
}

void vm_telemetry_set_capacity(VmTable t, int capacity) {
    if (t < 0 || t >= VM_TABLE__N) return;
    atomic_store(&g_tables[t].capacity, capacity > 0 ? capacity : 0);
}

void vm_telemetry_add(VmCounter c, uint64_t delta) {
    if (c < 0 || c >= VM_COUNTER__N) return;
    atomic_fetch_add(&g_counters[c], delta);
}
uint64_t vm_telemetry_get(VmCounter c) {
    if (c < 0 || c >= VM_COUNTER__N) return 0;
    return atomic_load(&g_counters[c]);
}

void vm_telemetry_set_watermark(double frac) {
    if (frac < 0.0) frac = 0.0;
    if (frac > 1.0) frac = 1.0;
    g_watermark = frac;
}

void vm_telemetry_gauge(VmTable t, int live) {
    if (t < 0 || t >= VM_TABLE__N) return;
    if (live < 0) live = 0;
    Gauge *g = &g_tables[t];
    atomic_store(&g->live, live);

    /* raise the high-water mark (CAS loop) */
#if !defined(VM_NO_ATOMICS)
    int prev = atomic_load(&g->peak);
    while (live > prev && !atomic_compare_exchange_weak(&g->peak, &prev, live)) { }
#else
    if (live > g->peak) g->peak = live;
#endif

    int cap = atomic_load(&g->capacity);
    if (cap <= 0) return;
    double f = (double)live / (double)cap;

    if (f >= g_watermark) {
        /* edge-trigger: warn once until it recovers below the watermark */
#if !defined(VM_NO_ATOMICS)
        int expected = 0;
        if (atomic_compare_exchange_strong(&g->warned, &expected, 1)) {
#else
        if (g->warned == 0) { g->warned = 1;
#endif
            VmResult r = vm_fail(VM_E_EXHAUSTED, "telemetry",
                                 "%s table near capacity: %d/%d (%.0f%%)",
                                 vm_table_name(t), live, cap, f * 100.0);
            vm_result_report(&r);
            vm_telemetry_add(VM_COUNTER_ERRORS, 1);
        }
    } else {
        atomic_store(&g->warned, 0);  /* re-arm when usage falls back */
    }
}

int vm_telemetry_live(VmTable t)     { return (t>=0&&t<VM_TABLE__N)?atomic_load(&g_tables[t].live):0; }
int vm_telemetry_peak(VmTable t)     { return (t>=0&&t<VM_TABLE__N)?atomic_load(&g_tables[t].peak):0; }
int vm_telemetry_capacity(VmTable t) { return (t>=0&&t<VM_TABLE__N)?atomic_load(&g_tables[t].capacity):0; }

const char *vm_table_name(VmTable t) {
    switch (t) {
        case VM_TABLE_BESTOF:   return "best-of";
        case VM_TABLE_SYNCHRO:  return "synchro";
        case VM_TABLE_MUNCTION: return "munction";
        case VM_TABLE_AI:       return "ai";
        case VM_TABLE_CANVAS:   return "canvas";
        default:                return "table";
    }
}

int vm_telemetry_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    int n = snprintf(buf, cap,
        "telemetry: steps=%llu allocs=%llu bridge=%llu errors=%llu\n",
        (unsigned long long)vm_telemetry_get(VM_COUNTER_STEPS),
        (unsigned long long)vm_telemetry_get(VM_COUNTER_ALLOCS),
        (unsigned long long)vm_telemetry_get(VM_COUNTER_BRIDGE),
        (unsigned long long)vm_telemetry_get(VM_COUNTER_ERRORS));
    for (int t = 0; t < VM_TABLE__N && n > 0 && (size_t)n < cap; ++t) {
        int capv = vm_telemetry_capacity((VmTable)t);
        if (capv <= 0) continue;
        n += snprintf(buf + n, cap - (size_t)n,
                      "  %-9s %d/%d (peak %d)\n", vm_table_name((VmTable)t),
                      vm_telemetry_live((VmTable)t), capv,
                      vm_telemetry_peak((VmTable)t));
    }
    return n;
}
