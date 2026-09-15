#pragma once

// SleelaTerminal(TM) shell -- L1 core model.
//
// Original SLeeLa work (see NOTICE): a clean-room shell, not derived from Bash.
// This header defines the shared value/token/AST types and the runtime
// Environment. Everything above depends on these; this file depends on nothing
// in the project.

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sleela::sh {

struct Value {
    std::string text;
    Value() = default;
    Value(std::string s) : text(std::move(s)) {}
    long asInt() const noexcept;
    static Value fromInt(long v);
};

enum class Tok {
    Word, Assignment, Pipe, Semi, Newline, AndIf, OrIf, Less, Great, DGreat,
    DLess, DLessDash, LParen, RParen, LBrace, RBrace,
    If, Then, Elif, Else, Fi, While, Until, Do, Done, For, In, Case, Esac,
    Amp, Bang, HeredocBody, Eof
};

struct Token {
    Tok kind = Tok::Eof;
    std::string text;
    int line = 0;
    int col = 0;
};

enum class RedirOp { In, Out, Append, Heredoc };
struct Redirection {
    RedirOp op = RedirOp::Out;
    int fd = -1;
    std::string target;
    std::string body;
    bool expand_body = true;
};

enum class NodeKind { Simple, Pipeline, AndOr, List, If, While, Until,
                      For, Case, FunctionDef };

struct Node;
using NodePtr = std::unique_ptr<Node>;

struct CaseItem {
    std::vector<std::string> patterns;
    NodePtr body;
};

struct Assignment {
    std::string name;
    std::string value;
};

enum class AndOrOp { And, Or };

struct Node {
    NodeKind kind;
    std::vector<Assignment> assigns;
    std::vector<std::string> words;
    std::vector<Redirection> redirs;
    std::vector<NodePtr> children;
    std::vector<bool> child_async;
    bool negated = false;
    std::vector<AndOrOp> andor_ops;
    std::vector<NodePtr> if_conds;
    std::vector<NodePtr> if_bodies;
    NodePtr else_body;
    NodePtr while_cond;
    NodePtr while_body;
    std::string for_var;
    std::vector<std::string> for_words;
    NodePtr for_body;
    std::string case_subject;
    std::vector<CaseItem> case_items;
    std::string func_name;
    NodePtr func_body;
    explicit Node(NodeKind k) : kind(k) {}
};

class Environment {
public:
    Environment();

    std::string get(const std::string& name) const;
    bool has(const std::string& name) const;
    void set(const std::string& name, const std::string& value);
    void unset(const std::string& name);
    void exportVar(const std::string& name);
    bool isExported(const std::string& name) const;
    std::vector<std::string> exportedEnviron() const;
    Environment scopedCopy() const;

    void defineFunction(const std::string& name, std::shared_ptr<Node> body);
    std::shared_ptr<Node> lookupFunction(const std::string& name) const;
    bool hasFunction(const std::string& name) const;

    void setPositionals(std::vector<std::string> args);
    std::vector<std::string> positionals() const { return positionals_; }
    std::string getPositional(std::size_t n) const;
    std::size_t positionalCount() const { return positionals_.size(); }

    int lastStatus() const noexcept { return last_status_; }
    void setLastStatus(int s) noexcept { last_status_ = s; }

    bool shouldExit() const noexcept { return should_exit_; }
    int exitCode() const noexcept { return exit_code_; }
    void requestExit(int code) noexcept { should_exit_ = true; exit_code_ = code; }

    const std::string& prompt() const noexcept { return prompt_; }
    void setPrompt(std::string prompt) { prompt_ = std::move(prompt); }

    void requestBreak(int levels) noexcept { loop_break_ = levels > 0 ? levels : 1; }
    void requestContinue(int levels) noexcept { loop_continue_ = levels > 0 ? levels : 1; }
    int breakLevels() const noexcept { return loop_break_; }
    int continueLevels() const noexcept { return loop_continue_; }
    bool loopSignal() const noexcept { return loop_break_ > 0 || loop_continue_ > 0; }
    char consumeLoopSignal() noexcept {
        if (loop_break_ > 0) {
            if (--loop_break_ > 0) return 'B';
            return 'B';
        }
        if (loop_continue_ > 0) {
            if (--loop_continue_ > 0) return 'B';
            return 'C';
        }
        return 0;
    }

    struct Job {
        int id = 0;
        long pid = 0;
        std::string command;
        bool running = true;
    };
    int addJob(long pid, const std::string& command);
    std::vector<Job>& jobs() noexcept { return jobs_; }
    const std::vector<Job>& jobs() const noexcept { return jobs_; }
    Job* findJob(int id) noexcept;
    void removeJob(int id) noexcept;

private:
    struct Var { std::string value; bool exported = false; };
    std::map<std::string, Var> vars_;
    std::map<std::string, std::shared_ptr<Node>> functions_;
    std::vector<std::string> positionals_;
    std::vector<Job> jobs_;
    std::string prompt_ = "slsh$ ";
    int next_job_id_ = 1;
    int last_status_ = 0;
    bool should_exit_ = false;
    int exit_code_ = 0;
    int loop_break_ = 0;
    int loop_continue_ = 0;
};

} // namespace sleela::sh
