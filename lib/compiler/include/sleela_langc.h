#ifndef SLEELA_LANGC_H
#define SLEELA_LANGC_H
/*
 * lib/compiler/include/sleela_langc.h
 * SLeeLa Compiler — multi-language front-end C ABI.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * This is the stable C boundary for the modular compiler framework. The SLeeLa
 * source layer (the .sleela units in lib/compiler and lib/compiler/frontends)
 * declares the language-neutral compiler contract and the per-language front
 * ends; this header is the implementation seam a developer builds against when a
 * front end needs native services (module registration, phase dispatch, and the
 * per-language capability record).
 *
 * It describes a *framework* for building compilers for arbitrary, publicly
 * known programming languages. It does not itself execute any input program,
 * link, or load native code: it classifies, plans, and reports. Execution of a
 * produced artifact remains behind the SLeeLa VM/OS and security boundaries.
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Compile phases, in canonical pipeline order. A language front end declares
 * which phases it provides; the framework walks them in this order. */
enum {
    SLEELA_LANG_PHASE_SOURCE   = 1, /* load/normalize source units            */
    SLEELA_LANG_PHASE_LEX      = 2, /* tokenize                               */
    SLEELA_LANG_PHASE_PARSE    = 3, /* build the language syntax tree         */
    SLEELA_LANG_PHASE_RESOLVE  = 4, /* names, scopes, imports, symbols        */
    SLEELA_LANG_PHASE_SEMANTIC = 5, /* types, capabilities, dependencies      */
    SLEELA_LANG_PHASE_IR       = 6, /* lower to target-neutral SLeeLa IR      */
    SLEELA_LANG_PHASE_LOWER    = 7, /* lower IR to VM-ready operations        */
    SLEELA_LANG_PHASE_CODEGEN  = 8, /* emit artifact                          */
    SLEELA_LANG_PHASE_LAST     = 8
};

/* Phase outcome. */
enum {
    SLEELA_LANG_OK        = 1, /* phase provided and complete                 */
    SLEELA_LANG_ABSENT    = 2, /* phase not provided by this front end        */
    SLEELA_LANG_INVALID   = 3, /* phase code out of range / bad descriptor    */
    SLEELA_LANG_BLOCKED   = 4  /* refused by a capability/security boundary   */
};

/* Output target a front end lowers toward. These are planning identities; the
 * framework never equates a target with permission to run its artifact. */
enum {
    SLEELA_LANG_TARGET_SLVM    = 1, /* SLeeLa VM artifact (authoritative)     */
    SLEELA_LANG_TARGET_SLJVM   = 2, /* SLeeLa JVM-family artifact             */
    SLEELA_LANG_TARGET_SLIR    = 3, /* stop at SLeeLa IR (library/analysis)   */
    SLEELA_LANG_TARGET_NATIVE  = 4  /* native plan only; emission gated       */
};

/*
 * A language front-end module descriptor. One of these is registered per
 * publicly known source language a developer wants to compile. It is pure data
 * plus a provided-phase bitmask — a developer declares the language identity in
 * SLeeLa and the framework reasons about it here.
 */
typedef struct sleela_lang_module {
    const char *language_name;   /* e.g. "C", "C++", "Java", "Python"         */
    const char *language_family; /* e.g. "C-family", "JVM", "scripting"       */
    const char *language_version;/* front-end's declared accepted version     */
    const char *source_extensions;/* comma list, e.g. ".c" or ".cc,.cpp,.cxx" */
    uint32_t    phase_mask;      /* OR of (1u << (phase-1)) for provided phases*/
    uint32_t    default_target;  /* one of SLEELA_LANG_TARGET_*               */
    uint32_t    reference_only;  /* 1 => identification/planning only, no emit */
} sleela_lang_module_t;

/* A single compile request against a registered module. */
typedef struct sleela_lang_request {
    const char *module_name;     /* language_name of a registered module      */
    const char *input_path;      /* source path (not opened by this ABI)      */
    uint32_t    requested_target;/* SLEELA_LANG_TARGET_*                       */
    uint32_t    strict;          /* 1 => any absent required phase fails closed*/
} sleela_lang_request_t;

/* A compile plan/result: which phases a module covers for a request, and
 * whether the request can reach a VM-ready artifact. */
typedef struct sleela_lang_plan_s {
    uint32_t covered_phase_mask; /* phases the module provides                 */
    uint32_t required_phase_mask;/* phases the request needs for its target    */
    uint32_t first_missing_phase;/* 0 when none missing                        */
    uint32_t target;             /* resolved target                            */
    uint32_t vm_ready;           /* 1 when every required phase is covered      */
} sleela_lang_plan_t;

/* ---- Module registry ---------------------------------------------------- */

/* Maximum number of language modules the framework holds. Generous for a
 * standard-library-sized catalog of public languages. */
#define SLEELA_LANG_REGISTRY_MAX 256

/* Reset the registry to empty. */
void sleela_lang_registry_reset(void);

/* Register (or replace by language_name) a language front-end module.
 * Returns SLEELA_LANG_OK on success, SLEELA_LANG_INVALID on a bad descriptor,
 * or SLEELA_LANG_BLOCKED when the registry is full. */
int sleela_lang_register(const sleela_lang_module_t *module);

/* Number of registered modules. */
size_t sleela_lang_registry_count(void);

/* Find a module by language name (case-insensitive). Returns NULL if absent. */
const sleela_lang_module_t *sleela_lang_find(const char *language_name);

/* Find a module whose source_extensions list contains `ext` (with or without a
 * leading dot, case-insensitive). Returns NULL if none claims it. */
const sleela_lang_module_t *sleela_lang_find_by_extension(const char *ext);

/* ---- Phase model -------------------------------------------------------- */

/* True (1) when `module` provides `phase_code`. */
int sleela_lang_provides(const sleela_lang_module_t *module, uint32_t phase_code);

/* The set of phases required to reach `target`. */
uint32_t sleela_lang_required_phases(uint32_t target);

/* Status of a single phase for a module (OK / ABSENT / INVALID). */
int sleela_lang_phase_status(const sleela_lang_module_t *module, uint32_t phase_code);

/* Canonical name for a phase code ("source", "lex", ...). */
const char *sleela_lang_phase_name(uint32_t phase_code);

/* Canonical name for a target code. */
const char *sleela_lang_target_name(uint32_t target_code);

/* ---- Planning ----------------------------------------------------------- */

/* Build a compile plan for `request` against the registry. Returns
 * SLEELA_LANG_OK when a plan was produced (even if not vm_ready),
 * SLEELA_LANG_ABSENT when no module matches, SLEELA_LANG_INVALID on bad input,
 * SLEELA_LANG_BLOCKED when the module is reference-only but emission was asked. */
int sleela_lang_plan(const sleela_lang_request_t *request, sleela_lang_plan_t *out_plan);

/* Human-readable name of a status code. */
const char *sleela_lang_status_name(int status_code);

#ifdef __cplusplus
}
#endif
#endif
