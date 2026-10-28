/*
 * Sleelavirin(TM) native content-signature database loader - implementation.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Original implementation. Portable C11. No third-party AV code.
 */
#include "sleelavirin_compat.h"
#include "sleelavirin_sig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLV_SIG_MAX_LINE 8192u
#define SLV_SIG_MAX_NAME 128u

static int hex_nibble(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* Decode a hex byte string (spaces allowed) into 'out'. Returns byte count,
 * or -1 on a malformed field. */
static long decode_hex(const char *s, uint8_t *out, size_t out_cap)
{
    size_t count = 0;
    int hi = -1;
    while (*s) {
        int c = (unsigned char)*s++;
        if (c == ' ' || c == '\t') continue;
        {
            int nib = hex_nibble(c);
            if (nib < 0) return -1;
            if (hi < 0) {
                hi = nib;
            } else {
                if (count >= out_cap) return -1;
                out[count++] = (uint8_t)((hi << 4) | nib);
                hi = -1;
            }
        }
    }
    if (hi >= 0) return -1; /* odd number of hex digits */
    if (count == 0) return -1;
    return (long)count;
}

static int flags_have_nocase(const char *flags)
{
    const char *p = flags;
    while (p && *p) {
        const char *start = p;
        size_t len;
        while (*p && *p != ',') ++p;
        len = (size_t)(p - start);
        if (len == 6 && strncmp(start, "nocase", 6) == 0) return 1;
        if (*p == ',') ++p;
    }
    return 0;
}

typedef struct {
    sleelavirin_ac_t *ac;
    uint32_t next_id;
    sleelavirin_sig_stats_t local;
} slv_sig_build_t;

static void stats_fail(sleelavirin_sig_stats_t *st, long line, const char *msg)
{
    if (!st) return;
    st->error_line = line;
    snprintf(st->error_msg, sizeof(st->error_msg), "%s", msg);
}

/* Parse one already-trimmed line (no trailing newline). Returns 0 on success,
 * -1 on format error (message written to st). Comment/blank -> success + skip. */
static int parse_line(slv_sig_build_t *b, char *line, long lineno,
                      sleelavirin_sig_stats_t *st)
{
    char *name, *sev_s, *flags, *hex_s, *p, *endp;
    unsigned long sev;
    uint8_t bytes[SLV_SIG_MAX_LINE / 2u];
    long nbytes;
    int nocase;

    /* skip leading whitespace */
    p = line;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == '\0' || *p == '#') { ++b->local.lines_skipped; return 0; }

    /* field 1: name */
    name = p;
    endp = strchr(p, ':');
    if (!endp) { stats_fail(st, lineno, "missing ':' after name"); return -1; }
    *endp = '\0';
    if (name[0] == '\0' || strlen(name) >= SLV_SIG_MAX_NAME) {
        stats_fail(st, lineno, "name must be 1..127 chars");
        return -1;
    }

    /* field 2: severity */
    sev_s = endp + 1;
    endp = strchr(sev_s, ':');
    if (!endp) { stats_fail(st, lineno, "missing ':' after severity"); return -1; }
    *endp = '\0';
    {
        char *e2 = NULL;
        sev = strtoul(sev_s, &e2, 10);
        if (!e2 || *e2 != '\0' || sev > 255u) {
            stats_fail(st, lineno, "severity must be 0..255");
            return -1;
        }
    }

    /* field 3: flags */
    flags = endp + 1;
    endp = strchr(flags, ':');
    if (!endp) { stats_fail(st, lineno, "missing ':' after flags"); return -1; }
    *endp = '\0';
    nocase = flags_have_nocase(flags);

    /* field 4: hex bytes (rest of line) */
    hex_s = endp + 1;
    nbytes = decode_hex(hex_s, bytes, sizeof(bytes));
    if (nbytes < 0) {
        stats_fail(st, lineno, "hex-bytes field is malformed or empty");
        return -1;
    }

    if (sleelavirin_ac_add(b->ac, bytes, (size_t)nbytes, b->next_id, name,
                           (uint8_t)sev, nocase) != 0) {
        stats_fail(st, lineno, "failed to add pattern (out of memory?)");
        return -1;
    }
    ++b->next_id;
    ++b->local.patterns_loaded;
    return 0;
}

static sleelavirin_ac_t *finish(slv_sig_build_t *b, sleelavirin_sig_stats_t *st,
                                int ok)
{
    if (st) {
        st->patterns_loaded = b->local.patterns_loaded;
        st->lines_skipped = b->local.lines_skipped;
        /* error_line/error_msg already set by caller on failure */
    }
    if (!ok) {
        sleelavirin_ac_destroy(b->ac);
        return NULL;
    }
    if (sleelavirin_ac_compile(b->ac) != 0) {
        if (st) stats_fail(st, 0, "automaton compilation failed");
        sleelavirin_ac_destroy(b->ac);
        return NULL;
    }
    return b->ac;
}

sleelavirin_ac_t *sleelavirin_sig_load_text(const char *text,
                                            sleelavirin_sig_stats_t *stats)
{
    slv_sig_build_t b;
    const char *cursor;
    long lineno = 0;
    char line[SLV_SIG_MAX_LINE];

    if (stats) memset(stats, 0, sizeof(*stats));
    memset(&b, 0, sizeof(b));
    if (!text) { stats_fail(stats, 0, "null signature text"); return NULL; }
    b.ac = sleelavirin_ac_create();
    if (!b.ac) { stats_fail(stats, 0, "out of memory"); return NULL; }

    cursor = text;
    while (*cursor) {
        size_t n = 0;
        ++lineno;
        while (cursor[n] && cursor[n] != '\n') ++n;
        if (n >= sizeof(line)) {
            stats_fail(stats, lineno, "line exceeds maximum length");
            return finish(&b, stats, 0);
        }
        memcpy(line, cursor, n);
        line[n] = '\0';
        if (n > 0 && line[n - 1] == '\r') line[n - 1] = '\0';
        if (parse_line(&b, line, lineno, stats) != 0)
            return finish(&b, stats, 0);
        cursor += n;
        if (*cursor == '\n') ++cursor;
    }
    return finish(&b, stats, 1);
}

sleelavirin_ac_t *sleelavirin_sig_load_file(const char *path,
                                            sleelavirin_sig_stats_t *stats)
{
    slv_sig_build_t b;
    FILE *f;
    long lineno = 0;
    char line[SLV_SIG_MAX_LINE];

    if (stats) memset(stats, 0, sizeof(*stats));
    memset(&b, 0, sizeof(b));
    if (!path) { stats_fail(stats, 0, "null path"); return NULL; }
    f = fopen(path, "r");
    if (!f) { stats_fail(stats, 0, "cannot open signature file"); return NULL; }
    b.ac = sleelavirin_ac_create();
    if (!b.ac) { fclose(f); stats_fail(stats, 0, "out of memory"); return NULL; }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        ++lineno;
        if (len + 1u >= sizeof(line) && line[len - 1] != '\n') {
            stats_fail(stats, lineno, "line exceeds maximum length");
            fclose(f);
            return finish(&b, stats, 0);
        }
        while (len && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';
        if (parse_line(&b, line, lineno, stats) != 0) {
            fclose(f);
            return finish(&b, stats, 0);
        }
    }
    if (ferror(f)) {
        stats_fail(stats, lineno, "read error");
        fclose(f);
        return finish(&b, stats, 0);
    }
    fclose(f);
    return finish(&b, stats, 1);
}
