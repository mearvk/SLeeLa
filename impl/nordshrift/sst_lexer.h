// ===========================================================================
// sst_lexer.h  --  Tokenizer for Nordshrift .sst Style Sheets.
// ===========================================================================
#ifndef NORDSHRIFT_SST_LEXER_H
#define NORDSHRIFT_SST_LEXER_H

#include <string>
#include <vector>

namespace nordshrift {

enum class Tok {
    // literals & names
    Number, Str, Ident, True, False,
    // keywords (structural + attach language)
    KwComponent, KwUses, KwLet, KwIf, KwElse, KwWhile,
    KwPrint, KwCall, KwProp,
    // punctuation / operators
    LBrace, RBrace, LParen, RParen,
    Colon, Semicolon, Comma, Dot,
    Assign, Plus, Minus, Star, Slash, Percent,
    EqEq, NotEq, Lt, Le, Gt, Ge,
    AndAnd, OrOr, Not,
    Eof
};

struct Token {
    Tok         kind;
    std::string text;   // lexeme, or decoded string contents for Str
    int         line;
    int         col;
};

class Lexer {
public:
    explicit Lexer(std::string src) : src_(std::move(src)) {}
    std::vector<Token> tokenize();     // throws std::runtime_error on error

private:
    std::string src_;
    size_t pos_  = 0;
    int    line_ = 1;
    int    col_  = 1;

    char peek(int off = 0) const;
    char advance();
    bool match(char c);
    bool atEnd() const { return pos_ >= src_.size(); }
    void skipTrivia();
    Token makeNumber();
    Token makeString();
    Token makeIdentOrKeyword();
    [[noreturn]] void error(const std::string& msg) const;
};

const char* tokName(Tok t);

} // namespace nordshrift

#endif // NORDSHRIFT_SST_LEXER_H
