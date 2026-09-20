/* ==========================================================================
 * http3_intactx.h -- SLeeLa HTTP 3.0 INTACTX host-integrity identity.
 *
 * INTACTX is a system-specific 64-bit value attached to EVERY HTTP 3.0 packet
 * (see http3_envelope.h). It answers a different question than the per-packet
 * DIGEST: the DIGEST asks "did this packet arrive intact on the wire?", while
 * INTACTX asks "is the machine that emitted this packet still the machine we
 * expect, running as it normally does?".
 *
 * It is derived from two things:
 *
 *   1. A stable OS/identity BASELINE -- OS name, architecture, hostname and
 *      user. This is fingerprinted once and PERSISTED to a baseline file so it
 *      survives process restarts and reboots. A machine that has been tampered
 *      with (re-imaged, cloned, user/host swapped) no longer matches its
 *      persisted baseline.
 *
 *   2. A "use-normality" sample -- lightweight, per-emit environmental facts
 *      that describe how the host is currently being used. Small day-to-day
 *      drift is expected; a large, sudden departure is the signal of interest.
 *
 * The emitted 64-bit value is VARIANCE-SCALED: the high bits encode how far the
 * current sample sits from the persisted baseline. A machine behaving normally
 * emits a small-magnitude INTACTX; a machine that has changed materially emits
 * a statically larger number. Receivers compare the encoded variance against a
 * threshold and RESET the exchange when it is exceeded (spec: tamper reset).
 *
 * This module is pure C with only libc + POSIX (uname, getenv, file I/O); it
 * carries no OpenSSL dependency, matching the protocol core's boundary.
 * ========================================================================== */
#ifndef HTTP3_INTACTX_H
#define HTTP3_INTACTX_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Layout of the 64-bit INTACTX value:
 *
 *   bits [63..48]  VARIANCE  -- 16-bit saturating distance from baseline.
 *                              0 == identical to baseline; larger == more drift.
 *   bits [47.. 0]  IDENTITY  -- low 48 bits of the current identity hash.
 *
 * Splitting it this way means "large variance reports a more statically large
 * number": the variance dominates the numeric magnitude of the whole value.
 */
#define HTTP3_INTACTX_VARIANCE_SHIFT 48
#define HTTP3_INTACTX_VARIANCE_MASK  0xFFFFULL
#define HTTP3_INTACTX_IDENTITY_MASK  0x0000FFFFFFFFFFFFULL

/* Default variance above which a receiver treats the host as tampered and
 * resets the exchange. Tunable per deployment. */
#define HTTP3_INTACTX_TAMPER_THRESHOLD 0x0400u /* 1024 of 65535 */

/* Default on-disk baseline location (overridable via http3_intactx_init). */
#define HTTP3_INTACTX_DEFAULT_BASELINE_PATH ".http3_intactx_baseline"

/*
 * The INTACTX engine for one host/process. Holds the persisted baseline hash
 * and the baseline-file path. Initialize once, then stamp every outgoing
 * packet with http3_intactx_compute().
 */
typedef struct {
    uint64_t baseline;          /* persisted OS/identity baseline hash        */
    char     baseline_path[256];/* where the baseline is (or will be) stored  */
    int      loaded;            /* 1 if baseline came from an existing file   */
} http3_intactx_t;

/*
 * Initialize the engine. If `baseline_path` is NULL the default path is used.
 * On first ever run the current OS/identity hash is computed and written to the
 * baseline file; on later runs the stored baseline is loaded so drift can be
 * measured against it. Returns 0 on success, -1 on error.
 */
int http3_intactx_init(http3_intactx_t *ix, const char *baseline_path);

/*
 * Compute the INTACTX value for an outgoing packet: samples current OS/identity
 * + use-normality, measures variance from the persisted baseline, and packs
 * VARIANCE|IDENTITY per the layout above. Returns the 64-bit value.
 */
uint64_t http3_intactx_compute(const http3_intactx_t *ix);

/* Extract the 16-bit variance field from an INTACTX value. */
uint16_t http3_intactx_variance(uint64_t intactx);

/*
 * Decide whether an INTACTX value signals tampering. Returns non-zero when the
 * encoded variance is at or above `threshold` (pass 0 to use the default
 * HTTP3_INTACTX_TAMPER_THRESHOLD).
 */
int http3_intactx_is_tampered(uint64_t intactx, uint16_t threshold);

/*
 * Force (re)establishment of the baseline from the current environment,
 * overwriting the baseline file. Use after a legitimate, deliberate change to
 * the host. Returns 0 on success.
 */
int http3_intactx_reset_baseline(http3_intactx_t *ix);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_INTACTX_H */
