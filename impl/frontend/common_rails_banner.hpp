// ===========================================================================
// common_rails_banner.hpp -- route toolchain banners through CommonRails.
// ===========================================================================
// A thin adapter over the native CommonRails "heritage" printing layer
// (common-rails/heritage/printing/cpp/CommonRailsPrinting.hpp), so the four
// SLeeLa output producers -- the compiler (Sleelvac), the Nordshrift
// transpiler, running programs, and the debugger -- share ONE professional
// printing surface instead of ad-hoc std::cout lines. This mirrors the
// Sleela-side /lib/common-rails PrintRail class: the same component lines,
// state-labelled status lines, progress square, and 21x21 cell model.
//
// CONTRACT PRESERVATION: several banner lines are machine-parsed by the test
// suite and by downstream tooling (e.g. "sleelvac: <src> -> <dst> (runnable
// Sleela Core artifact)", Nordshrift's "0 error(s)" / "OK: target-language").
// The helpers here are OPT-IN decoration for human-progress banners; the exact
// contract strings are emitted verbatim by their call sites and are NOT routed
// through a formatter that could change their text. Where a helper does wrap a
// contract token, it embeds that token unchanged.
//
// The CommonRails state vocabulary (Beautiful Design): START, WORKING,
// PROGRESS, COMPLETE, WARN, ERROR.
#ifndef SLEELA_COMMON_RAILS_BANNER_H
#define SLEELA_COMMON_RAILS_BANNER_H

#include "CommonRailsPrinting.hpp"   // commonrails::printing (heritage layer)
#include <ostream>
#include <iostream>
#include <string>
#include <cstdio>
#if defined(_WIN32)
#  include <io.h>
#  define SLEELA_ISATTY(fd) _isatty(fd)
#else
#  include <unistd.h>
#  define SLEELA_ISATTY(fd) isatty(fd)
#endif

namespace sleela { namespace rails {

using commonrails::printing::PrintState;
using commonrails::printing::PrintComponent;
using commonrails::printing::PrintFormatter;
using commonrails::printing::PrintRenderer;
using commonrails::printing::PrintProgress;
using commonrails::printing::PrintLayout;

// ---------------------------------------------------------------------------
// Product brand line.
//
// Prints the tool's trademarked product name (e.g. "Sleelvac(TM)") on its own
// line, in RED + CLASSIC (ANSI SGR 31 -- normal-intensity red, no bold/bright),
// then resets so the tool's natural output below stays the terminal default
// (white/classic). It is the first thing a user sees when they compile or run.
//
// COLOR IS PRESENTATION-ONLY (Beautiful Design sec 7: "Color ... Optional; it
// does not change width semantics" / "readable when color is unavailable").
// The whole line is therefore TTY-GATED: it is emitted ONLY when the target
// stream is an interactive terminal. Under a pipe, a redirect, or test capture
// (every harness uses $(... 2>&1)), nothing is emitted -- so machine-parsed
// output is byte-for-byte unchanged and no escape codes ever leak into logs.
//
// `streamFd` is the destination's file descriptor: 1 (stdout) for std::cout,
// 2 (stderr) for std::cerr. The brand goes to the same stream the natural
// output will use, so the two are never split across stdout/stderr.
inline bool brandColorEnabled(int streamFd) {
    return SLEELA_ISATTY(streamFd) != 0;
}
inline void brand(std::ostream& o, int streamFd, const std::string& product) {
    if (!brandColorEnabled(streamFd)) return;               // plain contexts: no brand line
    o << "\033[31m" << product << "\u2122" << "\033[0m\n";   // red + classic, then reset
}
// Convenience: the product identities, to stdout (fd 1) or stderr (fd 2).
inline void brandSleelvac(std::ostream& o, int fd)   { brand(o, fd, "Sleelvac"); }
inline void brandSleela(std::ostream& o, int fd)     { brand(o, fd, "Sleela"); }
inline void brandNordshrift(std::ostream& o, int fd) { brand(o, fd, "Nordshrift"); }
// Advantage(TM): the /lib/advantage library family's product identity, rendered
// in the same customary red + classic as Sleela and Sleelvac. TTY-gated, so it
// decorates interactive output only and never pollutes machine-parsed logs.
inline void brandAdvantage(std::ostream& o, int fd)  { brand(o, fd, "Advantage"); }

// A professional component line for a tool/unit currently being worked on:
//   -- : [Object ID: 0000000001] [Date: <date>] [Current: @<name>] . <msg> .
// `name` is the tool or unit (e.g. "Sleelvac", "Nordshrift", or a source file).
inline void component(std::ostream& o, const std::string& name,
                      unsigned long objectId, unsigned long date,
                      const std::string& message) {
    PrintComponent::write(o, name, objectId, date, message);
}

// A state-labelled status line: "[STATE] message", laid out to the rail width.
inline void status(std::ostream& o, PrintState state, const std::string& message) {
    PrintFormatter::status(o, state, message);
}

// Convenience state shims so call sites read intention-first.
inline void start(std::ostream& o, const std::string& m)    { status(o, PrintState::START, m); }
inline void working(std::ostream& o, const std::string& m)  { status(o, PrintState::WORKING, m); }
inline void progress(std::ostream& o, const std::string& m) { status(o, PrintState::PROGRESS, m); }
inline void complete(std::ostream& o, const std::string& m) { status(o, PrintState::COMPLETE, m); }
inline void warn(std::ostream& o, const std::string& m)     { status(o, PrintState::WARN, m); }
inline void error(std::ostream& o, const std::string& m)    { status(o, PrintState::ERROR, m); }

// The 21x21 progress square (441 cells) and its percent header.
inline void square(std::ostream& o, int filledCells) { PrintRenderer::square(o, filledCells); }
inline void progressPercent(std::ostream& o, int percent) { PrintProgress::write(o, percent); }

}} // namespace sleela::rails

#endif // SLEELA_COMMON_RAILS_BANNER_H
