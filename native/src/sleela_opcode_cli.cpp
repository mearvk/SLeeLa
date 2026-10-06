/*
 * native/src/sleela_opcode_cli.cpp
 * SLeeLa Native Opcode Bridge — command adapter and self-test.
 *
 * Exercises the C ABI the /lib/opcodes classes bind to: the canonical table of
 * 103 mnemonics (codes 0..102, base 98 at 0..97), code validity, the fetch step,
 * and the single-dispatch step including OP_HALT and unknown-opcode rejection.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_opcode.h"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void check(const char *name, bool cond) {
    std::printf("[%s] %s\n", cond ? "pass" : "FAIL", name);
    if (!cond) ++failures;
}

int main(int argc, char **argv) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::printf("usage: sleela-opcode-native [--help]\n");
        std::printf("  runs the SLeeLa opcode bridge self-test (103 opcodes, fetch + dispatch-one)\n");
        return 0;
    }

    /* Count and bounds. */
    check("total count is 103", SLEELA_OPCODE_TOTAL_COUNT == 103);
    check("base count is 98", SLEELA_OPCODE_BASE_COUNT == 98);
    check("code 0 valid", sleela_opcode_is_valid(0) == 1);
    check("code 97 valid (last of base 98)", sleela_opcode_is_valid(97) == 1);
    check("code 102 valid (last overall)", sleela_opcode_is_valid(102) == 1);
    check("code -1 invalid", sleela_opcode_is_valid(-1) == 0);
    check("code 103 invalid", sleela_opcode_is_valid(103) == 0);

    /* Mnemonics at known positions. */
    check("code 0 is OP_NOP", std::strcmp(sleela_opcode_mnemonic(0), "OP_NOP") == 0);
    check("code 8 is OP_ADD", std::strcmp(sleela_opcode_mnemonic(8), "OP_ADD") == 0);
    check("code 97 is OP_AUDIO_PLATFORM",
          std::strcmp(sleela_opcode_mnemonic(97), "OP_AUDIO_PLATFORM") == 0);
    check("code 98 is OP_NEWARRAY",
          std::strcmp(sleela_opcode_mnemonic(98), "OP_NEWARRAY") == 0);
    check("code 102 is OP_ARRPUSH",
          std::strcmp(sleela_opcode_mnemonic(102), "OP_ARRPUSH") == 0);
    check("unknown mnemonic", std::strcmp(sleela_opcode_mnemonic(500), "OP_UNKNOWN") == 0);

    /* Fetch + single dispatch. */
    int32_t ip = sleela_opcode_vm_next(0);
    check("fetch returns non-negative ip", ip >= 0);
    int32_t next = sleela_opcode_execute_one(0, 8 /*OP_ADD*/, ip);
    check("execute OP_ADD advances ip", next == ip + 1);

    /* OP_HALT stops the stream. */
    int halt = -1;
    for (int i = 0; i < SLEELA_OPCODE_TOTAL_COUNT; ++i)
        if (std::strcmp(sleela_opcode_mnemonic(i), "OP_HALT") == 0) halt = i;
    check("OP_HALT found", halt >= 0);
    check("OP_HALT halts (returns -1)",
          sleela_opcode_execute_one(0, halt, ip) == -1);

    /* Unknown opcode rejected (not a no-op). */
    check("unknown opcode rejected",
          sleela_opcode_execute_one(0, 999, ip) == -1);

    /* Invalid frame / ip rejected. */
    check("invalid frame rejected", sleela_opcode_vm_next(-5) == -1);
    check("invalid ip rejected", sleela_opcode_execute_one(0, 0, -1) == -1);

    /* Running-helper signals (grouping / conditional-reactive layer). */
    check("signal INDEX passthrough",
          sleela_opcode_signal(10, SLEELA_OPCODE_SIG_INDEX, 3) == 3);
    int32_t s0 = sleela_opcode_signal(10, SLEELA_OPCODE_SIG_IP, 0);
    int32_t s1 = sleela_opcode_signal(10, SLEELA_OPCODE_SIG_IP, 1);
    check("signal IP advances", s1 > s0);
    check("signal lock depth defaults 0",
          sleela_opcode_signal(10, SLEELA_OPCODE_SIG_LOCK_DEPTH, 0) == 0);
    check("signal invalid frame rejected",
          sleela_opcode_signal(-1, SLEELA_OPCODE_SIG_IP, 0) == -1);
    check("signal unknown selector rejected",
          sleela_opcode_signal(0, 999, 0) == -1);

    /* Warming / pre-stage. */
    check("prestage valid opcode", sleela_opcode_prestage(0, 8 /*OP_ADD*/) == 1);
    check("prestage invalid opcode rejected", sleela_opcode_prestage(0, 999) == 0);
    check("prestage invalid frame rejected", sleela_opcode_prestage(-1, 0) == 0);

    std::printf("%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
