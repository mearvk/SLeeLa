// SleelaTerminal(TM) shell -- L2 lexer implementation.
// Original SLeeLa work (see NOTICE).

#include "lexer.hpp"

#include <cctype>

namespace sleela::sh {

namespace {

// Is this token position the start of a command word (so a `name=value` word
// should be an assignment, and if/while/... are keywords)? True at the start
// and immediately after a command separator or opening keyword.
bool atCommandStart(const std::vector<Token>& out) {
    if (out.empty()) return true;
    switch (out.back().kind) {
        case Tok::Pipe: case Tok::Semi: case Tok::Newline:
        case Tok::AndIf: case Tok::OrIf:
        case Tok::If: case Tok::Then: case Tok::Elif: case Tok::Else:
        case Tok::While: case Tok::Do:
            return true;
        default:
            return false;
    }
}

// A well-formed assignment word is `name=...` where name is [A-Za-z_][A-Za-z0-9_]*.
bool looksLikeAssignment(const std::string& w) {
    const auto eq = w.find('=');
    if (eq == std::string::npos || eq == 0) return false;
    if (!(std::isalpha(static_cast<unsigned char>(w[0])) || w[0] == '_')) return false;
    for (std::size_t i = 1; i < eq; ++i) {
        if (!(std::isalnum(static_cast<unsigned char>(w[i])) || w[i] == '_'))
            return false;
    }
    return true;
}

Tok keywordKind(const std::string& w) {
    if (w == "if")    return Tok::If;
    if (w == "then")  return Tok::Then;
    if (w == "elif")  return Tok::Elif;
    if (w == "else")  return Tok::Else;
    if (w == "fi")    return Tok::Fi;
    if (w == "while") return Tok::While;
    if (w == "do")    return Tok::Do;
    if (w == "done")  return Tok::Done;
    return Tok::Word;
}

} // namespace

bool lex(const std::string& src, std::vector<Token>& out, LexError& err) {
    out.clear();
    std::size_t i = 0;
    const std::size_t n = src.size();
    int line = 1, col = 1;

    auto adv = [&](std::size_t k = 1) {
        for (std::size_t t = 0; t < k && i < n; ++t) {
            if (src[i] == '\n') { ++line; col = 1; } else { ++col; }
            ++i;
        }
    };
    auto push = [&](Tok kind, std::string text, int ln, int cl) {
        Token tk; tk.kind = kind; tk.text = std::move(text); tk.line = ln; tk.col = cl;
        out.push_back(std::move(tk));
    };

    while (i < n) {
        const char c = src[i];

        // whitespace (not newline)
        if (c == ' ' || c == '\t' || c == '\r') { adv(); continue; }

        // comment
        if (c == '#') { while (i < n && src[i] != '\n') adv(); continue; }

        // newline
        if (c == '\n') { push(Tok::Newline, "\n", line, col); adv(); continue; }

        // operators (maximal munch)
        const char nx = (i + 1 < n) ? src[i + 1] : '\0';
        if (c == '|') {
            if (nx == '|') { push(Tok::OrIf, "||", line, col); adv(2); }
            else { push(Tok::Pipe, "|", line, col); adv(); }
            continue;
        }
        if (c == '&') {
            if (nx == '&') { push(Tok::AndIf, "&&", line, col); adv(2); continue; }
            err = LexError{"unsupported '&' (background/async not in this milestone)", line, col};
            return false;
        }
        if (c == ';') { push(Tok::Semi, ";", line, col); adv(); continue; }
        if (c == '<') { push(Tok::Less, "<", line, col); adv(); continue; }
        if (c == '>') {
            if (nx == '>') { push(Tok::DGreat, ">>", line, col); adv(2); }
            else { push(Tok::Great, ">", line, col); adv(); }
            continue;
        }

        // a word (possibly with quoted spans), read until a delimiter
        const int wline = line, wcol = col;
        std::string word;
        bool sawQuote = false;
        while (i < n) {
            const char d = src[i];
            if (d == ' ' || d == '\t' || d == '\r' || d == '\n' ||
                d == '|' || d == '&' || d == ';' || d == '<' || d == '>' ||
                d == '#') {
                break;
            }
            if (d == '\'') {
                // single quote: literal until the next single quote
                sawQuote = true;
                adv();  // consume opening '
                while (i < n && src[i] != '\'') { word.push_back(src[i]); adv(); }
                if (i >= n) { err = LexError{"unterminated single quote", wline, wcol}; return false; }
                adv();  // consume closing '
                continue;
            }
            if (d == '"') {
                // double quote: keep contents; expansion handled later. We keep
                // the raw characters (including $), and support \" and \\ escapes.
                sawQuote = true;
                adv();  // consume opening "
                while (i < n && src[i] != '"') {
                    if (src[i] == '\\' && i + 1 < n &&
                        (src[i + 1] == '"' || src[i + 1] == '\\' || src[i + 1] == '$')) {
                        word.push_back(src[i + 1]); adv(2);
                    } else {
                        word.push_back(src[i]); adv();
                    }
                }
                if (i >= n) { err = LexError{"unterminated double quote", wline, wcol}; return false; }
                adv();  // consume closing "
                continue;
            }
            if (d == '\\') {
                // backslash escape of the next character
                if (i + 1 < n) { word.push_back(src[i + 1]); adv(2); }
                else { adv(); }
                continue;
            }
            // A $(( ... )) arithmetic span, or a ${ ... } parameter span, is a
            // single word span even when it contains spaces or operators.
            if (d == '$' && i + 2 < n && src[i + 1] == '(' && src[i + 2] == '(') {
                word.append("$((");
                adv(3);
                int depth = 1;
                while (i < n && depth > 0) {
                    if (src[i] == '(') { ++depth; word.push_back(src[i]); adv(); }
                    else if (src[i] == ')') {
                        // "))" closes the arithmetic span when depth reaches 0.
                        if (i + 1 < n && src[i + 1] == ')' && depth == 1) {
                            word.append("))"); adv(2); depth = 0;
                        } else { --depth; word.push_back(src[i]); adv(); }
                    } else { word.push_back(src[i]); adv(); }
                }
                if (depth != 0) { err = LexError{"unterminated $(( ))", wline, wcol}; return false; }
                continue;
            }
            if (d == '$' && i + 1 < n && src[i + 1] == '{') {
                word.append("${");
                adv(2);
                while (i < n && src[i] != '}') { word.push_back(src[i]); adv(); }
                if (i >= n) { err = LexError{"unterminated ${ }", wline, wcol}; return false; }
                word.push_back('}'); adv();
                continue;
            }
            word.push_back(d);
            adv();
        }

        // classify the word
        const bool cmdStart = atCommandStart(out);
        if (!sawQuote && cmdStart && looksLikeAssignment(word)) {
            push(Tok::Assignment, word, wline, wcol);
        } else if (!sawQuote && cmdStart) {
            push(keywordKind(word), word, wline, wcol);
        } else {
            push(Tok::Word, word, wline, wcol);
        }
    }

    push(Tok::Eof, "", line, col);
    return true;
}

} // namespace sleela::sh
