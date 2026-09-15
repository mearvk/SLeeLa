#pragma once

// SleelaTerminal(TM) shell -- L4 expansion.
//
// Original SLeeLa work (see NOTICE): a parsed word's raw text -> its final
// string, via ordered passes:
//   1. arithmetic expansion   $(( expr ))   (uses the L1 Arith engine)
//   2. parameter expansion     $name  ${name}  $?  $$
// Quote removal already happened in the lexer for this milestone, so expansion
// operates on the word's literal text with `$` sequences still present.
//
// Pure and testable: (word, environment) -> string.

#include "core.hpp"

#include <string>

namespace sleela::sh {

// Expand one word against the environment.
std::string expandWord(const std::string& word, const Environment& env);

} // namespace sleela::sh
