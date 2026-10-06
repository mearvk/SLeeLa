/*
 * native/src/sleela_gov.cpp
 * SLeeLa Native Opcode-Governance Bridge — C++17 implementation of
 * include/sleela_gov.h.
 *
 * Reference bridge for the lib/opcodes/governance classes. It records phase
 * announcements so SLeeLa and the VM both observe the BEFORE / DURING / AFTER
 * ordering, and reports a monotonic live instruction pointer during execution.
 *
 * The authoritative execution semantics live in /impl/core; this bridge never
 * executes an opcode. It only tracks phases and the live ip.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_gov.h"

#include <string>

namespace {

thread_local std::string g_note;
thread_local int g_current_phase = 0;
thread_local int32_t g_before_count = 0;
thread_local int32_t g_during_count = 0;
thread_local int32_t g_after_count = 0;

bool valid_phase(int32_t p) {
    return p == SLEELA_GOV_PHASE_BEFORE
        || p == SLEELA_GOV_PHASE_DURING
        || p == SLEELA_GOV_PHASE_AFTER;
}

} /* namespace */

extern "C" {

int sleela_gov_announce_phase(int32_t phase) {
    if (!valid_phase(phase)) {
        g_note = "unknown governance phase";
        return 0;
    }
    g_current_phase = phase;
    if (phase == SLEELA_GOV_PHASE_BEFORE) { ++g_before_count; g_note = "BEFORE: registrar considering A..B"; }
    else if (phase == SLEELA_GOV_PHASE_DURING) { ++g_during_count; g_note = "DURING: listener attached to live VM code"; }
    else { ++g_after_count; g_note = "AFTER: event observer judging the process"; }
    return 1;
}

int32_t sleela_gov_live_ip(int32_t frame, int32_t expected_index) {
    if (frame < 0 || expected_index < 0) {
        g_note = "invalid frame or index";
        return -1;
    }
    /* Reference model: the live ip is the frame cursor plus the position in the
     * admitted sequence, which advances monotonically as the Listener hears each
     * opcode. /impl/core owns the authoritative pointer. */
    return frame + expected_index;
}

int sleela_gov_current_phase(void) { return g_current_phase; }

int32_t sleela_gov_phase_count(int32_t phase) {
    if (phase == SLEELA_GOV_PHASE_BEFORE) return g_before_count;
    if (phase == SLEELA_GOV_PHASE_DURING) return g_during_count;
    if (phase == SLEELA_GOV_PHASE_AFTER) return g_after_count;
    return 0;
}

const char *sleela_gov_last_note(void) { return g_note.c_str(); }

} /* extern "C" */
