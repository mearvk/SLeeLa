/* ===========================================================================
 * sleela_memmgr.c -- the SLeeLa Memory Manager implementation.
 *
 * Allocation layout. Every block the manager returns is preceded by a small
 * aligned header carrying the payload size (and a magic tag to catch a raw
 * pointer being handed back by mistake). free()/realloc() read the header to
 * update the running byte total exactly, so no external bookkeeping table is
 * required.
 *
 *   +-------------------+---------------------------------------------------+
 *   | SLMMHeader (pad)  | payload (what the caller sees, returned pointer)  |
 *   +-------------------+---------------------------------------------------+
 *
 * The manager is thread-safe via a single mutex and can be enabled/disabled at
 * runtime. When disabled, allocations still get a header (so free() always
 * works) but are not counted against the limit or the live-byte total.
 * ===========================================================================
 */

#include "sleela_memmgr.h"
#include "sleela_memory.h"
#include "sleela_thread.h"

#include <stdio.h>
#include <string.h>

#define SLMM_MAGIC 0x534C4D4Du /* 'SLMM' */

/* Header prefixed to every managed block. Sized to a 16-byte multiple so the
 * payload stays suitably aligned for any scalar the caller might store. */
typedef struct {
    uint32_t magic;
    uint32_t counted;   /* 1 if this block's bytes are in live_bytes         */
    size_t   size;      /* payload size in bytes                             */
    size_t   _pad;      /* keep the struct a 16-byte multiple on LP64 & ILP32 */
} SLMMHeader;

/* ---- process-wide state, guarded by g_lock -------------------------------- */
static SLThreadMutex g_lock;
static int  g_lock_ready = 0;

static int      g_enabled = 0;
static size_t   g_limit = SLMM_DEFAULT_MEMORY_BYTES;
static size_t   g_live_bytes = 0;
static size_t   g_peak_bytes = 0;
static size_t   g_live_allocs = 0;
static uint64_t g_total_allocs = 0;
static uint64_t g_total_frees = 0;
static uint64_t g_refused = 0;
static SLMMStatus g_last_status = SLMM_OK;

/* Lazily initialize the guard mutex. Not itself lock-protected; callers reach
 * it through slmm_enable() / the alloc family, which the front end drives from
 * a single thread before any worker threads exist. */
static void ensure_lock(void) {
    if (!g_lock_ready) {
        slthread_mutex_init(&g_lock);
        g_lock_ready = 1;
    }
}
static void lock(void)   { ensure_lock(); slthread_mutex_lock(&g_lock); }
static void unlock(void) { slthread_mutex_unlock(&g_lock); }

static size_t header_span(void) {
    /* Round the header up so the payload begins on a 16-byte boundary. */
    size_t h = sizeof(SLMMHeader);
    size_t a = 16u;
    return (h + (a - 1)) & ~(a - 1);
}

static SLMMHeader* header_of(void* payload) {
    return (SLMMHeader*)((unsigned char*)payload - header_span());
}
static void* payload_of(SLMMHeader* h) {
    return (unsigned char*)h + header_span();
}

/* ---- public API ----------------------------------------------------------- */

int slmm_valid_limit(size_t limit_bytes) {
    return limit_bytes >= SLMM_MIN_MEMORY_BYTES && limit_bytes <= SLMM_MAX_MEMORY_BYTES;
}

static size_t normalize_limit(size_t limit_bytes) {
    return limit_bytes == 0 ? SLMM_DEFAULT_MEMORY_BYTES : limit_bytes;
}

int slmm_enable(size_t limit_bytes) {
    size_t normalized = normalize_limit(limit_bytes);
    if (!slmm_valid_limit(normalized)) return -1;
    lock();
    g_enabled = 1;
    g_limit = normalized;
    g_last_status = SLMM_OK;
    unlock();
    return 0;
}

void slmm_disable(void) {
    lock();
    g_enabled = 0;
    unlock();
}

int slmm_is_enabled(void) {
    int e;
    lock(); e = g_enabled; unlock();
    return e;
}

size_t slmm_limit(void) {
    size_t l;
    lock(); l = g_limit; unlock();
    return l;
}

void slmm_set_limit(size_t limit_bytes) {
    size_t normalized = normalize_limit(limit_bytes);
    if (!slmm_valid_limit(normalized)) return;
    lock(); g_limit = normalized; unlock();
}

/* Allocate size payload bytes plus the header. Assumes g_lock is held. */
static void* alloc_locked(size_t size) {
    size_t span = header_span();
    size_t total;
    SLMMHeader* h;

    if (size > (size_t)-1 - span) {          /* size + header overflow        */
        g_refused++;
        g_last_status = SLMM_OVERFLOW;
        return NULL;
    }
    total = span + size;

    if (g_enabled && g_limit && (size > g_limit || g_live_bytes > g_limit - size)) {
        g_refused++;
        g_last_status = SLMM_LIMIT;
        return NULL;
    }

    h = (SLMMHeader*)slmemory_alloc(total);
    if (!h) {
        g_refused++;
        g_last_status = SLMM_OOM;
        return NULL;
    }
    h->magic = SLMM_MAGIC;
    h->size = size;
    h->counted = g_enabled ? 1u : 0u;
    h->_pad = 0;

    if (h->counted) {
        g_live_bytes += size;
        if (g_live_bytes > g_peak_bytes) g_peak_bytes = g_live_bytes;
        g_live_allocs++;
        g_total_allocs++;
    }
    g_last_status = g_enabled ? SLMM_OK : SLMM_DISABLED;
    return payload_of(h);
}

void* slmm_alloc(size_t size) {
    void* p;
    lock();
    p = alloc_locked(size);
    unlock();
    return p;
}

void* slmm_calloc(size_t count, size_t size) {
    size_t bytes;
    void* p;
    if (count && size > (size_t)-1 / count) {   /* count*size overflow        */
        lock(); g_refused++; g_last_status = SLMM_OVERFLOW; unlock();
        return NULL;
    }
    bytes = count * size;
    lock();
    p = alloc_locked(bytes);
    unlock();
    if (p) memset(p, 0, bytes);
    return p;
}

void slmm_free(void* ptr) {
    SLMMHeader* h;
    if (!ptr) return;
    h = header_of(ptr);
    if (h->magic != SLMM_MAGIC) {
        /* Not one of ours -- refuse to corrupt the heap by guessing. */
        return;
    }
    lock();
    if (h->counted) {
        if (g_live_bytes >= h->size) g_live_bytes -= h->size; else g_live_bytes = 0;
        if (g_live_allocs) g_live_allocs--;
        g_total_frees++;
    }
    h->magic = 0; /* poison so a double free is caught above */
    unlock();
    slmemory_free(h);
}

void* slmm_realloc(void* ptr, size_t size) {
    SLMMHeader* oldh;
    size_t oldsize;
    void* np;

    if (!ptr) return slmm_alloc(size);
    if (size == 0) { slmm_free(ptr); return NULL; }

    oldh = header_of(ptr);
    if (oldh->magic != SLMM_MAGIC) return NULL;
    oldsize = oldh->size;

    /* Simplicity over in-place growth: allocate new, copy, free old. This keeps
     * accounting exact and correct across an enable/disable boundary. */
    np = slmm_alloc(size);
    if (!np) return NULL;                 /* old block untouched, as realloc  */
    memcpy(np, ptr, oldsize < size ? oldsize : size);
    slmm_free(ptr);
    return np;
}

void slmm_stats(SLMMStats* out) {
    if (!out) return;
    lock();
    out->enabled      = g_enabled;
    out->limit_bytes  = g_limit;
    out->live_bytes   = g_live_bytes;
    out->peak_bytes   = g_peak_bytes;
    out->live_allocs  = g_live_allocs;
    out->total_allocs = g_total_allocs;
    out->total_frees  = g_total_frees;
    out->refused      = g_refused;
    out->last_status  = g_last_status;
    unlock();
}

SLMMStatus slmm_last_status(void) {
    SLMMStatus s;
    lock(); s = g_last_status; unlock();
    return s;
}

const char* slmm_status_name(SLMMStatus status) {
    switch (status) {
        case SLMM_OK:       return "ok";
        case SLMM_DISABLED: return "disabled";
        case SLMM_LIMIT:    return "limit-exceeded";
        case SLMM_OOM:      return "out-of-memory";
        case SLMM_OVERFLOW: return "size-overflow";
        default:            return "unknown";
    }
}

void slmm_reset(void) {
    lock();
    /* Preserve live accounting when blocks are still outstanding. Resetting
     * live_bytes/live_allocs while those headers remain alive would corrupt the
     * manager's invariants. Counters are fully reset only for an empty manager. */
    if (g_live_allocs != 0 || g_live_bytes != 0) {
        g_peak_bytes = g_live_bytes;
        g_total_allocs = 0;
        g_total_frees = 0;
        g_refused = 0;
        g_last_status = SLMM_OK;
        unlock();
        return;
    }
    g_live_bytes = 0;
    g_peak_bytes = 0;
    g_live_allocs = 0;
    g_total_allocs = 0;
    g_total_frees = 0;
    g_refused = 0;
    g_last_status = SLMM_OK;
    unlock();
}

int slmm_format_report(char* buf, size_t cap) {
    SLMMStats s;
    slmm_stats(&s);
    return snprintf(buf, cap,
        "[memory-manager] backend=%s enabled=%s limit=%zu bytes\n"
        "[memory-manager] live=%zu bytes  peak=%zu bytes  live-allocs=%zu\n"
        "[memory-manager] allocs=%llu  frees=%llu  refused=%llu  last=%s\n",
        slmemory_platform_name(),
        s.enabled ? "yes" : "no",
        s.limit_bytes,
        s.live_bytes, s.peak_bytes, s.live_allocs,
        (unsigned long long)s.total_allocs,
        (unsigned long long)s.total_frees,
        (unsigned long long)s.refused,
        slmm_status_name(s.last_status));
}
