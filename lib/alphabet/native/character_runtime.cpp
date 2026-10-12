#include "character_runtime.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace sleela::alphabet {
namespace {
bool scalar(char32_t cp) {
    return cp <= 0x10FFFF && !(cp >= 0xD800 && cp <= 0xDFFF);
}
}

Utf8Result decode_utf8(std::string_view s) {
    Utf8Result out;
    for (std::size_t i = 0; i < s.size();) {
        const auto b0 = static_cast<unsigned char>(s[i]);
        char32_t cp = 0;
        std::size_t n = 0;
        if (b0 <= 0x7F) { cp = b0; n = 1; }
        else if (b0 >= 0xC2 && b0 <= 0xDF) { cp = b0 & 0x1F; n = 2; }
        else if (b0 >= 0xE0 && b0 <= 0xEF) { cp = b0 & 0x0F; n = 3; }
        else if (b0 >= 0xF0 && b0 <= 0xF4) { cp = b0 & 0x07; n = 4; }
        else { out.error = "invalid UTF-8 leading byte"; return out; }
        if (i + n > s.size()) { out.error = "truncated UTF-8 sequence"; return out; }
        for (std::size_t j = 1; j < n; ++j) {
            const auto bx = static_cast<unsigned char>(s[i + j]);
            if ((bx & 0xC0) != 0x80) { out.error = "invalid UTF-8 continuation byte"; return out; }
            cp = static_cast<char32_t>((cp << 6) | (bx & 0x3F));
        }
        if ((n == 2 && cp < 0x80) || (n == 3 && cp < 0x800) ||
            (n == 4 && cp < 0x10000) || !scalar(cp)) {
            out.error = "non-scalar, overlong, or out-of-range UTF-8 sequence"; return out;
        }
        out.codepoints.push_back(cp);
        i += n;
    }
    return out;
}

std::optional<std::string> encode_utf8(char32_t cp) {
    if (!scalar(cp)) return std::nullopt;
    std::string out;
    if (cp <= 0x7F) out.push_back(static_cast<char>(cp));
    else if (cp <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    return out;
}

std::optional<std::size_t> codepoint_count_utf8(std::string_view s) {
    auto decoded = decode_utf8(s);
    if (!decoded) return std::nullopt;
    return decoded.codepoints.size();
}

std::optional<std::size_t> parse_capacity_signature(std::string_view s) {
    if (s.size() < 2 || s[0] != 'C' || s[1] == '0') return std::nullopt;
    std::size_t n = 0;
    for (std::size_t i = 1; i < s.size(); ++i) {
        const char c = s[i];
        if (c < '0' || c > '9') return std::nullopt;
        const auto digit = static_cast<std::size_t>(c - '0');
        if (n > (1048576 - digit) / 10) return std::nullopt;
        n = n * 10 + digit;
    }
    if (n < 1 || n > 1048576) return std::nullopt;
    return n;
}

CharacterSet::CharacterSet(std::size_t capacity) : capacity_(capacity) {
    if (capacity < 1 || capacity > 1048576)
        throw std::invalid_argument("CharacterSet capacity must be 1..1048576");
}
bool CharacterSet::add(std::string symbol, std::string* error) {
    auto decoded = decode_utf8(symbol);
    if (!decoded || decoded.codepoints.empty()) {
        if (error) *error = decoded ? "symbol must not be empty" : decoded.error;
        return false;
    }
    if (std::find(symbols_.begin(), symbols_.end(), symbol) != symbols_.end()) {
        if (error) *error = "duplicate symbol";
        return false;
    }
    if (symbols_.size() >= capacity_) {
        if (error) *error = "character-set capacity exceeded";
        return false;
    }
    symbols_.push_back(std::move(symbol));
    if (error) error->clear();
    return true;
}
bool CharacterSet::contains(std::string_view symbol) const {
    auto decoded = decode_utf8(symbol);
    if (!decoded || decoded.codepoints.empty()) return false;
    return std::find(symbols_.begin(), symbols_.end(), symbol) != symbols_.end();
}
std::size_t CharacterSet::size() const noexcept { return symbols_.size(); }
std::size_t CharacterSet::capacity() const noexcept { return capacity_; }
const std::vector<std::string>& CharacterSet::ordered_symbols() const noexcept { return symbols_; }

} // namespace sleela::alphabet
