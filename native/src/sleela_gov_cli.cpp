/*
 * native/src/sleela_gov_cli.cpp
 * SLeeLa Native Opcode-Governance Bridge — command adapter and self-test.
 *
 * Exercises the C ABI the lib/opcodes/governance classes bind to: phase
 * announcement for BEFORE / DURING / AFTER, phase counting, current-phase
 * tracking, and the monotonic live-ip report.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_gov.h"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void check(const char *name, bool cond) {
    std::printf("[%s] %s\n", cond ? "pass" : "FAIL", name);
    if (!cond) ++failures;
}

int main(int argc, char **argv) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::printf("usage: sleela-gov-native [--help]\n");
        std::printf("  runs the SLeeLa opcode-governance bridge self-test (BEFORE/DURING/AFTER)\n");
        return 0;
    }

    /* Phase announcements in order. */
    check("announce BEFORE", sleela_gov_announce_phase(SLEELA_GOV_PHASE_BEFORE) == 1);
    check("current phase is BEFORE", sleela_gov_current_phase() == SLEELA_GOV_PHASE_BEFORE);
    check("announce DURING", sleela_gov_announce_phase(SLEELA_GOV_PHASE_DURING) == 1);
    check("current phase is DURING", sleela_gov_current_phase() == SLEELA_GOV_PHASE_DURING);
    check("announce AFTER", sleela_gov_announce_phase(SLEELA_GOV_PHASE_AFTER) == 1);
    check("current phase is AFTER", sleela_gov_current_phase() == SLEELA_GOV_PHASE_AFTER);

    /* Unknown phase rejected. */
    check("unknown phase rejected", sleela_gov_announce_phase(99) == 0);

    /* Phase counts. */
    check("BEFORE counted once", sleela_gov_phase_count(SLEELA_GOV_PHASE_BEFORE) == 1);
    check("DURING counted once", sleela_gov_phase_count(SLEELA_GOV_PHASE_DURING) == 1);
    check("AFTER counted once", sleela_gov_phase_count(SLEELA_GOV_PHASE_AFTER) == 1);

    /* Live ip is monotonic and non-negative; invalid inputs rejected. */
    int32_t ip0 = sleela_gov_live_ip(10, 0);
    int32_t ip1 = sleela_gov_live_ip(10, 1);
    check("live ip non-negative", ip0 >= 0);
    check("live ip advances", ip1 > ip0);
    check("invalid frame rejected", sleela_gov_live_ip(-1, 0) == -1);
    check("invalid index rejected", sleela_gov_live_ip(0, -1) == -1);

    std::printf("%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
