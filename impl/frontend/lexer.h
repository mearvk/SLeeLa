// ===========================================================================
// lexer.h  --  Tokenizer for the Java-like Sleela surface language.
// ===========================================================================
#ifndef SLEELA_LEXER_H
#define SLEELA_LEXER_H

#include <string>
#include <vector>

namespace sleela {

enum class Tok {
    // literals & names
    Int, Double, Str, Ident,
    // keywords
    KwClass, KwStatic, KwVoid, KwIntT, KwDoubleT, KwBoolT, KwStringT,
    KwIf, KwElse, KwWhile, KwFor, KwReturn, KwTrue, KwFalse, KwPrint, KwNull,
    // punctuation / operators
    LParen, RParen, LBrace, RBrace, Semicolon, Comma,
    Assign, Plus, Minus, Star, Slash, Percent,
    EqEq, NotEq, Lt, Le, Gt, Ge,
    AndAnd, OrOr, Not,
    // end
    Eof
};

struct Token {
    Tok         kind;
    std::string text;   // raw lexeme (or decoded string contents for Str)
    int         line;
    int         col;
};

class Lexer {
public:
    explicit Lexer(std::string src) : src_(std::move(src)) {}
    // Tokenize the whole input. Throws std::runtime_error on a lexical error.
    std::vector<Token> tokenize();

private:
    std::string src_;
    size_t pos_  = 0;
    int    line_ = 1;
    int    col_  = 1;

    char peek(int off = 0) const;
    char advance();
    bool match(char c);
    bool atEnd() const { return pos_ >= src_.size(); }
    void skipTrivia();                     // whitespace + // and /* */ comments
    Token makeNumber();
    Token makeString();
    Token makeIdentOrKeyword();
    [[noreturn]] void error(const std::string& msg) const;
};

const char* tokName(Tok t);   // for diagnostics

} // namespace sleela

#endif // SLEELA_LEXER_H
