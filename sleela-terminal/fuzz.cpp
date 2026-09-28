// SLeeLa Terminal lexer/parser fuzz harness.
// Build with: make fuzz
#include "lexer.hpp"
#include "parser.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    // Keep fuzz cases bounded so pathological inputs test parser safety rather
    // than consume unbounded resources.
    if (size > 1 << 16) size = 1 << 16;
    std::string source(reinterpret_cast<const char*>(data), size);

    std::vector<sleela::sh::Token> tokens;
    sleela::sh::LexError lexError;
    if (sleela::sh::lex(source, tokens, lexError)) {
        sleela::sh::ParseError parseError;
        (void)sleela::sh::parse(tokens, parseError);
    }
    return 0;
}
