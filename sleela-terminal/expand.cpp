// SleelaTerminal(TM) shell -- L4 expansion implementation.
// Original SLeeLa work (see NOTICE).

#include "expand.hpp"
#include "arith.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <dirent.h>
#include <pwd.h>
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
std::string expandDollar(const std::string& word, Environment& env,
                         const CommandRunner& run);

// Apply a ${name OP word} parameter operator. `content` is the text between the
// braces (operators and the trailing word); `env`/`run` allow the operator's
// word to itself be expanded, and := to assign back. Returns the resulting
// string; sets `err` non-empty on ${x:?msg} when x is unset/empty.
std::string applyParamBraces(const std::string& content, Environment& env,
                             const CommandRunner& run, std::string& err) {
    // ${#name} -> length of the value
    if (!content.empty() && content[0] == '#' && content.size() > 1) {
        const std::string name = content.substr(1);
        return std::to_string(lookupParam(name, env).size());
    }

    // find an operator: :- := :? :+ (with the ':' meaning "unset or empty")
    // or the bare forms - = ? + ("unset" only). We support the ':' forms and
    // treat the bare forms the same for this milestone.
    std::size_t op = std::string::npos;
    char opc = 0;
    bool colon = false;
    for (std::size_t k = 0; k < content.size(); ++k) {
        const char ch = content[k];
        if (ch == '-' || ch == '=' || ch == '?' || ch == '+') {
            op = k; opc = ch;
            colon = (k > 0 && content[k - 1] == ':');
            break;
        }
    }
    if (op == std::string::npos) {
        // plain ${name}
        return lookupParam(content, env);
    }

    const std::size_t nameEnd = colon ? op - 1 : op;
    const std::string name = content.substr(0, nameEnd);
    const std::string argRaw = content.substr(op + 1);
    const std::string arg = expandDollar(argRaw, env, run);  // recursive expand

    const std::string cur = lookupParam(name, env);
    const bool unsetOrEmpty = colon ? cur.empty() : !env.has(name);

    switch (opc) {
        case '-':  // use default if unset/empty
            return unsetOrEmpty ? arg : cur;
        case '=':  // assign default if unset/empty, then use it
            if (unsetOrEmpty) { env.set(name, arg); return arg; }
            return cur;
        case '?':  // error if unset/empty
            if (unsetOrEmpty) { err = name + ": " + (arg.empty() ? "parameter null or not set" : arg); return {}; }
            return cur;
        case '+':  // use alternate if set/non-empty
            return unsetOrEmpty ? std::string() : arg;
        default:
            return cur;
    }
}

std::string expandDollar(const std::string& word, Environment& env,
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

        // ${name} and ${name OP word} parameter expansion (nested { } allowed
        // in the operator word).
        if (d == '{') {
            std::size_t j = i + 2;
            std::string content;
            int depth = 1;
            while (j < n && depth > 0) {
                if (word[j] == '{') { ++depth; content.push_back(word[j]); ++j; }
                else if (word[j] == '}') { --depth; if (depth > 0) content.push_back(word[j]); ++j; }
                else { content.push_back(word[j]); ++j; }
            }
            if (depth != 0) { out.append(word, i, std::string::npos); break; }
            std::string err;
            out += applyParamBraces(content, env, run, err);
            // (an ${x:?msg} error is reported by the caller path in a fuller
            // shell; here we surface it via stderr and continue.)
            if (!err.empty()) std::fprintf(stderr, "slsh: %s\n", err.c_str());
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

// --- tilde expansion ---------------------------------------------------------
// Expand a leading ~ or ~user in a word. Only the prefix up to the first '/'
// (or end) is a tilde-prefix. ~ -> $HOME; ~user -> that user's home directory.
std::string tildeExpand(const std::string& word, const Environment& env) {
    if (word.empty() || word[0] != '~') return word;
    const std::size_t slash = word.find('/');
    const std::string prefix = (slash == std::string::npos) ? word : word.substr(0, slash);
    const std::string rest = (slash == std::string::npos) ? std::string() : word.substr(slash);

    std::string home;
    if (prefix == "~") {
        home = env.get("HOME");
        if (home.empty()) {
            if (const passwd* pw = ::getpwuid(::getuid())) home = pw->pw_dir;
        }
    } else {
        const std::string user = prefix.substr(1);  // after '~'
        if (const passwd* pw = ::getpwnam(user.c_str())) home = pw->pw_dir;
        else return word;  // unknown user -> leave literal
    }
    if (home.empty()) return word;
    return home + rest;
}

// --- brace expansion ---------------------------------------------------------
// Expand the first {..} group in `word` into fields, recursing so multiple
// groups combine. Supports comma lists {a,b,c} and numeric ranges {m..n}.
// If no valid group, returns {word}.
std::vector<std::string> braceExpand(const std::string& word) {
    // find a top-level '{'
    const std::size_t open = word.find('{');
    if (open == std::string::npos) return {word};

    // find its matching '}', tracking nesting
    std::size_t close = std::string::npos;
    int depth = 0;
    for (std::size_t k = open; k < word.size(); ++k) {
        if (word[k] == '{') ++depth;
        else if (word[k] == '}') { --depth; if (depth == 0) { close = k; break; } }
    }
    if (close == std::string::npos) return {word};

    const std::string pre = word.substr(0, open);
    const std::string body = word.substr(open + 1, close - open - 1);
    const std::string post = word.substr(close + 1);

    // collect the alternatives from `body`
    std::vector<std::string> alts;

    // numeric range {m..n}
    const std::size_t dots = body.find("..");
    bool numericRange = false;
    if (dots != std::string::npos && body.find(',') == std::string::npos) {
        const std::string a = body.substr(0, dots);
        const std::string b = body.substr(dots + 2);
        auto isInt = [](const std::string& s) {
            if (s.empty()) return false;
            std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
            if (i >= s.size()) return false;
            for (; i < s.size(); ++i) if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
            return true;
        };
        if (isInt(a) && isInt(b)) {
            numericRange = true;
            long lo = std::stol(a), hi = std::stol(b);
            if (lo <= hi) for (long v = lo; v <= hi; ++v) alts.push_back(std::to_string(v));
            else          for (long v = lo; v >= hi; --v) alts.push_back(std::to_string(v));
        }
    }

    if (!numericRange) {
        // comma list (respecting nested braces); a single element (no comma)
        // is NOT a brace expansion -> treat literally.
        std::vector<std::string> parts;
        int d = 0;
        std::string cur;
        for (char ch : body) {
            if (ch == '{') { ++d; cur.push_back(ch); }
            else if (ch == '}') { --d; cur.push_back(ch); }
            else if (ch == ',' && d == 0) { parts.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        parts.push_back(cur);
        if (parts.size() < 2) return {word};  // no real expansion
        alts = std::move(parts);
    }

    // combine pre + each alt (recursively brace-expanded) + brace-expanded post
    std::vector<std::string> out;
    std::vector<std::string> posts = braceExpand(post);
    for (const auto& alt : alts) {
        for (const auto& altExpanded : braceExpand(alt)) {
            for (const auto& p : posts) {
                out.push_back(pre + altExpanded + p);
            }
        }
    }
    return out;
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
                                    Environment& env,
                                    const CommandRunner& run) {
    std::vector<std::string> out;
    // Pass 1: brace expansion -> multiple raw words.
    for (const std::string& braced : braceExpand(word)) {
        // Pass 2: tilde expansion (word-start prefix).
        const std::string tilded = tildeExpand(braced, env);
        // Passes 3-5: command/arith/parameter expansion.
        const std::string expanded = expandDollar(tilded, env, run);
        // Pass 6: field splitting.
        std::vector<std::string> fields = fieldSplit(expanded);
        if (fields.empty()) {
            if (!expanded.empty()) fields.push_back(expanded);
            else continue;  // fully empty expansion contributes no field
        }
        // Pass 7: glob each field.
        for (const auto& f : fields) {
            auto g = globPattern(f);
            out.insert(out.end(), g.begin(), g.end());
        }
    }
    return out;
}

std::string expandWordSingle(const std::string& word,
                             Environment& env,
                             const CommandRunner& run) {
    // One string: tilde then $-expansion; no brace/split/glob multiplication.
    const std::string tilded = tildeExpand(word, env);
    return expandDollar(tilded, env, run);
}

} // namespace sleela::sh
