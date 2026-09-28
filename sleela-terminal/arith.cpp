// SleelaTerminal(TM) shell -- L1 arithmetic engine implementation.
// Original SLeeLa work (see NOTICE).
//
// Two stages: a small arithmetic lexer, then a precedence-climbing evaluator.
// Precedence climbing (a.k.a. Pratt-style binding powers) evaluates
//     parse(minbp): parse a unary/primary, then while the next binary operator
//     has left-binding-power >= minbp, consume it and recurse with its
//     right-binding-power.
// This yields correct precedence and left-associativity from one routine.

#include "arith.hpp"

#include <cctype>
#include <stdexcept>
#include <vector>

namespace sleela::sh {

namespace {

// --- arithmetic token ---
enum class AK {
    Num, Ident,
    Plus, Minus, Star, Slash, Percent,
    Lt, Le, Gt, Ge, EqEq, NeEq,
    AndAnd, OrOr, Not,
    LParen, RParen,
    End
};

struct AT {
    AK kind = AK::End;
    long num = 0;
    std::string ident;

    AT() = default;
    AT(AK k) : kind(k) {}
    AT(AK k, long n, std::string id) : kind(k), num(n), ident(std::move(id)) {}
};

// A parse/eval error carrying a message.
struct ArithError {
    std::string message;
};

class Lexer {
public:
    explicit Lexer(const std::string& s) : s_(s) {}

    std::vector<AT> tokenize() {
        std::vector<AT> out;
        while (pos_ < s_.size()) {
            char c = s_[pos_];
            if (std::isspace(static_cast<unsigned char>(c))) { ++pos_; continue; }
            if (std::isdigit(static_cast<unsigned char>(c))) {
                long v = 0;
                while (pos_ < s_.size() &&
                       std::isdigit(static_cast<unsigned char>(s_[pos_]))) {
                    v = v * 10 + (s_[pos_] - '0');
                    ++pos_;
                }
                out.push_back(AT{AK::Num, v, {}});
                continue;
            }
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                std::string id;
                while (pos_ < s_.size() &&
                       (std::isalnum(static_cast<unsigned char>(s_[pos_])) ||
                        s_[pos_] == '_')) {
                    id.push_back(s_[pos_]);
                    ++pos_;
                }
                out.push_back(AT{AK::Ident, 0, id});
                continue;
            }
            // operators (maximal munch for two-char forms)
            const char n = (pos_ + 1 < s_.size()) ? s_[pos_ + 1] : '\0';
            switch (c) {
                case '+': out.push_back({AK::Plus}); ++pos_; break;
                case '-': out.push_back({AK::Minus}); ++pos_; break;
                case '*': out.push_back({AK::Star}); ++pos_; break;
                case '/': out.push_back({AK::Slash}); ++pos_; break;
                case '%': out.push_back({AK::Percent}); ++pos_; break;
                case '(': out.push_back({AK::LParen}); ++pos_; break;
                case ')': out.push_back({AK::RParen}); ++pos_; break;
                case '<':
                    if (n == '=') { out.push_back({AK::Le}); pos_ += 2; }
                    else { out.push_back({AK::Lt}); ++pos_; }
                    break;
                case '>':
                    if (n == '=') { out.push_back({AK::Ge}); pos_ += 2; }
                    else { out.push_back({AK::Gt}); ++pos_; }
                    break;
                case '=':
                    if (n == '=') { out.push_back({AK::EqEq}); pos_ += 2; }
                    else throw ArithError{"unexpected '='"};
                    break;
                case '!':
                    if (n == '=') { out.push_back({AK::NeEq}); pos_ += 2; }
                    else { out.push_back({AK::Not}); ++pos_; }
                    break;
                case '&':
                    if (n == '&') { out.push_back({AK::AndAnd}); pos_ += 2; }
                    else throw ArithError{"unexpected '&'"};
                    break;
                case '|':
                    if (n == '|') { out.push_back({AK::OrOr}); pos_ += 2; }
                    else throw ArithError{"unexpected '|'"};
                    break;
                default:
                    throw ArithError{std::string("unexpected character '") + c + "'"};
            }
        }
        out.push_back(AT{AK::End, 0, {}});
        return out;
    }

private:
    const std::string& s_;
    std::size_t pos_ = 0;
};

// Left/right binding powers for binary operators (0 = not a binary operator).
// Left-associative operators use rbp = lbp + 1.
struct BindingPower { int lbp; int rbp; };

BindingPower binaryBp(AK k) {
    switch (k) {
        case AK::OrOr:    return {1, 2};
        case AK::AndAnd:  return {3, 4};
        case AK::EqEq:
        case AK::NeEq:    return {5, 6};
        case AK::Lt: case AK::Le: case AK::Gt: case AK::Ge: return {7, 8};
        case AK::Plus:
        case AK::Minus:   return {9, 10};
        case AK::Star:
        case AK::Slash:
        case AK::Percent: return {11, 12};
        default:          return {0, 0};
    }
}

class Evaluator {
public:
    Evaluator(std::vector<AT> toks, const VarLookup& lookup)
        : toks_(std::move(toks)), lookup_(lookup) {}

    long run() {
        long v = parse(0);
        if (peek().kind != AK::End) throw ArithError{"trailing tokens in expression"};
        return v;
    }

private:
    const AT& peek() const { return toks_[pos_]; }
    const AT& advance() { return toks_[pos_++]; }

    // Precedence-climbing core.
    long parse(int minbp) {
        long lhs = parseUnary();
        for (;;) {
            const BindingPower bp = binaryBp(peek().kind);
            if (bp.lbp == 0 || bp.lbp < minbp) break;
            const AK op = advance().kind;
            const long rhs = parse(bp.rbp);
            lhs = applyBinary(op, lhs, rhs);
        }
        return lhs;
    }

    long parseUnary() {
        const AK k = peek().kind;
        if (k == AK::Plus)  { advance(); return +parseUnary(); }
        if (k == AK::Minus) { advance(); return -parseUnary(); }
        if (k == AK::Not)   { advance(); return parseUnary() == 0 ? 1 : 0; }
        return parsePrimary();
    }

    long parsePrimary() {
        const AT& t = peek();
        if (t.kind == AK::Num)   { advance(); return t.num; }
        if (t.kind == AK::Ident) { advance(); return lookup_ ? lookup_(t.ident) : 0; }
        if (t.kind == AK::LParen) {
            advance();
            long v = parse(0);
            if (peek().kind != AK::RParen) throw ArithError{"expected ')'"};
            advance();
            return v;
        }
        throw ArithError{"expected a value"};
    }

    long applyBinary(AK op, long a, long b) {
        switch (op) {
            case AK::Plus:    return a + b;
            case AK::Minus:   return a - b;
            case AK::Star:    return a * b;
            case AK::Slash:
                if (b == 0) throw ArithError{"division by zero"};
                return a / b;
            case AK::Percent:
                if (b == 0) throw ArithError{"division by zero"};
                return a % b;
            case AK::Lt:   return a < b;
            case AK::Le:   return a <= b;
            case AK::Gt:   return a > b;
            case AK::Ge:   return a >= b;
            case AK::EqEq: return a == b;
            case AK::NeEq: return a != b;
            case AK::AndAnd: return (a != 0 && b != 0) ? 1 : 0;
            case AK::OrOr:   return (a != 0 || b != 0) ? 1 : 0;
            default: throw ArithError{"internal: bad binary operator"};
        }
    }

    std::vector<AT> toks_;
    const VarLookup& lookup_;
    std::size_t pos_ = 0;
};

} // namespace

ArithResult evalArith(const std::string& expr, const VarLookup& lookup) {
    try {
        Lexer lex(expr);
        Evaluator ev(lex.tokenize(), lookup);
        return ArithResult{ev.run(), true, {}};
    } catch (const ArithError& e) {
        return ArithResult{0, false, e.message};
    }
}

ArithResult evalArith(const std::string& expr) {
    static const VarLookup none = [](const std::string&) -> long { return 0; };
    return evalArith(expr, none);
}

} // namespace sleela::sh
