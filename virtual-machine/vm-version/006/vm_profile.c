/*
 * virtual-machine/vm-version/006/vm_profile.c
 * VM v006 -- dispatch profiler (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_profile.h"
#include "vm_telemetry.h"

#include <stdio.h>
#include <time.h>

static uint64_t g_count[VM_OP__N];
static uint64_t g_nanos[VM_OP__N];

static uint64_t now_ns(void) {
#if defined(CLOCK_MONOTONIC)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#endif
    return (uint64_t)clock() * (1000000000ull / CLOCKS_PER_SEC);
}

void vm_profile_init(void) {
    for (int i = 0; i < VM_OP__N; ++i) { g_count[i] = 0; g_nanos[i] = 0; }
}

uint64_t vm_profile_begin(VmOpFamily f) { (void)f; return now_ns(); }

void vm_profile_end(VmOpFamily f, uint64_t start_token) {
    if (f < 0 || f >= VM_OP__N) return;
    uint64_t dt = now_ns() - start_token;
    g_count[f] += 1;
    g_nanos[f] += dt;
    vm_telemetry_add(VM_COUNTER_STEPS, 1);
    if (f == VM_OP_BRIDGE) vm_telemetry_add(VM_COUNTER_BRIDGE, 1);
}

void vm_profile_tick(VmOpFamily f) {
    if (f < 0 || f >= VM_OP__N) return;
    g_count[f] += 1;
    vm_telemetry_add(VM_COUNTER_STEPS, 1);
    if (f == VM_OP_BRIDGE) vm_telemetry_add(VM_COUNTER_BRIDGE, 1);
}

uint64_t vm_profile_count(VmOpFamily f) { return (f>=0&&f<VM_OP__N)?g_count[f]:0; }
uint64_t vm_profile_nanos(VmOpFamily f) { return (f>=0&&f<VM_OP__N)?g_nanos[f]:0; }

VmOpFamily vm_profile_hot(void) {
    VmOpFamily hot = VM_OP_ARITH; uint64_t best = 0;
    for (int i = 0; i < VM_OP__N; ++i)
        if (g_nanos[i] > best) { best = g_nanos[i]; hot = (VmOpFamily)i; }
    return hot;
}

int vm_profile_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    /* rank families by accumulated nanos (simple selection, N==7) */
    int order[VM_OP__N];
    for (int i = 0; i < VM_OP__N; ++i) order[i] = i;
    for (int i = 0; i < VM_OP__N; ++i)
        for (int j = i + 1; j < VM_OP__N; ++j)
            if (g_nanos[order[j]] > g_nanos[order[i]]) { int t=order[i]; order[i]=order[j]; order[j]=t; }

    int n = snprintf(buf, cap, "dispatch profile (by time):\n");
    for (int k = 0; k < VM_OP__N && n > 0 && (size_t)n < cap; ++k) {
        int f = order[k];
        if (g_count[f] == 0) continue;
        double avg = (double)g_nanos[f] / (double)g_count[f];
        n += snprintf(buf + n, cap - (size_t)n,
                      "  %-10s count=%llu  total=%llu ns  avg=%.1f ns\n",
                      vm_op_family_name((VmOpFamily)f),
                      (unsigned long long)g_count[f],
                      (unsigned long long)g_nanos[f], avg);
    }
    return n;
}

const char *vm_op_family_name(VmOpFamily f) {
    switch (f) {
        case VM_OP_ARITH:     return "arith";
        case VM_OP_CONTROL:   return "control";
        case VM_OP_LOADSTORE: return "loadstore";
        case VM_OP_CALL:      return "call";
        case VM_OP_STRUCT:    return "struct";
        case VM_OP_BRIDGE:    return "bridge";
        case VM_OP_PRINT:     return "print";
        default:              return "op";
    }
}
