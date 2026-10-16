/*
 * virtual-machine/vm-version/001/vm_status.c
 * VM v001 ground floor -- status readout (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_status.h"
#include "vm_config.h"

#if defined(VM_HAVE_MEMMGR)
#include "sleela_memmgr.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void append(char **p, char *end, const char *fmt, ...) {
    if (*p >= end) return;
    va_list ap; va_start(ap, fmt);
    int n = vsnprintf(*p, (size_t)(end - *p), fmt, ap);
    va_end(ap);
    if (n > 0) *p += (n < (end - *p) ? n : (end - *p));
}

int vm_status_render(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    char *p = buf, *end = buf + cap;
    const VmConfig *c = vm_config_current();

    append(&p, end, "%s\n", VM_VERSION_STRING);

    if (c) {
        if (c->memory_manager_on && c->memory_limit_bytes)
            append(&p, end, "  memory : %llu MiB (manager on)\n",
                   (unsigned long long)(c->memory_limit_bytes / (1024ull*1024ull)));
        else
            append(&p, end, "  memory : manager off\n");
        append(&p, end, "  threads: %d (max %d)\n", c->thread_cap, VM_MAX_THREADS);
        append(&p, end, "  verbose: %s\n", vm_verbosity_name(c->verbosity));
        append(&p, end, "  caps   : %s%s%s%s%s%s%s%s%s%s\n",
               vm_config_has(c, VM_CAP_MEMORY)  ? "mem "   : "",
               vm_config_has(c, VM_CAP_FILE_IO) ? "io "    : "",
               vm_config_has(c, VM_CAP_NETWORK) ? "net "   : "",
               vm_config_has(c, VM_CAP_THREADS) ? "thr "   : "",
               vm_config_has(c, VM_CAP_DYNLIB)  ? "dynlib ": "",
               vm_config_has(c, VM_CAP_TERMINAL)? "term "  : "",
               vm_config_has(c, VM_CAP_PATHS)   ? "paths " : "",
               vm_config_has(c, VM_CAP_PROCESS) ? "proc "  : "",
               vm_config_has(c, VM_CAP_IPC)     ? "ipc "   : "",
               vm_config_has(c, VM_CAP_TIME)    ? "time"   : "");
    } else {
        append(&p, end, "  (no configuration applied yet)\n");
    }

#if defined(VM_HAVE_MEMMGR)
    if (slmm_is_enabled()) {
        SLMMStats s; slmm_stats(&s);
        append(&p, end, "  mem.live: %zu bytes (peak %zu, allocs live %zu, refused %llu)\n",
               s.live_bytes, s.peak_bytes, s.live_allocs,
               (unsigned long long)s.refused);
    }
#endif

    return (int)(p - buf);
}

void vm_status_banner(void) {
    const VmConfig *c = vm_config_current();
    if (c && c->verbosity == VM_VERBOSITY_QUIET) return;
    char buf[512];
    vm_status_render(buf, sizeof buf);
    fputs(buf, stdout);
}
