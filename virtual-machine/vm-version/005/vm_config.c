/*
 * virtual-machine/vm-version/001/vm_config.c
 * VM v001 ground floor -- user-level configuration surface (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Wraps the real core knobs. The memory calls bind to sleela_memmgr when it is
 * compiled in (VM_HAVE_MEMMGR); otherwise the configuration is still recorded
 * so vm_status can report it (useful when these units are built standalone).
 */
#include "vm_config.h"

#if defined(VM_HAVE_MEMMGR)
#include "sleela_memmgr.h"
#endif

static VmConfig g_current;
static int      g_applied = 0;

VmConfig vm_config_defaults(void) {
    VmConfig c;
    c.memory_limit_bytes = VM_MAX_MEMORY_BYTES;  /* 2 GiB */
    c.thread_cap         = VM_MAX_THREADS;       /* 128   */
    c.verbosity          = VM_VERBOSITY_NORMAL;
    c.capabilities       = VM_CAP_ALL;
    c.memory_manager_on  = 1;
    return c;
}

void vm_config_set_memory(VmConfig *c, uint64_t bytes) {
    if (!c) return;
    if (bytes == 0) { c->memory_manager_on = 0; c->memory_limit_bytes = 0; return; }
    if (bytes < VM_MIN_MEMORY_BYTES) bytes = VM_MIN_MEMORY_BYTES;
    if (bytes > VM_MAX_MEMORY_BYTES) bytes = VM_MAX_MEMORY_BYTES;
    c->memory_limit_bytes = bytes;
    c->memory_manager_on  = 1;
}

void vm_config_set_threads(VmConfig *c, int n) {
    if (!c) return;
    if (n < 1) n = 1;
    if (n > VM_MAX_THREADS) n = VM_MAX_THREADS;
    c->thread_cap = n;
}

void vm_config_set_verbosity(VmConfig *c, VmVerbosity v) {
    if (!c) return;
    if (v < VM_VERBOSITY_QUIET) v = VM_VERBOSITY_QUIET;
    if (v > VM_VERBOSITY_DEBUG) v = VM_VERBOSITY_DEBUG;
    c->verbosity = v;
}

void vm_config_enable(VmConfig *c, VmCapability cap)  { if (c) c->capabilities |=  (uint32_t)cap; }
void vm_config_disable(VmConfig *c, VmCapability cap) { if (c) c->capabilities &= ~(uint32_t)cap; }
int  vm_config_has(const VmConfig *c, VmCapability cap) {
    return c ? ((c->capabilities & (uint32_t)cap) != 0) : 0;
}

int vm_config_apply(const VmConfig *c) {
    if (!c) return -1;
    g_current = *c;
    g_applied = 1;
#if defined(VM_HAVE_MEMMGR)
    if (c->memory_manager_on && c->memory_limit_bytes != 0) {
        if (slmm_enable((size_t)c->memory_limit_bytes) != 0) return -2;
    } else {
        slmm_disable();
    }
#endif
    /* The thread cap and capability mask are recorded here and consulted by the
     * core's thread/bridge entry points; v001 records them for observability
     * (vm_status). Enforcement wiring is a later think-tank round. */
    return 0;
}

const VmConfig *vm_config_current(void) { return g_applied ? &g_current : NULL; }

const char *vm_verbosity_name(VmVerbosity v) {
    switch (v) {
        case VM_VERBOSITY_QUIET:   return "quiet";
        case VM_VERBOSITY_NORMAL:  return "normal";
        case VM_VERBOSITY_VERBOSE: return "verbose";
        case VM_VERBOSITY_DEBUG:   return "debug";
        default:                   return "normal";
    }
}
