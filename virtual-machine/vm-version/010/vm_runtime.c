/*
 * virtual-machine/vm-version/010/vm_runtime.c
 * VM v010 -- consolidated runtime facade (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Wires v001-009 into one ordered surface. Each call delegates to the layer
 * that owns the concern; the facade's job is correct ordering + a single
 * consolidated report + routing significant events into the audit trail.
 */
#include "vm_runtime.h"

#include "vm_report.h"
#include "vm_status.h"
#include "vm_memory.h"
#include "vm_telemetry.h"
#include "vm_gc.h"
#include "vm_profile.h"
#include "vm_capability.h"
#include "vm_platform.h"
#include "vm_audit.h"

#include <stdio.h>
#include <string.h>

static int g_up = 0;

int vm_runtime_init(const VmConfig *c) {
    if (!c) return -1;

    /* 1. apply the user configuration (sizes/enables the memory manager). */
    if (vm_config_apply(c) != 0) return -2;

    /* 2. platform + parity audit (warns on permitted-but-unsupported caps). */
    vm_platform_init();

    /* 3. capability enforcement counter. */
    vm_cap_reset();

    /* 4. gc quality + 5. telemetry + 6. profiler. */
    vm_gc_init();
    vm_telemetry_init();
    vm_profile_init();

    /* 7. audit trail, and record the startup config event. */
    vm_audit_init();
    const VmConfig *cur = vm_config_current();
    if (cur) {
        vm_audit_record(VM_AUDIT_CONFIG, "runtime",
            "init: %llu MiB, %d threads, verbosity=%s, platform=%s",
            (unsigned long long)(cur->memory_limit_bytes / (1024ull*1024ull)),
            cur->thread_cap, vm_verbosity_name(cur->verbosity),
            vm_platform_name(vm_platform_current()));
    }
    /* fold any platform parity mismatches into the trail */
    int mism = vm_platform_audit();
    if (mism > 0)
        vm_audit_record(VM_AUDIT_PLATFORM, "runtime",
                        "%d capability/platform mismatch(es)", mism);

    g_up = 1;
    return 0;
}

void vm_runtime_register_table(VmTable t, int capacity) {
    vm_telemetry_set_capacity(t, capacity);
}

void vm_runtime_poll(const VmGcSnapshot *gc_snapshot) {
    if (!g_up) return;

    VmMemCondition m = vm_memory_poll();
    if (m == VM_MEM_CRITICAL || m == VM_MEM_LIMIT) {
        char line[160]; vm_memory_report(line, sizeof line);
        vm_audit_record(VM_AUDIT_PRESSURE, "memory", "%s", line);
    }

    if (gc_snapshot) {
        VmGcCondition g = vm_gc_observe(gc_snapshot);
        if (g == VM_GC_PRESSURE || g == VM_GC_THRASH) {
            char line[160]; vm_gc_report(line, sizeof line);
            vm_audit_record(VM_AUDIT_PRESSURE, "gc", "%s", line);
        }
    }
}

int vm_runtime_permit(VmCapability cap, const char *subsystem,
                      const char *operation, VmResult *out) {
    VmResult r;
    if (vm_cap_check(cap, subsystem, operation, &r)) return 1;
    vm_audit_result(&r);          /* record the denial durably */
    if (out) *out = r;
    return 0;
}

void vm_runtime_note(const VmResult *r) {
    if (r && !vm_is_ok(r)) vm_audit_result(r);
}

int vm_runtime_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    char *p = buf; size_t left = cap; int w;

    w = snprintf(p, left, "== %s ==\n", VM_RUNTIME_VERSION);
    if (w > 0) { p += w; left -= (size_t)w; }

    w = vm_status_render(p, left);            if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_platform_report(p, left);          if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_memory_report(p, left);            if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = snprintf(p, left, "\n");              if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_gc_report(p, left);                if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = snprintf(p, left, "\n");              if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_telemetry_report(p, left);         if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_profile_report(p, left);           if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }
    w = vm_audit_report(p, left, 8);          if (w > 0 && (size_t)w < left) { p += w; left -= (size_t)w; }

    return (int)(cap - left);
}

void vm_runtime_shutdown(void) {
    if (!g_up) return;
    vm_audit_record(VM_AUDIT_INFO, "runtime", "shutdown: %llu steps, %llu errors",
                    (unsigned long long)vm_telemetry_get(VM_COUNTER_STEPS),
                    (unsigned long long)vm_telemetry_get(VM_COUNTER_ERRORS));
    g_up = 0;
}
