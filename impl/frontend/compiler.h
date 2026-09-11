// ===========================================================================
// compiler.h  --  Sleelvac™ lowers the Sleela AST into Sleela Core bytecode.
//
// The compiler owns semantic lowering, not execution. It walks the AST and
// drives the C core builder API. The resulting VM program can either be run
// immediately or persisted as a runnable .sleela artifact by artifact.h.
// ===========================================================================
#ifndef SLEELA_COMPILER_H
#define SLEELA_COMPILER_H

#include "ast.h"
#include "version.h"
#include "../catalog/sheet_catalog.h"

extern "C" {
#include "../core/sleela_core.h"
}

namespace sleela {

// Compile `prog` into `vm`. Returns the entry function index (the one holding
// `main`). Throws std::runtime_error on a semantic error (unknown variable,
// unknown function, missing main, etc.).
//
// `cat` (optional) is the SHEET.sheet catalog that backs Sleela's conducted
// methods -- the sheet-derived built-ins insight()/role()/route()/congruent()/
// conduct()/sysdepth()/degreemax(). When null, those built-ins still compile
// but resolve against an empty catalog.
//
// This is the common lowering stage used by both immediate execution and the
// persistent .sleela artifact path; there is no second semantic compiler in
// the runtime artifact loader.
int compile(const Program& prog, SLVM* vm, const catalog::Catalog* cat = nullptr,
            const SyntaxVersion& syntax = SyntaxVersion{1, 0});

} // namespace sleela

#endif // SLEELA_COMPILER_H
