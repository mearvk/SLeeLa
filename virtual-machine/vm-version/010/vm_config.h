/*
 * virtual-machine/vm-version/001/vm_config.h
 * VM v001 ground floor -- the user-level VM configuration surface.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * ONE place a user tunes the virtual machine before/at startup, instead of
 * reaching into individual bridges. It wraps the core's real knobs:
 *   - memory limit  -> sleela_memmgr (slmm_enable/slmm_set_limit; 256 MiB..2 GiB)
 *   - thread cap    -> the core's bounded concurrency (<= 128)
 *   - verbosity     -> how much the VM reports (see vm_report)
 *   - capabilities  -> which native bridges are permitted (mirrors SLPlatformCapability)
 *
 * This is the ground floor every later version's improvement is configured and
 * observed through. Pure C, no new dependencies.
 */
#ifndef VM_CONFIG_H
#define VM_CONFIG_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* How talkative the VM is (consumed by vm_report / vm_status). */
typedef enum {
    VM_VERBOSITY_QUIET  = 0,  /* errors only                                 */
    VM_VERBOSITY_NORMAL = 1,  /* errors + a startup banner (default)         */
    VM_VERBOSITY_VERBOSE= 2,  /* + per-stage notices                         */
    VM_VERBOSITY_DEBUG  = 3   /* + internal diagnostics                      */
} VmVerbosity;

/* Capability bits a user may permit/deny (mirror SLPlatformCapability order).
 * Combine by OR; VM_CAP_ALL is the default. */
typedef enum {
    VM_CAP_MEMORY   = 1u << 0,
    VM_CAP_FILE_IO  = 1u << 1,
    VM_CAP_NETWORK  = 1u << 2,
    VM_CAP_THREADS  = 1u << 3,
    VM_CAP_DYNLIB   = 1u << 4,
    VM_CAP_TERMINAL = 1u << 5,
    VM_CAP_PATHS    = 1u << 6,
    VM_CAP_PROCESS  = 1u << 7,
    VM_CAP_IPC      = 1u << 8,
    VM_CAP_TIME     = 1u << 9,
    VM_CAP_ALL      = 0x3FFu
} VmCapability;

/* The user-level configuration. Create with vm_config_defaults(), adjust the
 * fields (or via the setters, which clamp), then vm_config_apply() once. */
typedef struct {
    uint64_t    memory_limit_bytes;  /* 256 MiB..2 GiB; 0 = manager disabled */
    int         thread_cap;          /* 1..128                               */
    VmVerbosity verbosity;
    uint32_t    capabilities;        /* VM_CAP_* bitset                      */
    int         memory_manager_on;   /* enable the accounted allocator       */
} VmConfig;

/* Bounds (mirror the memory manager's own limits). */
#define VM_MIN_MEMORY_BYTES   (256ull * 1024ull * 1024ull)
#define VM_MAX_MEMORY_BYTES   (2048ull * 1024ull * 1024ull)
#define VM_MAX_THREADS        128

/* A sane default configuration: 2 GiB cap, 128 threads, NORMAL verbosity, all
 * capabilities, memory manager on. */
VmConfig vm_config_defaults(void);

/* Clamping setters (safe to call with any value). */
void vm_config_set_memory(VmConfig *c, uint64_t bytes);   /* clamped to range */
void vm_config_set_threads(VmConfig *c, int n);           /* clamped 1..128   */
void vm_config_set_verbosity(VmConfig *c, VmVerbosity v);
void vm_config_enable(VmConfig *c, VmCapability cap);
void vm_config_disable(VmConfig *c, VmCapability cap);
int  vm_config_has(const VmConfig *c, VmCapability cap);

/* Apply the configuration to the live core (enables/sizes the memory manager,
 * records the thread cap and capability mask). Returns 0 on success. Idempotent;
 * may be called again to re-tune. */
int  vm_config_apply(const VmConfig *c);

/* The configuration currently in force (after the last apply). */
const VmConfig *vm_config_current(void);

/* Human-readable names (never NULL). */
const char *vm_verbosity_name(VmVerbosity v);

#ifdef __cplusplus
}
#endif

#endif /* VM_CONFIG_H */
