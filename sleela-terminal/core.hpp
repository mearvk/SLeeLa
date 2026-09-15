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

// ---------------------------------------------------------------------------
// Value -- a shell value: a string with an integer view for arithmetic.
// ---------------------------------------------------------------------------
struct Value {
    std::string text;

    Value() = default;
    Value(std::string s) : text(std::move(s)) {}

    // Integer view: leading sign + digits; empty/non-numeric reads as 0.
    long asInt() const noexcept;
    static Value fromInt(long v);
};

// ---------------------------------------------------------------------------
// Token -- the lexer's output unit.
// ---------------------------------------------------------------------------
enum class Tok {
    Word,        // a (possibly quoted) word
    Assignment,  // name=value at command-word position
    Pipe,        // |
    Semi,        // ;
    Newline,     // \n
    AndIf,       // &&
    OrIf,        // ||
    Less,        // <
    Great,       // >
    DGreat,      // >>
    // keywords
    If, Then, Elif, Else, Fi,
    While, Do, Done,
    Eof
};

struct Token {
    Tok kind = Tok::Eof;
    std::string text;   // literal text (word content, assignment "n=v", etc.)
    int line = 0;
    int col = 0;
};

// ---------------------------------------------------------------------------
// AST -- a small, closed set of node kinds.
// ---------------------------------------------------------------------------

// A redirection attached to a simple command.
enum class RedirOp { In, Out, Append };  // <  >  >>
struct Redirection {
    RedirOp op = RedirOp::Out;
    int fd = -1;         // explicit fd, or -1 to use the default for the op
    std::string target; // filename (a word, expanded at run time)
};

enum class NodeKind { Simple, Pipeline, AndOr, List, If, While };

struct Node;
using NodePtr = std::unique_ptr<Node>;

// One assignment "name=word" carried by a simple command.
struct Assignment {
    std::string name;
    std::string value;  // unexpanded word text
};

// AndOr link operator.
enum class AndOrOp { And, Or };  // && ||

struct Node {
    NodeKind kind;

    // Simple:
    std::vector<Assignment> assigns;
    std::vector<std::string> words;     // unexpanded word texts (argv template)
    std::vector<Redirection> redirs;

    // Pipeline: children joined by '|'.
    // List: children separated by ';'/newline.
    std::vector<NodePtr> children;

    // AndOr: left/right with an operator between (chained left-assoc as a
    // sequence of {op, node}).
    std::vector<AndOrOp> andor_ops;     // size == children.size()-1 for AndOr

    // If: cond/body pairs (if + elif...), plus optional else body.
    std::vector<NodePtr> if_conds;      // condition Lists
    std::vector<NodePtr> if_bodies;     // corresponding then-bodies
    NodePtr else_body;                  // may be null

    // While: cond + body.
    NodePtr while_cond;
    NodePtr while_body;

    explicit Node(NodeKind k) : kind(k) {}
};

// ---------------------------------------------------------------------------
// Environment -- variables, exit status, working directory.
// ---------------------------------------------------------------------------
class Environment {
public:
    Environment();

    // Variable access. get() returns "" for an unset name.
    std::string get(const std::string& name) const;
    bool has(const std::string& name) const;
    void set(const std::string& name, const std::string& value);
    void unset(const std::string& name);
    void exportVar(const std::string& name);   // mark for child environments
    bool isExported(const std::string& name) const;

    // The exported subset, as "name=value" strings (for execve/posix_spawn).
    std::vector<std::string> exportedEnviron() const;

    // Exit status of the last command ($?).
    int lastStatus() const noexcept { return last_status_; }
    void setLastStatus(int s) noexcept { last_status_ = s; }

    // Whether an interactive/`exit` request has been made, and its code.
    bool shouldExit() const noexcept { return should_exit_; }
    int exitCode() const noexcept { return exit_code_; }
    void requestExit(int code) noexcept { should_exit_ = true; exit_code_ = code; }

private:
    struct Var { std::string value; bool exported = false; };
    std::map<std::string, Var> vars_;
    int last_status_ = 0;
    bool should_exit_ = false;
    int exit_code_ = 0;
};

} // namespace sleela::sh
