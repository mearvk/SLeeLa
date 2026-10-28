/*
 * Sleelavirin(TM) native content-signature database loader.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Loads a Sleelavirin content-signature file into an Aho-Corasick automaton.
 * This format is specified by Sleelavirin (NOT copied from ClamAV's .ndb); it
 * is a deliberately small literal-byte subset, consistent with the Tier-3 plan
 * in antivirus/DEFINITIONS-SOURCING.md.
 *
 * File format (UTF-8 text, one record per line):
 *
 *   # comments start with '#' and blank lines are ignored
 *   <name>:<severity>:<flags>:<hex-bytes>
 *
 *   name      1..127 chars, no ':' ; the threat/family name
 *   severity  decimal 0..255
 *   flags     comma-separated, may be empty. Recognized: "nocase".
 *   hex-bytes even number of hex digits (>= 2), optional spaces between bytes.
 *             e.g. "4d5a9000" or "4d 5a 90 00"
 *
 * Example:
 *   EICAR-Test-Signature:5::58354f2150254041505b345c505a58353428505e2937434329377d
 *   Demo.Marker:3:nocase:6d616c7761726520706179
 *
 * Returns a compiled automaton ready for sleelavirin_ac_scan().
 */
#ifndef SLEELAVIRIN_SIG_H
#define SLEELAVIRIN_SIG_H

#include <stddef.h>
#include "sleelavirin_ac.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t patterns_loaded; /* number of signatures successfully added */
    size_t lines_skipped;   /* comments/blank lines skipped            */
    long   error_line;      /* 1-based line of first format error, or 0 */
    char   error_msg[160];  /* human-readable reason for the error      */
} sleelavirin_sig_stats_t;

/*
 * Load content signatures from a file into a freshly created, COMPILED
 * automaton. On success returns the automaton (caller frees with
 * sleelavirin_ac_destroy) and *stats describes the load. On any format or I/O
 * error returns NULL with stats->error_line / error_msg populated.
 * 'stats' may be NULL if the caller does not want details.
 */
sleelavirin_ac_t *sleelavirin_sig_load_file(const char *path,
                                            sleelavirin_sig_stats_t *stats);

/*
 * Same, but parse from an in-memory NUL-terminated string. Useful for tests and
 * for embedding a built-in baseline signature set.
 */
sleelavirin_ac_t *sleelavirin_sig_load_text(const char *text,
                                            sleelavirin_sig_stats_t *stats);

#ifdef __cplusplus
}
#endif
#endif /* SLEELAVIRIN_SIG_H */
