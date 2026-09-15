// SleelaTerminal(TM) shell -- L5 executor implementation.
// Original SLeeLa work (see NOTICE).

#include "executor.hpp"
#include "expand.hpp"

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

namespace {

// A resolved simple command: argv + redirections, ready to run.
struct Resolved {
    std::vector<std::string> argv;
    std::vector<Redirection> redirs;  // targets already expanded
};

// Apply the resolved redirections in the current process (used post-fork or in
// a builtin's scoped context). Returns 0 on success, -1 on error.
int applyRedirs(const std::vector<Redirection>& redirs) {
    for (const auto& r : redirs) {
        int fd = -1;
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

// Resolve a Simple node into argv + expanded redirections.
Resolved resolveSimple(const Node& node, Environment& env) {
    Resolved r;
    for (const auto& w : node.words) r.argv.push_back(expandWord(w, env));
    for (const auto& rd : node.redirs) {
        Redirection e = rd;
        e.target = expandWord(rd.target, env);
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

// Forward declaration for recursion.
int execNode(const Node& node, Environment& env);

int execSimple(const Node& node, Environment& env) {
    // Assignments with no command word: set variables and return 0.
    Resolved r = resolveSimple(node, env);

    if (r.argv.empty()) {
        for (const auto& as : node.assigns) env.set(as.name, expandWord(as.value, env));
        return 0;
    }

    // Assignments preceding a command apply to the environment for this
    // command. (This milestone applies them to the shell env, which is a
    // simplification; a later milestone can scope them to the child only.)
    for (const auto& as : node.assigns) env.set(as.name, expandWord(as.value, env));

    const std::string& cmd = r.argv[0];
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
        if (cmds[i]->kind == NodeKind::Simple) {
            r = resolveSimple(*cmds[i], env);
            builtin = !r.argv.empty() && isBuiltin(r.argv[0]);
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

int execNode(const Node& node, Environment& env) {
    int status = 0;
    switch (node.kind) {
        case NodeKind::Simple:   status = execSimple(node, env); break;
        case NodeKind::Pipeline: status = execPipeline(node, env); break;
        case NodeKind::AndOr:    status = execAndOr(node, env); break;
        case NodeKind::List:     status = execList(node, env); break;
        case NodeKind::If:       status = execIf(node, env); break;
        case NodeKind::While:    status = execWhile(node, env); break;
    }
    env.setLastStatus(status);
    return status;
}

} // namespace

int execute(const Node& node, Environment& env) {
    return execNode(node, env);
}

} // namespace sleela::sh
