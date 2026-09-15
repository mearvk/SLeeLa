#pragma once

// SleelaTerminal(TM) shell -- L4 expansion.
//
// Original SLeeLa work (see NOTICE): a parsed word's raw text -> its final
// argv field(s), via ordered passes:
//   1. brace expansion         {a,b}  {m..n}   (produces multiple fields)
//   2. tilde expansion          ~ ~user         (at word start / after =:)
//   3. command substitution     $( ... )        (runs a command, captures stdout)
//   4. arithmetic expansion      $(( expr ))     (uses the L1 Arith engine)
//   5. parameter expansion       $name ${name} $? $$ $# $1..$9 $@
//                                ${x:-w} ${x:=w} ${x:?m} ${x:+w} ${#x}
//   6. field splitting on unquoted whitespace
//   7. pathname globbing (* ? [..]) against the filesystem
//
// Command substitution needs to run a command, which the executor provides via
// a CommandRunner callback -- so L4 stays below L5 with no dependency cycle.
// The Environment is mutable because ${x:=word} assigns back to it.
//
// A word may expand to zero, one, or many fields, so the primary entry point
// returns a vector. `expandWordSingle` gives exactly one string for contexts
// that want it (redirection targets, case subjects).

#include "core.hpp"

#include <functional>
#include <string>
#include <vector>

namespace sleela::sh {

// Run a command line (the text inside $( ... )) and return its captured stdout.
// Supplied by the executor.
using CommandRunner = std::function<std::string(const std::string& command)>;

// Expand one word into argv fields against the environment. `run` performs
// command substitution; if null, $( ... ) expands to empty.
std::vector<std::string> expandWord(const std::string& word,
                                    Environment& env,
                                    const CommandRunner& run);

// Expand to exactly one string (no field splitting / globbing kept as one).
std::string expandWordSingle(const std::string& word,
                             Environment& env,
                             const CommandRunner& run);

// Glob a single pattern against the filesystem. Returns matches (sorted); if
// there are no matches, returns the pattern unchanged (shell nullglob=off).
std::vector<std::string> globPattern(const std::string& pattern);

// True if `text` matches shell glob `pattern` (* ? [..]). Used by `case`.
bool globMatch(const std::string& pattern, const std::string& text);

} // namespace sleela::sh
