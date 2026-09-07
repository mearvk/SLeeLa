// ===========================================================================
// sleela_emit.h  --  Pipeline backend: transpile a Sleela Program into a target.
//
// This is the emission stage of the Nordshrift pipeline. Given a parsed Sleela
// program (from the shared Sleela front end) it renders the selected triplet
// target as source text:
//   TargetLang::Java   -> idiomatic Java (class + static methods; TGT-02)
//   TargetLang::Sleela -> canonical Sleela (pass-through pretty-print)
//   TargetLang::C      -> C with a tiny tagged-value runtime
//
// The Sleela target is additionally runnable on the C core (see the driver).
// ===========================================================================
#ifndef NORDSHRIFT_SLEELA_EMIT_H
#define NORDSHRIFT_SLEELA_EMIT_H

#include <string>

#include "sheet_model.h"
#include "../frontend/ast.h"

namespace nordshrift {

// Emit `prog` (a whole compilation unit) into the language named by `lang`.
// `packageRoot` (may be empty) is used as the Java package declaration.
std::string emitProgram(const sleela::Program& prog, TargetLang lang,
                        const std::string& packageRoot);

} // namespace nordshrift

#endif // NORDSHRIFT_SLEELA_EMIT_H
