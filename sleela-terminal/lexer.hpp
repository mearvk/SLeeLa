#pragma once

// SleelaTerminal(TM) shell -- L2 lexer.
//
// Original SLeeLa work (see NOTICE): text -> Token stream. Handles words,
// single quotes (literal), double quotes (expansion-eligible), operators with
// maximal munch (| < > >> ; && ||), comments (# to end of line), and
// name=value assignment recognition at command-word position. Keyword tokens
// (if/then/.../done) are recognised for unquoted words in command position.

#include "core.hpp"

#include <string>
#include <vector>

namespace sleela::sh {

struct LexError {
    std::string message;
    int line = 0;
    int col = 0;
};

// Tokenize `src`. On success returns true and fills `out`; on failure returns
// false and fills `err`.
bool lex(const std::string& src, std::vector<Token>& out, LexError& err);

} // namespace sleela::sh
