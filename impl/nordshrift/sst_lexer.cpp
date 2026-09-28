// ===========================================================================
// sst_lexer.cpp  --  Spec-conformant .sst tokenizer (NS-SST-0001 Part I).
//
// Strategy: process the source line by line. Blank and comment-only lines are
// skipped for structure (IND-04). For each significant line we compute its
// indentation, compare against an indent stack to emit INDENT / DEDENT, then
// tokenize the line's content and emit a terminating NEWLINE.
//
// The indent unit (2 or 4 spaces) is fixed by the first indented line (IND-01);
// every indentation must be a whole multiple of it, else NSS-E-0002.
// ===========================================================================
#include "sst_lexer.h"

#include <cctype>
#include <cstdint>

namespace nordshrift {

const char* tokName(Tok t) {
    switch (t) {
        case Tok::Newline:  return "<newline>";
        case Tok::Indent:   return "<indent>";
        case Tok::Dedent:   return "<dedent>";
        case Tok::Pragma:   return "pragma";
        case Tok::Ident:    return "identifier";
        case Tok::String:   return "string";
        case Tok::Integer:  return "integer";
        case Tok::Version:  return "version";
        case Tok::Colon:    return "':'";
        case Tok::Comma:    return "','";
        case Tok::LBracket: return "'['";
        case Tok::RBracket: return "']'";
        case Tok::Eof:      return "<eof>";
    }
    return "?";
}

namespace {

// Validate that a byte string is well-formed UTF-8 (NSS-E-0001).
bool validUtf8(const std::string& s) {
    size_t i = 0, n = s.size();
    while (i < n) {
        unsigned char c = (unsigned char)s[i];
        int extra;
        if (c < 0x80) extra = 0;
        else if ((c & 0xE0) == 0xC0) extra = 1;
        else if ((c & 0xF0) == 0xE0) extra = 2;
        else if ((c & 0xF8) == 0xF0) extra = 3;
        else return false;
        if (i + (size_t)extra >= n) return false;
        for (int k = 1; k <= extra; k++)
            if (((unsigned char)s[i + k] & 0xC0) != 0x80) return false;
        i += (size_t)extra + 1;
    }
    return true;
}

bool identStart(char c) { return std::isalpha((unsigned char)c) || c == '_'; }
bool identChar(char c)  { return std::isalnum((unsigned char)c) || c == '_' || c == '-'; }

// Net bracket depth of a line: '[' minus ']', skipping brackets that appear
// inside string literals or after a // line comment.
int bracketDelta(const std::string& s) {
    int depth = 0; bool inStr = false;
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (inStr) {
            if (c == '\\') { i++; continue; }
            if (c == '"') inStr = false;
            continue;
        }
        if (c == '"') { inStr = true; continue; }
        if (c == '/' && i + 1 < s.size() && s[i+1] == '/') break;
        if (c == '[') depth++;
        else if (c == ']') depth--;
    }
    return depth;
}

struct Lexer {
    const std::string& src;
    const std::string& file;
    DiagnosticBag& diags;
    std::vector<Token> out;

    int indentUnit = 0;                 // 0 = not yet determined
    std::vector<int> indentStack{0};    // column widths of open blocks
    std::string pendingDoc;             // last /// doc comment seen
    bool docPending = false;

    Lexer(const std::string& s, const std::string& f, DiagnosticBag& d)
        : src(s), file(f), diags(d) {}

    // Split into logical lines with LF/CRLF normalized (§1.1).
    std::vector<std::string> splitLines() {
        std::vector<std::string> lines;
        std::string cur;
        for (size_t i = 0; i < src.size(); i++) {
            char c = src[i];
            if (c == '\r') { if (i + 1 < src.size() && src[i+1] == '\n') i++; lines.push_back(cur); cur.clear(); }
            else if (c == '\n') { lines.push_back(cur); cur.clear(); }
            else cur += c;
        }
        lines.push_back(cur);
        return lines;
    }

    void emit(Tok k, const std::string& text, int line, int col) {
        Token t{ k, text, line, col, "" };
        if (docPending) { t.doc = pendingDoc; docPending = false; pendingDoc.clear(); }
        out.push_back(t);
    }

    std::vector<Token> run() {
        if (!validUtf8(src)) {
            diags.error("NSS-E-0001", file, 0, "file is not valid UTF-8", "§1.1");
            out.push_back(Token{ Tok::Eof, "", 0, 0, "" });
            return out;
        }
        auto lines = splitLines();
        bool inBlockComment = false;

        for (size_t li = 0; li < lines.size(); li++) {
            std::string raw = lines[li];
            int lineNo = (int)li + 1;

            // Multiline string stitching (§1.5): if this line opens a """ that
            // does not close on the same line, absorb following lines up to the
            // closing """ into one logical line, joining with '\n'. Leading
            // whitespace up to the closing-""" indent is stripped per line.
            {
                size_t open = raw.find("\"\"\"");
                if (open != std::string::npos &&
                    raw.find("\"\"\"", open + 3) == std::string::npos) {
                    std::string joined = raw;
                    size_t j = li + 1;
                    for (; j < lines.size(); j++) {
                        const std::string& nx = lines[j];
                        size_t close = nx.find("\"\"\"");
                        int strip = 0; while (strip < (int)nx.size() && nx[strip] == ' ') strip++;
                        joined += "\n" + nx.substr((size_t)strip);
                        if (close != std::string::npos) break;
                    }
                    li = j;
                    raw = joined;
                }
            }

            // List continuation stitching (§1.5): a bracketed list may wrap over
            // several physical lines; the continuation lines' indentation is not
            // block structure. If more '[' than ']' remain open on this line,
            // absorb following lines (space-joined) until the brackets balance.
            {
                int depth = bracketDelta(raw);
                if (depth > 0) {
                    std::string joined = raw;
                    size_t j = li + 1;
                    for (; j < lines.size() && depth > 0; j++) {
                        const std::string& nx = lines[j];
                        int strip = 0; while (strip < (int)nx.size() && nx[strip] == ' ') strip++;
                        std::string content = nx.substr((size_t)strip);
                        joined += " " + content;
                        depth += bracketDelta(content);
                    }
                    li = j - 1;
                    raw = joined;
                }
            }

            // measure leading spaces (tabs are not an indent unit here)
            int indent = 0;
            while (indent < (int)raw.size() && raw[indent] == ' ') indent++;
            std::string body = raw.substr((size_t)indent);
            // strip trailing whitespace (IND-05)
            while (!body.empty() && (body.back() == ' ' || body.back() == '\t')) body.pop_back();

            // handle an ongoing /* ... */ block comment
            if (inBlockComment) {
                size_t end = body.find("*/");
                if (end == std::string::npos) continue;
                body = body.substr(end + 2);
                // strip leading spaces of remainder
                size_t p = 0; while (p < body.size() && body[p] == ' ') p++;
                body = body.substr(p);
                inBlockComment = false;
                if (body.empty()) continue;
            }

            if (body.empty()) continue;                 // blank line (IND-04)

            // whole-line comments
            if (body.rfind("///", 0) == 0) {             // doc comment (§1.3)
                std::string text = body.substr(3);
                if (!text.empty() && text[0] == ' ') text.erase(0, 1);
                if (docPending) pendingDoc += "\n" + text; else pendingDoc = text;
                docPending = true;
                continue;
            }
            if (body.rfind("//", 0) == 0) continue;      // line comment
            if (body.rfind("/*", 0) == 0) {              // block comment start
                size_t end = body.find("*/", 2);
                if (end == std::string::npos) { inBlockComment = true; continue; }
                // inline block comment consuming the whole line
                std::string rest = body.substr(end + 2);
                size_t p = 0; while (p < rest.size() && rest[p] == ' ') p++;
                if (p >= rest.size()) continue;
                body = rest.substr(p);                   // (rare) code after */
            }

            // ---- indentation bookkeeping ----
            if (indent > indentStack.back()) {
                if (indentUnit == 0) {
                    if (indent != 2 && indent != 4) {
                        diags.error("NSS-E-0002", file, lineNo,
                            "first indent must be 2 or 4 spaces (found " + std::to_string(indent) + ")",
                            "IND-01");
                    }
                    indentUnit = (indent == 4) ? 4 : 2;
                }
                if (indentUnit && (indent - indentStack.back()) != indentUnit) {
                    diags.error("NSS-E-0002", file, lineNo,
                        "inconsistent indentation: expected " +
                        std::to_string(indentStack.back() + indentUnit) + " spaces, found " +
                        std::to_string(indent), "IND-01");
                }
                indentStack.push_back(indent);
                emit(Tok::Indent, "", lineNo, 1);
            } else {
                while (indent < indentStack.back()) {
                    indentStack.pop_back();
                    emit(Tok::Dedent, "", lineNo, 1);
                }
                if (indent != indentStack.back()) {
                    diags.error("NSS-E-0002", file, lineNo,
                        "dedent does not match any enclosing block indentation",
                        "IND-03");
                    indentStack.push_back(indent);   // recover
                }
            }

            // ---- tokenize the line body ----
            lexLineBody(body, lineNo);
            emit(Tok::Newline, "", lineNo, (int)raw.size() + 1);
        }

        // close any open blocks at EOF
        while (indentStack.size() > 1) { indentStack.pop_back(); emit(Tok::Dedent, "", (int)lines.size(), 1); }
        if (docPending)
            diags.warning("NSS-W-0001", file, (int)lines.size(),
                "documentation comment not followed by a directive or block", "§1.3");
        out.push_back(Token{ Tok::Eof, "", (int)lines.size() + 1, 1, "" });
        return out;
    }

    // Tokenize one line's content (after indentation). `base` is the line's
    // indent column, used to strip a multiline-string's leading whitespace.
    void lexLineBody(const std::string& s, int lineNo) {
        size_t i = 0, n = s.size();

        // pragma: line begins with '#'
        if (n > 0 && s[0] == '#') {
            size_t p = 1;
            std::string name;
            while (p < n && identChar(s[p])) name += s[p++];
            emit(Tok::Pragma, name, lineNo, 1);
            while (p < n && s[p] == ' ') p++;
            // the rest of the pragma line is its operand (version or ident)
            lexRest(s, p, lineNo);
            return;
        }
        lexRest(s, i, lineNo);
    }

    // Tokenize the remainder of a line starting at offset i.
    void lexRest(const std::string& s, size_t i, int lineNo) {
        size_t n = s.size();
        while (i < n) {
            char c = s[i];
            if (c == ' ' || c == '\t') { i++; continue; }
            if (c == '/' && i + 1 < n && s[i+1] == '/') break;             // trailing // comment
            if (c == '/' && i + 1 < n && s[i+1] == '*') {                  // inline /* */
                size_t end = s.find("*/", i + 2);
                if (end == std::string::npos) break;                       // rest is comment
                i = end + 2; continue;
            }
            int col = (int)i + 1;
            if (c == ':') { emit(Tok::Colon, ":", lineNo, col); i++; continue; }
            if (c == ',') { emit(Tok::Comma, ",", lineNo, col); i++; continue; }
            if (c == '[') { emit(Tok::LBracket, "[", lineNo, col); i++; continue; }
            if (c == ']') { emit(Tok::RBracket, "]", lineNo, col); i++; continue; }
            if (c == '"') { lexString(s, i, lineNo); continue; }
            if (std::isdigit((unsigned char)c)) { lexNumberOrVersion(s, i, lineNo); continue; }
            if (identStart(c)) {
                std::string id; int start = col;
                while (i < n && identChar(s[i])) id += s[i++];
                emit(Tok::Ident, id, lineNo, start);
                continue;
            }
            // unknown char: skip (a later parse error will report context)
            i++;
        }
    }

    void lexString(const std::string& s, size_t& i, int lineNo) {
        int col = (int)i + 1;
        // triple-quoted?
        if (s.compare(i, 3, "\"\"\"") == 0) {
            // single-line triple quote; multiline handled by caller merging is
            // out of scope here -- most sheets keep """ content on the line or
            // the parser stitches description text. Read until closing """.
            size_t end = s.find("\"\"\"", i + 3);
            std::string val = (end == std::string::npos) ? s.substr(i + 3)
                                                         : s.substr(i + 3, end - (i + 3));
            i = (end == std::string::npos) ? s.size() : end + 3;
            emit(Tok::String, val, lineNo, col);
            return;
        }
        i++; // opening quote
        std::string val;
        while (i < s.size() && s[i] != '"') {
            char c = s[i++];
            if (c == '\\' && i < s.size()) {
                char e = s[i++];
                switch (e) {
                    case 'n': val += '\n'; break;
                    case 't': val += '\t'; break;
                    case 'r': val += '\r'; break;
                    case '\\': val += '\\'; break;
                    case '"': val += '"'; break;
                    case 'u': {
                        // \uXXXX -> keep as-is decoded to a codepoint's bytes is
                        // beyond scope; store the escape literally for fidelity.
                        val += "\\u";
                        for (int k = 0; k < 4 && i < s.size(); k++) val += s[i++];
                        break;
                    }
                    default: val += e; break;
                }
            } else val += c;
        }
        if (i < s.size()) i++; // closing quote
        emit(Tok::String, val, lineNo, col);
    }

    void lexNumberOrVersion(const std::string& s, size_t& i, int lineNo) {
        int col = (int)i + 1;
        std::string raw;
        int dots = 0;
        while (i < s.size() && (std::isdigit((unsigned char)s[i]) || s[i] == '_' || s[i] == '.')) {
            if (s[i] == '.') {
                // only consume '.' if followed by a digit (version segment)
                if (i + 1 < s.size() && std::isdigit((unsigned char)s[i+1])) { dots++; raw += s[i++]; }
                else break;
            } else {
                raw += s[i++];
            }
        }
        if (dots >= 1) {
            emit(Tok::Version, raw, lineNo, col);
        } else {
            std::string digits;
            for (char ch : raw) if (ch != '_') digits += ch;
            emit(Tok::Integer, digits, lineNo, col);
        }
    }
};

} // namespace

std::vector<Token> lex(const std::string& src, const std::string& file, DiagnosticBag& diags) {
    Lexer lx(src, file, diags);
    return lx.run();
}

} // namespace nordshrift
