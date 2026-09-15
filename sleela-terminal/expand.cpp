// SleelaTerminal(TM) shell -- L4 expansion implementation.
// Original SLeeLa work (see NOTICE).

#include "expand.hpp"
#include "arith.hpp"

#include <cctype>
#include <unistd.h>

namespace sleela::sh {

namespace {

bool isNameStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}
bool isNameChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

// Read a variable's value, with the shell specials $? and $$.
std::string lookupParam(const std::string& name, const Environment& env) {
    if (name == "?") return std::to_string(env.lastStatus());
    return env.get(name);
}

} // namespace

std::string expandWord(const std::string& word, const Environment& env) {
    std::string out;
    const std::size_t n = word.size();
    std::size_t i = 0;

    auto varLookup = [&env](const std::string& id) -> long {
        return Value(env.get(id)).asInt();
    };

    while (i < n) {
        const char c = word[i];
        if (c != '$') { out.push_back(c); ++i; continue; }

        // $ at end -> literal
        if (i + 1 >= n) { out.push_back('$'); ++i; break; }

        const char d = word[i + 1];

        // arithmetic expansion: $(( expr ))
        if (d == '(' && i + 2 < n && word[i + 2] == '(') {
            std::size_t j = i + 3;
            int depth = 1;  // we've opened one "((" pair logically; match "))"
            std::string expr;
            // scan until the matching "))"
            while (j < n) {
                if (word[j] == '(' ) { ++depth; expr.push_back(word[j]); ++j; continue; }
                if (word[j] == ')') {
                    if (j + 1 < n && word[j + 1] == ')' && depth == 1) {
                        j += 2;  // consume "))"
                        depth = 0;
                        break;
                    }
                    --depth;
                    expr.push_back(word[j]);
                    ++j;
                    continue;
                }
                expr.push_back(word[j]);
                ++j;
            }
            if (depth != 0) {  // unterminated; emit literally
                out.append(word, i, std::string::npos);
                break;
            }
            ArithResult r = evalArith(expr, varLookup);
            out += r.ok ? std::to_string(r.value) : std::string("0");
            i = j;
            continue;
        }

        // ${name}
        if (d == '{') {
            std::size_t j = i + 2;
            std::string name;
            while (j < n && word[j] != '}') { name.push_back(word[j]); ++j; }
            if (j >= n) { out.append(word, i, std::string::npos); break; }  // unterminated
            ++j;  // consume '}'
            out += lookupParam(name, env);
            i = j;
            continue;
        }

        // $? and $$ specials
        if (d == '?') { out += std::to_string(env.lastStatus()); i += 2; continue; }
        if (d == '$') { out += std::to_string(static_cast<long>(::getpid())); i += 2; continue; }

        // $name
        if (isNameStart(d)) {
            std::size_t j = i + 1;
            std::string name;
            while (j < n && isNameChar(word[j])) { name.push_back(word[j]); ++j; }
            out += lookupParam(name, env);
            i = j;
            continue;
        }

        // $ followed by something else -> literal $
        out.push_back('$');
        ++i;
    }

    return out;
}

} // namespace sleela::sh
