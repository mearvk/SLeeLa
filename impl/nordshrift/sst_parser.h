// ===========================================================================
// sst_parser.h  --  Recursive-descent parser: .sst tokens -> component graph.
// ===========================================================================
#ifndef NORDSHRIFT_SST_PARSER_H
#define NORDSHRIFT_SST_PARSER_H

#include "sst_ast.h"
#include "sst_lexer.h"

namespace nordshrift {

class Parser {
public:
    explicit Parser(std::vector<Token> toks) : toks_(std::move(toks)) {}
    Sheet parseSheet();     // throws std::runtime_error on syntax error

private:
    std::vector<Token> toks_;
    size_t i_ = 0;

    const Token& cur() const { return toks_[i_]; }
    const Token& peek(int off) const;
    bool check(Tok k) const { return cur().kind == k; }
    bool accept(Tok k);
    const Token& expect(Tok k, const char* what);
    [[noreturn]] void error(const std::string& msg) const;

    Component parseComponent();
    // member name may be hyphenated: IDENT ('-' IDENT)*
    std::string parseMemberName();
    PropValue parseScalar();

    std::unique_ptr<Block> parseBlock();
    StmtP parseStmt();

    ExprP parseExpr();
    ExprP parseOr();
    ExprP parseAnd();
    ExprP parseEq();
    ExprP parseRel();
    ExprP parseAdd();
    ExprP parseMul();
    ExprP parseUnary();
    ExprP parsePrimary();
};

} // namespace nordshrift

#endif // NORDSHRIFT_SST_PARSER_H
