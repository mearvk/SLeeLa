/*
 * native/src/sleela_opcode.cpp
 * SLeeLa Native Opcode Bridge — C++17 implementation of include/sleela_opcode.h.
 *
 * Reference bridge for the /lib/opcodes single-opcode classes. It carries the
 * canonical 103-entry mnemonic table (codes 0..102) and models the fetch
 * (sleela_opcode_vm_next) and single dispatch (sleela_opcode_execute_one) steps
 * against a lightweight frame-pointer abstraction.
 *
 * The authoritative execution semantics live in /impl/core; this bridge
 * validates the opcode and advances the pointer the same way, rejecting unknown
 * opcodes rather than treating them as no-ops.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_opcode.h"

#include <cstring>
#include <string>

namespace {

thread_local std::string g_last_error;

void set_error(const char *m) { g_last_error = m ? m : ""; }

/* Canonical mnemonics in code order 0..102, matching lib/vm/InstructionSet.sleela
 * and lib/vm/OPCODE-MAP.md exactly. */
const char *const kMnemonics[SLEELA_OPCODE_TOTAL_COUNT] = {
    "OP_NOP", "OP_CONST", "OP_POP", "OP_DUP", "OP_LOADG", "OP_STOREG",
    "OP_LOADL", "OP_STOREL", "OP_ADD", "OP_SUB", "OP_MUL", "OP_DIV", "OP_MOD",
    "OP_NEG", "OP_EQ", "OP_NE", "OP_LT", "OP_LE", "OP_GT", "OP_GE", "OP_AND",
    "OP_OR", "OP_NOT", "OP_JMP", "OP_JMPF", "OP_CALL", "OP_RET", "OP_PRINT",
    "OP_SPAWN", "OP_JOINALL", "OP_LOCK", "OP_UNLOCK", "OP_SEND", "OP_RECV",
    "OP_LISTEN", "OP_ACCEPT", "OP_CONNECT", "OP_SOCKREAD", "OP_SOCKWRITE",
    "OP_SOCKCLOSE", "OP_PIPE", "OP_PIPEPEER", "OP_FIFO_MK", "OP_FILEOPEN",
    "OP_FILEREAD", "OP_FILEWRITE", "OP_FILECLOSE", "OP_FILEUNLINK", "OP_HALT",
    "OP_TIME_UTC_MS", "OP_TIME_UTC_NS", "OP_TIME_MONO_NS", "OP_TIME_PRECISION_MS",
    "OP_TIME_LOCATION", "OP_TIME_HTTP_DATE", "OP_TIME_JSON", "OP_TIME_NTP",
    "OP_TIME_SET_LOCATION", "OP_NEWSTRUCT", "OP_GETFIELD", "OP_SETFIELD",
    "OP_STRUCTPACK", "OP_STRUCTUNPACK", "OP_SYN_OPEN", "OP_SYN_DISPATCH",
    "OP_SYN_STAT", "OP_SYN_REPORT", "OP_SYN_CLOSE", "OP_MUN_START",
    "OP_MUN_CONNECT", "OP_MUN_ENABLE", "OP_MUN_SEND", "OP_MUN_THATCH",
    "OP_MUN_CONSUME", "OP_MUN_LATCH", "OP_MUN_RECEPTION", "OP_MUN_CLOSE",
    "OP_BEST_NEW", "OP_BEST_WEIGHT", "OP_BEST_MINVER", "OP_BEST_BUDGET",
    "OP_BEST_CAND", "OP_BEST_RECORD", "OP_BEST_SCORE", "OP_BEST_BEST",
    "OP_BEST_STAT", "OP_BEST_CHOICE", "OP_BEST_REPORT", "OP_BEST_ARCH",
    "OP_BEST_ARCH_STATE", "OP_BEST_CLOSE", "OP_AUDIO_NEW", "OP_AUDIO_ADD",
    "OP_AUDIO_CONTROLS", "OP_AUDIO_VALIDATE", "OP_AUDIO_RENDER", "OP_AUDIO_CLOSE",
    "OP_AUDIO_PLATFORM", "OP_NEWARRAY", "OP_ARRGET", "OP_ARRSET", "OP_ARRLEN",
    "OP_ARRPUSH"
};

/* OP_HALT's canonical code (index in the table above). */
int halt_code() {
    for (int i = 0; i < SLEELA_OPCODE_TOTAL_COUNT; ++i)
        if (std::strcmp(kMnemonics[i], "OP_HALT") == 0) return i;
    return -1;
}

} /* namespace */

extern "C" {

int sleela_opcode_is_valid(int32_t code) {
    return (code >= SLEELA_OPCODE_MIN_CODE && code <= SLEELA_OPCODE_MAX_CODE) ? 1 : 0;
}

const char *sleela_opcode_mnemonic(int32_t code) {
    if (!sleela_opcode_is_valid(code)) return "OP_UNKNOWN";
    return kMnemonics[code];
}

int32_t sleela_opcode_vm_next(int32_t frame) {
    if (frame < 0) { set_error("invalid VM frame"); return -1; }
    /* Fetch step: the pointer is advanced. The reference bridge models the ip as
     * a simple monotonic counter seeded from the frame handle; /impl/core owns
     * the authoritative pointer. We return the pointer the executed opcode will
     * run at (frame-relative, non-negative). */
    g_last_error.clear();
    return frame; /* next instruction pointer == current frame cursor */
}

int32_t sleela_opcode_execute_one(int32_t frame, int32_t code, int32_t ip) {
    if (frame < 0) { set_error("invalid VM frame"); return -1; }
    if (ip < 0) { set_error("invalid instruction pointer"); return -1; }
    if (!sleela_opcode_is_valid(code)) {
        /* Canonical map rule: unknown opcodes are rejected, not no-ops. */
        set_error("unsupported opcode");
        return -1;
    }
    if (code == halt_code()) {
        /* OP_HALT stops the stream: no next instruction. */
        return -1;
    }
    /* Execute exactly this one opcode (delegated to /impl/core in a full build),
     * then report the next instruction pointer. */
    return ip + 1;
}

const char *sleela_opcode_last_error(void) { return g_last_error.c_str(); }

} /* extern "C" */
