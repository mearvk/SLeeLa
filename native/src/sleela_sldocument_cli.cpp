/*
 * native/src/sleela_sldocument_cli.cpp
 * SLeeLa Native .sldocument Bridge — command adapter and self-test.
 *
 * Exercises the C ABI the lib/sldocument classes bind to: compile (frame
 * handle), per-step method invocation (the single binary veritable item), and
 * the kindness (well-formed/benign) flag.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_sldocument.h"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void check(const char *name, bool cond) {
    std::printf("[%s] %s\n", cond ? "pass" : "FAIL", name);
    if (!cond) ++failures;
}

int main(int argc, char **argv) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::printf("usage: sleela-sldocument-native [--help]\n");
        std::printf("  runs the SLeeLa .sldocument bridge self-test (compile, invoke, kind)\n");
        return 0;
    }

    /* Compile. */
    int32_t frame = sleela_sldocument_compile("report", "1.0", 2);
    check("compile yields frame", frame >= 0);
    check("compile rejects empty title", sleela_sldocument_compile("", "1.0", 0) == -1);
    check("compile rejects empty version", sleela_sldocument_compile("d", "", 0) == -1);
    check("compile rejects negative companions", sleela_sldocument_compile("d", "1.0", -1) == -1);

    /* Invoke a well-formed step: single binary veritable item == 1. */
    check("invoke well-formed step is veritable",
          sleela_sldocument_invoke(frame, "verify_inputs", 1) == 1);
    check("invoke respects order >= 1",
          sleela_sldocument_invoke(frame, "verify_inputs", 0) == -1);
    check("invoke rejects invalid frame",
          sleela_sldocument_invoke(-1, "verify_inputs", 1) == -1);
    check("invoke rejects malformed method",
          sleela_sldocument_invoke(frame, "1bad name", 1) == -1);
    check("invoke rejects empty method",
          sleela_sldocument_invoke(frame, "", 1) == -1);

    /* Kindness: well-formed benign symbol is kind; malformed is not. */
    check("well-formed method is kind",
          sleela_sldocument_kind(frame, "finalize_result") == 1);
    check("malformed method is not kind",
          sleela_sldocument_kind(frame, "has spaces") == 0);
    check("underscore-leading method is kind",
          sleela_sldocument_kind(frame, "_internal") == 1);
    check("digit-leading method is not kind",
          sleela_sldocument_kind(frame, "2step") == 0);

    /* Compile choice: .sldocument is a selectable form alongside .sleela. */
    check("compile choice .sldocument",
          sleela_compile_choice("report.sldocument", SLEELA_FORM_SLDOCUMENT) >= 0);
    check("compile choice .sleela",
          sleela_compile_choice("prog.sleela", SLEELA_FORM_SLEELA) >= 0);
    check("compile choice distinguishes forms",
          sleela_compile_choice("a.sldocument", SLEELA_FORM_SLDOCUMENT)
          != sleela_compile_choice("a.sleela", SLEELA_FORM_SLEELA));
    check("compile choice rejects unknown form",
          sleela_compile_choice("x", 99) == -1);
    check("compile choice rejects empty path",
          sleela_compile_choice("", SLEELA_FORM_SLEELA) == -1);

    /* Naming conventions for converting .sldocument -> .sleela. */
    char nm[128];
    check("synth name zero-padded",
          sleela_sldocument_synth_name("step", 3, nm, sizeof nm) >= 0
          && std::strcmp(nm, "step003") == 0);
    check("synth name rejects order 0",
          sleela_sldocument_synth_name("step", 0, nm, sizeof nm) == -1);

    check("sanitize illegal chars",
          sleela_sldocument_sanitize_identifier("post to register!", nm, sizeof nm) >= 0
          && std::strcmp(nm, "post_to_register_") == 0);
    check("sanitize digit-leading gets underscore",
          sleela_sldocument_sanitize_identifier("2nd", nm, sizeof nm) >= 0
          && nm[0] == '_');
    check("sanitize empty becomes underscore",
          sleela_sldocument_sanitize_identifier("", nm, sizeof nm) >= 0
          && std::strcmp(nm, "_") == 0);

    check("camel from role",
          sleela_sldocument_camel_from_role("reconcile accounts", nm, sizeof nm) >= 0
          && std::strcmp(nm, "reconcileAccounts") == 0);
    check("camel from hyphen role",
          sleela_sldocument_camel_from_role("post-to-register", nm, sizeof nm) >= 0
          && std::strcmp(nm, "postToRegister") == 0);

    /* Emit a .sleela class from a converted document. */
    char src[1024];
    int n = sleela_sldocument_emit_sleela("LedgerClose", "national-ledger", 3, src, sizeof src);
    check("emit sleela produces source", n > 0);
    check("emit sleela declares class", std::strstr(src, "class LedgerClose") != nullptr);
    check("emit sleela names anonymous steps", std::strstr(src, "step003()") != nullptr);
    check("emit sleela has run()", std::strstr(src, "int run()") != nullptr);
    check("emit sleela rejects empty class",
          sleela_sldocument_emit_sleela("", "t", 1, src, sizeof src) == -1);

    std::printf("%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
