// ===========================================================================
// lexer.cpp  --  Implementation of the Sleela tokenizer.
// ===========================================================================
#include "lexer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

namespace sleela {

const char* tokName(Tok t) {
    switch (t) {
        case Tok::Int: return "int-literal";
        case Tok::Double: return "double-literal";
        case Tok::Str: return "string-literal";
        case Tok::Ident: return "identifier";
        case Tok::KwClass: return "class";
        case Tok::KwStatic: return "static";
        case Tok::KwVoid: return "void";
        case Tok::KwIntT: return "int";
        case Tok::KwDoubleT: return "double";
        case Tok::KwBoolT: return "boolean";
        case Tok::KwStringT: return "String";
        case Tok::KwIf: return "if";
        case Tok::KwElse: return "else";
        case Tok::KwWhile: return "while";
        case Tok::KwFor: return "for";
        case Tok::KwReturn: return "return";
        case Tok::KwTrue: return "true";
        case Tok::KwFalse: return "false";
        case Tok::KwPrint: return "print";
        case Tok::KwNull: return "null";
        case Tok::LParen: return "("; case Tok::RParen: return ")";
        case Tok::LBrace: return "{"; case Tok::RBrace: return "}";
        case Tok::Semicolon: return ";"; case Tok::Comma: return ",";
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

bool Lexer::match(char c) {
    if (peek() == c) { advance(); return true; }
    return false;
}

void Lexer::error(const std::string& msg) const {
    throw std::runtime_error("Lexical error (line " + std::to_string(line_) +
                             ", col " + std::to_string(col_) + "): " + msg);
}

void Lexer::skipTrivia() {
    for (;;) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else if (c == '/' && peek(1) == '/') {
            while (!atEnd() && peek() != '\n') advance();
        } else if (c == '/' && peek(1) == '*') {
            advance(); advance();
            while (!atEnd() && !(peek() == '*' && peek(1) == '/')) advance();
            if (atEnd()) error("unterminated block comment");
            advance(); advance();
        } else {
            break;
        }
    }
}

Token Lexer::makeNumber() {
    int startLine = line_, startCol = col_;
    std::string s;
    bool isDouble = false;
    while (std::isdigit((unsigned char)peek())) s += advance();
    if (peek() == '.' && std::isdigit((unsigned char)peek(1))) {
        isDouble = true;
        s += advance(); // '.'
        while (std::isdigit((unsigned char)peek())) s += advance();
    }
    return Token{ isDouble ? Tok::Double : Tok::Int, s, startLine, startCol };
}

Token Lexer::makeString() {
    int startLine = line_, startCol = col_;
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
        } else {
            out += c;
        }
    }
    if (atEnd()) error("unterminated string literal");
    advance(); // closing quote
    return Token{ Tok::Str, out, startLine, startCol };
}

Token Lexer::makeIdentOrKeyword() {
    static const std::unordered_map<std::string, Tok> kw = {
        {"class", Tok::KwClass}, {"static", Tok::KwStatic}, {"void", Tok::KwVoid},
        {"int", Tok::KwIntT}, {"double", Tok::KwDoubleT}, {"boolean", Tok::KwBoolT},
        {"String", Tok::KwStringT}, {"if", Tok::KwIf}, {"else", Tok::KwElse},
        {"while", Tok::KwWhile}, {"for", Tok::KwFor}, {"return", Tok::KwReturn},
        {"true", Tok::KwTrue}, {"false", Tok::KwFalse}, {"print", Tok::KwPrint},
        {"null", Tok::KwNull},
    };
    int startLine = line_, startCol = col_;
    std::string s;
    while (std::isalnum((unsigned char)peek()) || peek() == '_') s += advance();
    auto it = kw.find(s);
    Tok t = (it != kw.end()) ? it->second : Tok::Ident;
    return Token{ t, s, startLine, startCol };
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
        auto emit = [&](Tok k, const char* txt){ toks.push_back(Token{ k, txt, L, C }); };
        switch (c) {
            case '(': emit(Tok::LParen, "("); break;
            case ')': emit(Tok::RParen, ")"); break;
            case '{': emit(Tok::LBrace, "{"); break;
            case '}': emit(Tok::RBrace, "}"); break;
            case ';': emit(Tok::Semicolon, ";"); break;
            case ',': emit(Tok::Comma, ","); break;
            case '+': emit(Tok::Plus, "+"); break;
            case '-': emit(Tok::Minus, "-"); break;
            case '*': emit(Tok::Star, "*"); break;
            case '/': emit(Tok::Slash, "/"); break;
            case '%': emit(Tok::Percent, "%"); break;
            case '=': emit(match('=') ? Tok::EqEq : Tok::Assign, "="); break;
            case '!': emit(match('=') ? Tok::NotEq : Tok::Not, "!"); break;
            case '<': emit(match('=') ? Tok::Le : Tok::Lt, "<"); break;
            case '>': emit(match('=') ? Tok::Ge : Tok::Gt, ">"); break;
            case '&':
                if (match('&')) emit(Tok::AndAnd, "&&");
                else error("unexpected '&' (did you mean '&&'?)");
                break;
            case '|':
                if (match('|')) emit(Tok::OrOr, "||");
                else error("unexpected '|' (did you mean '||'?)");
                break;
            default:
                error(std::string("unexpected character '") + c + "'");
        }
    }
    return toks;
}

} // namespace sleela
