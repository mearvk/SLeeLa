// SleelaTerminal(TM) shell -- L4 expansion implementation.
// Original SLeeLa work (see NOTICE).

#include "expand.hpp"
#include "arith.hpp"

#include <algorithm>
#include <cctype>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

namespace sleela::sh {

namespace {

bool isNameStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}
bool isNameChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

std::string lookupParam(const std::string& name, const Environment& env) {
    if (name == "?") return std::to_string(env.lastStatus());
    if (name == "#") return std::to_string(env.positionalCount());
    // $1..$9 etc.
    if (!name.empty() && std::all_of(name.begin(), name.end(),
            [](char c){ return std::isdigit(static_cast<unsigned char>(c)); })) {
        return env.getPositional(static_cast<std::size_t>(Value(name).asInt()));
    }
    return env.get(name);
}

// ---------------------------------------------------------------------------
// Pass 1-3: expand $ sequences of a word into a single string. We do NOT split
// or glob here; that happens after. Returns the expanded string.
// ---------------------------------------------------------------------------
std::string expandDollar(const std::string& word, const Environment& env,
                         const CommandRunner& run) {
    std::string out;
    const std::size_t n = word.size();
    std::size_t i = 0;

    auto varLookup = [&env](const std::string& id) -> long {
        // arithmetic sees $1.. and named vars alike
        if (!id.empty() && std::all_of(id.begin(), id.end(),
                [](char c){ return std::isdigit(static_cast<unsigned char>(c)); })) {
            return Value(id).asInt();
        }
        return Value(env.get(id)).asInt();
    };

    while (i < n) {
        const char c = word[i];
        if (c != '$') { out.push_back(c); ++i; continue; }
        if (i + 1 >= n) { out.push_back('$'); ++i; break; }
        const char d = word[i + 1];

        // $(( expr )) arithmetic
        if (d == '(' && i + 2 < n && word[i + 2] == '(') {
            std::size_t j = i + 3;
            int depth = 1;
            std::string expr;
            while (j < n) {
                if (word[j] == '(') { ++depth; expr.push_back(word[j]); ++j; }
                else if (word[j] == ')') {
                    if (j + 1 < n && word[j + 1] == ')' && depth == 1) { j += 2; depth = 0; break; }
                    --depth; expr.push_back(word[j]); ++j;
                } else { expr.push_back(word[j]); ++j; }
            }
            if (depth != 0) { out.append(word, i, std::string::npos); break; }
            ArithResult r = evalArith(expr, varLookup);
            out += r.ok ? std::to_string(r.value) : std::string("0");
            i = j;
            continue;
        }

        // $( ... ) command substitution
        if (d == '(') {
            std::size_t j = i + 2;
            int depth = 1;
            std::string cmd;
            while (j < n && depth > 0) {
                if (word[j] == '(') { ++depth; cmd.push_back(word[j]); ++j; }
                else if (word[j] == ')') { --depth; if (depth > 0) cmd.push_back(word[j]); ++j; }
                else { cmd.push_back(word[j]); ++j; }
            }
            if (depth != 0) { out.append(word, i, std::string::npos); break; }
            std::string captured = run ? run(cmd) : std::string();
            // trailing newlines are stripped by command substitution
            while (!captured.empty() && (captured.back() == '\n' || captured.back() == '\r'))
                captured.pop_back();
            out += captured;
            i = j;
            continue;
        }

        // ${name}
        if (d == '{') {
            std::size_t j = i + 2;
            std::string name;
            while (j < n && word[j] != '}') { name.push_back(word[j]); ++j; }
            if (j >= n) { out.append(word, i, std::string::npos); break; }
            ++j;
            out += lookupParam(name, env);
            i = j;
            continue;
        }

        // $? $$ $# specials
        if (d == '?') { out += std::to_string(env.lastStatus()); i += 2; continue; }
        if (d == '#') { out += std::to_string(env.positionalCount()); i += 2; continue; }
        if (d == '$') { out += std::to_string(static_cast<long>(::getpid())); i += 2; continue; }

        // $@ -> all positionals joined by space (this pass; splitting later)
        if (d == '@') {
            const auto ps = env.positionals();
            for (std::size_t k = 0; k < ps.size(); ++k) {
                if (k) out.push_back(' ');
                out += ps[k];
            }
            i += 2;
            continue;
        }

        // $1..$9 (single digit positionals)
        if (std::isdigit(static_cast<unsigned char>(d))) {
            std::size_t j = i + 1;
            std::string num;
            while (j < n && std::isdigit(static_cast<unsigned char>(word[j]))) { num.push_back(word[j]); ++j; }
            out += env.getPositional(static_cast<std::size_t>(Value(num).asInt()));
            i = j;
            continue;
        }

        // $name
        if (isNameStart(d)) {
            std::size_t j = i + 1;
            std::string name;
            while (j < n && isNameChar(word[j])) { name.push_back(word[j]); ++j; }
            out += lookupParam(name, env);
            i = j;
            continue;
        }

        out.push_back('$');
        ++i;
    }
    return out;
}

// ---------------------------------------------------------------------------
// Pass 4: split on unquoted whitespace. This milestone splits the whole
// expanded string (quotes were removed by the lexer, so we cannot know which
// spaces were quoted; we treat the *original word* having had no spaces as a
// hint by only splitting fields that arose from expansion). To keep behaviour
// predictable and safe, we split on runs of spaces/tabs/newlines.
// ---------------------------------------------------------------------------
std::vector<std::string> fieldSplit(const std::string& s) {
    std::vector<std::string> fields;
    std::string cur;
    bool inField = false;
    for (char c : s) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (inField) { fields.push_back(cur); cur.clear(); inField = false; }
        } else {
            cur.push_back(c);
            inField = true;
        }
    }
    if (inField) fields.push_back(cur);
    return fields;
}

// ---------------------------------------------------------------------------
// Pass 5: glob matching. globMatch implements * ? [..]; globPattern walks a
// path segment-by-segment against the current directory tree.
// ---------------------------------------------------------------------------

// Match text against pattern with * ? and [set] (supporting [a-z] and [!..]).
bool matchHere(const char* p, const char* t);

bool matchClass(const char*& p, char ch, bool& matched) {
    // p points just after '['
    bool negate = false;
    if (*p == '!' || *p == '^') { negate = true; ++p; }
    matched = false;
    bool first = true;
    while (*p && (*p != ']' || first)) {
        first = false;
        if (p[0] && p[1] == '-' && p[2] && p[2] != ']') {
            char lo = p[0], hi = p[2];
            if (ch >= lo && ch <= hi) matched = true;
            p += 3;
        } else {
            if (*p == ch) matched = true;
            ++p;
        }
    }
    if (*p == ']') ++p;  // consume closing ]
    if (negate) matched = !matched;
    return true;
}

bool matchHere(const char* p, const char* t) {
    while (*p) {
        if (*p == '*') {
            ++p;
            if (*p == '\0') return true;  // trailing * matches the rest
            for (const char* s = t; ; ++s) {
                if (matchHere(p, s)) return true;
                if (*s == '\0') return false;
            }
        }
        if (*t == '\0') return false;
        if (*p == '?') { ++p; ++t; continue; }
        if (*p == '[') {
            const char* pp = p + 1;
            bool matched = false;
            matchClass(pp, *t, matched);
            if (!matched) return false;
            p = pp; ++t;
            continue;
        }
        if (*p != *t) return false;
        ++p; ++t;
    }
    return *t == '\0';
}

bool hasGlobMeta(const std::string& s) {
    return s.find_first_of("*?[") != std::string::npos;
}

} // namespace

bool globMatch(const std::string& pattern, const std::string& text) {
    return matchHere(pattern.c_str(), text.c_str());
}

std::vector<std::string> globPattern(const std::string& pattern) {
    if (!hasGlobMeta(pattern)) return {pattern};

    // Split into directory prefix and the final segment; only glob a single
    // path segment here (M2 scope). Deeper path globbing is a later milestone.
    const auto slash = pattern.find_last_of('/');
    std::string dir = (slash == std::string::npos) ? "." : pattern.substr(0, slash + 1);
    std::string seg = (slash == std::string::npos) ? pattern : pattern.substr(slash + 1);
    if (!hasGlobMeta(seg)) return {pattern};

    const std::string scandir = dir.empty() ? "." : (slash == std::string::npos ? "." : dir);
    DIR* dp = ::opendir(scandir.c_str());
    if (!dp) return {pattern};

    std::vector<std::string> matches;
    struct dirent* de;
    while ((de = ::readdir(dp)) != nullptr) {
        const std::string name = de->d_name;
        if (name == "." || name == "..") continue;
        // a leading '.' is only matched by an explicit leading '.' in seg
        if (!name.empty() && name[0] == '.' && !(seg.size() && seg[0] == '.')) continue;
        if (globMatch(seg, name)) {
            matches.push_back((slash == std::string::npos) ? name : dir + name);
        }
    }
    ::closedir(dp);

    if (matches.empty()) return {pattern};   // nullglob off: literal pattern
    std::sort(matches.begin(), matches.end());
    return matches;
}

std::vector<std::string> expandWord(const std::string& word,
                                    const Environment& env,
                                    const CommandRunner& run) {
    const std::string expanded = expandDollar(word, env, run);
    std::vector<std::string> fields = fieldSplit(expanded);

    // If nothing produced a field (e.g. word expanded to empty), keep a single
    // empty field only when the original word was itself non-empty and had no
    // expansions that vanished -- otherwise drop. Simplest safe rule: if the
    // original word had no whitespace-producing $ expansion, keep one field.
    if (fields.empty()) {
        if (!expanded.empty()) return {expanded};
        return {};  // fully empty expansion contributes no field
    }

    // glob each field
    std::vector<std::string> out;
    for (const auto& f : fields) {
        auto g = globPattern(f);
        out.insert(out.end(), g.begin(), g.end());
    }
    return out;
}

std::string expandWordSingle(const std::string& word,
                             const Environment& env,
                             const CommandRunner& run) {
    // No field splitting or globbing; join the raw $-expansion.
    return expandDollar(word, env, run);
}

} // namespace sleela::sh
