// ===========================================================================
// xlt_lexer.cpp  --  Implementation of the XLT (.xlt) tokenizer.
// ===========================================================================
#include "xlt_lexer.h"

#include <cctype>

namespace xlt {

const char* tokName(Tok t) {
    switch (t) {
        case Tok::Pragma:    return "pragma";
        case Tok::Ident:     return "identifier";
        case Tok::String:    return "string";
        case Tok::Integer:   return "integer";
        case Tok::Version:   return "version";
        case Tok::LBrace:    return "{";
        case Tok::RBrace:    return "}";
        case Tok::Semicolon: return ";";
        case Tok::Eof:       return "<eof>";
    }
    return "?";
}

namespace {
bool isIdentStart(char c) { return std::isalpha((unsigned char)c) || c == '_'; }
bool isIdentChar(char c)  { return std::isalnum((unsigned char)c) || c == '_'; }
}

std::vector<Token> lex(const std::string& src, const std::string& file,
                       DiagnosticBag& diags) {
    std::vector<Token> toks;
    size_t i = 0;
    int line = 1, col = 1;
    auto advance = [&](int n = 1) { while (n-- > 0) { if (src[i] == '\n') { line++; col = 1; } else { col++; } i++; } };

    while (i < src.size()) {
        char c = src[i];
        // whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { advance(); continue; }
        // line comment
        if (c == '/' && i + 1 < src.size() && src[i + 1] == '/') {
            while (i < src.size() && src[i] != '\n') advance();
            continue;
        }
        int startLine = line, startCol = col;
        // pragma: '#' + identifier
        if (c == '#') {
            advance();
            std::string name;
            while (i < src.size() && isIdentChar(src[i])) { name += src[i]; advance(); }
            toks.push_back(Token{ Tok::Pragma, name, startLine, startCol });
            continue;
        }
        // punctuation
        if (c == '{') { advance(); toks.push_back(Token{ Tok::LBrace, "{", startLine, startCol }); continue; }
        if (c == '}') { advance(); toks.push_back(Token{ Tok::RBrace, "}", startLine, startCol }); continue; }
        if (c == ';') { advance(); toks.push_back(Token{ Tok::Semicolon, ";", startLine, startCol }); continue; }
        // string
        if (c == '"') {
            advance();
            std::string out;
            while (i < src.size() && src[i] != '"') {
                if (src[i] == '\\' && i + 1 < src.size()) { advance(); out += src[i]; advance(); }
                else { out += src[i]; advance(); }
            }
            if (i >= src.size()) {
                diags.error("XLT-E-0001", file, startLine, "unterminated string literal", "XLT-LEX-01");
            } else { advance(); }
            toks.push_back(Token{ Tok::String, out, startLine, startCol });
            continue;
        }
        // number / version
        if (std::isdigit((unsigned char)c)) {
            // Radix-prefixed integer literals, matching the SLeeLa frontend
            // (SLEELA.syntax 1.4): 0x/0X hex, 0b/0B binary, 0o/0O octal, with
            // optional '_' digit separators. The prefix is kept in the token
            // text; the parser decodes the value. A non-prefixed run stays a
            // decimal integer, or a dotted Version (e.g. the #xlt 1.0 pragma).
            auto isHex = [](char d) {
                return (d >= '0' && d <= '9') || (d >= 'a' && d <= 'f') || (d >= 'A' && d <= 'F');
            };
            if (c == '0' && i + 1 < src.size() &&
                (src[i + 1] == 'x' || src[i + 1] == 'X' ||
                 src[i + 1] == 'b' || src[i + 1] == 'B' ||
                 src[i + 1] == 'o' || src[i + 1] == 'O')) {
                char radix = src[i + 1];
                std::string num; num += src[i]; num += radix; advance(2);
                bool any = false;
                while (i < src.size()) {
                    char d = src[i];
                    if (d == '_') { advance(); continue; }
                    bool ok = (radix == 'x' || radix == 'X') ? isHex(d)
                            : (radix == 'b' || radix == 'B') ? (d == '0' || d == '1')
                            : (d >= '0' && d <= '7');  // octal
                    if (!ok) break;
                    num += d; any = true; advance();
                }
                if (!any) {
                    diags.error("XLT-E-0003", file, startLine,
                                std::string("radix literal '") + num + "' has no digits", "XLT-LEX-03");
                }
                toks.push_back(Token{ Tok::Integer, num, startLine, startCol });
                continue;
            }
            std::string num; bool dotted = false;
            while (i < src.size() && (std::isdigit((unsigned char)src[i]) || src[i] == '.' || src[i] == '_')) {
                if (src[i] == '.') dotted = true;
                if (src[i] != '_') num += src[i];
                advance();
            }
            toks.push_back(Token{ dotted ? Tok::Version : Tok::Integer, num, startLine, startCol });
            continue;
        }
        // identifier / keyword
        if (isIdentStart(c)) {
            std::string id;
            while (i < src.size() && isIdentChar(src[i])) { id += src[i]; advance(); }
            toks.push_back(Token{ Tok::Ident, id, startLine, startCol });
            continue;
        }
        // unknown character
        diags.error("XLT-E-0002", file, startLine,
                    std::string("unexpected character '") + c + "'", "XLT-LEX-02");
        advance();
    }
    toks.push_back(Token{ Tok::Eof, "", line, col });
    return toks;
}

} // namespace xlt
