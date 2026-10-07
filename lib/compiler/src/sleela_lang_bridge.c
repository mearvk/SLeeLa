/*
 * lib/compiler/src/sleela_lang_bridge.c
 * SLeeLa Compiler — VM/OS bridge for the multi-language front-end layer.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Adapts the pure C ABI (sleela_langc.h) to the flat-scalar / plan-handle
 * shapes the .sleela compiler-framework classes call. A fixed ring of recent
 * plans backs the integer handles so a plan's fields can be read one accessor
 * at a time. No allocation, no I/O, no execution.
 */

#include "../include/sleela_lang_bridge.h"
#include "../include/sleela_langc.h"

#include <string.h>

/* --- registry bridge ----------------------------------------------------- */

int sleela_lang_register_module(const char *language_name,
                                const char *language_family,
                                const char *language_version,
                                const char *source_extensions,
                                uint32_t phase_mask,
                                uint32_t default_target,
                                uint32_t reference_only) {
    sleela_lang_module_t m;
    memset(&m, 0, sizeof(m));
    m.language_name     = language_name;
    m.language_family   = language_family;
    m.language_version  = language_version;
    m.source_extensions = source_extensions;
    m.phase_mask        = phase_mask;
    m.default_target    = default_target;
    m.reference_only    = reference_only;
    return sleela_lang_register(&m);
}

int sleela_lang_has(const char *language_name) {
    return sleela_lang_find(language_name) != NULL ? 1 : 0;
}

const char *sleela_lang_resolve_extension(const char *file_name) {
    const char *dot;
    const sleela_lang_module_t *m;
    if (!file_name) return "";
    /* find the last '.' to isolate the extension */
    dot = strrchr(file_name, '.');
    if (!dot) return "";
    m = sleela_lang_find_by_extension(dot);
    if (!m || !m->language_name) return "";
    return m->language_name;
}

/* --- plan bridge --------------------------------------------------------- */

#define SLEELA_LANG_PLAN_SLOTS 32

typedef struct {
    int               used;
    int               status;
    sleela_lang_plan_t plan;
} plan_slot_t;

static plan_slot_t g_plans[SLEELA_LANG_PLAN_SLOTS];
static int g_plan_next = 0;

int sleela_lang_plan_request(const char *module_name,
                             const char *input_path,
                             uint32_t requested_target,
                             uint32_t strict) {
    sleela_lang_request_t req;
    int slot = g_plan_next;

    memset(&req, 0, sizeof(req));
    req.module_name      = module_name;
    req.input_path       = input_path;
    req.requested_target = requested_target;
    req.strict           = strict;

    g_plan_next = (g_plan_next + 1) % SLEELA_LANG_PLAN_SLOTS;

    g_plans[slot].used   = 1;
    g_plans[slot].status = sleela_lang_plan(&req, &g_plans[slot].plan);
    return slot;
}

static int plan_valid(int handle) {
    return handle >= 0 && handle < SLEELA_LANG_PLAN_SLOTS && g_plans[handle].used;
}

int sleela_lang_plan_status(int handle) {
    return plan_valid(handle) ? g_plans[handle].status : SLEELA_LANG_INVALID;
}

uint32_t sleela_lang_plan_covered(int handle) {
    return plan_valid(handle) ? g_plans[handle].plan.covered_phase_mask : 0u;
}

uint32_t sleela_lang_plan_required(int handle) {
    return plan_valid(handle) ? g_plans[handle].plan.required_phase_mask : 0u;
}

uint32_t sleela_lang_plan_first_missing(int handle) {
    return plan_valid(handle) ? g_plans[handle].plan.first_missing_phase : 0u;
}

uint32_t sleela_lang_plan_target(int handle) {
    return plan_valid(handle) ? g_plans[handle].plan.target : 0u;
}

uint32_t sleela_lang_plan_vm_ready(int handle) {
    return plan_valid(handle) ? g_plans[handle].plan.vm_ready : 0u;
}
