// ===========================================================================
// parser.cpp  --  Recursive-descent parser for the Java-like Sleela subset.
// ===========================================================================
#include "parser.h"

#include <stdexcept>

namespace sleela {

const Token& Parser::peek(int off) const {
    size_t p = i_ + (size_t)off;
    if (p >= toks_.size()) return toks_.back(); // Eof
    return toks_[p];
}

bool Parser::accept(Tok k) {
    if (check(k)) { i_++; return true; }
    return false;
}

const Token& Parser::expect(Tok k, const char* what) {
    if (!check(k)) {
        error(std::string("expected ") + what + " but found '" +
              (cur().text.empty() ? tokName(cur().kind) : cur().text) + "'");
    }
    return toks_[i_++];
}

void Parser::error(const std::string& msg) const {
    const Token& t = cur();
    throw std::runtime_error("Syntax error (line " + std::to_string(t.line) +
                             ", col " + std::to_string(t.col) + "): " + msg);
}

Program Parser::parseProgram() {
    Program prog;
    while (!check(Tok::Eof)) {
        if (check(Tok::KwClass)) {
            prog.classes.push_back(parseClass());
        } else {
            error("expected 'class' at top level");
        }
    }
    if (prog.classes.empty()) error("program contains no classes");
    return prog;
}

ClassDecl Parser::parseClass() {
    expect(Tok::KwClass, "'class'");
    ClassDecl c;
    c.name = expect(Tok::Ident, "class name").text;
    expect(Tok::LBrace, "'{'");
    while (!check(Tok::RBrace) && !check(Tok::Eof)) {
        c.methods.push_back(parseMethod());
    }
    expect(Tok::RBrace, "'}'");
    return c;
}

bool Parser::isTypeTok(Tok k) const {
    return k == Tok::KwVoid || k == Tok::KwIntT || k == Tok::KwDoubleT ||
           k == Tok::KwBoolT || k == Tok::KwStringT;
}

std::string Parser::parseType() {
    const Token& t = cur();
    if (!isTypeTok(t.kind)) error("expected a type");
    i_++;
    return t.text;
}

Method Parser::parseMethod() {
    accept(Tok::KwStatic);                 // 'static' optional (all methods static here)
    Method m;
    m.retType = parseType();
    m.name = expect(Tok::Ident, "method name").text;
    expect(Tok::LParen, "'('");
    if (!check(Tok::RParen)) {
        do {
            Param p;
            p.type = parseType();
            p.name = expect(Tok::Ident, "parameter name").text;
            m.params.push_back(p);
        } while (accept(Tok::Comma));
    }
    expect(Tok::RParen, "')'");
    m.body = parseBlock();
    return m;
}

std::unique_ptr<Block> Parser::parseBlock() {
    expect(Tok::LBrace, "'{'");
    auto b = std::make_unique<Block>();
    while (!check(Tok::RBrace) && !check(Tok::Eof)) {
        b->stmts.push_back(parseStatement());
    }
    expect(Tok::RBrace, "'}'");
    return b;
}

StmtP Parser::parseStatement() {
    if (check(Tok::LBrace)) return parseBlock();

    if (accept(Tok::KwIf)) {
        auto s = std::make_unique<IfStmt>();
        expect(Tok::LParen, "'('");
        s->cond = parseExpr();
        expect(Tok::RParen, "')'");
        s->thenS = parseStatement();
        if (accept(Tok::KwElse)) s->elseS = parseStatement();
        return s;
    }
    if (accept(Tok::KwWhile)) {
        auto s = std::make_unique<WhileStmt>();
        expect(Tok::LParen, "'('");
        s->cond = parseExpr();
        expect(Tok::RParen, "')'");
        s->body = parseStatement();
        return s;
    }
    if (accept(Tok::KwFor)) {
        auto s = std::make_unique<ForStmt>();
        expect(Tok::LParen, "'('");
        if (!check(Tok::Semicolon)) s->init = parseSimpleStatement();
        expect(Tok::Semicolon, "';'");
        if (!check(Tok::Semicolon)) s->cond = parseExpr();
        expect(Tok::Semicolon, "';'");
        if (!check(Tok::RParen)) s->update = parseSimpleStatement();
        expect(Tok::RParen, "')'");
        s->body = parseStatement();
        return s;
    }
    if (accept(Tok::KwReturn)) {
        auto s = std::make_unique<ReturnStmt>();
        if (!check(Tok::Semicolon)) s->value = parseExpr();
        expect(Tok::Semicolon, "';'");
        return s;
    }
    if (accept(Tok::KwPrint)) {
        auto s = std::make_unique<PrintStmt>();
        expect(Tok::LParen, "'('");
        s->expr = parseExpr();
        expect(Tok::RParen, "')'");
        expect(Tok::Semicolon, "';'");
        return s;
    }

    StmtP s = parseSimpleStatement();
    expect(Tok::Semicolon, "';'");
    return s;
}

// A simple statement without the trailing ';' (used in for-headers too).
StmtP Parser::parseSimpleStatement() {
    // typed local declaration:  <type> ident [= expr]
    if (isTypeTok(cur().kind)) {
        auto d = std::make_unique<VarDecl>();
        d->type = parseType();
        d->name = expect(Tok::Ident, "variable name").text;
        if (accept(Tok::Assign)) d->init = parseExpr();
        return d;
    }
    // assignment:  ident = expr    (lookahead)
    if (check(Tok::Ident) && peek(1).kind == Tok::Assign) {
        auto a = std::make_unique<Assign>();
        a->name = cur().text; i_++;   // ident
        expect(Tok::Assign, "'='");
        a->value = parseExpr();
        return a;
    }
    // otherwise an expression statement (e.g. a call)
    auto e = std::make_unique<ExprStmt>();
    e->expr = parseExpr();
    return e;
}

// ---- expressions: precedence climbing ------------------------------------
ExprP Parser::parseExpr()      { return parseOr(); }

ExprP Parser::parseOr() {
    ExprP e = parseAnd();
    while (check(Tok::OrOr)) { i_++; e = std::make_unique<Binary>("||", std::move(e), parseAnd()); }
    return e;
}
ExprP Parser::parseAnd() {
    ExprP e = parseEquality();
    while (check(Tok::AndAnd)) { i_++; e = std::make_unique<Binary>("&&", std::move(e), parseEquality()); }
    return e;
}
ExprP Parser::parseEquality() {
    ExprP e = parseComparison();
    for (;;) {
        if (check(Tok::EqEq)) { i_++; e = std::make_unique<Binary>("==", std::move(e), parseComparison()); }
        else if (check(Tok::NotEq)) { i_++; e = std::make_unique<Binary>("!=", std::move(e), parseComparison()); }
        else break;
    }
    return e;
}
ExprP Parser::parseComparison() {
    ExprP e = parseAdditive();
    for (;;) {
        if (check(Tok::Lt)) { i_++; e = std::make_unique<Binary>("<",  std::move(e), parseAdditive()); }
        else if (check(Tok::Le)) { i_++; e = std::make_unique<Binary>("<=", std::move(e), parseAdditive()); }
        else if (check(Tok::Gt)) { i_++; e = std::make_unique<Binary>(">",  std::move(e), parseAdditive()); }
        else if (check(Tok::Ge)) { i_++; e = std::make_unique<Binary>(">=", std::move(e), parseAdditive()); }
        else break;
    }
    return e;
}
ExprP Parser::parseAdditive() {
    ExprP e = parseMultiplicative();
    for (;;) {
        if (check(Tok::Plus)) { i_++; e = std::make_unique<Binary>("+", std::move(e), parseMultiplicative()); }
        else if (check(Tok::Minus)) { i_++; e = std::make_unique<Binary>("-", std::move(e), parseMultiplicative()); }
        else break;
    }
    return e;
}
ExprP Parser::parseMultiplicative() {
    ExprP e = parseUnary();
    for (;;) {
        if (check(Tok::Star)) { i_++; e = std::make_unique<Binary>("*", std::move(e), parseUnary()); }
        else if (check(Tok::Slash)) { i_++; e = std::make_unique<Binary>("/", std::move(e), parseUnary()); }
        else if (check(Tok::Percent)) { i_++; e = std::make_unique<Binary>("%", std::move(e), parseUnary()); }
        else break;
    }
    return e;
}
ExprP Parser::parseUnary() {
    if (check(Tok::Minus)) { i_++; return std::make_unique<Unary>("-", parseUnary()); }
    if (check(Tok::Not))   { i_++; return std::make_unique<Unary>("!", parseUnary()); }
    return parsePrimary();
}
ExprP Parser::parsePrimary() {
    const Token& t = cur();
    switch (t.kind) {
        case Tok::Int:    i_++; return std::make_unique<IntLit>(std::stoll(t.text));
        case Tok::Double: i_++; return std::make_unique<DoubleLit>(std::stod(t.text));
        case Tok::Str:    i_++; return std::make_unique<StrLit>(t.text);
        case Tok::KwTrue:  i_++; return std::make_unique<BoolLit>(true);
        case Tok::KwFalse: i_++; return std::make_unique<BoolLit>(false);
        case Tok::KwNull:  i_++; return std::make_unique<NullLit>();
        case Tok::LParen: {
            i_++;
            ExprP e = parseExpr();
            expect(Tok::RParen, "')'");
            return e;
        }
        case Tok::Ident: {
            std::string name = t.text; i_++;
            if (accept(Tok::LParen)) {                 // call
                auto call = std::make_unique<Call>(name);
                if (!check(Tok::RParen)) {
                    do { call->args.push_back(parseExpr()); } while (accept(Tok::Comma));
                }
                expect(Tok::RParen, "')'");
                return call;
            }
            return std::make_unique<VarExpr>(name);   // variable
        }
        default:
            error(std::string("unexpected token '") +
                  (t.text.empty() ? tokName(t.kind) : t.text) + "' in expression");
    }
}

} // namespace sleela
