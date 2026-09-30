// ===========================================================================
// lexer.h  --  Tokenizer for the Java-compatible Sleela surface language.
// ===========================================================================
#ifndef SLEELA_LEXER_H
#define SLEELA_LEXER_H
#include <string>
#include <vector>
namespace sleela {
enum class Tok {
    Int, Double, Str, Ident, At,
    KwClass, KwInterface, KwEnum, KwRecord,
    KwStatic, KwProtected, KwPublic, KwPrivate, KwFinal, KwAbstract, KwNative,
    KwSynchronized, KwVolatile, KwTransient, KwStrictfp, KwSealed, KwNonSealed,
    KwDefault, KwExtends, KwImplements, KwThrows,
    KwVoid, KwIntT, KwDoubleT, KwBoolT, KwStringT,
    KwIf, KwElse, KwWhile, KwFor, KwReturn, KwTrue, KwFalse, KwPrint, KwNull,
    KwImport, KwStruct, KwNew,
    LParen, RParen, LBrace, RBrace, LBracket, RBracket, Semicolon, Comma, Dot,
    Assign, Plus, Minus, Star, Slash, Percent,
    EqEq, NotEq, Lt, Le, Gt, Ge, AndAnd, OrOr, Not,
    Eof
};
struct Token { Tok kind; std::string text; int line; int col; };
class Lexer {
public:
    explicit Lexer(std::string src) : src_(std::move(src)) {}
    std::vector<Token> tokenize();
private:
    std::string src_; size_t pos_=0; int line_=1; int col_=1;
    char peek(int off=0) const; char advance(); bool match(char c);
    bool atEnd() const { return pos_ >= src_.size(); }
    void skipTrivia(); Token makeNumber(); Token makeString();
    Token makeIdentOrKeyword(); [[noreturn]] void error(const std::string& msg) const;
};
const char* tokName(Tok t);
bool isContextualBuiltin(const std::string& name);
} // namespace sleela
#endif // SLEELA_LEXER_H
