#pragma once

// SleelaTerminal(TM) shell -- L4 expansion.
//
// Original SLeeLa work (see NOTICE): a parsed word's raw text -> its final
// argv field(s), via ordered passes:
//   1. command substitution   $( ... )     (runs a command, captures stdout)
//   2. arithmetic expansion    $(( expr ))  (uses the L1 Arith engine)
//   3. parameter expansion      $name ${name} $? $$ $# $1..$9 $@
//   4. field splitting on unquoted whitespace
//   5. pathname globbing (* ? [..]) against the filesystem
//
// Command substitution needs to run a command, which the executor provides via
// a CommandRunner callback -- so L4 stays below L5 with no dependency cycle.
//
// A word may expand to zero, one, or many fields (splitting + globbing), so the
// primary entry point returns a vector. `expandWordSingle` joins the result for
// contexts that want exactly one string (redirection targets, case subjects).

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
                                    const Environment& env,
                                    const CommandRunner& run);

// Expand to exactly one string (no field splitting / globbing kept as one).
std::string expandWordSingle(const std::string& word,
                             const Environment& env,
                             const CommandRunner& run);

// Glob a single pattern against the filesystem. Returns matches (sorted); if
// there are no matches, returns the pattern unchanged (shell nullglob=off).
std::vector<std::string> globPattern(const std::string& pattern);

// True if `text` matches shell glob `pattern` (* ? [..]). Used by `case`.
bool globMatch(const std::string& pattern, const std::string& text);

} // namespace sleela::sh
