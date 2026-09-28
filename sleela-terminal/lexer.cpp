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
        case Tok::AndIf: case Tok::OrIf: case Tok::Amp: case Tok::Bang:
        case Tok::If: case Tok::Then: case Tok::Elif: case Tok::Else:
        case Tok::While: case Tok::Until: case Tok::Do:
        case Tok::In: case Tok::LBrace: case Tok::RParen:
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

// Compound-command openers: recognised only at command-start position.
Tok openerKind(const std::string& w) {
    if (w == "if")    return Tok::If;
    if (w == "while") return Tok::While;
    if (w == "until") return Tok::Until;
    if (w == "for")   return Tok::For;
    if (w == "case")  return Tok::Case;
    return Tok::Word;
}

// Structural keywords: recognised whenever an unquoted bareword matches, since
// they only make sense inside a compound command (then/do/in/...). This lets
// e.g. `for i in ...` see `in` even though it follows the word `i`.
Tok structuralKind(const std::string& w) {
    if (w == "then")  return Tok::Then;
    if (w == "elif")  return Tok::Elif;
    if (w == "else")  return Tok::Else;
    if (w == "fi")    return Tok::Fi;
    if (w == "do")    return Tok::Do;
    if (w == "done")  return Tok::Done;
    if (w == "in")    return Tok::In;
    if (w == "esac")  return Tok::Esac;
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
            push(Tok::Amp, "&", line, col); adv(); continue;   // background
        }
        if (c == ';') { push(Tok::Semi, ";", line, col); adv(); continue; }
        if (c == '<') {
            if (nx == '<') {
                // here-document: << or <<-
                const bool dash = (i + 2 < n && src[i + 2] == '-');
                const int hline = line, hcol = col;
                adv(dash ? 3 : 2);
                push(dash ? Tok::DLessDash : Tok::DLess, dash ? "<<-" : "<<", hline, hcol);
                // read the delimiter word (skip spaces)
                while (i < n && (src[i] == ' ' || src[i] == '\t')) adv();
                std::string delim;
                bool quotedDelim = false;
                while (i < n && src[i] != '\n' && src[i] != ' ' && src[i] != '\t' &&
                       src[i] != ';' && src[i] != '|' && src[i] != '&') {
                    if (src[i] == '\'' || src[i] == '"') { quotedDelim = true; adv(); continue; }
                    delim.push_back(src[i]); adv();
                }
                if (delim.empty()) { err = LexError{"expected here-document delimiter after <<", hline, hcol}; return false; }
                push(Tok::Word, delim, hline, hcol);
                // consume the rest of the current line up to and including \n
                while (i < n && src[i] != '\n') adv();
                if (i < n) adv();  // the newline
                // collect body lines until a line equal to the delimiter
                std::string bodyText;
                bool closed = false;
                while (i < n) {
                    // read one physical line
                    std::string ln;
                    while (i < n && src[i] != '\n') { ln.push_back(src[i]); adv(); }
                    if (i < n) adv();  // consume newline
                    // for <<- strip leading tabs from the line (and the delim compare)
                    std::string cmp = ln;
                    if (dash) {
                        std::size_t t = 0; while (t < cmp.size() && cmp[t] == '\t') ++t;
                        cmp = cmp.substr(t);
                        std::size_t t2 = 0; while (t2 < ln.size() && ln[t2] == '\t') ++t2;
                        ln = ln.substr(t2);
                    }
                    if (cmp == delim) { closed = true; break; }
                    bodyText += ln;
                    bodyText.push_back('\n');
                }
                if (!closed) { err = LexError{"unterminated here-document (missing " + delim + ")", hline, hcol}; return false; }
                // A here-doc body is expandable unless the delimiter was quoted.
                // Encode expandability as a leading flag char the parser strips:
                //   'E' = expand $ in the body, 'L' = literal.
                push(Tok::HeredocBody, (quotedDelim ? "L" : "E") + bodyText, hline, hcol);
                continue;
            }
            push(Tok::Less, "<", line, col); adv(); continue;
        }
        if (c == '>') {
            if (nx == '>') { push(Tok::DGreat, ">>", line, col); adv(2); }
            else { push(Tok::Great, ">", line, col); adv(); }
            continue;
        }
        // Grouping/definition punctuation. A leading '$' before '(' or '{' is
        // handled inside the word loop as a substitution span, so a bare '(' or
        // '{' here is structural.
        if (c == '(') { push(Tok::LParen, "(", line, col); adv(); continue; }
        if (c == ')') { push(Tok::RParen, ")", line, col); adv(); continue; }
        if (c == '{') { push(Tok::LBrace, "{", line, col); adv(); continue; }
        if (c == '}') { push(Tok::RBrace, "}", line, col); adv(); continue; }

        // a word (possibly with quoted spans), read until a delimiter
        const int wline = line, wcol = col;
        std::string word;
        bool sawQuote = false;
        bool sawQuoteBeforeEq = false;
        while (i < n) {
            const char d = src[i];
            if (d == ' ' || d == '\t' || d == '\r' || d == '\n' ||
                d == '|' || d == '&' || d == ';' || d == '<' || d == '>' ||
                d == '#' || d == '(' || d == ')') {
                break;
            }
            // '{' and '}' are only structural (function body / brace group) when
            // they stand alone; when adjacent to word characters they belong to
            // the word (e.g. brace expansion item{1,2,3}, or ${...}). Break the
            // word only for a standalone brace at the very start of a word.
            if ((d == '{' || d == '}') && word.empty()) {
                // standalone if followed by whitespace/eol/operator
                const char after = (i + 1 < n) ? src[i + 1] : '\0';
                if (after == '\0' || after == ' ' || after == '\t' ||
                    after == '\n' || after == '\r' || after == ';' ||
                    after == '|' || after == '&') {
                    break;  // let the operator scanner emit LBrace/RBrace
                }
            }
            if (d == '\'') {
                // single quote: literal until the next single quote
                sawQuote = true;
                if (word.find('=') == std::string::npos) sawQuoteBeforeEq = true;
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
                if (word.find('=') == std::string::npos) sawQuoteBeforeEq = true;
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
            // A $( ... ) command-substitution span is a single word span with
            // balanced parentheses (checked after $(( above).
            if (d == '$' && i + 1 < n && src[i + 1] == '(') {
                word.append("$(");
                adv(2);
                int depth = 1;
                while (i < n && depth > 0) {
                    if (src[i] == '(') ++depth;
                    else if (src[i] == ')') --depth;
                    if (depth > 0) { word.push_back(src[i]); adv(); }
                    else { word.push_back(')'); adv(); }  // closing ')'
                }
                if (depth != 0) { err = LexError{"unterminated $( )", wline, wcol}; return false; }
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
        Tok structural = sawQuote ? Tok::Word : structuralKind(word);
        if (!sawQuoteBeforeEq && cmdStart && looksLikeAssignment(word)) {
            push(Tok::Assignment, word, wline, wcol);
        } else if (structural != Tok::Word) {
            // then/do/done/in/esac/... are keywords wherever they appear bare.
            push(structural, word, wline, wcol);
        } else if (!sawQuote && cmdStart && word == "!") {
            // '!' at command-start negates the pipeline that follows.
            push(Tok::Bang, "!", wline, wcol);
        } else if (!sawQuote && cmdStart) {
            push(openerKind(word), word, wline, wcol);
        } else {
            push(Tok::Word, word, wline, wcol);
        }
    }

    push(Tok::Eof, "", line, col);
    return true;
}

} // namespace sleela::sh
