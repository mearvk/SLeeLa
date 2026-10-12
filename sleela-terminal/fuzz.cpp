// SLeeLa Terminal lexer/parser/expansion fuzz harness.
// Build with: make fuzz
#include "lexer.hpp"
#include "parser.hpp"
#include "expand.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    // Keep fuzz cases bounded so pathological inputs test parser safety rather
    // than consume unbounded resources.
    if (size > (1u << 16)) size = (1u << 16);
    std::string source(reinterpret_cast<const char*>(data), size);

    std::vector<sleela::sh::Token> tokens;
    sleela::sh::LexError lexError;
    if (sleela::sh::lex(source, tokens, lexError)) {
        sleela::sh::ParseError parseError;
        (void)sleela::sh::parse(tokens, parseError);
    }
    // Exercise expansion with command substitution disabled, so fuzz input cannot execute code.
    sleela::sh::Environment env;
    (void)sleela::sh::expandWord(source, env, nullptr);
    return 0;
}
