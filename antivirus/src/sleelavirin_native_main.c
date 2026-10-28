/*
 * Sleelavirin(TM) native scanner CLI.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Two independent, composable detection passes over each target file:
 *   --database FILE      whole-file SHA-256 hash index (exact match)
 *   --content-db FILE    Sleelavirin content-signature file (byte-pattern match
 *                        via the native Aho-Corasick engine)
 * At least one of the two must be supplied. A NO_MATCH / CLEAN result is NEVER
 * proof that a file is safe.
 */
#include "sleelavirin_compat.h"
#include "sleelavirin_native.h"
#include "sleelavirin_ac.h"
#include "sleelavirin_sig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr,
        "Usage: %s [--database FILE] [--content-db FILE] <file> [file ...]\n",
        program);
    fprintf(stderr,
        "  --database FILE     whole-file SHA-256 hash index (exact match)\n"
        "  --content-db FILE   Sleelavirin content signatures (byte patterns)\n"
        "  --help              show this help\n");
    fprintf(stderr,
        "At least one of --database / --content-db is required.\n"
        "A NO_MATCH / CLEAN result is not proof that a file is safe.\n");
}

/* Read an entire file into a malloc'd buffer. Returns 0 on success. */
static int read_file(const char *path, uint8_t **out, size_t *out_len)
{
    FILE *f = fopen(path, "rb");
    uint64_t end = 0;
    uint8_t *buf;
    size_t n;
    *out = NULL; *out_len = 0;
    if (!f) return -1;
    if (slv_file_size(f, &end) != 0) { fclose(f); return -1; }
    /* Content scanning loads the whole file into memory; refuse what will not
     * fit in size_t (e.g. a >4 GiB file on a 32-bit XP build). */
    if (end > (uint64_t)(size_t)-1) { fclose(f); return -1; }
    buf = (uint8_t *)malloc((size_t)end ? (size_t)end : 1u);
    if (!buf) { fclose(f); return -1; }
    n = fread(buf, 1, (size_t)end, f);
    if (ferror(f)) { free(buf); fclose(f); return -1; }
    fclose(f);
    *out = buf; *out_len = n;
    return 0;
}

struct content_hits {
    unsigned long count;
    char first_name[256];
};

static int content_cb(const sleelavirin_ac_match_t *m, void *ud)
{
    struct content_hits *h = (struct content_hits *)ud;
    if (h->count == 0)
        snprintf(h->first_name, sizeof(h->first_name), "%s",
                 m->name ? m->name : "");
    ++h->count;
    return 0; /* collect all matches */
}

int main(int argc, char **argv)
{
    const char *database = NULL;
    const char *content_db = NULL;
    sleelavirin_ac_t *ac = NULL;
    int i = 1, status = 0, scanned = 0;

    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "--help") == 0) { usage(argv[0]); return 0; }
        if (strcmp(argv[i], "--database") == 0) {
            if (i + 1 >= argc) { usage(argv[0]); return 2; }
            database = argv[i + 1]; i += 2; continue;
        }
        if (strcmp(argv[i], "--content-db") == 0) {
            if (i + 1 >= argc) { usage(argv[0]); return 2; }
            content_db = argv[i + 1]; i += 2; continue;
        }
        usage(argv[0]); return 2;
    }

    if ((!database && !content_db) || i >= argc) { usage(argv[0]); return 2; }

    if (content_db) {
        sleelavirin_sig_stats_t st;
        ac = sleelavirin_sig_load_file(content_db, &st);
        if (!ac) {
            fprintf(stderr, "ERROR cannot load content database %s", content_db);
            if (st.error_line) fprintf(stderr, " (line %ld: %s)",
                                       st.error_line, st.error_msg);
            fprintf(stderr, "\n");
            return 2;
        }
        fprintf(stderr, "Loaded %zu content signatures from %s\n",
                st.patterns_loaded, content_db);
    }

    for (; i < argc; ++i) {
        const char *path = argv[i];
        int detected = 0;
        char hash_name[512];

        ++scanned;

        if (database) {
            int r = sleelavirin_native_scan_file(path, database,
                                                 hash_name, sizeof(hash_name));
            if (r < 0) {
                fprintf(stderr,
                    "ERROR %s (file or hash database unavailable/unreadable)\n",
                    path);
                if (status < 2) status = 2;
                continue;
            }
            if (r == 1) {
                /* Preserve the original hash-match line format:
                 * "DETECTED <path>[ : <name>]" for backward compatibility. */
                printf("DETECTED %s%s%s\n", path,
                       hash_name[0] ? " : " : "", hash_name);
                detected = 1;
            }
        }

        if (ac) {
            uint8_t *buf = NULL; size_t len = 0;
            if (read_file(path, &buf, &len) != 0) {
                fprintf(stderr, "ERROR %s (file unreadable)\n", path);
                if (status < 2) status = 2;
                continue;
            }
            {
                struct content_hits h; long n;
                memset(&h, 0, sizeof(h));
                n = sleelavirin_ac_scan(ac, buf, len, content_cb, &h);
                free(buf);
                if (n < 0) {
                    fprintf(stderr, "ERROR %s (content scan failed)\n", path);
                    if (status < 2) status = 2;
                    continue;
                }
                if (h.count > 0) {
                    printf("DETECTED %s : content:%s (%lu hit%s)\n", path,
                           h.first_name[0] ? h.first_name : "pattern",
                           h.count, h.count == 1 ? "" : "s");
                    detected = 1;
                }
            }
        }

        if (detected) { if (status == 0) status = 1; }
        else printf("NO_MATCH %s\n", path);
    }

    sleelavirin_ac_destroy(ac);
    return scanned ? status : 2;
}
