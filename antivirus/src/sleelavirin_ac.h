/*
 * Sleelavirin(TM) native Aho-Corasick content matcher.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * A from-scratch, portable C11 implementation of the Aho-Corasick multi-pattern
 * string-matching automaton. It finds every occurrence of any of many literal
 * byte patterns in a buffer in a single pass, independent of the pattern count.
 *
 * This is original Sleelavirin code. It is NOT derived from, and copies no code
 * from, any GPL antivirus engine. The algorithm itself is public computer
 * science (Aho & Corasick, 1975); the implementation here is our own.
 *
 * See antivirus/SCANNING-CONCEPTS-GLOSSARY.md (Aho-Corasick, trie, failure
 * links) and antivirus/CLAMAV-REFERENCE-ANALYSIS.md for the design rationale.
 *
 * Capability: literal byte patterns (the "content signature" that whole-file
 * hashing cannot catch after trivial mutation). Wildcards / anchored byte ranges
 * are intentionally out of scope for this first native matcher.
 */
#ifndef SLEELAVIRIN_AC_H
#define SLEELAVIRIN_AC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque compiled automaton ("the content database in memory"). */
typedef struct sleelavirin_ac sleelavirin_ac_t;

/* One match reported during a scan. */
typedef struct {
    uint32_t pattern_id;   /* caller-supplied id of the matched pattern        */
    uint64_t end_offset;   /* offset of the byte AFTER the match in the buffer */
    uint32_t length;       /* length of the matched pattern in bytes           */
    const char *name;      /* pattern/threat name (owned by the automaton)     */
    uint8_t severity;      /* caller-supplied severity 0..255                  */
} sleelavirin_ac_match_t;

/*
 * Callback invoked once per match. Return 0 to continue scanning, non-zero to
 * stop early (the scan returns the number of matches found so far). The
 * user_data pointer is passed through unchanged.
 */
typedef int (*sleelavirin_ac_cb)(const sleelavirin_ac_match_t *match,
                                 void *user_data);

/* Create an empty, editable automaton. Returns NULL on allocation failure. */
sleelavirin_ac_t *sleelavirin_ac_create(void);

/* Destroy an automaton created with sleelavirin_ac_create(). Safe on NULL. */
void sleelavirin_ac_destroy(sleelavirin_ac_t *ac);

/*
 * Add one literal byte pattern before compilation.
 *   pattern/length : the bytes to match (length must be > 0).
 *   pattern_id     : caller id echoed back on match.
 *   name           : optional threat name (copied); may be NULL.
 *   severity       : caller severity echoed back on match.
 *   nocase         : if non-zero, match ASCII letters case-insensitively.
 * Returns 0 on success, -1 on error, -2 if the automaton was already compiled.
 */
int sleelavirin_ac_add(sleelavirin_ac_t *ac,
                       const uint8_t *pattern, size_t length,
                       uint32_t pattern_id, const char *name,
                       uint8_t severity, int nocase);

/*
 * Finalize the automaton (build goto/failure/output links). Must be called once
 * after all sleelavirin_ac_add() calls and before scanning. Returns 0 on
 * success, -1 on error. Idempotent: calling again is a no-op that returns 0.
 */
int sleelavirin_ac_compile(sleelavirin_ac_t *ac);

/*
 * Scan a buffer. Invokes cb once per match (in increasing end-offset order).
 * Returns the number of matches reported, or -1 on error (e.g. not compiled).
 * The buffer is not modified; absence of matches is NOT proof of safety.
 */
long sleelavirin_ac_scan(const sleelavirin_ac_t *ac,
                         const uint8_t *buffer, size_t length,
                         sleelavirin_ac_cb cb, void *user_data);

/* Number of patterns added. */
size_t sleelavirin_ac_pattern_count(const sleelavirin_ac_t *ac);

/* Length (in bytes) of the longest pattern added (0 if none). */
size_t sleelavirin_ac_max_pattern_len(const sleelavirin_ac_t *ac);

#ifdef __cplusplus
}
#endif
#endif /* SLEELAVIRIN_AC_H */
