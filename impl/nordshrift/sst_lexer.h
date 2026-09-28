// ===========================================================================
// sst_lexer.h  --  Spec-conformant tokenizer for Nordshrift .sst sheets.
//
// Implements NS-SST-0001 Part I (Lexical Conventions). The .sst format is
// indentation-significant: the lexer produces synthetic INDENT / DEDENT
// tokens (Part XIV, §14.1) so the parser can work over block structure without
// re-deriving indentation. One NEWLINE token terminates each logical line.
//
// This replaces the earlier brace-delimited component-language lexer; the .sst
// file is now a build-control sheet, not a program.
// ===========================================================================
#ifndef NORDSHRIFT_SST_LEXER_H
#define NORDSHRIFT_SST_LEXER_H

#include <string>
#include <vector>

#include "diagnostics.h"

namespace nordshrift {

enum class Tok {
    // structure
    Newline,      // end of a logical line
    Indent,       // block open  (one indent unit deeper)
    Dedent,       // block close
    // atoms
    Pragma,       // '#' + identifier   (text = pragma name, e.g. "nordshrift")
    Ident,        // identifier / bare keyword (keywords are contextual)
    String,       // "..." or """...""" (text = decoded contents)
    Integer,      // 1_000            (text = digits, separators removed)
    Version,      // 1.0.0 or 1.0     (text = raw)
    // punctuation
    Colon,        // ':'
    Comma,        // ','
    LBracket,     // '['
    RBracket,     // ']'
    // sentinel
    Eof
};

struct Token {
    Tok         kind;
    std::string text;      // lexeme / decoded value
    int         line;
    int         col;
    // For a directive/block line, the documentation comment (///) that
    // immediately preceded it, if any (Part I §1.3). Empty otherwise.
    std::string doc;
};

// Tokenize a .sst source. Lexical diagnostics (NSS-E-0001, NSS-E-0002,
// NSS-W-0001) are appended to `diags`. Returns the token stream (always
// terminated by Eof). On a fatal lexical error the returned stream may be
// partial; callers should check diags.hasErrors().
std::vector<Token> lex(const std::string& src, const std::string& file,
                       DiagnosticBag& diags);

const char* tokName(Tok t);

} // namespace nordshrift

#endif // NORDSHRIFT_SST_LEXER_H
