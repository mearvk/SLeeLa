#pragma once

// SleelaTerminal(TM) shell -- L3 parser.
//
// Original SLeeLa work (see NOTICE): tokens -> AST via recursive descent, one
// routine per grammar production:
//     list      := andor ( (';' | '\n') andor )*
//     andor     := pipeline ( ('&&' | '||') pipeline )*
//     pipeline  := command ( '|' command )*
//     command   := if_clause | while_clause | simple_command
//     simple    := (assignment)* (word | redirection)+
//     if        := 'if' list 'then' list ('elif' list 'then' list)*
//                  ['else' list] 'fi'
//     while     := 'while' list 'do' list 'done'
// Parse errors are structured, never crashes.

#include "core.hpp"

#include <string>
#include <vector>

namespace sleela::sh {

struct ParseError {
    std::string message;
    int line = 0;
    int col = 0;
};

// Parse a token stream into a top-level List node. On success returns the node
// and leaves err.message empty; on failure returns nullptr and fills err.
NodePtr parse(const std::vector<Token>& tokens, ParseError& err);

} // namespace sleela::sh
