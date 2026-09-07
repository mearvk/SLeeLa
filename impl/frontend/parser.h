// ===========================================================================
// parser.h  --  Recursive-descent parser: tokens -> Sleela AST.
// ===========================================================================
#ifndef SLEELA_PARSER_H
#define SLEELA_PARSER_H

#include "ast.h"
#include "lexer.h"

namespace sleela {

class Parser {
public:
    explicit Parser(std::vector<Token> toks) : toks_(std::move(toks)) {}
    Program parseProgram();      // throws std::runtime_error on syntax error

private:
    std::vector<Token> toks_;
    size_t i_ = 0;

    const Token& peek(int off = 0) const;
    const Token& cur() const { return toks_[i_]; }
    bool check(Tok k) const { return cur().kind == k; }
    bool accept(Tok k);
    const Token& expect(Tok k, const char* what);
    [[noreturn]] void error(const std::string& msg) const;

    // declarations
    ClassDecl parseClass();
    Method    parseMethod();
    bool      isTypeTok(Tok k) const;
    std::string parseType();

    // statements
    StmtP parseStatement();
    std::unique_ptr<Block> parseBlock();
    StmtP parseSimpleStatement();   // decl / assign / expr (no trailing ;)

    // expressions (precedence climbing)
    ExprP parseExpr();
    ExprP parseOr();
    ExprP parseAnd();
    ExprP parseEquality();
    ExprP parseComparison();
    ExprP parseAdditive();
    ExprP parseMultiplicative();
    ExprP parseUnary();
    ExprP parsePrimary();
};

} // namespace sleela

#endif // SLEELA_PARSER_H
