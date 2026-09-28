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
            case Tok::Esac: case Tok::RBrace:
                return true;
            default:
                return false;
        }
    }

    // list := andor ( (';' | '\n') andor )*
    NodePtr parseList() {
        auto list = std::make_unique<Node>(NodeKind::List);
        list->children.push_back(parseAndOr());
        list->child_async.push_back(false);
        for (;;) {
            // A separator is ';', newline, or '&' (which marks the *previous*
            // and-or as asynchronous / background).
            bool sawSep = false;
            while (at(Tok::Semi) || at(Tok::Newline) || at(Tok::Amp)) {
                if (at(Tok::Amp)) list->child_async.back() = true;
                advance();
                sawSep = true;
            }
            if (!sawSep) break;
            if (isListTerminator()) break;
            list->children.push_back(parseAndOr());
            list->child_async.push_back(false);
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

    // pipeline := [ '!' ] command ( '|' command )*
    NodePtr parsePipeline() {
        bool negated = false;
        while (at(Tok::Bang)) { negated = !negated; advance(); }

        auto first = parseCommand();
        if (!at(Tok::Pipe)) {
            if (negated) {
                // Wrap a single command in a Pipeline node to carry the flag.
                auto node = std::make_unique<Node>(NodeKind::Pipeline);
                node->negated = true;
                node->children.push_back(std::move(first));
                return node;
            }
            return first;
        }

        auto node = std::make_unique<Node>(NodeKind::Pipeline);
        node->negated = negated;
        node->children.push_back(std::move(first));
        while (at(Tok::Pipe)) {
            advance();
            while (at(Tok::Newline)) advance();
            node->children.push_back(parseCommand());
        }
        return node;
    }

    // command := if | while | for | case | function_def | brace_group | simple
    NodePtr parseCommand() {
        if (at(Tok::If)) return parseIf();
        if (at(Tok::While)) return parseWhile();
        if (at(Tok::Until)) return parseUntil();
        if (at(Tok::For)) return parseFor();
        if (at(Tok::Case)) return parseCase();
        if (at(Tok::LBrace)) return parseBraceGroup();
        // function definition: WORD '(' ')' '{' ... '}'
        if (at(Tok::Word) && peek2().kind == Tok::LParen) return parseFunctionDef();
        return parseSimple();
    }

    // brace_group := '{' list '}'   (executed in the current shell)
    NodePtr parseBraceGroup() {
        expect(Tok::LBrace, "'{'");
        while (at(Tok::Newline)) advance();
        auto list = parseList();
        while (at(Tok::Newline) || at(Tok::Semi)) advance();
        expect(Tok::RBrace, "'}'");
        return list;
    }

    // function_def := WORD '(' ')' brace_group
    NodePtr parseFunctionDef() {
        auto node = std::make_unique<Node>(NodeKind::FunctionDef);
        node->func_name = peek().text;
        advance();                     // name
        expect(Tok::LParen, "'('");
        expect(Tok::RParen, "')'");
        while (at(Tok::Newline)) advance();
        node->func_body = parseBraceGroup();
        return node;
    }

    // for := 'for' NAME [ 'in' word* ] (';'|'\n') 'do' list 'done'
    NodePtr parseFor() {
        auto node = std::make_unique<Node>(NodeKind::For);
        expect(Tok::For, "'for'");
        if (!at(Tok::Word)) throw PErr{"expected a variable name after 'for'", peek().line, peek().col};
        node->for_var = peek().text;
        advance();
        // optional word list after 'in'
        if (at(Tok::In)) {
            advance();
            while (at(Tok::Word)) { node->for_words.push_back(peek().text); advance(); }
        } else {
            // no explicit list -> iterate the positional parameters ("$@")
            node->for_words.push_back("$@");
        }
        // separator, then do..done
        while (at(Tok::Semi) || at(Tok::Newline)) advance();
        expect(Tok::Do, "'do'");
        while (at(Tok::Newline)) advance();
        node->for_body = parseList();
        expect(Tok::Done, "'done'");
        return node;
    }

    // case := 'case' word 'in' ( [ '(' ] pattern ('|' pattern)* ')' list ';;' )* 'esac'
    NodePtr parseCase() {
        auto node = std::make_unique<Node>(NodeKind::Case);
        expect(Tok::Case, "'case'");
        if (!at(Tok::Word)) throw PErr{"expected a word after 'case'", peek().line, peek().col};
        node->case_subject = peek().text;
        advance();
        expect(Tok::In, "'in'");
        while (at(Tok::Newline) || at(Tok::Semi)) advance();

        while (!at(Tok::Esac)) {
            CaseItem item;
            if (at(Tok::LParen)) advance();  // optional leading '('
            // patterns separated by '|', terminated by ')'
            for (;;) {
                if (!at(Tok::Word)) throw PErr{"expected a case pattern", peek().line, peek().col};
                item.patterns.push_back(peek().text);
                advance();
                if (at(Tok::Pipe)) { advance(); continue; }
                break;
            }
            expect(Tok::RParen, "')'");
            while (at(Tok::Newline)) advance();
            // body: a list up to ';;' (represented as Semi Semi) or 'esac'
            item.body = parseCaseBody();
            node->case_items.push_back(std::move(item));
            while (at(Tok::Newline) || at(Tok::Semi)) advance();
        }
        expect(Tok::Esac, "'esac'");
        return node;
    }

    // Parse a case arm body: a list that ends at ';;' or 'esac'. We detect ';;'
    // as two consecutive Semi tokens.
    NodePtr parseCaseBody() {
        auto list = std::make_unique<Node>(NodeKind::List);
        // empty body (immediate ';;' or 'esac')
        if (isCaseArmEnd()) return list;
        list->children.push_back(parseAndOr());
        for (;;) {
            // ';;' ends the arm
            if (at(Tok::Semi) && peek2().kind == Tok::Semi) { advance(); advance(); break; }
            bool sawSep = false;
            while (at(Tok::Semi) || at(Tok::Newline)) {
                if (at(Tok::Semi) && peek2().kind == Tok::Semi) break;  // leave ';;'
                advance(); sawSep = true;
            }
            if (at(Tok::Semi) && peek2().kind == Tok::Semi) { advance(); advance(); break; }
            if (at(Tok::Esac)) break;
            if (!sawSep) break;
            if (at(Tok::Esac)) break;
            list->children.push_back(parseAndOr());
        }
        return list;
    }

    bool isCaseArmEnd() const {
        if (at(Tok::Esac)) return true;
        if (at(Tok::Semi) && peek2().kind == Tok::Semi) return true;
        return false;
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
        while (at(Tok::Newline)) advance();
        node->while_body = parseList();
        expect(Tok::Done, "'done'");
        return node;
    }

    // until := 'until' list 'do' list 'done'  (runs body while cond is FALSE)
    NodePtr parseUntil() {
        auto node = std::make_unique<Node>(NodeKind::Until);
        expect(Tok::Until, "'until'");
        node->while_cond = parseList();
        expect(Tok::Do, "'do'");
        while (at(Tok::Newline)) advance();
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
            } else if (at(Tok::DLess) || at(Tok::DLessDash)) {
                // here-document: << DELIM  followed by a HeredocBody token
                Redirection r;
                r.op = RedirOp::Heredoc;
                advance();
                if (!at(Tok::Word)) {
                    throw PErr{"expected here-document delimiter", peek().line, peek().col};
                }
                r.target = peek().text;   // the delimiter (informational)
                advance();
                if (!at(Tok::HeredocBody)) {
                    throw PErr{"missing here-document body", peek().line, peek().col};
                }
                {
                    const std::string& bt = peek().text;
                    r.expand_body = !bt.empty() && bt[0] == 'E';
                    r.body = bt.empty() ? std::string() : bt.substr(1);
                }
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
