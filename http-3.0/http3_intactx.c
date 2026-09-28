/* ==========================================================================
 * http3_intactx.c -- implementation of the INTACTX host-integrity identity.
 *
 * The baseline is a 64-bit FNV-1a hash over stable OS/identity facts (OS name,
 * release, machine architecture, hostname, user). It is persisted so a later
 * run can measure how far the current environment has drifted. Each emit also
 * folds in a "use-normality" sample; the Hamming distance between the current
 * identity hash and the persisted baseline becomes the 16-bit variance that
 * dominates the emitted value's magnitude.
 * ========================================================================== */
/* Request POSIX declarations (gethostname, etc.) under strict -std=c11. */
#ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE 1
#endif
#ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200112L
#endif

#include "http3_intactx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#  include <unistd.h>
#  include <sys/utsname.h>
#  define HTTP3_INTACTX_HAVE_POSIX 1
#endif

/* ---- 64-bit FNV-1a --------------------------------------------------------- */

#define FNV64_OFFSET 14695981039346656037ULL
#define FNV64_PRIME  1099511628211ULL

static uint64_t fnv1a(uint64_t h, const void *data, size_t len)
{
    const unsigned char *p = (const unsigned char *)data;
    size_t i;
    for (i = 0; i < len; ++i) {
        h ^= (uint64_t)p[i];
        h *= FNV64_PRIME;
    }
    return h;
}

static uint64_t fnv1a_str(uint64_t h, const char *s)
{
    if (s == NULL) {
        s = "";
    }
    return fnv1a(h, s, strlen(s));
}

/* ---- Environment sampling -------------------------------------------------- */

/*
 * Stable OS/identity hash. These facts change only when the machine is
 * re-imaged, cloned, renamed, or run as a different user -- i.e. the kinds of
 * change that "the computer has been tampered with" is meant to catch.
 */
static uint64_t sample_identity(void)
{
    uint64_t h = FNV64_OFFSET;
#ifdef HTTP3_INTACTX_HAVE_POSIX
    struct utsname u;
    char host[256];
    if (uname(&u) == 0) {
        h = fnv1a_str(h, u.sysname);
        h = fnv1a_str(h, u.release);
        h = fnv1a_str(h, u.version);
        h = fnv1a_str(h, u.machine);
        h = fnv1a_str(h, u.nodename);
    }
    if (gethostname(host, sizeof(host)) == 0) {
        host[sizeof(host) - 1] = '\0';
        h = fnv1a_str(h, host);
    }
    h = fnv1a_str(h, getenv("USER"));
    h = fnv1a_str(h, getenv("LOGNAME"));
    h = fnv1a_str(h, getenv("HOME"));
#else
    /* Portable fallback: the compiler/target triple is the best stable fact we
     * have without POSIX. Still deterministic per build/host. */
    h = fnv1a_str(h, getenv("USERNAME"));
    h = fnv1a_str(h, getenv("COMPUTERNAME"));
    h = fnv1a_str(h, getenv("OS"));
    h = fnv1a_str(h, getenv("PROCESSOR_ARCHITECTURE"));
#endif
    return h;
}

/*
 * Use-normality sample: lightweight facts about how the host is being used
 * right now. Expected to drift a little between emits; a large jump is the
 * anomaly INTACTX exists to surface. Folded on top of the identity hash.
 */
static uint64_t sample_use_normality(uint64_t identity)
{
    uint64_t h = identity;
    h = fnv1a_str(h, getenv("SHELL"));
    h = fnv1a_str(h, getenv("PWD"));
    h = fnv1a_str(h, getenv("TERM"));
    h = fnv1a_str(h, getenv("LANG"));
    return h;
}

/* Popcount without relying on a builtin (portable). */
static unsigned popcount64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        x &= (x - 1);
        ++c;
    }
    return c;
}

/*
 * Variance = bit-distance between the current sample and the baseline, scaled
 * so that a larger structural change yields a larger 16-bit number. A 64-bit
 * Hamming distance is 0..64; we scale it up so the field spans its range and a
 * material change clears the tamper threshold.
 */
static uint16_t variance_from(uint64_t current, uint64_t baseline)
{
    unsigned dist = popcount64(current ^ baseline); /* 0..64 */
    uint32_t scaled = (uint32_t)dist * 1024u;       /* 0..65536 */
    if (scaled > HTTP3_INTACTX_VARIANCE_MASK) {
        scaled = (uint32_t)HTTP3_INTACTX_VARIANCE_MASK;
    }
    return (uint16_t)scaled;
}

/* ---- Baseline persistence -------------------------------------------------- */

static int baseline_write(const char *path, uint64_t baseline)
{
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return -1;
    }
    /* Store as text for portability + easy inspection. */
    if (fprintf(f, "%020llu\n", (unsigned long long)baseline) < 0) {
        fclose(f);
        return -1;
    }
    return fclose(f) == 0 ? 0 : -1;
}

static int baseline_read(const char *path, uint64_t *out)
{
    unsigned long long v = 0;
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    if (fscanf(f, "%llu", &v) != 1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = (uint64_t)v;
    return 0;
}

/* ---- Public API ------------------------------------------------------------ */

int http3_intactx_init(http3_intactx_t *ix, const char *baseline_path)
{
    if (ix == NULL) {
        return -1;
    }
    memset(ix, 0, sizeof(*ix));
    if (baseline_path == NULL) {
        baseline_path = HTTP3_INTACTX_DEFAULT_BASELINE_PATH;
    }
    strncpy(ix->baseline_path, baseline_path, sizeof(ix->baseline_path) - 1);
    ix->baseline_path[sizeof(ix->baseline_path) - 1] = '\0';

    if (baseline_read(ix->baseline_path, &ix->baseline) == 0) {
        ix->loaded = 1;
        return 0;
    }
    /* First ever run: establish and persist the baseline from this host. The
     * baseline captures the SAME shape as an emit sample (identity folded with
     * use-normality) so a healthy, unchanged host measures near-zero variance
     * and only genuine drift raises the number. */
    ix->baseline = sample_use_normality(sample_identity());
    ix->loaded = 0;
    return baseline_write(ix->baseline_path, ix->baseline);
}

uint64_t http3_intactx_compute(const http3_intactx_t *ix)
{
    uint64_t identity, sample;
    uint16_t variance;
    if (ix == NULL) {
        return 0;
    }
    identity = sample_identity();
    sample = sample_use_normality(identity);
    variance = variance_from(sample, ix->baseline);
    return ((uint64_t)variance << HTTP3_INTACTX_VARIANCE_SHIFT) |
           (identity & HTTP3_INTACTX_IDENTITY_MASK);
}

uint16_t http3_intactx_variance(uint64_t intactx)
{
    return (uint16_t)((intactx >> HTTP3_INTACTX_VARIANCE_SHIFT) &
                      HTTP3_INTACTX_VARIANCE_MASK);
}

int http3_intactx_is_tampered(uint64_t intactx, uint16_t threshold)
{
    if (threshold == 0u) {
        threshold = HTTP3_INTACTX_TAMPER_THRESHOLD;
    }
    return http3_intactx_variance(intactx) >= threshold ? 1 : 0;
}

int http3_intactx_reset_baseline(http3_intactx_t *ix)
{
    if (ix == NULL) {
        return -1;
    }
    ix->baseline = sample_use_normality(sample_identity());
    ix->loaded = 1;
    return baseline_write(ix->baseline_path, ix->baseline);
}
