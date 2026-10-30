#include "speculation_invite.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct sl_spec_registry {
    sl_speculation *items;
    size_t count;
    size_t capacity;
};

static const char *k_invitation =
    "Invitation to adult-psychiatry professionals:\n"
    "You are invited to offer careful, speculative commentary ABOUT this\n"
    "software's programming reach -- what it can and cannot responsibly do.\n"
    "Please frame each note as a hypothesis, attribute it to yourself, state\n"
    "the scope, and say how it could be shown wrong. This collects good\n"
    "scientific speculation about the software; it is not a clinical\n"
    "assessment of any person, and no diagnosis or treatment advice is sought\n"
    "or accepted.";

static const char *k_disclaimer =
    "Disclaimer: the notes above are self-attested speculations ABOUT the\n"
    "software, recorded in the author's own words. They are hypotheses, not\n"
    "findings. Roles are self-reported and unverified; nothing here is a\n"
    "diagnosis, a treatment or medication recommendation, or a verified\n"
    "professional opinion. This module is not an emergency service.";

const char *sl_spec_invitation_text(void) { return k_invitation; }
const char *sl_spec_disclaimer_text(void) { return k_disclaimer; }

/* ---- helpers ----------------------------------------------------------- */
static bool copy_req(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || cap == 0u || !src) return false;
    n = strlen(src);
    if (n == 0u || n >= cap) return false;
    memcpy(dst, src, n + 1u);
    return true;
}
static bool copy_opt(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || cap == 0u) return false;
    if (!src) { dst[0] = '\0'; return true; }
    n = strlen(src);
    if (n >= cap) return false;
    memcpy(dst, src, n + 1u);
    return true;
}
static bool valid_aspect(sl_spec_aspect a) {
    return (unsigned)a <= (unsigned)SL_ASPECT_RESEARCH_DIRECTION;
}
static bool valid_conf(sl_spec_confidence c) {
    return (unsigned)c <= (unsigned)SL_CONF_WELL_REASONED;
}

/* ---- lifecycle --------------------------------------------------------- */
sl_spec_registry *sl_spec_registry_create(size_t capacity) {
    sl_spec_registry *r;
    if (capacity == 0u) capacity = SL_SPEC_DEFAULT_CAPACITY;
    r = (sl_spec_registry *)calloc(1u, sizeof(*r));
    if (!r) return NULL;
    r->items = (sl_speculation *)calloc(capacity, sizeof(sl_speculation));
    if (!r->items) { free(r); return NULL; }
    r->capacity = capacity;
    return r;
}
void sl_spec_registry_destroy(sl_spec_registry *r) {
    if (!r) return;
    free(r->items);
    free(r);
}
size_t sl_spec_registry_count(const sl_spec_registry *r) { return r ? r->count : 0u; }

/* ---- build and submit -------------------------------------------------- */
sl_spec_status sl_speculation_init(
    sl_speculation *out, uint64_t id, sl_spec_aspect aspect,
    sl_spec_confidence confidence, const char *author, const char *role,
    const char *scope, const char *text, const char *falsifiability,
    bool self_attested) {
    sl_speculation s;
    if (!out || id == 0u || !valid_aspect(aspect) || !valid_conf(confidence))
        return SL_SPEC_INVALID_ARGUMENT;
    if (!self_attested) return SL_SPEC_NOT_ATTESTED;
    memset(&s, 0, sizeof(s));
    if (!copy_req(s.author, sizeof(s.author), author) ||
        !copy_req(s.text, sizeof(s.text), text))
        return SL_SPEC_INVALID_ARGUMENT;
    /* Falsifiability is required: a speculation with no way to be wrong is
     * not scientific and is rejected here. */
    if (!falsifiability || falsifiability[0] == '\0')
        return SL_SPEC_MISSING_FALSIFIABILITY;
    if (!copy_req(s.falsifiability, sizeof(s.falsifiability), falsifiability))
        return SL_SPEC_INVALID_ARGUMENT;
    if (!copy_opt(s.role, sizeof(s.role), role) ||
        !copy_opt(s.scope, sizeof(s.scope), scope))
        return SL_SPEC_INVALID_ARGUMENT;
    s.id = id;
    s.aspect = aspect;
    s.confidence = confidence;
    s.self_attested = true;
    s.about_software_not_person = true; /* forced: always about the software */
    *out = s;
    return SL_SPEC_OK;
}

sl_spec_status sl_spec_registry_submit(sl_spec_registry *r, const sl_speculation *s) {
    size_t i;
    if (!r || !s || s->id == 0u) return SL_SPEC_INVALID_ARGUMENT;
    if (!s->self_attested) return SL_SPEC_NOT_ATTESTED;
    if (s->falsifiability[0] == '\0') return SL_SPEC_MISSING_FALSIFIABILITY;
    if (!s->about_software_not_person || s->author[0] == '\0' || s->text[0] == '\0')
        return SL_SPEC_INVALID_ARGUMENT;
    if (r->count >= r->capacity) return SL_SPEC_FULL;
    for (i = 0u; i < r->count; ++i)
        if (r->items[i].id == s->id) return SL_SPEC_INVALID_ARGUMENT; /* dup id */
    r->items[r->count++] = *s;
    return SL_SPEC_OK;
}

const sl_speculation *sl_spec_registry_at(const sl_spec_registry *r, size_t i) {
    if (!r || i >= r->count) return NULL;
    return &r->items[i];
}

/* ---- careful scientific output ----------------------------------------- */
int sl_spec_registry_write_report(const sl_spec_registry *r, FILE *out) {
    size_t i;
    if (!r || !out) return 0;
    fputs("==== Adult-psychiatry speculations on programming reach ====\n\n", out);
    fputs(sl_spec_invitation_text(), out);
    fputs("\n\n", out);
    fprintf(out, "Collected speculations: %zu\n", r->count);
    fputs("(Each is a hypothesis about the software, in the author's own words.)\n\n", out);
    for (i = 0u; i < r->count; ++i) {
        const sl_speculation *s = &r->items[i];
        fprintf(out, "[%zu] id=%" PRIu64 " | %s | confidence: %s\n",
                i + 1u, s->id, sl_spec_aspect_string(s->aspect),
                sl_spec_confidence_string(s->confidence));
        fprintf(out, "    author: %s", s->author);
        if (s->role[0]) fprintf(out, "  (role, self-reported: %s)", s->role);
        fputc('\n', out);
        if (s->scope[0]) fprintf(out, "    scope: %s\n", s->scope);
        fprintf(out, "    speculation: %s\n", s->text);
        fprintf(out, "    could be shown wrong if: %s\n\n", s->falsifiability);
    }
    fputs(sl_spec_disclaimer_text(), out);
    fputc('\n', out);
    return 1;
}

/* ---- stringization ----------------------------------------------------- */
const char *sl_spec_aspect_string(sl_spec_aspect a) {
    switch (a) {
        case SL_ASPECT_CAPABILITY: return "capability (what it can reach)";
        case SL_ASPECT_LIMITATION: return "limitation (what it should not reach)";
        case SL_ASPECT_SAFETY: return "safety";
        case SL_ASPECT_ETHICS: return "ethics";
        case SL_ASPECT_RESEARCH_DIRECTION: return "research direction";
        default: return "unknown";
    }
}
const char *sl_spec_confidence_string(sl_spec_confidence c) {
    switch (c) {
        case SL_CONF_TENTATIVE: return "tentative (a hunch)";
        case SL_CONF_PLAUSIBLE: return "plausible";
        case SL_CONF_WELL_REASONED: return "well-reasoned (still a hypothesis)";
        default: return "unknown";
    }
}
const char *sl_spec_status_string(sl_spec_status s) {
    switch (s) {
        case SL_SPEC_OK: return "ok";
        case SL_SPEC_INVALID_ARGUMENT: return "invalid argument";
        case SL_SPEC_NOT_ATTESTED: return "not attested by author";
        case SL_SPEC_MISSING_FALSIFIABILITY: return "missing falsifiability note";
        case SL_SPEC_FULL: return "registry full";
        case SL_SPEC_NOT_FOUND: return "not found";
        default: return "unknown";
    }
}
