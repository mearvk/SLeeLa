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
bool ForwardingPolicy::valid(const ForwardingAnnotation& f) noexcept{return valid_destination(f.next);}
}
