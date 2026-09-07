// ===========================================================================
// sheet_catalog.cpp  --  Implementation of the shared SHEET.sheet parser.
// ===========================================================================
#include "sheet_catalog.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace catalog {
namespace {

// Strip a trailing `# ...` comment (outside this format there are no strings
// on structural lines, so a bare '#' always starts a comment).
std::string stripComment(const std::string& line) {
    size_t h = line.find('#');
    return h == std::string::npos ? line : line.substr(0, h);
}

// Count leading spaces (indentation).
int indentOf(const std::string& line) {
    int n = 0;
    while (n < (int)line.size() && line[n] == ' ') n++;
    return n;
}

std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t')) a++;
    while (b > a && (s[b-1] == ' ' || s[b-1] == '\t' || s[b-1] == '\r')) b--;
    return s.substr(a, b - a);
}

// Split a comma-separated list; "-" yields an empty list.
std::vector<std::string> splitList(const std::string& s) {
    std::vector<std::string> out;
    std::string t = trim(s);
    if (t == "-" || t.empty()) return out;
    std::string cur;
    for (char c : t) {
        if (c == ',') { std::string v = trim(cur); if (!v.empty()) out.push_back(v); cur.clear(); }
        else cur += c;
    }
    std::string v = trim(cur); if (!v.empty()) out.push_back(v);
    return out;
}

// Extract `# role: X` from a raw section line (before comment stripping).
std::string roleOf(const std::string& rawLine) {
    size_t p = rawLine.find("role:");
    if (p == std::string::npos) return "";
    return trim(rawLine.substr(p + 5));
}

} // namespace

bool Catalog::congruent(const std::string& a, const std::string& b) const {
    if (a == b) return true;
    const Object* oa = find(a);
    const Object* ob = find(b);
    if (!oa || !ob) return false;
    if (oa->role == ob->role) return true;           // same conduct role
    for (auto& s : oa->siblings) if (s == b) return true;
    for (auto& s : ob->siblings) if (s == a) return true;
    return false;
}

Catalog parseCatalogText(const std::string& text, bool* ok) {
    Catalog cat;
    if (ok) *ok = true;

    std::istringstream in(text);
    std::string raw;
    // Use indices, not pointers: the vectors grow during the parse and would
    // otherwise invalidate any held pointer on reallocation.
    int curSection = -1;
    int curObject  = -1;

    while (std::getline(in, raw)) {
        // header directive: `#key value` at column 0 (not a `# comment`)
        if (!raw.empty() && raw[0] == '#') {
            std::string body = trim(raw.substr(1));
            // a directive is `key value`; a comment usually starts with a space
            if (!body.empty() && body[0] != ' ' && body[0] != '#' &&
                body[0] != '=' && body[0] != '-') {
                size_t sp = body.find_first_of(" \t");
                std::string k = sp == std::string::npos ? body : body.substr(0, sp);
                std::string v = sp == std::string::npos ? "" : trim(body.substr(sp));
                // only accept simple identifier keys (kind/for/rev/format/machine)
                bool ident = true;
                for (char c : k) if (!(isalnum((unsigned char)c) || c=='-' || c=='_')) { ident = false; break; }
                if (ident && !k.empty()) cat.header[k] = v;
            }
            continue;
        }

        std::string line = stripComment(raw);
        std::string t = trim(line);
        if (t.empty()) continue;                     // blank / comment-only

        int ind = indentOf(line);

        // section header: `section <name>:`
        if (t.rfind("section ", 0) == 0) {
            Section s;
            std::string rest = trim(t.substr(8));
            if (!rest.empty() && rest.back() == ':') rest.pop_back();
            s.name = trim(rest);
            s.role = roleOf(raw);                    // from the raw `# role:` tag
            cat.sections.push_back(s);
            curSection = (int)cat.sections.size() - 1;
            curObject = -1;
            continue;
        }
        if (curSection < 0) continue;                // ignore anything before first section

        // object attribute lines (indent >= 4): children/siblings/note
        if (ind >= 4 && curObject >= 0) {
            size_t colon = t.find(':');
            if (colon != std::string::npos) {
                std::string key = trim(t.substr(0, colon));
                std::string val = trim(t.substr(colon + 1));
                Object& o = cat.objects[(size_t)curObject];
                if (key == "children") o.children = splitList(val);
                else if (key == "siblings") o.siblings = splitList(val);
                else if (key == "note") o.note = val;
            }
            continue;
        }

        // indent == 2: either a section-level `key: value` or an Object name
        size_t colon = t.find(':');
        if (colon != std::string::npos) {
            // section-level value (e.g. depth: 3024)
            std::string key = trim(t.substr(0, colon));
            std::string val = trim(t.substr(colon + 1));
            cat.sections[(size_t)curSection].values[key] = val;
            if (cat.sections[(size_t)curSection].name == "system") {
                if (key == "depth") cat.depth = std::atoll(val.c_str());
                else if (key == "congruent-linear-systems-max") cat.congruentLinearMax = std::atoll(val.c_str());
                else if (key == "complexity-degree-max") cat.complexityDegreeMax = std::atoll(val.c_str());
            }
            curObject = -1;
            continue;
        }

        // otherwise: a bare Object name
        Object o;
        o.name = t;
        o.section = cat.sections[(size_t)curSection].name;
        o.role = cat.sections[(size_t)curSection].role;
        cat.objects.push_back(o);
        cat.objectIndex[o.name] = (int)cat.objects.size() - 1;
        cat.sections[(size_t)curSection].objects.push_back(o.name);
        curObject = (int)cat.objects.size() - 1;
    }

    if (cat.objects.empty() && ok) *ok = false;
    return cat;
}

Catalog parseCatalogFile(const std::string& path, bool* ok) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { if (ok) *ok = false; return Catalog{}; }
    std::ostringstream ss; ss << f.rdbuf();
    return parseCatalogText(ss.str(), ok);
}

} // namespace catalog
