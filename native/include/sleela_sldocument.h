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

/* Last human-readable error (never NULL). */
const char *sleela_sldocument_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_SLDOCUMENT_H */
