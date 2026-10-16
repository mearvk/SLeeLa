// ===========================================================================
// xlt_lexer.h  --  Tokenizer for XLT (.xlt) CPU-model sheets.
//
// Unlike the indentation-significant .sst lexer, .xlt is brace-delimited
// (model Name { ... }), so the token set is small and C-like: a `#xlt` pragma,
// identifiers, double-quoted strings, integers, a dotted version, and the
// punctuation { } ;. Comments are `//` to end of line. Whitespace/newlines are
// insignificant between tokens.
// ===========================================================================
#ifndef SLEELA_XLT_LEXER_H
#define SLEELA_XLT_LEXER_H

#include <string>
#include <vector>

#include "xlt_diagnostics.h"

namespace xlt {

enum class Tok {
    Pragma,    // '#' + identifier  (text = pragma name, e.g. "xlt")
    Ident,     // identifier / bare keyword (keywords are contextual)
    String,    // "..."  (text = decoded contents)
    Integer,   // 4096   (text = digits)
    Version,   // 1.0 / 1.0.0 (text = raw)
    LBrace,    // '{'
    RBrace,    // '}'
    Semicolon, // ';'
    Eof
};

struct Token {
    Tok         kind;
    std::string text;
    int         line;
    int         col;
};

std::vector<Token> lex(const std::string& src, const std::string& file,
                       DiagnosticBag& diags);

const char* tokName(Tok t);

} // namespace xlt

#endif // SLEELA_XLT_LEXER_H
