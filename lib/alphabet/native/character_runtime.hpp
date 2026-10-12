#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace sleela::alphabet {

struct Utf8Result {
    std::vector<char32_t> codepoints;
    std::string error;
    explicit operator bool() const noexcept { return error.empty(); }
};

Utf8Result decode_utf8(std::string_view bytes);
std::optional<std::string> encode_utf8(char32_t codepoint);
std::optional<std::size_t> codepoint_count_utf8(std::string_view bytes);
std::optional<std::size_t> parse_capacity_signature(std::string_view signature);

class CharacterSet {
public:
    explicit CharacterSet(std::size_t capacity);
    bool add(std::string symbol, std::string* error = nullptr);
    bool contains(std::string_view symbol) const;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    const std::vector<std::string>& ordered_symbols() const noexcept;
private:
    std::size_t capacity_;
    std::vector<std::string> symbols_;
};

} // namespace sleela::alphabet
