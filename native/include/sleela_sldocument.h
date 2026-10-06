/*
 * native/include/sleela_sldocument.h
 * SLeeLa Native .sldocument Bridge — stable C ABI.
 *
 * This header is the explicit VM/OS bridge below the SLeeLa lib/sldocument
 * classes (SLDocument, SLDocumentStep, SLDocumentCompiler, SLVeritable, ...).
 * An .sldocument is an ordered document whose annotated method steps run
 * top-down and compile against/with standard SLeeLa source; each step usually
 * returns a single binary "veritable and kind" value.
 *
 * The document *semantics* live in SLeeLa source; the actual compile and the
 * per-step method invocation are delegated here. The authoritative compilation
 * remains the SLeeLa Compiler (lib/compiler / impl/frontend).
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#ifndef SLEELA_SLDOCUMENT_H
#define SLEELA_SLDOCUMENT_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Compile a document and its companion .sleela sources through the SLeeLa
 * Compiler path. `title` and `version` identify the document; `companion_count`
 * is the number of registered companion sources. Returns a VM frame handle
 * (>= 0) the document runs against, or -1 on failure.
 */
int32_t sleela_sldocument_compile(const char *title, const char *version,
                                  int32_t companion_count);

/*
 * Invoke one ordered step's compiled method on the VM frame and return its
 * single binary veritable item: 1 (veritable/true) or 0 (not). Returns -1 on an
 * invalid frame or an unresolved method.
 */
int32_t sleela_sldocument_invoke(int32_t frame, const char *method, int32_t order);

/*
 * Report whether the last value produced by `method` on this frame is "kind"
 * (well-formed and benign). Returns 1 when kind, 0 otherwise.
 */
int sleela_sldocument_kind(int32_t frame, const char *method);

/* ---- Source-form compile choice (lib/compiler/SLCompileChoice) ---- */

/* Source-form selectors; must match lib/compiler/SLSourceForm.sleela. */
#define SLEELA_FORM_SLEELA     1
#define SLEELA_FORM_SLDOCUMENT 2
#define SLEELA_FORM_SLSCRIPT   3

/*
 * Compile `path` as the chosen source form. The .sldocument form routes through
 * the document-aware path; .sleela through the ordinary program path. Returns a
 * VM frame handle (>= 0) or -1 when the form is unknown / the path is empty.
 */
int32_t sleela_compile_choice(const char *path, int32_t form);

/* ---- Naming conventions (lib/sldocument/SLDocumentNaming, SLDocumentConverter) ---- */

/*
 * Synthesize a stable .sleela method name for an anonymous document step from a
 * prefix and 1-based order, e.g. ("step", 3) -> "step003". Writes a
 * NUL-terminated identifier into `out` (capacity `out_cap`). Returns the written
 * length, or -1 on bad arguments / insufficient buffer.
 */
int sleela_sldocument_synth_name(const char *prefix, int32_t order,
                                 char *out, size_t out_cap);

/*
 * Make `raw` a legal SLeeLa identifier (letters/digits/underscore, not starting
 * with a digit; illegal characters become underscores; empty becomes "_"). Writes
 * into `out` (capacity `out_cap`). Returns the written length or -1.
 */
int sleela_sldocument_sanitize_identifier(const char *raw, char *out, size_t out_cap);

/*
 * Convert a human role ("reconcile accounts") to a camelCase identifier
 * ("reconcileAccounts"). Writes into `out` (capacity `out_cap`). Returns the
 * written length or -1.
 */
int sleela_sldocument_camel_from_role(const char *role, char *out, size_t out_cap);

/*
 * Emit a .sleela class source that preserves a converted document's ordered
 * steps as named methods plus a run() that calls them in order. Writes into
 * `out` (capacity `out_cap`). Returns the written length or -1.
 */
int sleela_sldocument_emit_sleela(const char *class_name, const char *doc_title,
                                  int32_t step_count, char *out, size_t out_cap);

/*
 * Write converted .sleela `source` to `path` for safekeeping. Returns 1 on
 * success, 0 on failure. The path should carry the .sleela extension.
 */
int sleela_sldocument_write(const char *path, const char *source);

/* Last human-readable error (never NULL). */
const char *sleela_sldocument_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_SLDOCUMENT_H */
