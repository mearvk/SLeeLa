// SleelaTerminal(TM) shell -- L3 parser implementation.
// Original SLeeLa work (see NOTICE).

#include "parser.hpp"

#include <stdexcept>

namespace sleela::sh {

namespace {

// Thrown internally to unwind to parse(); converted to a ParseError there.
struct PErr {
    std::string message;
    int line;
    int col;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& toks) : t_(toks) {}

    NodePtr parseProgram() {
        skipSeparators();
        auto list = parseList();
        if (peek().kind != Tok::Eof) {
            throw PErr{"unexpected token '" + peek().text + "'", peek().line, peek().col};
        }
        return list;
    }

private:
    const Token& peek() const { return t_[pos_]; }
    const Token& peek2() const { return pos_ + 1 < t_.size() ? t_[pos_ + 1] : t_.back(); }
    const Token& advance() { return t_[pos_ < t_.size() - 1 ? pos_++ : pos_]; }
    bool at(Tok k) const { return peek().kind == k; }

    void expect(Tok k, const char* what) {
        if (peek().kind != k) {
            throw PErr{std::string("expected ") + what + " but found '" + peek().text + "'",
                       peek().line, peek().col};
        }
        advance();
    }

    void skipSeparators() {
        while (at(Tok::Semi) || at(Tok::Newline)) advance();
    }

    bool isListTerminator() const {
        switch (peek().kind) {
            case Tok::Eof: case Tok::Then: case Tok::Elif: case Tok::Else:
            case Tok::Fi: case Tok::Do: case Tok::Done:
                return true;
            default:
                return false;
        }
    }

    // list := andor ( (';' | '\n') andor )*
    NodePtr parseList() {
        auto list = std::make_unique<Node>(NodeKind::List);
        list->children.push_back(parseAndOr());
        for (;;) {
            // consume one-or-more separators
            bool sawSep = false;
            while (at(Tok::Semi) || at(Tok::Newline)) { advance(); sawSep = true; }
            if (!sawSep) break;
            if (isListTerminator()) break;
            list->children.push_back(parseAndOr());
        }
        return list;
    }

    // andor := pipeline ( ('&&' | '||') pipeline )*
    NodePtr parseAndOr() {
        auto first = parsePipeline();
        if (!at(Tok::AndIf) && !at(Tok::OrIf)) return first;

        auto node = std::make_unique<Node>(NodeKind::AndOr);
        node->children.push_back(std::move(first));
        while (at(Tok::AndIf) || at(Tok::OrIf)) {
            node->andor_ops.push_back(at(Tok::AndIf) ? AndOrOp::And : AndOrOp::Or);
            advance();
            // allow a newline after && / ||
            while (at(Tok::Newline)) advance();
            node->children.push_back(parsePipeline());
        }
        return node;
    }

    // pipeline := command ( '|' command )*
    NodePtr parsePipeline() {
        auto first = parseCommand();
        if (!at(Tok::Pipe)) return first;

        auto node = std::make_unique<Node>(NodeKind::Pipeline);
        node->children.push_back(std::move(first));
        while (at(Tok::Pipe)) {
            advance();
            while (at(Tok::Newline)) advance();
            node->children.push_back(parseCommand());
        }
        return node;
    }

    // command := if_clause | while_clause | simple_command
    NodePtr parseCommand() {
        if (at(Tok::If)) return parseIf();
        if (at(Tok::While)) return parseWhile();
        return parseSimple();
    }

    // if := 'if' list 'then' list ('elif' list 'then' list)* ['else' list] 'fi'
    NodePtr parseIf() {
        auto node = std::make_unique<Node>(NodeKind::If);
        expect(Tok::If, "'if'");
        node->if_conds.push_back(parseList());
        expect(Tok::Then, "'then'");
        node->if_bodies.push_back(parseList());
        while (at(Tok::Elif)) {
            advance();
            node->if_conds.push_back(parseList());
            expect(Tok::Then, "'then'");
            node->if_bodies.push_back(parseList());
        }
        if (at(Tok::Else)) {
            advance();
            node->else_body = parseList();
        }
        expect(Tok::Fi, "'fi'");
        return node;
    }

    // while := 'while' list 'do' list 'done'
    NodePtr parseWhile() {
        auto node = std::make_unique<Node>(NodeKind::While);
        expect(Tok::While, "'while'");
        node->while_cond = parseList();
        expect(Tok::Do, "'do'");
        node->while_body = parseList();
        expect(Tok::Done, "'done'");
        return node;
    }

    // simple := (assignment)* (word | redirection)+   (all optional if assigns)
    NodePtr parseSimple() {
        auto node = std::make_unique<Node>(NodeKind::Simple);

        // leading assignments
        while (at(Tok::Assignment)) {
            const std::string& s = peek().text;
            const auto eq = s.find('=');
            node->assigns.push_back(Assignment{s.substr(0, eq), s.substr(eq + 1)});
            advance();
        }

        // words and redirections, in any order
        for (;;) {
            if (at(Tok::Word)) {
                node->words.push_back(peek().text);
                advance();
            } else if (at(Tok::Less) || at(Tok::Great) || at(Tok::DGreat)) {
                Redirection r;
                r.op = at(Tok::Less) ? RedirOp::In
                     : at(Tok::Great) ? RedirOp::Out : RedirOp::Append;
                advance();
                if (!at(Tok::Word)) {
                    throw PErr{"expected filename after redirection", peek().line, peek().col};
                }
                r.target = peek().text;
                advance();
                node->redirs.push_back(std::move(r));
            } else {
                break;
            }
        }

        if (node->assigns.empty() && node->words.empty() && node->redirs.empty()) {
            throw PErr{"expected a command but found '" + peek().text + "'",
                       peek().line, peek().col};
        }
        return node;
    }

    const std::vector<Token>& t_;
    std::size_t pos_ = 0;
};

} // namespace

NodePtr parse(const std::vector<Token>& tokens, ParseError& err) {
    try {
        Parser p(tokens);
        auto node = p.parseProgram();
        err = ParseError{};
        return node;
    } catch (const PErr& e) {
        err = ParseError{e.message, e.line, e.col};
        return nullptr;
    }
}

} // namespace sleela::sh
