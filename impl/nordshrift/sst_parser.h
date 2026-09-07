// ===========================================================================
// sst_parser.h  --  Spec-conformant .sst parser (NS-SST-0001 Part XIV).
//
// Consumes the INDENT/DEDENT token stream from the lexer and builds a Sheet.
// Structural and semantic diagnostics (NSS-E-0003, 0004, 0010, 0040, 0050,
// 0051, 0080, 0110, ...) are appended to the DiagnosticBag.
// ===========================================================================
#ifndef NORDSHRIFT_SST_PARSER_H
#define NORDSHRIFT_SST_PARSER_H

#include "sheet_model.h"
#include "sst_lexer.h"
#include "diagnostics.h"

namespace nordshrift {

// Parse a token stream into a Sheet. Always returns a Sheet (possibly partial);
// callers should consult diags.hasErrors().
Sheet parseSheet(const std::vector<Token>& toks, const std::string& file,
                 DiagnosticBag& diags);

} // namespace nordshrift

#endif // NORDSHRIFT_SST_PARSER_H
