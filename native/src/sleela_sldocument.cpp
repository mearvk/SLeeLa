/*
 * native/src/sleela_sldocument.cpp
 * SLeeLa Native .sldocument Bridge — C++17 implementation of
 * include/sleela_sldocument.h.
 *
 * Reference bridge for the lib/sldocument classes. It models the compile step
 * (returning a frame handle) and the per-step method invocation (returning the
 * single binary veritable item plus a kindness flag).
 *
 * The authoritative compilation and execution live in lib/compiler /
 * impl/frontend / impl/core; this bridge gives deterministic reference behaviour
 * so the document semantics can be exercised in isolation.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_sldocument.h"

#include <cstring>
#include <string>

namespace {

thread_local std::string g_error;

void set_error(const char *m) { g_error = m ? m : ""; }

/* A method name is well-formed (kind) when it is a non-empty identifier made of
 * ASCII letters, digits, or underscore and does not start with a digit. This is
 * the reference bridge's notion of a "kind" (benign, well-formed) symbol. */
bool well_formed_method(const char *method) {
    if (method == nullptr || method[0] == '\0') return false;
    char c0 = method[0];
    bool start_ok = (c0 == '_') || (c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z');
    if (!start_ok) return false;
    for (const char *p = method; *p; ++p) {
        char c = *p;
        bool ok = (c == '_') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
        if (!ok) return false;
    }
    return true;
}

} /* namespace */

extern "C" {

int32_t sleela_sldocument_compile(const char *title, const char *version,
                                  int32_t companion_count) {
    if (title == nullptr || title[0] == '\0') { set_error("document has no title"); return -1; }
    if (version == nullptr || version[0] == '\0') { set_error("document has no version"); return -1; }
    if (companion_count < 0) { set_error("negative companion count"); return -1; }
    g_error.clear();
    /* Reference model: a successful compile yields a non-negative frame handle.
     * A full build routes title/version/companions through the SLeeLa Compiler.
     * The handle is derived deterministically from the companion count so the
     * document and its steps share one frame. */
    return 1000 + companion_count;
}

int32_t sleela_sldocument_invoke(int32_t frame, const char *method, int32_t order) {
    if (frame < 0) { set_error("invalid VM frame"); return -1; }
    if (order < 1) { set_error("step order must be >= 1"); return -1; }
    if (!well_formed_method(method)) { set_error("unresolved or malformed method"); return -1; }
    /* Reference model: a well-formed step method on a valid frame evaluates to a
     * veritable (true) binary item. A full build invokes the compiled method and
     * returns its boolean result. */
    g_error.clear();
    return 1;
}

int sleela_sldocument_kind(int32_t frame, const char *method) {
    if (frame < 0) { set_error("invalid VM frame"); return 0; }
    /* Kindness == the symbol is well-formed and benign. */
    return well_formed_method(method) ? 1 : 0;
}

const char *sleela_sldocument_last_error(void) { return g_error.c_str(); }

} /* extern "C" */
