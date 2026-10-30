#include "help_desk.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sl_help_desk {
    sl_contact *contacts;
    size_t count;
    size_t capacity;
    size_t company_size;
};

/* ---- small helpers ----------------------------------------------------- */
static bool copy_bounded(char *dst, size_t cap, const char *src, bool empty_ok) {
    size_t n;
    if (!dst || cap == 0u) return false;
    if (!src) { if (!empty_ok) return false; dst[0] = '\0'; return true; }
    n = strlen(src);
    if ((!empty_ok && n == 0u) || n >= cap) return false;
    memcpy(dst, src, n + 1u);
    return true;
}

static void to_lower_copy(char *dst, size_t cap, const char *src) {
    size_t i = 0u;
    if (!dst || cap == 0u) return;
    if (src) for (; src[i] && i + 1u < cap; ++i) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

static bool valid_role(sl_help_role r) { return (unsigned)r < (unsigned)SL_ROLE_COUNT; }

/* ---- lifecycle --------------------------------------------------------- */
sl_help_desk *sl_help_desk_create(size_t capacity, size_t company_size) {
    sl_help_desk *d;
    if (capacity == 0u) capacity = SL_HELP_DEFAULT_CAPACITY;
    if (company_size == 0u) company_size = SL_HELP_DEFAULT_COMPANY_SIZE;
    d = (sl_help_desk *)calloc(1u, sizeof(*d));
    if (!d) return NULL;
    d->contacts = (sl_contact *)calloc(capacity, sizeof(sl_contact));
    if (!d->contacts) { free(d); return NULL; }
    d->capacity = capacity;
    d->company_size = company_size;
    return d;
}
void sl_help_desk_destroy(sl_help_desk *d) {
    if (!d) return;
    free(d->contacts);
    free(d);
}
size_t sl_help_desk_count(const sl_help_desk *d) { return d ? d->count : 0u; }
size_t sl_help_desk_capacity(const sl_help_desk *d) { return d ? d->capacity : 0u; }
size_t sl_help_desk_company_size(const sl_help_desk *d) { return d ? d->company_size : 0u; }

/* ---- population -------------------------------------------------------- */
sl_help_status sl_contact_init(
    sl_contact *out, uint64_t id, sl_help_role role, const char *name,
    const char *organization, const char *email, const char *phone,
    const char *remote_ip, uint16_t remote_port, const char *topic_tags,
    bool alive, bool known_of_science) {
    sl_contact c;
    if (!out || id == 0u || !valid_role(role) || !name) return SL_HELP_INVALID_ARGUMENT;
    memset(&c, 0, sizeof(c));
    if (!copy_bounded(c.name, sizeof(c.name), name, false) ||
        !copy_bounded(c.organization, sizeof(c.organization), organization, true) ||
        !copy_bounded(c.email, sizeof(c.email), email, true) ||
        !copy_bounded(c.phone, sizeof(c.phone), phone, true) ||
        !copy_bounded(c.remote_ip, sizeof(c.remote_ip), remote_ip, true))
        return SL_HELP_INVALID_ARGUMENT;
    /* topic tags stored lowercased for case-insensitive matching. */
    to_lower_copy(c.topic_tags, sizeof(c.topic_tags), topic_tags);
    c.id = id;
    c.role = role;
    c.remote_port = remote_port;
    c.alive = alive;
    c.known_of_science = known_of_science;
    c.channels = 0u;
    if (c.email[0]) c.channels |= SL_CHANNEL_EMAIL;
    if (c.phone[0]) c.channels |= SL_CHANNEL_PHONE;
    if (c.remote_ip[0] && remote_port != 0u) c.channels |= SL_CHANNEL_REMOTE;
    *out = c;
    return SL_HELP_OK;
}

sl_help_status sl_help_desk_add_contact(sl_help_desk *d, const sl_contact *c) {
    size_t i;
    if (!d || !c || c->id == 0u || !valid_role(c->role) || c->name[0] == '\0')
        return SL_HELP_INVALID_ARGUMENT;
    if (d->count >= d->capacity) return SL_HELP_FULL;
    for (i = 0u; i < d->count; ++i)
        if (d->contacts[i].id == c->id) return SL_HELP_DUPLICATE_ID;
    d->contacts[d->count++] = *c;
    return SL_HELP_OK;
}

/* ---- synthetic placeholder generator (RFC-safe) ------------------------ */
static const char *k_first[] = {
    "Avery","Blair","Casey","Devon","Ellis","Finley","Gray","Harper",
    "Indigo","Jordan","Kai","Logan","Marlow","Noor","Oakley","Payton",
    "Quinn","Reese","Sage","Tatum","Uma","Vesper","Wren","Xan","Yael","Zephyr"
};
static const char *k_last[] = {
    "Archer","Bello","Castro","Dunn","Everett","Flores","Gauss","Hwang",
    "Ingram","Jves","Khan","Lindqvist","Mbeki","Nilsson","Owusu","Park",
    "Quist","Rossi","Santos","Takahashi","Udell","Vance","Wu","Xu","Yamada","Zola"
};
static const char *k_topics[] = {
    "schedule budget policy", "series authorship chapters", "broadcast media interview",
    "city zoning transit", "public statement press", "research science methods",
    "community outreach help", "economy metrics report", "education curriculum",
    "safety anatomy health"
};
/* RFC 5737 documentation ranges, rotated per index. */
static const char *k_doc_nets[] = { "192.0.2.", "198.51.100.", "203.0.113." };

static void placeholder_net(char *dst, size_t cap, uint64_t idx) {
    const char *net = k_doc_nets[idx % (sizeof(k_doc_nets) / sizeof(k_doc_nets[0]))];
    unsigned host = (unsigned)(1u + (idx % 254u)); /* .1 .. .254 */
    snprintf(dst, cap, "%s%u", net, host);
}

size_t sl_help_desk_seed_placeholders(sl_help_desk *d, size_t count, uint64_t seed) {
    size_t added = 0u;
    uint64_t i;
    if (!d) return 0u;
    for (i = 0u; added < count && d->count < d->capacity; ++i) {
        sl_contact c;
        char name[SL_HELP_NAME_CAP];
        char email[SL_HELP_EMAIL_CAP];
        char phone[SL_HELP_PHONE_CAP];
        char ip[SL_HELP_IP_CAP];
        uint64_t h = seed + i;
        const char *fn = k_first[h % (sizeof(k_first) / sizeof(k_first[0]))];
        const char *ln = k_last[(h / 7u) % (sizeof(k_last) / sizeof(k_last[0]))];
        sl_help_role role = (sl_help_role)(h % (uint64_t)SL_ROLE_COUNT);
        const char *topic = k_topics[(h / 3u) % (sizeof(k_topics) / sizeof(k_topics[0]))];
        uint64_t id = (seed << 20) + i + 1u;
        /* 555-01xx is the reserved fictitious exchange. */
        unsigned last4 = (unsigned)(100u + (h % 99u)); /* 0100..0198 */
        uint16_t port = (uint16_t)(1024u + (h % 60000u));
        bool alive = (h % 5u) != 0u;            /* ~80% alive */
        bool science = ((h / 11u) % 3u) == 0u;  /* ~33% known of Science */

        snprintf(name, sizeof(name), "%s %s", fn, ln);
        snprintf(email, sizeof(email), "%s.%s@example.com", fn, ln);
        snprintf(phone, sizeof(phone), "+1-555-01%02u", last4 % 100u);
        placeholder_net(ip, sizeof(ip), h);

        if (sl_contact_init(&c, id, role, name, "Placeholder Co-Series",
                            email, phone, ip, port, topic, alive, science) == SL_HELP_OK) {
            if (sl_help_desk_add_contact(d, &c) == SL_HELP_OK) ++added;
        }
    }
    return added;
}

/* ---- query ------------------------------------------------------------- */
/* Count how many whitespace-separated terms of `query_lc` appear in any of the
 * contact's searchable fields. */
static unsigned score_contact(const sl_contact *c, const char *query_lc) {
    char haystack[SL_HELP_NAME_CAP + SL_HELP_ORG_CAP + SL_HELP_TAGS_CAP + 64u];
    char name_lc[SL_HELP_NAME_CAP], org_lc[SL_HELP_ORG_CAP];
    const char *role_lc = sl_help_role_string(c->role);
    const char *term;
    char terms[256];
    unsigned score = 0u;

    to_lower_copy(name_lc, sizeof(name_lc), c->name);
    to_lower_copy(org_lc, sizeof(org_lc), c->organization);
    snprintf(haystack, sizeof(haystack), "%s %s %s %s",
             name_lc, org_lc, c->topic_tags, role_lc);

    /* tokenize a working copy of the query. */
    strncpy(terms, query_lc, sizeof(terms) - 1u);
    terms[sizeof(terms) - 1u] = '\0';
    for (term = strtok(terms, " \t\r\n"); term; term = strtok(NULL, " \t\r\n")) {
        if (term[0] && strstr(haystack, term)) ++score;
    }
    return score;
}

static size_t run_query(
    const sl_help_desk *d, const char *question, int role_filter,
    sl_respondent *out, size_t max_out, size_t *total) {
    char query_lc[256];
    size_t i, written = 0u, matched = 0u;
    if (total) *total = 0u;
    if (!d || !question) return 0u;
    to_lower_copy(query_lc, sizeof(query_lc), question);

    for (i = 0u; i < d->count; ++i) {
        const sl_contact *c = &d->contacts[i];
        unsigned s;
        if (role_filter >= 0 && (int)c->role != role_filter) continue;
        s = score_contact(c, query_lc);
        if (s == 0u) continue;
        ++matched;
        if (out && written < max_out) {
            /* insertion sort by descending score within the written window. */
            size_t j = written;
            out[written].contact = c;
            out[written].match_score = s;
            ++written;
            while (j > 0u && out[j - 1u].match_score < out[j].match_score) {
                sl_respondent tmp = out[j - 1u];
                out[j - 1u] = out[j];
                out[j] = tmp;
                --j;
            }
        }
    }
    if (total) *total = matched;
    return written;
}

size_t sl_help_desk_query(
    const sl_help_desk *d, const char *question,
    sl_respondent *out, size_t max_out, size_t *total) {
    return run_query(d, question, -1, out, max_out, total);
}
size_t sl_help_desk_query_role(
    const sl_help_desk *d, const char *question, sl_help_role role,
    sl_respondent *out, size_t max_out, size_t *total) {
    int filter = (role < SL_ROLE_COUNT) ? (int)role : -1;
    return run_query(d, question, filter, out, max_out, total);
}

/* ---- stringization ----------------------------------------------------- */
const char *sl_help_role_string(sl_help_role r) {
    switch (r) {
        case SL_ROLE_MANAGEMENT: return "management";
        case SL_ROLE_CO_SERIES_AUTHOR: return "co-series author";
        case SL_ROLE_TELEGENIC_AUTHOR: return "telegenically-aware author";
        case SL_ROLE_MAYOR: return "mayor";
        case SL_ROLE_CITY_SPOKESPERSON: return "city spokesperson";
        default: return "unknown";
    }
}
const char *sl_help_status_string(sl_help_status s) {
    switch (s) {
        case SL_HELP_OK: return "ok";
        case SL_HELP_INVALID_ARGUMENT: return "invalid argument";
        case SL_HELP_FULL: return "directory full";
        case SL_HELP_DUPLICATE_ID: return "duplicate id";
        case SL_HELP_NOT_FOUND: return "not found";
        default: return "unknown";
    }
}
const char *sl_help_alive_string(bool alive) { return alive ? "alive" : "deceased"; }
const char *sl_help_science_string(bool k) { return k ? "known of Science" : "not known of Science"; }
