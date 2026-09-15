#pragma once

// SleelaTerminal(TM) shell -- L1 arithmetic engine.
//
// Original SLeeLa work (see NOTICE): an integer expression evaluator used by
// $(( ... )) and the shell's own numeric needs. Math-driven core -- a
// precedence-climbing evaluator over an explicit operator-precedence table, so
// precedence and associativity are correct by construction rather than by an
// ad-hoc cascade of grammar functions.
//
// A variable lookup callback resolves identifiers (e.g. shell variables) to
// integers, so the engine stays independent of the Environment type.

#include <functional>
#include <string>

namespace sleela::sh {

// Result of evaluating an arithmetic expression.
struct ArithResult {
    long value = 0;
    bool ok = true;
    std::string error;   // set when ok == false (e.g. "division by zero")
};

// Resolve an identifier to an integer. Unknown names should map to 0 (shell
// semantics), which is the default.
using VarLookup = std::function<long(const std::string& name)>;

// Evaluate an integer arithmetic expression. Supported operators, low to high
// precedence: || ; && ; == != ; < <= > >= ; + - ; * / % ; unary + - ! ;
// parentheses / literal / identifier. Division and modulo by zero are reported
// as errors rather than invoking undefined behaviour.
ArithResult evalArith(const std::string& expr, const VarLookup& lookup);

// Convenience overload with no variables (all identifiers resolve to 0).
ArithResult evalArith(const std::string& expr);

} // namespace sleela::sh
