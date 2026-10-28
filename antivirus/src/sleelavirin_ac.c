/*
 * Sleelavirin(TM) native Aho-Corasick content matcher - implementation.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Original implementation. No code copied from any third-party AV engine.
 *
 * Design (see antivirus/SCANNING-CONCEPTS-GLOSSARY.md):
 *   1. Insert every pattern into a trie (one child slot per byte value, 0..255).
 *   2. BFS from the root to build failure links (longest proper suffix that is
 *      also a trie node) and to chain each node to the nearest failure-ancestor
 *      that itself carries outputs ("output link").
 *   3. Scan: for each input byte follow goto edges, using failure links on a
 *      miss, and emit every pattern terminating at the current node.
 *
 * Case handling. Global lowercasing of the whole input is WRONG when some
 * patterns are case-sensitive and contain ASCII letters (their bytes would be
 * folded too). We therefore keep TWO independent single-case automata:
 *   - a case-sensitive trie scanned over the raw buffer, and
 *   - a case-insensitive trie (patterns stored lowercased) scanned over a
 *     lazily-built lowercased copy of the buffer.
 * The public sleelavirin_ac_t owns both; each pattern goes into exactly one.
 */
#include "sleelavirin_ac.h"

#include <stdlib.h>
#include <string.h>

#define SLV_AC_ALPHABET 256u

typedef struct slv_ac_output {
    uint32_t pattern_id;
    uint32_t length;
    char *name; /* owned copy, or NULL */
    uint8_t severity;
    struct slv_ac_output *next;
} slv_ac_output_t;

typedef struct slv_ac_node {
    int32_t children[SLV_AC_ALPHABET]; /* node index, or -1 */
    int32_t fail;
    slv_ac_output_t *outputs;
    int32_t out_link; /* nearest failure-ancestor with outputs, or -1 */
} slv_ac_node_t;

/* One single-case automaton. */
typedef struct {
    slv_ac_node_t *nodes;
    size_t node_count;
    size_t node_capacity;
    size_t pattern_count;
} slv_ac_trie_t;

struct sleelavirin_ac {
    slv_ac_trie_t cs; /* case-sensitive  */
    slv_ac_trie_t ci; /* case-insensitive */
    size_t pattern_count;
    size_t max_pattern_len;
    int compiled;
};

static uint8_t lower_ascii(uint8_t c)
{
    return (c >= 'A' && c <= 'Z') ? (uint8_t)(c + ('a' - 'A')) : c;
}

static int32_t trie_node_alloc(slv_ac_trie_t *t)
{
    size_t i;
    if (t->node_count == t->node_capacity) {
        size_t newcap = t->node_capacity ? t->node_capacity * 2u : 64u;
        slv_ac_node_t *grown;
        if (newcap > (size_t)INT32_MAX) return -1;
        grown = (slv_ac_node_t *)realloc(t->nodes, newcap * sizeof(*grown));
        if (!grown) return -1;
        t->nodes = grown;
        t->node_capacity = newcap;
    }
    {
        slv_ac_node_t *n = &t->nodes[t->node_count];
        for (i = 0; i < SLV_AC_ALPHABET; ++i) n->children[i] = -1;
        n->fail = 0;
        n->outputs = NULL;
        n->out_link = -1;
    }
    return (int32_t)t->node_count++;
}

static int trie_init(slv_ac_trie_t *t)
{
    memset(t, 0, sizeof(*t));
    return (trie_node_alloc(t) == 0) ? 0 : -1; /* root is node 0 */
}

static void trie_free(slv_ac_trie_t *t)
{
    size_t i;
    for (i = 0; i < t->node_count; ++i) {
        slv_ac_output_t *o = t->nodes[i].outputs;
        while (o) {
            slv_ac_output_t *next = o->next;
            free(o->name);
            free(o);
            o = next;
        }
    }
    free(t->nodes);
    t->nodes = NULL;
    t->node_count = t->node_capacity = t->pattern_count = 0;
}

static int trie_add(slv_ac_trie_t *t, const uint8_t *pattern, size_t length,
                    int lower, uint32_t pattern_id, const char *name,
                    uint8_t severity)
{
    int32_t cur = 0;
    size_t i;
    slv_ac_output_t *out;
    for (i = 0; i < length; ++i) {
        uint8_t b = lower ? lower_ascii(pattern[i]) : pattern[i];
        int32_t next = t->nodes[cur].children[b];
        if (next < 0) {
            next = trie_node_alloc(t);
            if (next < 0) return -1;
            /* realloc may have moved the array; re-read cur each iteration. */
            t->nodes[cur].children[b] = next;
        }
        cur = next;
    }
    out = (slv_ac_output_t *)calloc(1, sizeof(*out));
    if (!out) return -1;
    out->pattern_id = pattern_id;
    out->length = (uint32_t)length;
    out->severity = severity;
    if (name) {
        size_t n = strlen(name);
        out->name = (char *)malloc(n + 1u);
        if (!out->name) { free(out); return -1; }
        memcpy(out->name, name, n + 1u);
    }
    out->next = t->nodes[cur].outputs;
    t->nodes[cur].outputs = out;
    ++t->pattern_count;
    return 0;
}

static int trie_compile(slv_ac_trie_t *t)
{
    int32_t *queue;
    size_t head = 0, tail = 0;
    uint32_t c;
    if (t->node_count == 0) return 0;
    queue = (int32_t *)malloc(t->node_count * sizeof(*queue));
    if (!queue) return -1;

    for (c = 0; c < SLV_AC_ALPHABET; ++c) {
        int32_t child = t->nodes[0].children[c];
        if (child >= 0) {
            t->nodes[child].fail = 0;
            queue[tail++] = child;
        }
    }
    while (head < tail) {
        int32_t u = queue[head++];
        for (c = 0; c < SLV_AC_ALPHABET; ++c) {
            int32_t v = t->nodes[u].children[c];
            int32_t f;
            if (v < 0) continue;
            /* failure link: walk u's failure chain to a node with edge c. */
            f = t->nodes[u].fail;
            while (f != 0 && t->nodes[f].children[c] < 0)
                f = t->nodes[f].fail;
            if (t->nodes[f].children[c] >= 0 && t->nodes[f].children[c] != v)
                f = t->nodes[f].children[c];
            else
                f = 0;
            t->nodes[v].fail = f;
            /* output link: nearest failure-ancestor carrying outputs. */
            t->nodes[v].out_link =
                (t->nodes[f].outputs != NULL) ? f : t->nodes[f].out_link;
            queue[tail++] = v;
        }
    }
    free(queue);
    return 0;
}

static long trie_scan(const slv_ac_trie_t *t, const uint8_t *buffer,
                      size_t length, sleelavirin_ac_cb cb, void *user_data,
                      int *stop)
{
    int32_t cur = 0;
    size_t i;
    long matches = 0;
    if (t->node_count == 0) return 0;
    for (i = 0; i < length; ++i) {
        uint8_t b = buffer[i];
        int32_t emit;
        while (cur != 0 && t->nodes[cur].children[b] < 0)
            cur = t->nodes[cur].fail;
        if (t->nodes[cur].children[b] >= 0)
            cur = t->nodes[cur].children[b];
        emit = cur;
        while (emit >= 0) {
            slv_ac_output_t *o = t->nodes[emit].outputs;
            while (o) {
                sleelavirin_ac_match_t m;
                m.pattern_id = o->pattern_id;
                m.end_offset = (uint64_t)i + 1u;
                m.length = o->length;
                m.name = o->name;
                m.severity = o->severity;
                ++matches;
                if (cb(&m, user_data) != 0) { *stop = 1; return matches; }
                o = o->next;
            }
            emit = t->nodes[emit].out_link;
        }
    }
    return matches;
}

sleelavirin_ac_t *sleelavirin_ac_create(void)
{
    sleelavirin_ac_t *ac = (sleelavirin_ac_t *)calloc(1, sizeof(*ac));
    if (!ac) return NULL;
    if (trie_init(&ac->cs) != 0 || trie_init(&ac->ci) != 0) {
        trie_free(&ac->cs);
        trie_free(&ac->ci);
        free(ac);
        return NULL;
    }
    return ac;
}

void sleelavirin_ac_destroy(sleelavirin_ac_t *ac)
{
    if (!ac) return;
    trie_free(&ac->cs);
    trie_free(&ac->ci);
    free(ac);
}

int sleelavirin_ac_add(sleelavirin_ac_t *ac,
                       const uint8_t *pattern, size_t length,
                       uint32_t pattern_id, const char *name,
                       uint8_t severity, int nocase)
{
    int rc;
    if (!ac || !pattern || length == 0) return -1;
    if (ac->compiled) return -2;
    if (length > (size_t)UINT32_MAX) return -1;
    rc = nocase
             ? trie_add(&ac->ci, pattern, length, 1, pattern_id, name, severity)
             : trie_add(&ac->cs, pattern, length, 0, pattern_id, name, severity);
    if (rc != 0) return -1;
    ++ac->pattern_count;
    if (length > ac->max_pattern_len) ac->max_pattern_len = length;
    return 0;
}

int sleelavirin_ac_compile(sleelavirin_ac_t *ac)
{
    if (!ac) return -1;
    if (ac->compiled) return 0;
    if (trie_compile(&ac->cs) != 0) return -1;
    if (trie_compile(&ac->ci) != 0) return -1;
    ac->compiled = 1;
    return 0;
}

long sleelavirin_ac_scan(const sleelavirin_ac_t *ac,
                         const uint8_t *buffer, size_t length,
                         sleelavirin_ac_cb cb, void *user_data)
{
    long total = 0, n;
    int stop = 0;
    if (!ac || !ac->compiled || !cb) return -1;
    if (!buffer && length) return -1;

    /* Case-sensitive pass over raw bytes. */
    n = trie_scan(&ac->cs, buffer, length, cb, user_data, &stop);
    if (n < 0) return -1;
    total += n;
    if (stop) return total;

    /* Case-insensitive pass over a lowercased copy (only if such patterns
     * exist, to avoid an allocation and second pass otherwise). */
    if (ac->ci.pattern_count > 0 && length > 0) {
        uint8_t *lowered = (uint8_t *)malloc(length);
        size_t i;
        if (!lowered) return -1;
        for (i = 0; i < length; ++i) lowered[i] = lower_ascii(buffer[i]);
        n = trie_scan(&ac->ci, lowered, length, cb, user_data, &stop);
        free(lowered);
        if (n < 0) return -1;
        total += n;
    }
    return total;
}

size_t sleelavirin_ac_pattern_count(const sleelavirin_ac_t *ac)
{
    return ac ? ac->pattern_count : 0u;
}

size_t sleelavirin_ac_max_pattern_len(const sleelavirin_ac_t *ac)
{
    return ac ? ac->max_pattern_len : 0u;
}
