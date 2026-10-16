// ===========================================================================
// xlt_parser.h  --  Parser for XLT (.xlt) CPU-model sheets.
//
// Parses a token stream into an XltModel. The grammar is deliberately small:
//
//   sheet   := pragma model
//   pragma  := '#' 'xlt' VERSION
//   model   := 'model' IDENT '{' directive* '}'
//   directive := slot STRING [INTEGER] ';'   |   'target' IDENT ';'
//   slot    := 'cpu' | 'board' | 'memory' | 'cadence' | 'storage' | 'bus'
//
// Orthogonality is enforced here: declaring the same slot twice is a structured
// error (XLT-E-0020), not a silent override. Diagnostics accumulate; a parse
// with errors still returns a best-effort model so the resolver can add its own
// congruence reasons on top.
// ===========================================================================
#ifndef SLEELA_XLT_PARSER_H
#define SLEELA_XLT_PARSER_H

#include "xlt_lexer.h"
#include "xlt_model.h"
#include "xlt_diagnostics.h"

#include <vector>

namespace xlt {

XltModel parseModel(const std::vector<Token>& toks, const std::string& file,
                    DiagnosticBag& diags);

} // namespace xlt

#endif // SLEELA_XLT_PARSER_H
