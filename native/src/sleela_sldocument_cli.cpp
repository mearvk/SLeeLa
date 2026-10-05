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

    std::printf("%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
