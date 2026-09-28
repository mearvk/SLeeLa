#pragma once

// SleelaTerminal(TM) shell -- L5 executor.
//
// Original SLeeLa work (see NOTICE): evaluate the AST against an Environment.
//   · simple command: apply assignments, expand words, dispatch to a builtin
//     or an external program (fork+execvp), applying redirections
//   · pipeline: wire stdout->stdin across children; status is the last
//   · and-or: short-circuit on $?
//   · if / while: branch on the condition list's exit status (0 == true)
// Exit status is threaded through as $? on the Environment.

#include "core.hpp"

namespace sleela::sh {

// Execute a node, returning its exit status (also stored as env.$?).
int execute(const Node& node, Environment& env);

} // namespace sleela::sh
