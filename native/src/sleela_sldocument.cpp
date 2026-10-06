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

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

namespace {

thread_local std::string g_error;

void set_error(const char *m) { g_error = m ? m : ""; }

/* Copy a std::string into a caller buffer; returns length or -1 if it will not
 * fit (or the buffer is null/zero). */
int write_out(const std::string &s, char *out, size_t out_cap) {
    if (out == nullptr || out_cap == 0) { g_error = "null/zero output buffer"; return -1; }
    if (s.size() + 1 > out_cap) { g_error = "output buffer too small"; return -1; }
    std::memcpy(out, s.c_str(), s.size() + 1);
    return static_cast<int>(s.size());
}

bool ident_start(char c) { return c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool ident_part(char c) { return ident_start(c) || (c >= '0' && c <= '9'); }

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

int32_t sleela_compile_choice(const char *path, int32_t form) {
    if (path == nullptr || path[0] == '\0') { set_error("empty source path"); return -1; }
    if (form != SLEELA_FORM_SLEELA && form != SLEELA_FORM_SLDOCUMENT && form != SLEELA_FORM_SLSCRIPT) {
        set_error("unknown source form");
        return -1;
    }
    g_error.clear();
    /* Reference model: both forms route to the SLeeLa Compiler and yield a frame
     * handle. The handle encodes the chosen form so the document path and the
     * program path are distinguishable. */
    return 2000 + form;
}

int sleela_sldocument_synth_name(const char *prefix, int32_t order,
                                 char *out, size_t out_cap) {
    if (prefix == nullptr || prefix[0] == '\0') { set_error("empty prefix"); return -1; }
    if (order < 1) { set_error("order must be >= 1"); return -1; }
    char buf[16];
    std::snprintf(buf, sizeof buf, "%03d", order);
    std::string name = std::string(prefix) + buf;
    return write_out(name, out, out_cap);
}

int sleela_sldocument_sanitize_identifier(const char *raw, char *out, size_t out_cap) {
    if (raw == nullptr) { set_error("null identifier"); return -1; }
    std::string s;
    for (const char *p = raw; *p; ++p) {
        char c = *p;
        if (ident_part(c)) { s.push_back(c); }
        else { s.push_back('_'); }
    }
    if (s.empty()) { s = "_"; }
    if (!ident_start(s[0])) { s = "_" + s; } /* must not start with a digit */
    return write_out(s, out, out_cap);
}

int sleela_sldocument_camel_from_role(const char *role, char *out, size_t out_cap) {
    if (role == nullptr) { set_error("null role"); return -1; }
    std::string s;
    bool upNext = false;
    bool first = true;
    for (const char *p = role; *p; ++p) {
        char c = *p;
        if (c == ' ' || c == '-' || c == '_' || c == '.' || c == '\t') { upNext = !first; continue; }
        if (!ident_part(c)) { continue; }
        if (first) {
            /* keep first word lowercase for camelCase; ensure legal start */
            s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            first = false;
        } else if (upNext) {
            s.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            upNext = false;
        } else {
            s.push_back(c);
        }
    }
    if (s.empty()) { s = "_"; }
    if (!ident_start(s[0])) { s = "_" + s; }
    return write_out(s, out, out_cap);
}

int sleela_sldocument_emit_sleela(const char *class_name, const char *doc_title,
                                  int32_t step_count, char *out, size_t out_cap) {
    if (class_name == nullptr || class_name[0] == '\0') { set_error("empty class name"); return -1; }
    if (step_count < 0) { set_error("negative step count"); return -1; }
    const char *title = (doc_title && doc_title[0]) ? doc_title : "(untitled)";
    std::string src;
    src += "/* Converted from .sldocument \"";
    src += title;
    src += "\" for safekeeping. */\n";
    src += "#sleela 1.3\n";
    src += "class ";
    src += class_name;
    src += " {\n";
    /* run() calls the ordered step methods; each returns a single binary item. */
    src += "  int run() {\n";
    src += "    int acc = 1;\n";
    for (int32_t i = 1; i <= step_count; ++i) {
        char line[64];
        std::snprintf(line, sizeof line, "    acc = acc & step%03d();\n", i);
        src += line;
    }
    src += "    return acc;\n";
    src += "  }\n";
    for (int32_t i = 1; i <= step_count; ++i) {
        char sig[64];
        std::snprintf(sig, sizeof sig, "  int step%03d() { return 1; }\n", i);
        src += sig;
    }
    src += "}\n";
    return write_out(src, out, out_cap);
}

int sleela_sldocument_write(const char *path, const char *source) {
    if (path == nullptr || path[0] == '\0') { set_error("empty path"); return 0; }
    if (source == nullptr) { set_error("null source"); return 0; }
    std::FILE *f = std::fopen(path, "w");
    if (f == nullptr) { set_error("cannot open path for writing"); return 0; }
    size_t len = std::strlen(source);
    size_t wrote = std::fwrite(source, 1, len, f);
    std::fclose(f);
    if (wrote != len) { set_error("short write"); return 0; }
    return 1;
}

const char *sleela_sldocument_last_error(void) { return g_error.c_str(); }

} /* extern "C" */
