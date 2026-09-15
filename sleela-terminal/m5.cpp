// SleelaTerminal(TM) shell -- M5 orchestration layer.
// Original SLeeLa work (see NOTICE).
//
// M5 deliberately sits above the existing lexer/parser/executor. This keeps
// the mature M1-M4 grammar stable while adding shell constructs whose syntax
// needs process/signal resources before ordinary parsing begins.

#include "m5.hpp"

#include <cerrno>
#include <cctype>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace sleela::sh {
namespace {

volatile std::sig_atomic_t pending_signal = 0;

void m5SignalHandler(int sig) {
    pending_signal = sig;
}

struct Trap {
    std::string action;
    bool installed = false;
};

Trap traps[NSIG];

bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool startsWordAt(const std::string& s, std::size_t p, const char* word) {
    const std::size_t n = std::strlen(word);
    if (s.compare(p, n, word) != 0) return false;
    if (p > 0 && !isSpace(s[p - 1]) && s[p - 1] != ';') return false;
    if (p + n < s.size() && !isSpace(s[p + n]) && s[p + n] != ';') return false;
    return true;
}

std::size_t skipWord(const std::string& s, std::size_t p) {
    char quote = 0;
    while (p < s.size()) {
        char c = s[p];
        if (quote) {
            if (c == '\\' && p + 1 < s.size()) { p += 2; continue; }
            if (c == quote) quote = 0;
            ++p;
            continue;
        }
        if (c == '\'' || c == '"') { quote = c; ++p; continue; }
        if (isSpace(c) || c == ';') break;
        ++p;
    }
    return p;
}

std::string trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && isSpace(s[a])) ++a;
    while (b > a && isSpace(s[b - 1])) --b;
    return s.substr(a, b - a);
}

bool splitSelect(const std::string& source, std::string& var,
                 std::vector<std::string>& words, std::string& body) {
    std::size_t p = 0;
    while (p < source.size() && isSpace(source[p])) ++p;
    if (!startsWordAt(source, p, "select")) return false;
    p += 6;
    while (p < source.size() && isSpace(source[p]) && source[p] != '\n') ++p;

    std::size_t varEnd = skipWord(source, p);
    if (varEnd == p) return false;
    var = source.substr(p, varEnd - p);
    p = varEnd;
    while (p < source.size() && isSpace(source[p])) ++p;
    if (!startsWordAt(source, p, "in")) return false;
    p += 2;
    const std::size_t listStart = p;

    std::size_t listEnd = std::string::npos;
    std::size_t scan = p;
    char quote = 0;
    while (scan < source.size()) {
        char c = source[scan];
        if (quote) {
            if (c == '\\' && scan + 1 < source.size()) { scan += 2; continue; }
            if (c == quote) quote = 0;
            ++scan;
            continue;
        }
        if (c == '\'' || c == '"') { quote = c; ++scan; continue; }
        if (c == ';' || c == '\n') {
            std::size_t q = scan + 1;
            while (q < source.size() && isSpace(source[q])) ++q;
            if (startsWordAt(source, q, "do")) { listEnd = scan; p = q + 2; break; }
        }
        ++scan;
    }
    if (listEnd == std::string::npos) return false;

    std::string list = trim(source.substr(listStart, listEnd - listStart));
    std::size_t i = 0;
    while (i < list.size()) {
        while (i < list.size() && isSpace(list[i])) ++i;
        if (i >= list.size()) break;
        std::string w;
        char q = 0;
        while (i < list.size()) {
            char c = list[i];
            if (q) {
                if (c == '\\' && i + 1 < list.size()) { w.push_back(list[i + 1]); i += 2; continue; }
                if (c == q) { q = 0; ++i; continue; }
                w.push_back(c); ++i; continue;
            }
            if (c == '\'' || c == '"') { q = c; ++i; continue; }
            if (isSpace(c)) break;
            w.push_back(c); ++i;
        }
        if (!w.empty()) words.push_back(w);
    }

    std::size_t bodyStart = p;
    int depth = 1;
    scan = p;
    quote = 0;
    while (scan < source.size()) {
        if (quote) {
            if (source[scan] == '\\' && scan + 1 < source.size()) { scan += 2; continue; }
            if (source[scan] == quote) quote = 0;
            ++scan;
            continue;
        }
        if (source[scan] == '\'' || source[scan] == '"') { quote = source[scan]; ++scan; continue; }
        if ((scan == 0 || isSpace(source[scan - 1]) || source[scan - 1] == ';') &&
            startsWordAt(source, scan, "do")) { ++depth; scan += 2; continue; }
        if ((scan == 0 || isSpace(source[scan - 1]) || source[scan - 1] == ';') &&
            startsWordAt(source, scan, "done")) {
            --depth;
            if (depth == 0) {
                body = trim(source.substr(bodyStart, scan - bodyStart));
                return true;
            }
            scan += 4;
            continue;
        }
        ++scan;
    }
    return false;
}

int signalNumber(const std::string& name) {
    if (name == "INT") return SIGINT;
    if (name == "TERM") return SIGTERM;
    if (name == "HUP") return SIGHUP;
    if (name == "QUIT") return SIGQUIT;
    if (name == "USR1") return SIGUSR1;
    if (name == "USR2") return SIGUSR2;
    if (name == "PIPE") return SIGPIPE;
    if (name == "ALRM") return SIGALRM;
    return std::atoi(name.c_str());
}

void installTrap(int sig, const std::string& action) {
    if (sig <= 0 || sig >= NSIG) return;
    traps[sig].action = action;
    traps[sig].installed = true;
    struct sigaction sa{};
    sa.sa_handler = m5SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    ::sigaction(sig, &sa, nullptr);
}

bool parseTrap(const std::string& line, int& sig, std::string& action) {
    std::string t = trim(line);
    if (!startsWordAt(t, 0, "trap")) return false;
    std::size_t p = 4;
    while (p < t.size() && isSpace(t[p])) ++p;
    if (p >= t.size()) return false;

    if (t[p] == '\'' || t[p] == '"') {
        char q = t[p++];
        std::string a;
        while (p < t.size() && t[p] != q) {
            if (t[p] == '\\' && p + 1 < t.size()) { a.push_back(t[p + 1]); p += 2; }
            else { a.push_back(t[p]); ++p; }
        }
        if (p >= t.size()) return false;
        action = a;
        ++p;
        while (p < t.size() && isSpace(t[p])) ++p;
        sig = signalNumber(trim(t.substr(p)));
        return sig > 0;
    }

    std::size_t q = p;
    while (q < t.size() && !isSpace(t[q])) ++q;
    action = t.substr(p, q - p);
    while (q < t.size() && isSpace(t[q])) ++q;
    sig = signalNumber(trim(t.substr(q)));
    return sig > 0;
}

bool rewriteProcessSubstitution(const std::string& src, std::string& rewritten,
                                std::vector<pid_t>& children,
                                Environment& env, const M5Runner& runner) {
    rewritten.clear();
    bool found = false;
    for (std::size_t i = 0; i < src.size();) {
        if ((src[i] == '<' || src[i] == '>') && i + 1 < src.size() && src[i + 1] == '(') {
            const char mode = src[i];
            std::size_t j = i + 2;
            int depth = 1;
            char quote = 0;
            std::string cmd;
            while (j < src.size() && depth > 0) {
                char c = src[j];
                if (quote) {
                    cmd.push_back(c);
                    if (c == '\\' && j + 1 < src.size()) cmd.push_back(src[++j]);
                    else if (c == quote) quote = 0;
                    ++j;
                    continue;
                }
                if (c == '\'' || c == '"') { quote = c; cmd.push_back(c); ++j; continue; }
                if (c == '(') { ++depth; cmd.push_back(c); ++j; continue; }
                if (c == ')') { --depth; if (depth) cmd.push_back(c); ++j; continue; }
                cmd.push_back(c); ++j;
            }
            if (depth != 0) return false;

            char tmpl[] = "/tmp/slsh-m5-XXXXXX";
            int fd = ::mkstemp(tmpl);
            if (fd < 0) return false;
            ::close(fd);
            ::unlink(tmpl);
            if (::mkfifo(tmpl, 0600) != 0) return false;

            pid_t pid = ::fork();
            if (pid < 0) { ::unlink(tmpl); return false; }
            if (pid == 0) {
                Environment childEnv = env;
                int io = ::open(tmpl, mode == '<' ? O_WRONLY : O_RDONLY);
                if (io < 0) _exit(126);
                ::dup2(io, mode == '<' ? STDOUT_FILENO : STDIN_FILENO);
                ::close(io);
                int rc = runner(cmd, childEnv);
                _exit(rc & 0xff);
            }
            children.push_back(pid);
            rewritten += tmpl;
            i = j;
            found = true;
            continue;
        }
        rewritten.push_back(src[i++]);
    }
    return found;
}

int runSelect(const std::string& source, Environment& env,
              const M5Runner& runner) {
    std::string var, body;
    std::vector<std::string> words;
    if (!splitSelect(source, var, words, body)) return 2;

    const std::string ps3 = env.has("PS3") ? env.get("PS3") : "#? ";
    int status = 0;
    for (;;) {
        for (std::size_t i = 0; i < words.size(); ++i)
            std::cerr << (i + 1) << ") " << words[i] << '\n';
        std::cerr << ps3 << std::flush;

        std::string line;
        if (!std::getline(std::cin, line)) { std::cerr << '\n'; break; }
        env.set("REPLY", line);
        if (line.empty()) continue;
        char* end = nullptr;
        long choice = std::strtol(line.c_str(), &end, 10);
        if (end == line.c_str() || *end != '\0' || choice < 1 ||
            static_cast<std::size_t>(choice) > words.size()) {
            env.set(var, "");
        } else {
            env.set(var, words[static_cast<std::size_t>(choice) - 1]);
        }
        status = runner(body, env);
        if (env.shouldExit()) break;
        if (env.loopSignal()) {
            char sig = env.consumeLoopSignal();
            if (sig == 'B') break;
        }
    }
    return status;
}

} // namespace

bool hasProcessSubstitution(const std::string& source) {
    char quote = 0;
    for (std::size_t i = 0; i + 1 < source.size(); ++i) {
        if (quote) {
            if (source[i] == '\\') ++i;
            else if (source[i] == quote) quote = 0;
            continue;
        }
        if (source[i] == '\'' || source[i] == '"') { quote = source[i]; continue; }
        if ((source[i] == '<' || source[i] == '>') && source[i + 1] == '(') return true;
    }
    return false;
}

bool runM5(const std::string& source, Environment& env,
           const M5Runner& runner, int& status) {
    std::string prepared = source;
    // M5 select commonly uses PS3=... immediately before the select block.
    // Consume only a simple leading PS3 assignment here; all other assignments
    // remain under the normal lexer/parser.
    std::istringstream assignLines(prepared);
    std::string firstLine;
    std::string afterAssignments;
    bool consumedAssignment = false;
    while (std::getline(assignLines, firstLine)) {
        std::string t = trim(firstLine);
        const std::size_t eq = t.find('=');
        bool simpleName = eq != std::string::npos && eq > 0;
        if (simpleName) {
            for (std::size_t k = 0; k < eq; ++k) {
                char c = t[k];
                if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_') ||
                    (k == 0 && std::isdigit(static_cast<unsigned char>(c)))) {
                    simpleName = false; break;
                }
            }
        }
        if (!consumedAssignment && simpleName && t.compare(0, eq, "PS3") == 0) {
            std::string value = t.substr(eq + 1);
            if (value.size() >= 2 &&
                ((value.front() == '\'' && value.back() == '\'') ||
                 (value.front() == '"' && value.back() == '"')))
                value = value.substr(1, value.size() - 2);
            env.set("PS3", value);
            consumedAssignment = true;
            continue;
        }
        afterAssignments += firstLine;
        afterAssignments.push_back('\n');
        std::string tail;
        while (std::getline(assignLines, tail)) {
            afterAssignments += tail;
            afterAssignments.push_back('\n');
        }
        break;
    }
    if (consumedAssignment) prepared = afterAssignments;

    std::istringstream lines(prepared);
    std::string line;
    std::string remainder;
    bool consumedTrap = false;
    while (std::getline(lines, line)) {
        int sig = 0; std::string action;
        if (parseTrap(line, sig, action)) {
            installTrap(sig, action);
            consumedTrap = true;
        } else {
            remainder += line;
            remainder.push_back('\n');
        }
    }

    std::vector<pid_t> children;
    std::string rewritten;
    if (hasProcessSubstitution(remainder)) {
        if (!rewriteProcessSubstitution(remainder, rewritten, children, env, runner)) {
            status = 2;
            return true;
        }
        status = runner(rewritten, env);
        for (pid_t pid : children) {
            int st = 0;
            ::waitpid(pid, &st, 0);
        }
        return true;
    }

    std::string var, body;
    std::vector<std::string> words;
    if (splitSelect(remainder, var, words, body)) {
        status = runSelect(remainder, env, runner);
        return true;
    }

    bool anyTrap = consumedTrap;
    for (int s = 1; s < NSIG && !anyTrap; ++s) anyTrap = traps[s].installed;
    if (anyTrap) {
        status = remainder.empty() ? 0 : runner(remainder, env);
        const int sig = pending_signal;
        if (sig > 0 && sig < NSIG && traps[sig].installed && !traps[sig].action.empty()) {
            pending_signal = 0;
            status = runner(traps[sig].action, env);
        }
        return true;
    }

    return false;
}

} // namespace sleela::sh
