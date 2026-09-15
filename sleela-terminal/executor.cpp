// SleelaTerminal(TM) shell -- L5 executor implementation.
// Original SLeeLa work (see NOTICE).

#include "executor.hpp"
#include "expand.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace sleela::sh {

// forward declaration (defined below): run source text, capture its stdout.
static std::string captureCommand(const std::string& src, Environment& env);

namespace {

// A resolved simple command: argv + redirections, ready to run.
struct Resolved {
    std::vector<std::string> argv;
    std::vector<Redirection> redirs;  // targets already expanded
};

// Build a CommandRunner bound to this environment for command substitution.
CommandRunner makeRunner(Environment& env) {
    return [&env](const std::string& command) -> std::string {
        return captureCommand(command, env);
    };
}

// Apply the resolved redirections in the current process (used post-fork or in
// a builtin's scoped context). Returns 0 on success, -1 on error.
int applyRedirs(const std::vector<Redirection>& redirs) {
    for (const auto& r : redirs) {
        int fd = -1;
        if (r.op == RedirOp::Heredoc) {
            // Write the here-document body into a pipe and dup its read end onto
            // stdin. The body is small (fits the pipe buffer for typical use);
            // we write from a forked helper to avoid blocking on large bodies.
            int hp[2];
            if (::pipe(hp) != 0) { std::perror("heredoc pipe"); return -1; }
            pid_t w = ::fork();
            if (w == 0) {
                ::close(hp[0]);
                const std::string& b = r.body;
                std::size_t off = 0;
                while (off < b.size()) {
                    ssize_t k = ::write(hp[1], b.data() + off, b.size() - off);
                    if (k <= 0) break;
                    off += static_cast<std::size_t>(k);
                }
                ::close(hp[1]);
                _exit(0);
            }
            ::close(hp[1]);
            ::dup2(hp[0], r.fd >= 0 ? r.fd : 0);
            ::close(hp[0]);
            continue;
        }
        if (r.op == RedirOp::In) {
            fd = ::open(r.target.c_str(), O_RDONLY);
            if (fd < 0) { std::perror(r.target.c_str()); return -1; }
            ::dup2(fd, r.fd >= 0 ? r.fd : 0);
        } else {
            const int flags = O_WRONLY | O_CREAT | (r.op == RedirOp::Append ? O_APPEND : O_TRUNC);
            fd = ::open(r.target.c_str(), flags, 0644);
            if (fd < 0) { std::perror(r.target.c_str()); return -1; }
            ::dup2(fd, r.fd >= 0 ? r.fd : 1);
        }
        ::close(fd);
    }
    return 0;
}

// ---- builtins ----
// A builtin runs in-process. It returns an exit status. Output goes to the
// current stdout (fd 1), which the caller may have redirected.
using Builtin = std::function<int(const std::vector<std::string>&, Environment&)>;

int biEcho(const std::vector<std::string>& a, Environment&) {
    bool newline = true;
    std::size_t start = 1;
    if (a.size() > 1 && a[1] == "-n") { newline = false; start = 2; }
    for (std::size_t i = start; i < a.size(); ++i) {
        if (i > start) std::cout << ' ';
        std::cout << a[i];
    }
    if (newline) std::cout << '\n';
    std::cout.flush();
    return 0;
}

int biPwd(const std::vector<std::string>&, Environment&) {
    char buf[4096];
    if (::getcwd(buf, sizeof buf)) { std::cout << buf << '\n'; std::cout.flush(); return 0; }
    std::perror("pwd");
    return 1;
}

int biCd(const std::vector<std::string>& a, Environment& env) {
    std::string dir = (a.size() > 1) ? a[1] : env.get("HOME");
    if (dir.empty()) dir = "/";
    if (::chdir(dir.c_str()) != 0) { std::perror(("cd: " + dir).c_str()); return 1; }
    char buf[4096];
    if (::getcwd(buf, sizeof buf)) env.set("PWD", buf);
    return 0;
}

int biExport(const std::vector<std::string>& a, Environment& env) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        const auto eq = a[i].find('=');
        if (eq == std::string::npos) {
            env.exportVar(a[i]);
        } else {
            const std::string name = a[i].substr(0, eq);
            env.set(name, a[i].substr(eq + 1));
            env.exportVar(name);
        }
    }
    return 0;
}

int biUnset(const std::vector<std::string>& a, Environment& env) {
    for (std::size_t i = 1; i < a.size(); ++i) env.unset(a[i]);
    return 0;
}

int biTrue(const std::vector<std::string>&, Environment&) { return 0; }
int biFalse(const std::vector<std::string>&, Environment&) { return 1; }
int biColon(const std::vector<std::string>&, Environment&) { return 0; }

int biBreak(const std::vector<std::string>& a, Environment& env) {
    int levels = (a.size() > 1) ? static_cast<int>(Value(a[1]).asInt()) : 1;
    env.requestBreak(levels);
    return 0;
}

int biContinue(const std::vector<std::string>& a, Environment& env) {
    int levels = (a.size() > 1) ? static_cast<int>(Value(a[1]).asInt()) : 1;
    env.requestContinue(levels);
    return 0;
}

int biExit(const std::vector<std::string>& a, Environment& env) {
    int code = env.lastStatus();
    if (a.size() > 1) code = static_cast<int>(Value(a[1]).asInt());
    env.requestExit(code);
    return code;
}

int biSet(const std::vector<std::string>&, Environment& env) {
    // Minimal: with no args, print exported variables (name=value).
    for (const auto& kv : env.exportedEnviron()) std::cout << kv << '\n';
    std::cout.flush();
    return 0;
}

// ---- job-control builtins (M4) ----

// Reap any jobs that have already terminated, marking them not-running.
// Non-blocking: uses WNOHANG so `jobs` reflects reality without stalling.
void reapFinishedJobs(Environment& env) {
    for (auto& j : env.jobs()) {
        if (!j.running) continue;
        int st = 0;
        pid_t r = ::waitpid(static_cast<pid_t>(j.pid), &st, WNOHANG);
        if (r == static_cast<pid_t>(j.pid)) j.running = false;
    }
}

// jobs -- list background jobs and their state.
int biJobs(const std::vector<std::string>&, Environment& env) {
    reapFinishedJobs(env);
    for (const auto& j : env.jobs()) {
        std::cout << "[" << j.id << "] " << (j.running ? "Running" : "Done")
                  << "  " << j.command << "  (" << j.pid << ")\n";
    }
    std::cout.flush();
    return 0;
}

// Parse a job spec: "%3" or "3" -> job id 3; empty -> the last job (0 if none).
int jobIdFromSpec(const std::string& s, Environment& env) {
    if (s.empty()) {
        return env.jobs().empty() ? 0 : env.jobs().back().id;
    }
    std::string t = (s[0] == '%') ? s.substr(1) : s;
    return static_cast<int>(Value(t).asInt());
}

// wait [id] -- wait for a background job (or all jobs) to finish.
int biWait(const std::vector<std::string>& a, Environment& env) {
    if (a.size() > 1) {
        int id = jobIdFromSpec(a[1], env);
        Environment::Job* j = env.findJob(id);
        if (!j) return 127;
        int st = 0;
        if (j->running) ::waitpid(static_cast<pid_t>(j->pid), &st, 0);
        j->running = false;
        int rc = WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
        env.removeJob(id);
        return rc;
    }
    // No arg: wait for every job.
    int rc = 0;
    for (auto& j : env.jobs()) {
        if (j.running) {
            int st = 0;
            ::waitpid(static_cast<pid_t>(j.pid), &st, 0);
            j.running = false;
            rc = WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
        }
    }
    // Clear all reaped jobs.
    while (!env.jobs().empty()) env.removeJob(env.jobs().front().id);
    return rc;
}

// fg [id] -- bring a background job to the foreground (wait for it here).
int biFg(const std::vector<std::string>& a, Environment& env) {
    int id = jobIdFromSpec(a.size() > 1 ? a[1] : "", env);
    Environment::Job* j = env.findJob(id);
    if (!j) { std::fprintf(stderr, "fg: no such job\n"); return 1; }
    std::cout << j->command << "\n";
    std::cout.flush();
    // Continue in case it was stopped, then wait for completion.
    ::kill(static_cast<pid_t>(j->pid), SIGCONT);
    int st = 0;
    ::waitpid(static_cast<pid_t>(j->pid), &st, 0);
    env.removeJob(id);
    return WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
}

// bg [id] -- resume a stopped job in the background.
int biBg(const std::vector<std::string>& a, Environment& env) {
    int id = jobIdFromSpec(a.size() > 1 ? a[1] : "", env);
    Environment::Job* j = env.findJob(id);
    if (!j) { std::fprintf(stderr, "bg: no such job\n"); return 1; }
    ::kill(static_cast<pid_t>(j->pid), SIGCONT);
    j->running = true;
    std::cout << "[" << j->id << "] " << j->command << " &\n";
    std::cout.flush();
    return 0;
}

// ---- test / [ (M4) ----
// Evaluate a single test expression given the operands (without the leading
// "test" / "[" and without a trailing "]"). Supports unary file/string ops,
// binary string comparisons, binary numeric comparisons, and `!` negation.
bool evalTest(const std::vector<std::string>& t);

bool evalTestPrimary(const std::vector<std::string>& t) {
    if (t.empty()) return false;                       // `test` -> false
    if (t.size() == 1) return !t[0].empty();           // `test STR` -> nonempty
    if (t.size() == 2) {
        const std::string& op = t[0];
        const std::string& x = t[1];
        if (op == "-z") return x.empty();
        if (op == "-n") return !x.empty();
        if (op == "-e") { struct stat st; return ::stat(x.c_str(), &st) == 0; }
        if (op == "-f") { struct stat st; return ::stat(x.c_str(), &st) == 0 && S_ISREG(st.st_mode); }
        if (op == "-d") { struct stat st; return ::stat(x.c_str(), &st) == 0 && S_ISDIR(st.st_mode); }
        return false;
    }
    if (t.size() == 3) {
        const std::string& l = t[0];
        const std::string& op = t[1];
        const std::string& r = t[2];
        if (op == "=" || op == "==") return l == r;
        if (op == "!=") return l != r;
        long a = Value(l).asInt(), b = Value(r).asInt();
        if (op == "-eq") return a == b;
        if (op == "-ne") return a != b;
        if (op == "-lt") return a < b;
        if (op == "-le") return a <= b;
        if (op == "-gt") return a > b;
        if (op == "-ge") return a >= b;
        return false;
    }
    return false;
}

bool evalTest(const std::vector<std::string>& t) {
    if (!t.empty() && t[0] == "!") {
        return !evalTest(std::vector<std::string>(t.begin() + 1, t.end()));
    }
    return evalTestPrimary(t);
}

int biTest(const std::vector<std::string>& a, Environment&) {
    // a[0] is "test" or "[". For "[", require a closing "]" and drop it.
    std::vector<std::string> operands(a.begin() + 1, a.end());
    if (!a.empty() && a[0] == "[") {
        if (operands.empty() || operands.back() != "]") {
            std::fprintf(stderr, "[: missing ']'\n");
            return 2;
        }
        operands.pop_back();
    }
    return evalTest(operands) ? 0 : 1;
}

// ---- read (M4) ----
// read [-r] NAME... -- read one line from stdin, split on whitespace into the
// named variables (the last variable gets the remainder). With no names, set
// REPLY. Returns 1 at end-of-file.
int biRead(const std::vector<std::string>& a, Environment& env) {
    std::size_t start = 1;
    // (-r is accepted for compatibility; this reader does not process
    // backslash escapes, so -r is effectively always on.)
    if (a.size() > 1 && a[1] == "-r") start = 2;

    std::string line;
    int c;
    bool got = false;
    while ((c = std::fgetc(stdin)) != EOF) {
        got = true;
        if (c == '\n') break;
        line.push_back(static_cast<char>(c));
    }
    if (!got && line.empty()) return 1;  // EOF, nothing read

    std::vector<std::string> names(a.begin() + start, a.end());
    if (names.empty()) { env.set("REPLY", line); return 0; }

    // Split on whitespace; assign one field per name, remainder to the last.
    std::vector<std::string> fields;
    std::size_t i = 0;
    for (std::size_t n = 0; n + 1 < names.size(); ++n) {
        while (i < line.size() && std::isspace((unsigned char)line[i])) ++i;
        std::size_t j = i;
        while (j < line.size() && !std::isspace((unsigned char)line[j])) ++j;
        fields.push_back(line.substr(i, j - i));
        i = j;
    }
    // Remainder (trimmed of leading whitespace) to the final name.
    while (i < line.size() && std::isspace((unsigned char)line[i])) ++i;
    fields.push_back(line.substr(i));
    for (std::size_t n = 0; n < names.size(); ++n)
        env.set(names[n], n < fields.size() ? fields[n] : std::string());
    return 0;
}

// ---- getopts (M4) ----
// getopts OPTSTRING NAME [ARG...] -- parse the next option from the positional
// parameters (or the supplied ARGs) using OPTIND to track position. Sets NAME
// to the option letter (or "?"), OPTARG for options that take a value, and
// returns 1 when the options are exhausted.
int biGetopts(const std::vector<std::string>& a, Environment& env) {
    if (a.size() < 3) { std::fprintf(stderr, "getopts: usage: getopts optstring name [arg...]\n"); return 2; }
    const std::string optstring = a[1];
    const std::string name = a[2];

    // The words to scan: explicit ARGs after NAME, else the positionals.
    std::vector<std::string> args;
    if (a.size() > 3) args.assign(a.begin() + 3, a.end());
    else args = env.positionals();

    long optind = env.has("OPTIND") ? Value(env.get("OPTIND")).asInt() : 1;
    if (optind < 1) optind = 1;

    // OPTIND is 1-based over the arg list.
    std::size_t idx = static_cast<std::size_t>(optind - 1);
    if (idx >= args.size()) { env.set(name, "?"); return 1; }

    const std::string& cur = args[idx];
    if (cur.size() < 2 || cur[0] != '-' || cur == "--") {
        // Not an option (or explicit end): stop.
        if (cur == "--") env.set("OPTIND", std::to_string(optind + 1));
        env.set(name, "?");
        return 1;
    }

    // Track sub-index within a bundled option group via OPTSUB (SLeeLa ext).
    long sub = env.has("OPTSUB") ? Value(env.get("OPTSUB")).asInt() : 1;
    if (sub < 1) sub = 1;
    if (static_cast<std::size_t>(sub) >= cur.size()) {
        // Exhausted this group; advance and retry from the next word.
        env.set("OPTIND", std::to_string(optind + 1));
        env.set("OPTSUB", "1");
        return biGetopts(a, env);
    }

    char opt = cur[static_cast<std::size_t>(sub)];
    std::size_t pos = optstring.find(opt);
    if (pos == std::string::npos) {
        env.set(name, "?");
        env.set("OPTARG", std::string(1, opt));
        // advance sub-index
        env.set("OPTSUB", std::to_string(sub + 1));
        if (static_cast<std::size_t>(sub + 1) >= cur.size()) {
            env.set("OPTIND", std::to_string(optind + 1));
            env.set("OPTSUB", "1");
        }
        return 0;
    }

    bool takesArg = (pos + 1 < optstring.size() && optstring[pos + 1] == ':');
    if (takesArg) {
        std::string arg;
        if (static_cast<std::size_t>(sub) + 1 < cur.size()) {
            arg = cur.substr(static_cast<std::size_t>(sub) + 1);  // -oVALUE
            env.set("OPTIND", std::to_string(optind + 1));
        } else if (idx + 1 < args.size()) {
            arg = args[idx + 1];                                  // -o VALUE
            env.set("OPTIND", std::to_string(optind + 2));
        } else {
            env.set(name, "?");
            env.set("OPTARG", std::string(1, opt));
            env.set("OPTIND", std::to_string(optind + 1));
            return 0;
        }
        env.set("OPTSUB", "1");
        env.set(name, std::string(1, opt));
        env.set("OPTARG", arg);
        return 0;
    }

    // A flag with no argument: consume just this letter.
    env.set(name, std::string(1, opt));
    env.set("OPTARG", "");
    env.set("OPTSUB", std::to_string(sub + 1));
    if (static_cast<std::size_t>(sub + 1) >= cur.size()) {
        env.set("OPTIND", std::to_string(optind + 1));
        env.set("OPTSUB", "1");
    }
    return 0;
}

const std::map<std::string, Builtin>& builtins() {
    static const std::map<std::string, Builtin> table = {
        {"echo", biEcho}, {"pwd", biPwd}, {"cd", biCd},
        {"export", biExport}, {"unset", biUnset},
        {"true", biTrue}, {"false", biFalse}, {":", biColon},
        {"exit", biExit}, {"set", biSet},
        {"break", biBreak}, {"continue", biContinue},
        {"jobs", biJobs}, {"wait", biWait}, {"fg", biFg}, {"bg", biBg},
        {"test", biTest}, {"[", biTest},
        {"read", biRead}, {"getopts", biGetopts},
    };
    return table;
}

bool isBuiltin(const std::string& name) {
    return builtins().find(name) != builtins().end();
}

// Resolve a Simple node into argv + expanded redirections. Each word may
// expand into several argv fields (field splitting + globbing).
Resolved resolveSimple(const Node& node, Environment& env) {
    Resolved r;
    CommandRunner run = makeRunner(env);
    for (const auto& w : node.words) {
        auto fields = expandWord(w, env, run);
        for (auto& f : fields) r.argv.push_back(std::move(f));
    }
    for (const auto& rd : node.redirs) {
        Redirection e = rd;
        if (rd.op == RedirOp::Heredoc) {
            // Expand $ in the body unless the delimiter was quoted; the
            // delimiter/target itself is not a filename here.
            e.body = rd.expand_body ? expandWordSingle(rd.body, env, run) : rd.body;
        } else {
            e.target = expandWordSingle(rd.target, env, run);
        }
        r.redirs.push_back(std::move(e));
    }
    return r;
}

// Run an external program with the given argv/redirs in a child process.
// Returns the child's exit status.
int runExternal(const std::vector<std::string>& argv,
                const std::vector<Redirection>& redirs,
                Environment& env) {
    if (argv.empty()) return 0;

    std::vector<char*> cargv;
    cargv.reserve(argv.size() + 1);
    for (const auto& s : argv) cargv.push_back(const_cast<char*>(s.c_str()));
    cargv.push_back(nullptr);

    std::vector<std::string> envv = env.exportedEnviron();
    std::vector<char*> cenv;
    cenv.reserve(envv.size() + 1);
    for (auto& s : envv) cenv.push_back(const_cast<char*>(s.c_str()));
    cenv.push_back(nullptr);

    pid_t pid = ::fork();
    if (pid < 0) { std::perror("fork"); return 127; }
    if (pid == 0) {
        if (applyRedirs(redirs) != 0) _exit(1);
        ::execvpe(cargv[0], cargv.data(), cenv.data());
        std::fprintf(stderr, "slsh: %s: %s\n", cargv[0], std::strerror(errno));
        _exit(127);
    }
    int status = 0;
    ::waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
}

// Forward declarations for recursion.
int execNode(const Node& node, Environment& env);
int callFunction(const std::string& name, const std::vector<std::string>& argv,
                 Environment& env);

int execSimple(const Node& node, Environment& env) {
    // Assignments with no command word: set variables and return 0.
    Resolved r = resolveSimple(node, env);

    CommandRunner run = makeRunner(env);
    if (r.argv.empty()) {
        for (const auto& as : node.assigns)
            env.set(as.name, expandWordSingle(as.value, env, run));
        return 0;
    }

    const std::string& cmd = r.argv[0];

    // Shell functions execute in the current shell environment in this
    // milestone. External commands receive a scoped environment so temporary
    // assignments do not unexpectedly mutate the parent shell.
    if (env.hasFunction(cmd)) {
        for (const auto& as : node.assigns)
            env.set(as.name, expandWordSingle(as.value, env, run));
        return callFunction(cmd, r.argv, env);
    }

    if (isBuiltin(cmd)) {
        // Run a builtin with redirections applied to dup'd fds, then restored.
        int saved_out = -1, saved_in = -1;
        if (!r.redirs.empty()) {
            saved_out = ::dup(1);
            saved_in = ::dup(0);
            if (applyRedirs(r.redirs) != 0) {
                if (saved_out >= 0) { ::dup2(saved_out, 1); ::close(saved_out); }
                if (saved_in >= 0) { ::dup2(saved_in, 0); ::close(saved_in); }
                return 1;
            }
        }
        int rc = builtins().at(cmd)(r.argv, env);
        if (!r.redirs.empty()) {
            std::cout.flush();
            if (saved_out >= 0) { ::dup2(saved_out, 1); ::close(saved_out); }
            if (saved_in >= 0) { ::dup2(saved_in, 0); ::close(saved_in); }
        }
        return rc;
    }

    Environment childEnv = env.scopedCopy();
    for (const auto& as : node.assigns)
        childEnv.set(as.name, expandWordSingle(as.value, env, run));
    return runExternal(r.argv, r.redirs, childEnv);
}

int execPipeline(const Node& node, Environment& env) {
    const auto& cmds = node.children;
    if (cmds.size() == 1) {
        int rc = execNode(*cmds[0], env);
        return node.negated ? (rc == 0 ? 1 : 0) : rc;
    }

    // Build pipes between the N children.
    const std::size_t n = cmds.size();
    std::vector<int> pipes((n - 1) * 2, -1);
    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (::pipe(&pipes[i * 2]) != 0) { std::perror("pipe"); return 1; }
    }

    std::vector<pid_t> pids(n, -1);
    for (std::size_t i = 0; i < n; ++i) {
        // Only simple commands run as external children in a pipeline for this
        // milestone; builtins in a pipeline run in the child too (their output
        // goes down the pipe).
        Resolved r;
        bool builtin = false;
        bool function = false;
        if (cmds[i]->kind == NodeKind::Simple) {
            r = resolveSimple(*cmds[i], env);
            function = !r.argv.empty() && env.hasFunction(r.argv[0]);
            builtin = !function && !r.argv.empty() && isBuiltin(r.argv[0]);
        }

        pid_t pid = ::fork();
        if (pid < 0) { std::perror("fork"); return 1; }
        if (pid == 0) {
            // wire stdin from previous pipe, stdout to next pipe
            if (i > 0) ::dup2(pipes[(i - 1) * 2], 0);
            if (i + 1 < n) ::dup2(pipes[i * 2 + 1], 1);
            for (int fd : pipes) if (fd >= 0) ::close(fd);

            if (cmds[i]->kind == NodeKind::Simple) {
                if (applyRedirs(r.redirs) != 0) _exit(1);
                if (function) {
                    Environment child = env;  // isolated copy for the stage
                    _exit(callFunction(r.argv[0], r.argv, child) & 0xFF);
                }
                if (builtin) {
                    Environment child = env;  // isolated copy
                    _exit(builtins().at(r.argv[0])(r.argv, child) & 0xFF);
                }
                // external
                std::vector<char*> cargv;
                for (auto& s : r.argv) cargv.push_back(const_cast<char*>(s.c_str()));
                cargv.push_back(nullptr);
                std::vector<std::string> envv = env.exportedEnviron();
                std::vector<char*> cenv;
                for (auto& s : envv) cenv.push_back(const_cast<char*>(s.c_str()));
                cenv.push_back(nullptr);
                ::execvpe(cargv[0], cargv.data(), cenv.data());
                std::fprintf(stderr, "slsh: %s: %s\n", cargv[0], std::strerror(errno));
                _exit(127);
            } else {
                Environment child = env;
                _exit(execNode(*cmds[i], child) & 0xFF);
            }
        }
        pids[i] = pid;
    }

    for (int fd : pipes) if (fd >= 0) ::close(fd);

    int last = 0;
    for (std::size_t i = 0; i < n; ++i) {
        int status = 0;
        ::waitpid(pids[i], &status, 0);
        int rc = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
        if (i == n - 1) last = rc;
    }
    return node.negated ? (last == 0 ? 1 : 0) : last;
}

int execAndOr(const Node& node, Environment& env) {
    int status = execNode(*node.children[0], env);
    for (std::size_t i = 0; i < node.andor_ops.size(); ++i) {
        const bool prevTrue = (status == 0);
        const AndOrOp op = node.andor_ops[i];
        if (op == AndOrOp::And && !prevTrue) continue;  // skip RHS
        if (op == AndOrOp::Or && prevTrue) continue;    // skip RHS
        status = execNode(*node.children[i + 1], env);
    }
    return status;
}

int execList(const Node& node, Environment& env) {
    int status = 0;
    for (std::size_t i = 0; i < node.children.size(); ++i) {
        const bool async = i < node.child_async.size() && node.child_async[i];
        if (async) {
            // Run this and-or in the background: fork a child that executes it,
            // register the job, and report "[id] pid". Do not wait.
            const Node& child = *node.children[i];
            pid_t pid = ::fork();
            if (pid < 0) { std::perror("fork"); status = 1; continue; }
            if (pid == 0) {
                Environment bg = env;      // isolated copy for the job
                int rc = execNode(child, bg);
                std::cout.flush();
                _exit(rc & 0xFF);
            }
            int id = env.addJob(static_cast<long>(pid), "background job");
            std::cout << "[" << id << "] " << pid << "\n";
            std::cout.flush();
            status = 0;  // async command returns success immediately
        } else {
            status = execNode(*node.children[i], env);
            if (env.shouldExit()) break;
            if (env.loopSignal()) break;  // break/continue: stop this list
        }
    }
    return status;
}

int execIf(const Node& node, Environment& env) {
    for (std::size_t i = 0; i < node.if_conds.size(); ++i) {
        int cond = execNode(*node.if_conds[i], env);
        if (cond == 0) return execNode(*node.if_bodies[i], env);
    }
    if (node.else_body) return execNode(*node.else_body, env);
    return 0;
}

int execWhile(const Node& node, Environment& env) {
    int status = 0;
    const int kGuard = 1000000;  // guard against runaway loops
    for (int iter = 0; iter < kGuard; ++iter) {
        if (execNode(*node.while_cond, env) != 0) break;
        status = execNode(*node.while_body, env);
        if (env.shouldExit()) break;
        if (env.loopSignal()) {
            char sig = env.consumeLoopSignal();
            if (sig == 'B') break;      // break: leave this loop
            /* sig == 'C' -> continue: fall through to next iteration */
        }
    }
    return status;
}

// until: run the body WHILE the condition is non-zero (i.e. false), i.e. loop
// until the condition succeeds -- the inverse of while.
int execUntil(const Node& node, Environment& env) {
    int status = 0;
    const int kGuard = 1000000;
    for (int iter = 0; iter < kGuard; ++iter) {
        if (execNode(*node.while_cond, env) == 0) break;  // stop when cond true
        status = execNode(*node.while_body, env);
        if (env.shouldExit()) break;
        if (env.loopSignal()) {
            char sig = env.consumeLoopSignal();
            if (sig == 'B') break;
        }
    }
    return status;
}

// for NAME in words...; do body; done
int execFor(const Node& node, Environment& env) {
    CommandRunner run = makeRunner(env);
    // Expand the iteration list into fields (splitting + globbing).
    std::vector<std::string> items;
    for (const auto& w : node.for_words) {
        auto fields = expandWord(w, env, run);
        for (auto& f : fields) items.push_back(std::move(f));
    }
    int status = 0;
    for (const auto& item : items) {
        env.set(node.for_var, item);
        status = execNode(*node.for_body, env);
        if (env.shouldExit()) break;
        if (env.loopSignal()) {
            char sig = env.consumeLoopSignal();
            if (sig == 'B') break;
        }
    }
    return status;
}

// case SUBJECT in pattern) body ;; ... esac  -- first matching arm runs.
int execCase(const Node& node, Environment& env) {
    CommandRunner run = makeRunner(env);
    const std::string subject = expandWordSingle(node.case_subject, env, run);
    for (const auto& item : node.case_items) {
        for (const auto& pat : item.patterns) {
            const std::string p = expandWordSingle(pat, env, run);
            if (p == "*" || globMatch(p, subject)) {
                return item.body ? execNode(*item.body, env) : 0;
            }
        }
    }
    return 0;  // no arm matched
}

// name() { body; } -- register the function; definition has status 0.
int execFunctionDef(const Node& node, Environment& env) {
    // Share the body so it survives the AST if needed during a call.
    auto shared = std::make_shared<Node>(NodeKind::List);
    // Move the parsed body's children into the shared node.
    if (node.func_body) {
        shared->children = std::move(const_cast<Node&>(node).func_body->children);
    }
    env.defineFunction(node.func_name, shared);
    return 0;
}

int execNode(const Node& node, Environment& env) {
    int status = 0;
    switch (node.kind) {
        case NodeKind::Simple:      status = execSimple(node, env); break;
        case NodeKind::Pipeline:    status = execPipeline(node, env); break;
        case NodeKind::AndOr:       status = execAndOr(node, env); break;
        case NodeKind::List:        status = execList(node, env); break;
        case NodeKind::If:          status = execIf(node, env); break;
        case NodeKind::While:       status = execWhile(node, env); break;
        case NodeKind::Until:       status = execUntil(node, env); break;
        case NodeKind::For:         status = execFor(node, env); break;
        case NodeKind::Case:        status = execCase(node, env); break;
        case NodeKind::FunctionDef: status = execFunctionDef(node, env); break;
    }
    env.setLastStatus(status);
    return status;
}

// Call a shell function: run its body with $1.. set from argv[1..], restoring
// the previous positional parameters afterward.
int callFunction(const std::string& name, const std::vector<std::string>& argv,
                 Environment& env) {
    auto body = env.lookupFunction(name);
    if (!body) return 127;

    std::vector<std::string> saved = env.positionals();
    std::vector<std::string> params(argv.begin() + (argv.empty() ? 0 : 1), argv.end());
    env.setPositionals(std::move(params));

    int status = execNode(*body, env);

    env.setPositionals(std::move(saved));
    return status;
}

} // namespace

int execute(const Node& node, Environment& env) {
    return execNode(node, env);
}

// Run `src` (shell text) in a child process with stdout captured; return the
// captured output. Defined outside the anonymous namespace so the runner
// callback can reach it. Declared at the top of this file.
std::string captureCommand(const std::string& src, Environment& env) {
    int fds[2];
    if (::pipe(fds) != 0) return {};

    pid_t pid = ::fork();
    if (pid < 0) { ::close(fds[0]); ::close(fds[1]); return {}; }
    if (pid == 0) {
        ::dup2(fds[1], 1);      // child stdout -> pipe write end
        ::close(fds[0]);
        ::close(fds[1]);
        // Parse and run the substituted command in a copy of the environment.
        std::vector<Token> toks; LexError le;
        if (lex(src, toks, le)) {
            ParseError pe;
            NodePtr ast = parse(toks, pe);
            if (ast) {
                Environment child = env;
                execute(*ast, child);
            }
        }
        std::cout.flush();
        _exit(0);
    }
    ::close(fds[1]);
    std::string out;
    char buf[4096];
    ssize_t r;
    while ((r = ::read(fds[0], buf, sizeof buf)) > 0) out.append(buf, static_cast<std::size_t>(r));
    ::close(fds[0]);
    int st = 0;
    ::waitpid(pid, &st, 0);
    return out;
}

} // namespace sleela::sh
