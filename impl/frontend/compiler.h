// ===========================================================================
// compiler.h  --  Lowers the Sleela AST into Sleela Core bytecode.
//
// The compiler owns no execution logic. It walks the AST and drives the C
// core purely through the builder helpers declared in sleela_core.h (which
// are themselves wrappers over slcore_exchange). The result is a program
// loaded into the SLVM, ready for slvm_run().
// ===========================================================================
#ifndef SLEELA_COMPILER_H
#define SLEELA_COMPILER_H

#include "ast.h"
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
int compile(const Program& prog, SLVM* vm, const catalog::Catalog* cat = nullptr);

} // namespace sleela

#endif // SLEELA_COMPILER_H
