/*
 * lib/compiler/tests/langc_selftest.c
 * SLeeLa Compiler — behavioral self-test for the multi-language framework ABI.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Exercises the registry, extension resolution, phase planning, and the
 * fail-closed reference-only / strict paths with real numeric assertions.
 * Build/run: make -C lib/compiler selftest
 */

#include "../include/sleela_langc.h"
#include "../include/sleela_lang_bridge.h"

#include <stdio.h>
#include <string.h>

static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", (msg)); ++g_failures; } \
    else { printf("ok:   %s\n", (msg)); } \
} while (0)

/* full pipeline: source..codegen */
static uint32_t full_mask(void) {
    uint32_t m = 0u, p;
    for (p = SLEELA_LANG_PHASE_SOURCE; p <= SLEELA_LANG_PHASE_CODEGEN; ++p)
        m |= (1u << (p - 1u));
    return m;
}

int main(void) {
    sleela_lang_registry_reset();
    CHECK(sleela_lang_registry_count() == 0, "registry starts empty");

    /* Register a few language front ends via the bridge (flat-scalar form). */
    uint32_t full = full_mask();
    int rc;
    rc = sleela_lang_register_module("C", "C-family", "C17", ".c",
                                     full, SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(rc == SLEELA_LANG_OK, "register C");
    rc = sleela_lang_register_module("C++", "C-family", "C++17", ".cc,.cpp,.cxx,.c++",
                                     full, SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(rc == SLEELA_LANG_OK, "register C++");
    rc = sleela_lang_register_module("Java", "JVM", "Java 21", ".java",
                                     full, SLEELA_LANG_TARGET_SLJVM, 0u);
    CHECK(rc == SLEELA_LANG_OK, "register Java");

    CHECK(sleela_lang_registry_count() == 3, "three modules registered");

    /* Re-registering by the same name replaces, not duplicates. */
    rc = sleela_lang_register_module("Java", "JVM", "Java 22", ".java",
                                     full, SLEELA_LANG_TARGET_SLJVM, 0u);
    CHECK(rc == SLEELA_LANG_OK, "re-register Java replaces");
    CHECK(sleela_lang_registry_count() == 3, "count unchanged after replace");

    /* Lookup by name is case-insensitive. */
    CHECK(sleela_lang_has("c") == 1, "find C case-insensitively");
    CHECK(sleela_lang_has("Fortran") == 0, "unregistered language absent");

    /* Extension resolution, including a multi-extension list and dot handling. */
    CHECK(strcmp(sleela_lang_resolve_extension("main.cpp"), "C++") == 0,
          "resolve .cpp -> C++");
    CHECK(strcmp(sleela_lang_resolve_extension("util.c"), "C") == 0,
          "resolve .c -> C");
    CHECK(strcmp(sleela_lang_resolve_extension("App.java"), "Java") == 0,
          "resolve .java -> Java");
    CHECK(strcmp(sleela_lang_resolve_extension("notes.txt"), "") == 0,
          "unclaimed extension resolves to empty");

    /* Plan a C compile to SLVM: fully covered => VM-ready. */
    int h = sleela_lang_plan_request("C", "util.c", SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(sleela_lang_plan_status(h) == SLEELA_LANG_OK, "C->SLVM plan OK");
    CHECK(sleela_lang_plan_vm_ready(h) == 1u, "C->SLVM vm-ready");
    CHECK(sleela_lang_plan_first_missing(h) == 0u, "C->SLVM no missing phase");

    /* A front end that stops at IR cannot reach SLVM: first missing is LOWER. */
    rc = sleela_lang_register_module("Partial", "demo", "0", ".pt",
            (1u << (SLEELA_LANG_PHASE_SOURCE   - 1u)) |
            (1u << (SLEELA_LANG_PHASE_LEX      - 1u)) |
            (1u << (SLEELA_LANG_PHASE_PARSE    - 1u)) |
            (1u << (SLEELA_LANG_PHASE_RESOLVE  - 1u)) |
            (1u << (SLEELA_LANG_PHASE_SEMANTIC - 1u)) |
            (1u << (SLEELA_LANG_PHASE_IR       - 1u)),
            SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(rc == SLEELA_LANG_OK, "register Partial (source..IR only)");
    h = sleela_lang_plan_request("Partial", "x.pt", SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(sleela_lang_plan_vm_ready(h) == 0u, "Partial->SLVM not vm-ready");
    CHECK(sleela_lang_plan_first_missing(h) == SLEELA_LANG_PHASE_LOWER,
          "Partial first missing phase is lower");
    /* But the same front end IS enough to reach SLIR. */
    h = sleela_lang_plan_request("Partial", "x.pt", SLEELA_LANG_TARGET_SLIR, 0u);
    CHECK(sleela_lang_plan_vm_ready(h) == 1u, "Partial->SLIR vm-ready");

    /* Strict request fails closed when not fully covered. */
    h = sleela_lang_plan_request("Partial", "x.pt", SLEELA_LANG_TARGET_SLVM, 1u);
    CHECK(sleela_lang_plan_status(h) == SLEELA_LANG_BLOCKED,
          "strict incomplete plan blocked");

    /* Reference-only module must not be asked to emit. */
    rc = sleela_lang_register_module("RefOnly", "demo", "0", ".ro",
                                     full, SLEELA_LANG_TARGET_SLVM, 1u);
    CHECK(rc == SLEELA_LANG_OK, "register reference-only module");
    h = sleela_lang_plan_request("RefOnly", "x.ro", SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(sleela_lang_plan_status(h) == SLEELA_LANG_BLOCKED,
          "reference-only emit blocked");

    /* An unknown module is absent, not guessed. */
    h = sleela_lang_plan_request("Nonesuch", "x.zz", SLEELA_LANG_TARGET_SLVM, 0u);
    CHECK(sleela_lang_plan_status(h) == SLEELA_LANG_ABSENT, "unknown module absent");

    /* Phase/target names round-trip to readable strings. */
    CHECK(strcmp(sleela_lang_phase_name(SLEELA_LANG_PHASE_CODEGEN), "codegen") == 0,
          "phase name codegen");
    CHECK(strcmp(sleela_lang_target_name(SLEELA_LANG_TARGET_SLJVM), "SLJVM") == 0,
          "target name SLJVM");

    printf("\n%s (%d failure%s)\n",
           g_failures == 0 ? "ALL PASS" : "FAILURES",
           g_failures, g_failures == 1 ? "" : "s");
    return g_failures == 0 ? 0 : 1;
}
