// ===========================================================================
// sst_lexer.cpp  --  Implementation of the .sst tokenizer.
//
// Note on hyphens: attach names like `on-start` are lexed as three tokens
// (Ident '-' Ident). The parser reassembles a member name from the pattern
// IDENT ('-' IDENT)* when it is followed by ':'. This keeps the lexer fully
// context-free while still allowing `-` as subtraction inside expressions.
// ===========================================================================
#include "sst_lexer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

namespace nordshrift {

const char* tokName(Tok t) {
    switch (t) {
        case Tok::Number: return "number";
        case Tok::Str: return "string";
        case Tok::Ident: return "identifier";
        case Tok::True: return "true";
        case Tok::False: return "false";
        case Tok::KwComponent: return "component";
        case Tok::KwUses: return "uses";
        case Tok::KwLet: return "let";
        case Tok::KwIf: return "if";
        case Tok::KwElse: return "else";
        case Tok::KwWhile: return "while";
        case Tok::KwPrint: return "print";
        case Tok::KwCall: return "call";
        case Tok::KwProp: return "prop";
        case Tok::LBrace: return "{"; case Tok::RBrace: return "}";
        case Tok::LParen: return "("; case Tok::RParen: return ")";
        case Tok::Colon: return ":"; case Tok::Semicolon: return ";";
        case Tok::Comma: return ","; case Tok::Dot: return ".";
        case Tok::Assign: return "="; case Tok::Plus: return "+";
        case Tok::Minus: return "-"; case Tok::Star: return "*";
        case Tok::Slash: return "/"; case Tok::Percent: return "%";
        case Tok::EqEq: return "=="; case Tok::NotEq: return "!=";
        case Tok::Lt: return "<"; case Tok::Le: return "<=";
        case Tok::Gt: return ">"; case Tok::Ge: return ">=";
        case Tok::AndAnd: return "&&"; case Tok::OrOr: return "||";
        case Tok::Not: return "!"; case Tok::Eof: return "<eof>";
    }
    return "?";
}

char Lexer::peek(int off) const {
    size_t p = pos_ + (size_t)off;
    return p < src_.size() ? src_[p] : '\0';
}
char Lexer::advance() {
    char c = src_[pos_++];
    if (c == '\n') { line_++; col_ = 1; } else { col_++; }
    return c;
}
bool Lexer::match(char c) { if (peek() == c) { advance(); return true; } return false; }

void Lexer::error(const std::string& msg) const {
    throw std::runtime_error("Nordshrift lexical error (line " + std::to_string(line_) +
                             ", col " + std::to_string(col_) + "): " + msg);
}

void Lexer::skipTrivia() {
    for (;;) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') advance();
        else if (c == '/' && peek(1) == '/') { while (!atEnd() && peek() != '\n') advance(); }
        else if (c == '/' && peek(1) == '*') {
            advance(); advance();
            while (!atEnd() && !(peek() == '*' && peek(1) == '/')) advance();
            if (atEnd()) error("unterminated block comment");
            advance(); advance();
        } else break;
    }
}

Token Lexer::makeNumber() {
    int L = line_, C = col_;
    std::string s;
    while (std::isdigit((unsigned char)peek())) s += advance();
    if (peek() == '.' && std::isdigit((unsigned char)peek(1))) {
        s += advance();
        while (std::isdigit((unsigned char)peek())) s += advance();
    }
    return Token{ Tok::Number, s, L, C };
}

Token Lexer::makeString() {
    int L = line_, C = col_;
    advance(); // opening quote
    std::string out;
    while (!atEnd() && peek() != '"') {
        char c = advance();
        if (c == '\\') {
            char e = advance();
            switch (e) {
                case 'n': out += '\n'; break;
                case 't': out += '\t'; break;
                case 'r': out += '\r'; break;
                case '\\': out += '\\'; break;
                case '"': out += '"'; break;
                default: out += e; break;
            }
        } else out += c;
    }
    if (atEnd()) error("unterminated string literal");
    advance(); // closing quote
    return Token{ Tok::Str, out, L, C };
}

Token Lexer::makeIdentOrKeyword() {
    static const std::unordered_map<std::string, Tok> kw = {
        {"component", Tok::KwComponent}, {"uses", Tok::KwUses},
        {"let", Tok::KwLet}, {"if", Tok::KwIf}, {"else", Tok::KwElse},
        {"while", Tok::KwWhile}, {"print", Tok::KwPrint}, {"call", Tok::KwCall},
        {"prop", Tok::KwProp}, {"true", Tok::True}, {"false", Tok::False},
    };
    int L = line_, C = col_;
    std::string s;
    while (std::isalnum((unsigned char)peek()) || peek() == '_') s += advance();
    auto it = kw.find(s);
    return Token{ it != kw.end() ? it->second : Tok::Ident, s, L, C };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> toks;
    for (;;) {
        skipTrivia();
        if (atEnd()) { toks.push_back(Token{ Tok::Eof, "", line_, col_ }); break; }
        int L = line_, C = col_;
        char c = peek();

        if (std::isdigit((unsigned char)c)) { toks.push_back(makeNumber()); continue; }
        if (c == '"') { toks.push_back(makeString()); continue; }
        if (std::isalpha((unsigned char)c) || c == '_') { toks.push_back(makeIdentOrKeyword()); continue; }

        advance();
        auto emit = [&](Tok k, const char* t){ toks.push_back(Token{ k, t, L, C }); };
        switch (c) {
            case '{': emit(Tok::LBrace, "{"); break;
            case '}': emit(Tok::RBrace, "}"); break;
            case '(': emit(Tok::LParen, "("); break;
            case ')': emit(Tok::RParen, ")"); break;
            case ':': emit(Tok::Colon, ":"); break;
            case ';': emit(Tok::Semicolon, ";"); break;
            case ',': emit(Tok::Comma, ","); break;
            case '.': emit(Tok::Dot, "."); break;
            case '+': emit(Tok::Plus, "+"); break;
            case '-': emit(Tok::Minus, "-"); break;
            case '*': emit(Tok::Star, "*"); break;
            case '/': emit(Tok::Slash, "/"); break;
            case '%': emit(Tok::Percent, "%"); break;
            case '=': emit(match('=') ? Tok::EqEq : Tok::Assign, "="); break;
            case '!': emit(match('=') ? Tok::NotEq : Tok::Not, "!"); break;
            case '<': emit(match('=') ? Tok::Le : Tok::Lt, "<"); break;
            case '>': emit(match('=') ? Tok::Ge : Tok::Gt, ">"); break;
            case '&': if (match('&')) emit(Tok::AndAnd, "&&"); else error("expected '&&'"); break;
            case '|': if (match('|')) emit(Tok::OrOr, "||"); else error("expected '||'"); break;
            default: error(std::string("unexpected character '") + c + "'");
        }
    }
    return toks;
}

} // namespace nordshrift
