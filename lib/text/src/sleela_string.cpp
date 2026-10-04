/*
 * lib/text/src/sleela_string.cpp
 * SLeeLa Standard Library - text family native boundary (C++ implementation).
 * Max Rupplin - MEARVK LLC - 2026
 *
 * The SleelaString facade is header-inline over the C ABI. This translation
 * unit provides the non-inline orchestration helpers for callers that prefer
 * free functions, and anchors the C++ object file in the package build.
 */
#include "../include/sleela_string.hpp"

namespace sleela {
namespace text {

/* Join a sequence of fields with a separator (inverse of SleelaString::split). */
SleelaString join(const std::vector<SleelaString> &parts, const SleelaString &sep) {
    SleelaString acc;
    bool first = true;
    for (const SleelaString &p : parts) {
        if (!first) acc = acc.concat(sep);
        acc = acc.concat(p);
        first = false;
    }
    return acc;
}

/* Normalize whitespace: trim ends, then collapse any interior run of ASCII
 * whitespace (space, tab, newline, CR, FF, VT) to a single space. */
SleelaString normalizeWhitespace(const SleelaString &s) {
    const std::string &in = s.trim().str();
    std::string out;
    out.reserve(in.size());
    bool in_space = false;
    for (char c : in) {
        bool ws = (c == ' ' || c == '\t' || c == '\n' ||
                   c == '\r' || c == '\f' || c == '\v');
        if (ws) {
            in_space = true;
        } else {
            if (in_space && !out.empty()) out.push_back(' ');
            in_space = false;
            out.push_back(c);
        }
    }
    return SleelaString(out);
}

} /* namespace text */
} /* namespace sleela */
