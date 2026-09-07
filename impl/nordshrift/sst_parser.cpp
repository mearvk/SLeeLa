// ===========================================================================
// sst_parser.cpp  --  Parser for Nordshrift .sst Style Sheets.
// ===========================================================================
#include "sst_parser.h"

#include <stdexcept>

namespace nordshrift {

const Token& Parser::peek(int off) const {
    size_t p = i_ + (size_t)off;
    if (p >= toks_.size()) return toks_.back();
    return toks_[p];
}
bool Parser::accept(Tok k) { if (check(k)) { i_++; return true; } return false; }

const Token& Parser::expect(Tok k, const char* what) {
    if (!check(k))
        error(std::string("expected ") + what + " but found '" +
              (cur().text.empty() ? tokName(cur().kind) : cur().text) + "'");
    return toks_[i_++];
}

void Parser::error(const std::string& msg) const {
    const Token& t = cur();
    throw std::runtime_error("Nordshrift syntax error (line " + std::to_string(t.line) +
                             ", col " + std::to_string(t.col) + "): " + msg);
}

Sheet Parser::parseSheet() {
    Sheet sheet;
    while (!check(Tok::Eof)) {
        if (check(Tok::KwComponent)) {
            Component c = parseComponent();
            if (sheet.index.count(c.name))
                error("duplicate component '" + c.name + "'");
            sheet.index[c.name] = (int)sheet.components.size();
            sheet.components.push_back(std::move(c));
        } else {
            error("expected 'component' at top level");
        }
    }
    if (sheet.components.empty()) error("style sheet defines no components");
    return sheet;
}

std::string Parser::parseMemberName() {
    // IDENT ('-' IDENT)*   -> reassembled as "a-b-c"
    std::string name = expect(Tok::Ident, "member name").text;
    while (check(Tok::Minus) && peek(1).kind == Tok::Ident) {
        i_++;                       // consume '-'
        name += "-";
        name += toks_[i_++].text;   // next ident
    }
    return name;
}

Component Parser::parseComponent() {
    expect(Tok::KwComponent, "'component'");
    Component c;
    c.name = expect(Tok::Ident, "component name").text;
    expect(Tok::LBrace, "'{'");

    while (!check(Tok::RBrace) && !check(Tok::Eof)) {
        if (accept(Tok::KwUses)) {
            expect(Tok::Colon, "':'");
            do {
                c.uses.push_back(expect(Tok::Ident, "component name in 'uses'").text);
            } while (accept(Tok::Comma));
            expect(Tok::Semicolon, "';'");
            continue;
        }

        std::string member = parseMemberName();
        expect(Tok::Colon, "':'");

        if (check(Tok::LBrace)) {
            // functional attach
            Attach a;
            a.name = member;
            a.body = parseBlock();
            c.attaches.push_back(std::move(a));
        } else {
            // scalar property
            if (c.props.count(member)) error("duplicate property '" + member + "'");
            c.props[member] = parseScalar();
            expect(Tok::Semicolon, "';'");
        }
    }
    expect(Tok::RBrace, "'}'");
    return c;
}

PropValue Parser::parseScalar() {
    const Token& t = cur();
    PropValue v;
    switch (t.kind) {
        case Tok::Number: {
            i_++;
            v.kind = ValKind::Number;
            v.isInt = (t.text.find('.') == std::string::npos);
            v.num = std::stod(t.text);
            v.s = t.text;
            return v;
        }
        case Tok::Str:   i_++; v.kind = ValKind::Str;  v.s = t.text; return v;
        case Tok::True:  i_++; v.kind = ValKind::Bool; v.b = true;  return v;
        case Tok::False: i_++; v.kind = ValKind::Bool; v.b = false; return v;
        case Tok::Ident: i_++; v.kind = ValKind::Ident; v.s = t.text; return v;
        default: error("expected a scalar value (number, string, boolean, or identifier)");
    }
}

// ---- attach statements ----------------------------------------------------
std::unique_ptr<Block> Parser::parseBlock() {
    expect(Tok::LBrace, "'{'");
    auto b = std::make_unique<Block>();
    while (!check(Tok::RBrace) && !check(Tok::Eof))
        b->stmts.push_back(parseStmt());
    expect(Tok::RBrace, "'}'");
    return b;
}

StmtP Parser::parseStmt() {
    if (check(Tok::LBrace)) return parseBlock();

    if (accept(Tok::KwLet)) {
        auto s = std::make_unique<LetStmt>();
        s->name = expect(Tok::Ident, "variable name").text;
        expect(Tok::Assign, "'='");
        s->init = parseExpr();
        expect(Tok::Semicolon, "';'");
        return s;
    }
    if (accept(Tok::KwIf)) {
        auto s = std::make_unique<IfStmt>();
        expect(Tok::LParen, "'('");
        s->cond = parseExpr();
        expect(Tok::RParen, "')'");
        s->thenS = parseStmt();
        if (accept(Tok::KwElse)) s->elseS = parseStmt();
        return s;
    }
    if (accept(Tok::KwWhile)) {
        auto s = std::make_unique<WhileStmt>();
        expect(Tok::LParen, "'('");
        s->cond = parseExpr();
        expect(Tok::RParen, "')'");
        s->body = parseStmt();
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
    if (accept(Tok::KwCall)) {
        auto s = std::make_unique<CallStmt>();
        s->comp = expect(Tok::Ident, "component name").text;
        expect(Tok::Dot, "'.'");
        s->attach = parseMemberName();
        expect(Tok::LParen, "'('");
        expect(Tok::RParen, "')'");
        expect(Tok::Semicolon, "';'");
        return s;
    }
    // assignment: IDENT = expr ;
    if (check(Tok::Ident) && peek(1).kind == Tok::Assign) {
        auto s = std::make_unique<AssignStmt>();
        s->name = cur().text; i_++;
        expect(Tok::Assign, "'='");
        s->value = parseExpr();
        expect(Tok::Semicolon, "';'");
        return s;
    }
    error("expected a statement");
}

// ---- expressions ----------------------------------------------------------
ExprP Parser::parseExpr() { return parseOr(); }

ExprP Parser::parseOr() {
    ExprP e = parseAnd();
    while (check(Tok::OrOr)) { i_++; e = std::make_unique<Binary>("||", std::move(e), parseAnd()); }
    return e;
}
ExprP Parser::parseAnd() {
    ExprP e = parseEq();
    while (check(Tok::AndAnd)) { i_++; e = std::make_unique<Binary>("&&", std::move(e), parseEq()); }
    return e;
}
ExprP Parser::parseEq() {
    ExprP e = parseRel();
    for (;;) {
        if (check(Tok::EqEq)) { i_++; e = std::make_unique<Binary>("==", std::move(e), parseRel()); }
        else if (check(Tok::NotEq)) { i_++; e = std::make_unique<Binary>("!=", std::move(e), parseRel()); }
        else break;
    }
    return e;
}
ExprP Parser::parseRel() {
    ExprP e = parseAdd();
    for (;;) {
        if (check(Tok::Lt)) { i_++; e = std::make_unique<Binary>("<", std::move(e), parseAdd()); }
        else if (check(Tok::Le)) { i_++; e = std::make_unique<Binary>("<=", std::move(e), parseAdd()); }
        else if (check(Tok::Gt)) { i_++; e = std::make_unique<Binary>(">", std::move(e), parseAdd()); }
        else if (check(Tok::Ge)) { i_++; e = std::make_unique<Binary>(">=", std::move(e), parseAdd()); }
        else break;
    }
    return e;
}
ExprP Parser::parseAdd() {
    ExprP e = parseMul();
    for (;;) {
        if (check(Tok::Plus)) { i_++; e = std::make_unique<Binary>("+", std::move(e), parseMul()); }
        else if (check(Tok::Minus)) { i_++; e = std::make_unique<Binary>("-", std::move(e), parseMul()); }
        else break;
    }
    return e;
}
ExprP Parser::parseMul() {
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
        case Tok::Number: {
            i_++;
            bool isInt = (t.text.find('.') == std::string::npos);
            return std::make_unique<NumLit>(std::stod(t.text), isInt);
        }
        case Tok::Str:   i_++; return std::make_unique<StrLit>(t.text);
        case Tok::True:  i_++; return std::make_unique<BoolLit>(true);
        case Tok::False: i_++; return std::make_unique<BoolLit>(false);
        case Tok::Ident: i_++; return std::make_unique<VarRef>(t.text);
        case Tok::KwProp: {
            i_++;
            expect(Tok::LParen, "'('");
            std::string name = expect(Tok::Ident, "property name").text;
            expect(Tok::RParen, "')'");
            return std::make_unique<PropRef>(name);
        }
        case Tok::LParen: {
            i_++;
            ExprP e = parseExpr();
            expect(Tok::RParen, "')'");
            return e;
        }
        default:
            error(std::string("unexpected token '") +
                  (t.text.empty() ? tokName(t.kind) : t.text) + "' in expression");
    }
}

} // namespace nordshrift
