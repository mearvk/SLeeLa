/*
 * operating-system/include/sleela_os_ideology.h
 * SLeeLa OS ideology inspection & conversion -- the C backing of the
 * IDEOLOGY-INSPECTING / CONVERSION capability of lib/os.
 *
 * Two related concerns, kept deliberately neutral and factual:
 *
 *  (1) Jurisdictional / origin-scope inspection. Every SLeeLa artifact carries
 *      a metaOrigin scope in the Metadata Class standard (SLEELA.syntax Sec
 *      15.4): National -> a named nation -> International -> Galactic, an
 *      ordered pace-setting scale. This header classifies an origin string into
 *      that ordered scope so an OS build can be inspected for the breadth of
 *      jurisdiction it claims, and two artifacts' scopes can be compared. It is
 *      descriptive only: it records and compares the scope an artifact DECLARES;
 *      it does not judge, rank, or discriminate among nations or ideologies.
 *
 *  (2) Shape conversion. "Conversion" in lib/os is OS-shape reshaping plus the
 *      host->guest compatibility layer that lets one shape run another's
 *      executables (SLCompatLayer). This header names the runtime for any
 *      host->guest pair in the 3x3 Linux/Windows/macOS matrix, so a converter
 *      can be inspected before it is loaded.
 *
 * Pure value logic: no syscalls, no allocation.
 */
#ifndef SLEELA_OS_IDEOLOGY_H
#define SLEELA_OS_IDEOLOGY_H

#include "sleela_os_shape.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The ordered jurisdictional scope of an artifact's declared origin, matching
 * the Metadata standard's metaOrigin scale (SLEELA.syntax Sec 15.4). The
 * ordinal increases with breadth of claimed jurisdiction. */
typedef enum sleela_os_origin_scope {
    SLEELA_OS_SCOPE_UNKNOWN       = 0, /* "none"/"unknown"/unrecognised */
    SLEELA_OS_SCOPE_NATIONAL      = 1, /* "National" or a specific nation name */
    SLEELA_OS_SCOPE_INTERNATIONAL = 2, /* "International" (standards-body seated) */
    SLEELA_OS_SCOPE_GALACTIC      = 3  /* "Galactic" (scope beyond terrestrial) */
} sleela_os_origin_scope;

/* Classify a metaOrigin string into its scope. "International"/"Galactic" map
 * to their tokens; "none"/"unknown"/empty map to UNKNOWN; anything else
 * (including a literal "National" or a specific nation name) is NATIONAL --
 * a nation name is a more-specific value of National, per the standard. This is
 * a descriptive classification, never a judgement of the origin. */
sleela_os_origin_scope sleela_os_origin_classify(const char *meta_origin);

/* The canonical label for a scope ("Unknown"/"National"/"International"/
 * "Galactic"). */
const char *sleela_os_origin_scope_name(sleela_os_origin_scope scope);

/* Compare two origin scopes by breadth: negative if a<b, 0 if equal, positive
 * if a>b. Lets a build report which of its inputs claims the broader scope. */
int sleela_os_origin_scope_cmp(sleela_os_origin_scope a, sleela_os_origin_scope b);

/* 1 if the origin string is a faithful, usable value (not "none"/"unknown"/
 * empty), else 0. Mirrors the standard's honesty rule that unknown values must
 * be "none"/"unknown" and never fabricated. */
int sleela_os_origin_is_known(const char *meta_origin);

/* ---- Shape conversion (the host->guest compatibility runtime) ---------- */

/* The translation runtime that lets `host` run `guest` executables, matching
 * SLCompatLayer.runtime() for every pair in the 3x3 matrix. Returns "" (empty,
 * not NULL) when host==guest (native, no layer needed). */
const char *sleela_os_convert_runtime(sleela_os_shape host, sleela_os_shape guest);

/* 1 if running `guest` on `host` needs no conversion layer (host==guest). */
int sleela_os_convert_is_native(sleela_os_shape host, sleela_os_shape guest);

/* The number of components the host->guest layer loads (0 native; 3 when the
 * host has a filesystem interpreter -- Linux binfmt_misc; else 2). Mirrors
 * SLCompatLayer.componentCount(). */
int sleela_os_convert_component_count(sleela_os_shape host, sleela_os_shape guest);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_IDEOLOGY_H */
