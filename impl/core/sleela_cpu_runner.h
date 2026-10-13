#ifndef SLEELA_CPU_RUNNER_H
#define SLEELA_CPU_RUNNER_H
/* ==========================================================================
 * sleela_cpu_runner.h -- OS-specific native fast path for the /lib/cpu machine.
 *
 * This is the native backend behind the `native sleela_cpu_runner_*` bridge
 * calls in lib/cpu/SLNativeRunner.sleela. It executes the SLeeLa CPU model
 * (SLCPU + a compiled SLProgram image) in native C, as quickly as the host
 * allows, while keeping every safety and security concern explicit:
 *
 *   - Trusted load:    sleela_cpu_runner_sha256() measures an artifact so the
 *                      caller can match it against the repository's trusted
 *                      SHA-256 before the native core is ever opened.
 *   - Capability gate: sleela_cpu_runner_open() takes a capability MASK (bits
 *                      match SLPlatformCapability in sleela_platform.h) and
 *                      refuses any capability not granted. Deny-by-default.
 *   - Bounded handles: every open/create returns a VM-local integer handle
 *                      (>0) into a bounded table -- never a raw pointer. The
 *                      same handle discipline as sockets/files/threads.
 *   - Ownership:       the backend owns each native resource and releases it on
 *                      destroy/close. Stale handles fail closed (rejected).
 *   - Deadline/cancel: a run executes in bounded slices so the caller can poll
 *                      a cancellation safe point between slices and enforce a
 *                      wall-clock deadline; a run can never hang the host.
 *
 * Platform: the execution core itself is OS-neutral C (it computes over a
 * VM-owned word store); the OS-specific part is loading the fast core as the
 * host's native library -- .so via dlopen (Linux), .dll via LoadLibrary
 * (Windows), .dylib (macOS) -- through the existing sleela_library boundary.
 * ========================================================================== */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* A VM-local, bounded handle. 0 or negative means "no handle" / failure. */
typedef int32_t SLCpuRunnerHandle;
#define SL_CPU_RUNNER_INVALID 0

/* Capability bit positions in the mask passed to sleela_cpu_runner_open().
 * These mirror SLPlatformCapability ordinals in sleela_platform.h so a bit set
 * here authorizes exactly the one native boundary it names. */
#define SL_CPU_CAP_BIT(cap) (1u << (cap))

/* Bounded-table ceilings (same discipline as SL_MAX_SOCKETS etc.). */
#define SL_CPU_RUNNER_MAX_CORES    64        /* native runner instances       */
#define SL_CPU_RUNNER_MAX_LIBS     32        /* loaded native fast cores       */
#define SL_CPU_RUNNER_MEM_WORDS    (1u << 22)/* 4Mi-word modeled address space */

/* ---- Trusted load -------------------------------------------------------- */
/* Measure the SHA-256 of the artifact at `path` and return it as a lowercase
 * hex String id in the VM's string store (the bridge returns the id). The
 * caller (SLNativeGuard) matches it against the trusted digest before loading.
 * Returns an empty string if the file cannot be read. */
const char *sleela_cpu_runner_sha256(const char *path);

/* ---- Load / create (capability-gated, bounded handle) -------------------- */
/* Open the OS-native fast core for `platform` (SLPlatform ordinal) from `path`,
 * resolving `symbol` as its entry, requiring native ABI >= (abi_major,
 * abi_minor), and permitting only the capabilities set in `capability_mask`.
 * Returns a bounded library handle (>0) or <=0 on refusal/failure. */
SLCpuRunnerHandle sleela_cpu_runner_open(int platform, const char *path,
                                         const char *symbol,
                                         int abi_major, int abi_minor,
                                         uint32_t capability_mask);

/* Create a runner instance bound to a loaded core, with a wall-clock deadline
 * in milliseconds (0 = no explicit cap). Returns a bounded runner handle (>0)
 * or <=0 on failure. */
SLCpuRunnerHandle sleela_cpu_runner_create(SLCpuRunnerHandle library,
                                           int deadline_ms);

/* ---- Bind the model ------------------------------------------------------ */
/* Mirror an SLCPU's shape into the native core. Returns 0 on success, -1 on a
 * stale handle or an out-of-range configuration. */
int sleela_cpu_runner_bind_cpu(SLCpuRunnerHandle runner,
                               int cores, int width, int clock_mhz);

/* Declare a program image: load address, instruction count, entry point, and
 * the declared core MHz from the image's timing manifest (0 if none, in which
 * case the native core does NOT assume a frequency). Returns 0 or -1. */
int sleela_cpu_runner_load_program(SLCpuRunnerHandle runner, int load_addr,
                                   int length, int entry, int declared_core_mhz);

/* Write one encoded instruction word into the native word store at `addr`.
 * Out-of-range addresses are rejected (fail-closed). Returns 0 or -1. */
int sleela_cpu_runner_poke(SLCpuRunnerHandle runner, int addr, int word);

/* Read one word back from the native word store (for result inspection). */
int sleela_cpu_runner_peek(SLCpuRunnerHandle runner, int addr);

/* ---- Run (bounded, cancellable) ------------------------------------------ */
/* Execute at most `max_steps` instructions on the native fast path and return
 * the number actually executed (>=0), or -1 on a fault. The caller runs this
 * in a loop, polling a cancellation safe point between calls and checking the
 * deadline, so a long run stays cancellable and time-bounded. */
int sleela_cpu_runner_run(SLCpuRunnerHandle runner, int max_steps);

/* 1 if the native core has halted, 0 if it can still step, -1 on stale handle. */
int sleela_cpu_runner_halted(SLCpuRunnerHandle runner);

/* The exit status of the last run (0 = clean), or -1 on stale handle. */
int sleela_cpu_runner_exit_status(SLCpuRunnerHandle runner);

/* The last observable value the program printed (SYS_WRITE/OP_PRINT path). */
int sleela_cpu_runner_last_output(SLCpuRunnerHandle runner);

/* ---- Teardown (ordered, idempotent) -------------------------------------- */
/* Destroy a runner instance (quiesce + release its word store). Idempotent;
 * a stale handle is a silent no-op (fail-closed, never reused). */
void sleela_cpu_runner_destroy(SLCpuRunnerHandle runner);

/* Close a loaded native core (library handle). Idempotent. */
void sleela_cpu_runner_close(SLCpuRunnerHandle library);

/* ---- Introspection ------------------------------------------------------- */
/* The last nameful error at the native boundary (empty string if none). */
const char *sleela_cpu_runner_last_error(void);
/* Count of live runner instances / loaded cores (for leak diagnostics). */
int sleela_cpu_runner_live_runners(void);
int sleela_cpu_runner_live_libraries(void);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_CPU_RUNNER_H */
