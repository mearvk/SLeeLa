#ifndef SLEELA_LANG_BRIDGE_H
#define SLEELA_LANG_BRIDGE_H
/*
 * lib/compiler/include/sleela_lang_bridge.h
 * SLeeLa Compiler — VM/OS bridge for the SLeeLa multi-language front-end layer.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * These are the `native` entry points the .sleela compiler-framework classes
 * call across the explicit VM/OS bridge (SLLanguageCompiler, SLCompilerRegistry,
 * SLCompilePlan). They adapt the pure C ABI in sleela_langc.h to the shapes the
 * SLeeLa layer uses: flat scalar arguments for registration and an integer
 * plan-handle whose fields are read back one accessor at a time.
 *
 * The bridge holds a small, fixed table of recent plans so a handle stays valid
 * for its accessors. It performs no I/O and executes nothing: it classifies and
 * plans only, exactly like the ABI beneath it.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Registry bridge (SLCompilerRegistry / SLLanguageCompiler.register) --- */

void sleela_lang_registry_reset(void); /* re-exported from the core ABI */

/* Register a module from flat scalar fields (as the .sleela layer supplies).
 * Returns the core status code (OK=1, INVALID=3, BLOCKED=4). */
int sleela_lang_register_module(const char *language_name,
                                const char *language_family,
                                const char *language_version,
                                const char *source_extensions,
                                uint32_t phase_mask,
                                uint32_t default_target,
                                uint32_t reference_only);

/* The .sleela layer reads the registry count through the core ABI's
 * sleela_lang_registry_count() (declared in sleela_langc.h), so no flat-scalar
 * duplicate is defined here. */

/* 1 when a module with `language_name` is registered, else 0. */
int sleela_lang_has(const char *language_name);

/* Resolve a file name's extension to a registered module's language name.
 * Returns the language name, or "" when no registered front end claims it. */
const char *sleela_lang_resolve_extension(const char *file_name);

/* --- Plan bridge (SLCompilePlan) ---
 * Build a plan and return an opaque non-negative handle, or -1 on failure.
 * The handle's fields are then read with the accessors below. */
int sleela_lang_plan_request(const char *module_name,
                             const char *input_path,
                             uint32_t requested_target,
                             uint32_t strict);

int      sleela_lang_plan_status(int handle);        /* core status code      */
uint32_t sleela_lang_plan_covered(int handle);       /* covered phase mask    */
uint32_t sleela_lang_plan_required(int handle);      /* required phase mask   */
uint32_t sleela_lang_plan_first_missing(int handle); /* first missing phase   */
uint32_t sleela_lang_plan_target(int handle);        /* resolved target       */
uint32_t sleela_lang_plan_vm_ready(int handle);      /* 1 when VM-ready        */

#ifdef __cplusplus
}
#endif
#endif
