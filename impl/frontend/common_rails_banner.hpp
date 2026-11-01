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
#include <string>

namespace sleela { namespace rails {

using commonrails::printing::PrintState;
using commonrails::printing::PrintComponent;
using commonrails::printing::PrintFormatter;
using commonrails::printing::PrintRenderer;
using commonrails::printing::PrintProgress;
using commonrails::printing::PrintLayout;

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
