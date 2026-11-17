#include "ForwardingAnnotation.hpp"
namespace sleela::annotation {
// A @next destination is a relative architectural path such as
// "system/network/multiplexing". It is accepted only when it cannot name a place
// outside the tree it is resolved against:
//   - not empty, and not absolute ("/x"; a drive or scheme such as "C:/x" or
//     "file:x" is absolute too, so ':' is refused);
//   - no ".." (parent) component and no backslash separator;
//   - no '%': a percent sequence such as "%2e%2e" would turn into ".." in any
//     consumer that decodes it, so the encoding is refused rather than trusted;
//   - no control characters.
// The parser joins an annotation's tokens with single spaces, so "a/../b" reaches
// this function as "a / . . / b". Whitespace is therefore ignored when judging the
// text; otherwise ". ." would hide a parent component from the ".." test.
bool ForwardingPolicy::valid_destination(const std::string& v) noexcept {
    std::string c;
    c.reserve(v.size());
    for (unsigned char ch : v) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f') continue;
        if (ch < 0x20 || ch == 0x7f) return false;
        c.push_back(static_cast<char>(ch));
    }
    return !c.empty() && c.front() != '/' && c.find("..") == std::string::npos &&
           c.find('\\') == std::string::npos && c.find(':') == std::string::npos &&
           c.find('%') == std::string::npos;
}
std::string ForwardingPolicy::normalize_destination(const std::string& v) {
    auto word = [](char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'); };
    std::string out;
    out.reserve(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        const char ch = v[i];
        if (ch == ' ' || ch == '\t') {
            const char before = out.empty() ? '\0' : out.back();
            size_t j = i + 1;
            while (j < v.size() && (v[j] == ' ' || v[j] == '\t')) ++j;
            const char after = j < v.size() ? v[j] : '\0';
            if (word(before) && word(after)) out.push_back(' ');   // "a b" stays two words
            i = j - 1;
            continue;
        }
        out.push_back(ch);
    }
    return out;
}
bool ForwardingPolicy::valid(const ForwardingAnnotation& f) noexcept{return valid_destination(f.next);}
}
