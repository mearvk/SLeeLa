#ifndef SLEELA_MEMMGR_H
#define SLEELA_MEMMGR_H

/* ===========================================================================
 * sleela_memmgr -- the SLeeLa Memory Manager.
 *
 * A thin, OS-neutral memory manager that sits on top of the OS allocator
 * abstraction (sleela_memory.*) and adds *accounting* and *limits*:
 *
 *   * every allocation carries a small header recording its size, so free()
 *     and realloc() can update the running total exactly (no per-pointer table
 *     is needed);
 *   * live bytes, peak bytes, and live/total allocation counts are tracked;
 *   * an optional hard byte limit fails allocations closed (returns NULL)
 *     instead of letting an untrusted native workload exhaust the host;
 *   * the whole thing is thread-safe (a single manager mutex) so it can back
 *     the VM and a spawned native at the same time.
 *
 * The manager is *opt-in*. When disabled (the default), the wrappers forward
 * straight to sleela_memory with no accounting overhead, so linking the
 * manager in costs nothing until a --memory-manager flag (or an on-need
 * native launch) enables it.
 *
 * This is deliberately independent of the mark-sweep GC (runtime/garbage_-
 * collector.*) and the per-class quota supervisor (runtime/security_-
 * supervisor.*): those track *managed objects* and *class/role* budgets,
 * whereas this manager accounts for *raw process memory* -- exactly the axis
 * that matters when SLeeLa is asked to run an arbitrary native executable.
 * ===========================================================================
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLMM_MIN_MEMORY_BYTES (256ull * 1024ull * 1024ull) /* 256 MiB */
#define SLMM_MAX_MEMORY_BYTES (2048ull * 1024ull * 1024ull) /* 2 GiB */
#define SLMM_DEFAULT_MEMORY_BYTES SLMM_MAX_MEMORY_BYTES

/* Why the manager refused an allocation (readable via slmm_last_status). */
typedef enum {
    SLMM_OK = 0,          /* last allocation succeeded (or none yet)        */
    SLMM_DISABLED = 1,    /* manager disabled; call forwarded untracked     */
    SLMM_LIMIT = 2,       /* would exceed the configured hard byte limit    */
    SLMM_OOM = 3,         /* the underlying OS allocator returned NULL      */
    SLMM_OVERFLOW = 4     /* requested size + header overflowed size_t      */
} SLMMStatus;

typedef struct {
    int      enabled;         /* nonzero once slmm_enable() has run          */
    size_t   limit_bytes;     /* hard cap on live bytes, 0 == unlimited      */
    size_t   live_bytes;      /* bytes currently handed out (payload only)   */
    size_t   peak_bytes;      /* high-water mark of live_bytes               */
    size_t   live_allocs;     /* allocations not yet freed                   */
    uint64_t total_allocs;    /* cumulative successful allocations           */
    uint64_t total_frees;     /* cumulative frees                            */
    uint64_t refused;         /* allocations refused (limit/oom/overflow)    */
    SLMMStatus last_status;   /* result of the most recent alloc attempt     */
} SLMMStats;

/* Enable the process-wide manager with a hard limit on live bytes.
 * The configured limit is constrained to 256 MiB..2 GiB. A zero value selects
 * the default 2 GiB limit. Safe to call again to raise/lower the limit; resetting
 * counters is done by slmm_reset(). Returns 0 on success. */
int  slmm_enable(size_t limit_bytes);

/* Disable the manager. Memory already handed out stays valid and is still
 * freed correctly (the header is always present), but new allocations are no
 * longer counted or limited. */
void slmm_disable(void);

int    slmm_is_enabled(void);
size_t slmm_limit(void);
void   slmm_set_limit(size_t limit_bytes);
int    slmm_valid_limit(size_t limit_bytes);

/* Accounted allocation family. When the manager is disabled these still return
 * usable memory (so callers never branch), just without accounting. Every
 * pointer they return MUST be released with slmm_free / resized with
 * slmm_realloc -- never the raw C free(), because of the size header. */
void* slmm_alloc(size_t size);
void* slmm_calloc(size_t count, size_t size);
void* slmm_realloc(void* ptr, size_t size);
void  slmm_free(void* ptr);

/* Introspection. */
void       slmm_stats(SLMMStats* out);
SLMMStatus slmm_last_status(void);
const char* slmm_status_name(SLMMStatus status);
void       slmm_reset(void);   /* zero the counters (keeps enabled/limit)   */

/* Render a one-block human-readable report to a caller buffer; returns the
 * number of bytes that would be written (like snprintf). */
int slmm_format_report(char* buf, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_MEMMGR_H */
