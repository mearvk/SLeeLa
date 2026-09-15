// SleelaTerminal(TM) shell -- L5 executor implementation.
// Original SLeeLa work (see NOTICE).

#include "executor.hpp"
#include "expand.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <map>
#include <string>
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

const std::map<std::string, Builtin>& builtins() {
    static const std::map<std::string, Builtin> table = {
        {"echo", biEcho}, {"pwd", biPwd}, {"cd", biCd},
        {"export", biExport}, {"unset", biUnset},
        {"true", biTrue}, {"false", biFalse}, {":", biColon},
        {"exit", biExit}, {"set", biSet},
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

    // Assignments preceding a command apply to the environment for this
    // command. (This milestone applies them to the shell env, which is a
    // simplification; a later milestone can scope them to the child only.)
    for (const auto& as : node.assigns)
        env.set(as.name, expandWordSingle(as.value, env, run));

    const std::string& cmd = r.argv[0];

    // A user-defined shell function takes precedence over an external command.
    if (env.hasFunction(cmd)) {
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

    return runExternal(r.argv, r.redirs, env);
}

int execPipeline(const Node& node, Environment& env) {
    const auto& cmds = node.children;
    if (cmds.size() == 1) return execNode(*cmds[0], env);

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
    return last;
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
    for (const auto& child : node.children) {
        status = execNode(*child, env);
        if (env.shouldExit()) break;
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
