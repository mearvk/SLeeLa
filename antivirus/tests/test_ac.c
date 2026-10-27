/*
 * Sleelavirin(TM) native Aho-Corasick + signature-loader unit test.
 * Max Rupplin - MEARVK LLC - 2026.
 */
#include "sleelavirin_ac.h"
#include "sleelavirin_sig.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int count_cb(const sleelavirin_ac_match_t *m, void *ud)
{
    (void)m;
    (*(int *)ud)++;
    return 0;
}

struct rec {
    int hits;
    char last[64];
    uint32_t last_id;
    uint64_t last_end;
};

static int rec_cb(const sleelavirin_ac_match_t *m, void *ud)
{
    struct rec *r = (struct rec *)ud;
    r->hits++;
    snprintf(r->last, sizeof(r->last), "%s", m->name ? m->name : "");
    r->last_id = m->pattern_id;
    r->last_end = m->end_offset;
    return 0;
}

int main(void)
{
    /* Classic Aho-Corasick overlap test: {he, she, his, hers} in "ushers".
     * Expected: she(1..3), he(2..3), hers(2..5) -> 3 matches. */
    sleelavirin_ac_t *ac = sleelavirin_ac_create();
    int hits = 0;
    long n;
    struct rec r;
    sleelavirin_sig_stats_t st;
    static const uint8_t payload[] = {
        0x4d, 0x5a, 0x90, 0x00, 'X', 'M', 'A', 'L', 'W', 'A', 'R', 'E'};
    const char *db =
        "# demo sigs\n"
        "Demo.MZ:7::4d5a9000\n"
        "Demo.Word:3:nocase:6d616c77617265\n" /* "malware" */
        "\n";

    assert(ac != NULL);
    assert(sleelavirin_ac_add(ac, (const uint8_t *)"he", 2, 1, "he", 1, 0) == 0);
    assert(sleelavirin_ac_add(ac, (const uint8_t *)"she", 3, 2, "she", 1, 0) == 0);
    assert(sleelavirin_ac_add(ac, (const uint8_t *)"his", 3, 3, "his", 1, 0) == 0);
    assert(sleelavirin_ac_add(ac, (const uint8_t *)"hers", 4, 4, "hers", 1, 0) == 0);
    assert(sleelavirin_ac_compile(ac) == 0);
    assert(sleelavirin_ac_pattern_count(ac) == 4);
    assert(sleelavirin_ac_max_pattern_len(ac) == 4);
    n = sleelavirin_ac_scan(ac, (const uint8_t *)"ushers", 6, count_cb, &hits);
    assert(n == 3 && hits == 3);
    sleelavirin_ac_destroy(ac);

    /* Case-insensitive match; end offset points just past the match. */
    ac = sleelavirin_ac_create();
    assert(ac != NULL);
    assert(sleelavirin_ac_add(ac, (const uint8_t *)"MAL", 3, 10, "mal", 5, 1) == 0);
    assert(sleelavirin_ac_compile(ac) == 0);
    memset(&r, 0, sizeof(r));
    n = sleelavirin_ac_scan(ac, (const uint8_t *)"a mAl here", 10, rec_cb, &r);
    assert(n == 1 && strcmp(r.last, "mal") == 0 && r.last_end == 5);
    sleelavirin_ac_destroy(ac);

    /* Signature loader from in-memory text. */
    ac = sleelavirin_sig_load_text(db, &st);
    assert(ac != NULL);
    assert(st.patterns_loaded == 2);
    memset(&r, 0, sizeof(r));
    n = sleelavirin_ac_scan(ac, payload, sizeof(payload), rec_cb, &r);
    assert(n == 2);
    sleelavirin_ac_destroy(ac);

    /* Malformed record is rejected with a 1-based line number. */
    ac = sleelavirin_sig_load_text("Bad.Odd:1::4d5\n", &st);
    assert(ac == NULL && st.error_line == 1);

    printf("Sleelavirin AC/signature self-test: PASS\n");
    return 0;
}
