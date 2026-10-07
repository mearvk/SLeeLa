/*
 * lib/compiler/src/sleela_langc.c
 * SLeeLa Compiler — multi-language front-end C ABI implementation.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Stable C implementation of the modular compiler framework boundary. It holds
 * the language-module registry and answers phase/target/plan questions. It is
 * deliberately allocation-free and side-effect-free beyond the registry: it
 * never opens, parses, links, loads, or executes an input program. All of that
 * belongs to the SLeeLa front ends above and the VM/OS boundary below.
 */

#include "../include/sleela_langc.h"

#include <string.h>
#include <ctype.h>

static sleela_lang_module_t g_registry[SLEELA_LANG_REGISTRY_MAX];
static size_t g_registry_count = 0u;

/* ---- small helpers ------------------------------------------------------ */

static int sl_ieq(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

/* Does the comma-separated extension list `list` contain `ext`? Both sides are
 * matched case-insensitively; a missing leading dot on either side is tolerated. */
static int sl_ext_list_has(const char *list, const char *ext) {
    if (!list || !ext) return 0;
    if (*ext == '.') ++ext;              /* normalize query to no-dot */
    size_t extlen = strlen(ext);
    if (extlen == 0u) return 0;

    const char *p = list;
    while (*p) {
        /* skip separators / leading whitespace */
        while (*p == ',' || *p == ' ' || *p == '\t') ++p;
        const char *start = p;
        while (*p && *p != ',') ++p;
        const char *end = p;             /* [start,end) is one token */
        /* trim trailing whitespace */
        while (end > start && (end[-1] == ' ' || end[-1] == '\t')) --end;
        const char *tok = start;
        if (tok < end && *tok == '.') ++tok;   /* normalize token to no-dot */
        size_t toklen = (size_t)(end - tok);
        if (toklen == extlen) {
            size_t i;
            int same = 1;
            for (i = 0; i < toklen; ++i) {
                if (tolower((unsigned char)tok[i]) != tolower((unsigned char)ext[i])) {
                    same = 0;
                    break;
                }
            }
            if (same) return 1;
        }
    }
    return 0;
}

static int sl_phase_in_range(uint32_t phase_code) {
    return phase_code >= SLEELA_LANG_PHASE_SOURCE && phase_code <= SLEELA_LANG_PHASE_LAST;
}

static int sl_valid_module(const sleela_lang_module_t *m) {
    if (!m || !m->language_name || m->language_name[0] == '\0') return 0;
    if (m->default_target < SLEELA_LANG_TARGET_SLVM ||
        m->default_target > SLEELA_LANG_TARGET_NATIVE) return 0;
    return 1;
}

/* ---- registry ----------------------------------------------------------- */

void sleela_lang_registry_reset(void) {
    g_registry_count = 0u;
    memset(g_registry, 0, sizeof(g_registry));
}

int sleela_lang_register(const sleela_lang_module_t *module) {
    size_t i;
    if (!sl_valid_module(module)) return SLEELA_LANG_INVALID;

    /* replace in place when a module with the same name already exists */
    for (i = 0; i < g_registry_count; ++i) {
        if (sl_ieq(g_registry[i].language_name, module->language_name)) {
            g_registry[i] = *module;
            return SLEELA_LANG_OK;
        }
    }
    if (g_registry_count >= SLEELA_LANG_REGISTRY_MAX) return SLEELA_LANG_BLOCKED;
    g_registry[g_registry_count++] = *module;
    return SLEELA_LANG_OK;
}

size_t sleela_lang_registry_count(void) {
    return g_registry_count;
}

const sleela_lang_module_t *sleela_lang_find(const char *language_name) {
    size_t i;
    if (!language_name) return NULL;
    for (i = 0; i < g_registry_count; ++i) {
        if (sl_ieq(g_registry[i].language_name, language_name)) return &g_registry[i];
    }
    return NULL;
}

const sleela_lang_module_t *sleela_lang_find_by_extension(const char *ext) {
    size_t i;
    if (!ext) return NULL;
    for (i = 0; i < g_registry_count; ++i) {
        if (sl_ext_list_has(g_registry[i].source_extensions, ext)) return &g_registry[i];
    }
    return NULL;
}

/* ---- phase model -------------------------------------------------------- */

int sleela_lang_provides(const sleela_lang_module_t *m, uint32_t phase_code) {
    if (!m || !sl_phase_in_range(phase_code)) return 0;
    return (m->phase_mask & (1u << (phase_code - 1u))) != 0u;
}

uint32_t sleela_lang_required_phases(uint32_t target) {
    /* phases 1..6 (source..IR) are the common minimum for any produced model. */
    uint32_t base = 0u;
    uint32_t p;
    for (p = SLEELA_LANG_PHASE_SOURCE; p <= SLEELA_LANG_PHASE_IR; ++p)
        base |= (1u << (p - 1u));
    switch (target) {
        case SLEELA_LANG_TARGET_SLIR:
            return base;                                  /* stop at IR */
        case SLEELA_LANG_TARGET_SLVM:
        case SLEELA_LANG_TARGET_SLJVM:
        case SLEELA_LANG_TARGET_NATIVE:
            return base
                 | (1u << (SLEELA_LANG_PHASE_LOWER   - 1u))
                 | (1u << (SLEELA_LANG_PHASE_CODEGEN - 1u));
        default:
            return 0u;
    }
}

int sleela_lang_phase_status(const sleela_lang_module_t *m, uint32_t phase_code) {
    if (!m || !sl_phase_in_range(phase_code)) return SLEELA_LANG_INVALID;
    return sleela_lang_provides(m, phase_code) ? SLEELA_LANG_OK : SLEELA_LANG_ABSENT;
}

const char *sleela_lang_phase_name(uint32_t phase_code) {
    switch (phase_code) {
        case SLEELA_LANG_PHASE_SOURCE:   return "source";
        case SLEELA_LANG_PHASE_LEX:      return "lex";
        case SLEELA_LANG_PHASE_PARSE:    return "parse";
        case SLEELA_LANG_PHASE_RESOLVE:  return "resolve";
        case SLEELA_LANG_PHASE_SEMANTIC: return "semantic";
        case SLEELA_LANG_PHASE_IR:       return "ir";
        case SLEELA_LANG_PHASE_LOWER:    return "lower";
        case SLEELA_LANG_PHASE_CODEGEN:  return "codegen";
        default:                         return "unknown";
    }
}

const char *sleela_lang_target_name(uint32_t target_code) {
    switch (target_code) {
        case SLEELA_LANG_TARGET_SLVM:   return "SLVM";
        case SLEELA_LANG_TARGET_SLJVM:  return "SLJVM";
        case SLEELA_LANG_TARGET_SLIR:   return "SLIR";
        case SLEELA_LANG_TARGET_NATIVE: return "NATIVE";
        default:                        return "UNKNOWN";
    }
}

/* ---- planning ----------------------------------------------------------- */

int sleela_lang_plan(const sleela_lang_request_t *req, sleela_lang_plan_t *out) {
    const sleela_lang_module_t *m;
    uint32_t target, required, p, missing;

    if (!req || !out) return SLEELA_LANG_INVALID;
    memset(out, 0, sizeof(*out));

    m = sleela_lang_find(req->module_name);
    if (!m) return SLEELA_LANG_ABSENT;

    target = req->requested_target ? req->requested_target : m->default_target;
    required = sleela_lang_required_phases(target);
    if (required == 0u) return SLEELA_LANG_INVALID;

    /* a reference-only module may plan up to IR but must not be asked to emit. */
    if (m->reference_only &&
        (target == SLEELA_LANG_TARGET_SLVM ||
         target == SLEELA_LANG_TARGET_SLJVM ||
         target == SLEELA_LANG_TARGET_NATIVE)) {
        out->covered_phase_mask  = m->phase_mask;
        out->required_phase_mask = required;
        out->target              = target;
        out->first_missing_phase = SLEELA_LANG_PHASE_CODEGEN;
        out->vm_ready            = 0u;
        return SLEELA_LANG_BLOCKED;
    }

    out->covered_phase_mask  = m->phase_mask;
    out->required_phase_mask = required;
    out->target              = target;

    missing = 0u;
    for (p = SLEELA_LANG_PHASE_SOURCE; p <= SLEELA_LANG_PHASE_LAST; ++p) {
        uint32_t bit = (1u << (p - 1u));
        if ((required & bit) && !(m->phase_mask & bit)) {
            missing = p;
            break;
        }
    }
    out->first_missing_phase = missing;
    out->vm_ready = (missing == 0u) ? 1u : 0u;

    /* strict requests fail closed: no plan unless fully covered. */
    if (req->strict && !out->vm_ready) return SLEELA_LANG_BLOCKED;
    return SLEELA_LANG_OK;
}

const char *sleela_lang_status_name(int status_code) {
    switch (status_code) {
        case SLEELA_LANG_OK:      return "OK";
        case SLEELA_LANG_ABSENT:  return "ABSENT";
        case SLEELA_LANG_INVALID: return "INVALID";
        case SLEELA_LANG_BLOCKED: return "BLOCKED";
        default:                  return "UNKNOWN";
    }
}
