/*
 * native/src/sleela_crypto.cpp
 * SLeeLa Native Cryptography Bridge — C++17 implementation of the stable C ABI
 * declared in include/sleela_crypto.h.
 *
 * Implements the deterministic mixed-radix converters (bases 1..2055), the
 * ordered block/intermix transforms, the comparison gate, and the national
 * register call that back the SLeeLa /lib/crypto sophistication classes.
 *
 * SECURITY NOTE: deterministic, reversible obfuscation — NOT cryptography.
 * See the header for the full warning.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_crypto.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {

thread_local std::string g_last_error;

void set_error(const char *msg) { g_last_error = msg ? msg : ""; }

/* Extended digit alphabet for positional bases 2..36 uses 0-9A-Z. Bases above
 * 36 use a delimited digit form "(d1).(d2).(d3)" where each dN is the decimal
 * value of a positional digit, most-significant first. This keeps every base up
 * to 2055 representable as text without needing 2055 distinct glyphs. */
const char *kAlphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool valid_base(int base) {
    return base >= SLEELA_CRYPTO_MIN_BASE && base <= SLEELA_CRYPTO_MAX_BASE;
}

std::string to_unary(int64_t value) {
    /* Base 1: tally. Non-negative magnitude as a run of '1'. */
    std::string s;
    int64_t v = value < 0 ? -value : value;
    if (value < 0) s.push_back('-');
    for (int64_t i = 0; i < v; ++i) s.push_back('1');
    if (v == 0) s.push_back('0');
    return s;
}

std::string to_positional(int base, int64_t value) {
    bool negative = value < 0;
    uint64_t v = negative ? static_cast<uint64_t>(-(value + 1)) + 1u
                          : static_cast<uint64_t>(value);
    if (v == 0) return "0";

    if (base <= 36) {
        std::string s;
        while (v > 0) {
            s.push_back(kAlphabet[v % static_cast<uint64_t>(base)]);
            v /= static_cast<uint64_t>(base);
        }
        if (negative) s.push_back('-');
        std::reverse(s.begin(), s.end());
        return s;
    }

    /* Delimited decimal-digit form for large bases. */
    std::string s;
    while (v > 0) {
        uint64_t digit = v % static_cast<uint64_t>(base);
        std::string d = std::to_string(digit);
        if (!s.empty()) s = d + "." + s;
        else s = d;
        v /= static_cast<uint64_t>(base);
    }
    if (negative) s = "-" + s;
    return s;
}

int64_t from_unary(const std::string &digits, bool &ok) {
    ok = true;
    size_t start = 0;
    bool neg = false;
    if (!digits.empty() && digits[0] == '-') { neg = true; start = 1; }
    if (digits.size() == start + 1 && digits[start] == '0') return 0;
    int64_t count = 0;
    for (size_t i = start; i < digits.size(); ++i) {
        if (digits[i] != '1') { ok = false; return 0; }
        ++count;
    }
    return neg ? -count : count;
}

int digit_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 10 + (c - 'a');
    return -1;
}

int64_t from_positional(int base, const std::string &digits, bool &ok) {
    ok = true;
    if (digits.empty()) { ok = false; return 0; }
    size_t start = 0;
    bool neg = false;
    if (digits[0] == '-') { neg = true; start = 1; }

    int64_t acc = 0;
    if (base <= 36) {
        for (size_t i = start; i < digits.size(); ++i) {
            int d = digit_value(digits[i]);
            if (d < 0 || d >= base) { ok = false; return 0; }
            acc = acc * base + d;
        }
    } else {
        /* Parse the delimited decimal-digit form. */
        std::string token;
        auto flush = [&](void) -> bool {
            if (token.empty()) return false;
            int d = std::atoi(token.c_str());
            if (d < 0 || d >= base) return false;
            acc = acc * base + d;
            token.clear();
            return true;
        };
        for (size_t i = start; i < digits.size(); ++i) {
            char c = digits[i];
            if (c == '.') { if (!flush()) { ok = false; return 0; } }
            else if (std::isdigit(static_cast<unsigned char>(c))) token.push_back(c);
            else { ok = false; return 0; }
        }
        if (!flush()) { ok = false; return 0; }
    }
    return neg ? -acc : acc;
}

/* Parse a decimal integer from arbitrary running text; non-digits are skipped so
 * the obfuscation transforms stay total, mirroring the lenient reference. */
int64_t parse_running(const char *running) {
    if (!running) return 0;
    std::string digits;
    bool neg = false;
    bool seen = false;
    for (const char *p = running; *p; ++p) {
        if (!seen && *p == '-') { neg = true; }
        if (std::isdigit(static_cast<unsigned char>(*p))) { digits.push_back(*p); seen = true; }
    }
    if (digits.empty()) return 0;
    int64_t v = std::strtoll(digits.c_str(), nullptr, 10);
    return neg ? -v : v;
}

int write_out(const std::string &s, char *out, size_t out_cap) {
    if (!out || out_cap == 0) { set_error("null/zero output buffer"); return -1; }
    if (s.size() + 1 > out_cap) { set_error("output buffer too small"); return -1; }
    std::memcpy(out, s.c_str(), s.size() + 1);
    return static_cast<int>(s.size());
}

/* The shared radix-then-OR alteration used by block advance and the row/position
 * intermix helpers. */
std::string radix_or(int base, int64_t mask, int64_t value, bool &ok) {
    ok = true;
    if (!valid_base(base)) { ok = false; return ""; }
    /* Round-trip through the base (as the reference does), then OR with mask. */
    std::string encoded = (base == 1) ? to_unary(value) : to_positional(base, value);
    bool dec_ok = false;
    int64_t decoded = (base == 1) ? from_unary(encoded, dec_ok)
                                   : from_positional(base, encoded, dec_ok);
    if (!dec_ok) { ok = false; return ""; }
    int64_t result = decoded | mask;
    return std::to_string(result);
}

} /* namespace */

extern "C" {

int sleela_crypto_radix_to_base(int base, int64_t value, char *out, size_t out_cap) {
    if (!valid_base(base)) { set_error("radix out of range 1..2055"); return -1; }
    std::string s = (base == 1) ? to_unary(value) : to_positional(base, value);
    return write_out(s, out, out_cap);
}

int64_t sleela_crypto_radix_from_base(int base, const char *digits, int *ok) {
    if (!valid_base(base)) { set_error("radix out of range 1..2055"); if (ok) *ok = 0; return 0; }
    if (!digits) { set_error("null digit string"); if (ok) *ok = 0; return 0; }
    bool parsed = false;
    std::string d(digits);
    int64_t v = (base == 1) ? from_unary(d, parsed) : from_positional(base, d, parsed);
    if (ok) *ok = parsed ? 1 : 0;
    if (!parsed) set_error("invalid digit for base");
    return v;
}

int sleela_crypto_block_advance(int order, int base, int64_t mask,
                                const char *running, char *out, size_t out_cap) {
    if (order < SLEELA_CRYPTO_MIN_ORDER || order > SLEELA_CRYPTO_MAX_ORDER) {
        set_error("order out of range 1..255");
        return -1;
    }
    bool ok = false;
    std::string s = radix_or(base, mask, parse_running(running), ok);
    if (!ok) { set_error("block advance failed"); return -1; }
    return write_out(s, out, out_cap);
}

int sleela_crypto_block_two_rows(const char *running,
                                 int base_row2, int64_t mask_row2,
                                 int base_row7, int64_t mask_row7,
                                 int base_row6, int64_t mask_row6,
                                 char *out, size_t out_cap) {
    int64_t v = parse_running(running);
    bool ok2 = false, ok7 = false, ok6 = false;
    std::string r2 = radix_or(base_row2, mask_row2, v, ok2);
    std::string r7 = radix_or(base_row7, mask_row7, v, ok7);
    std::string r6 = radix_or(base_row6, mask_row6, v, ok6);
    if (!ok2 || !ok7 || !ok6) { set_error("two-rows intermix failed"); return -1; }
    /* Row order honoured: 2nd, then 7th, then 6th, as in the reference pass two. */
    std::string s = r2 + ":" + r7 + ":" + r6;
    return write_out(s, out, out_cap);
}

int sleela_crypto_intermix_primary(const char *running, int span,
                                   int pos7, int base7, int64_t mask7,
                                   int pos2, int base2, int64_t mask2,
                                   int pos6, int base6, int64_t mask6,
                                   int pos1, int base1, int64_t mask1,
                                   char *out, size_t out_cap) {
    int64_t v = parse_running(running);
    std::string acc;
    for (int i = 1; i < span; ++i) {
        bool ok = true;
        if (i == pos7)      acc += radix_or(base7, mask7, v, ok);
        else if (i == pos2) acc += radix_or(base2, mask2, v, ok);
        else if (i == pos6) acc += radix_or(base6, mask6, v, ok);
        else if (i == pos1) acc += radix_or(base1, mask1, v, ok);
        if (!ok) { set_error("primary intermix failed"); return -1; }
    }
    if (acc.empty()) acc = std::to_string(v);
    return write_out(acc, out, out_cap);
}

int sleela_crypto_intermix_secondary(const char *running, int span,
                                     int pos17, int base17, int64_t mask17,
                                     int pos2,  int base2,  int64_t mask2,
                                     int pos3,  int base3,  int64_t mask3,
                                     char *out, size_t out_cap) {
    int64_t v = parse_running(running);
    std::string acc;
    for (int i = 1; i < span; ++i) {
        bool ok = true;
        if (i == pos17)     acc += radix_or(base17, mask17, v, ok);
        else if (i == pos2) acc += radix_or(base2, mask2, v, ok);
        else if (i == pos3) acc += radix_or(base3, mask3, v, ok);
        if (!ok) { set_error("secondary intermix failed"); return -1; }
    }
    if (acc.empty()) acc = std::to_string(v);
    return write_out(acc, out, out_cap);
}

int sleela_crypto_compare(const char *expected, const char *actual) {
    if (!expected || !actual) { set_error("null comparison operand"); return 0; }
    size_t le = std::strlen(expected);
    size_t la = std::strlen(actual);
    /* Non-short-circuiting: scan the full longer operand, accumulate difference. */
    size_t n = std::max(le, la);
    unsigned diff = (le == la) ? 0u : 1u;
    for (size_t i = 0; i < n; ++i) {
        char ce = i < le ? expected[i] : 0;
        char ca = i < la ? actual[i] : 0;
        diff |= static_cast<unsigned>(ce ^ ca);
    }
    return diff == 0 ? 1 : 0;
}

int sleela_crypto_national_register(const char *registry_id, const char *value) {
    if (!registry_id || !value || value[0] == '\0') {
        set_error("national register refused empty submission");
        return 0;
    }
    /* Reference bridge: deterministic record to stderr; a production bridge
     * would hand off to the OS register facility below this boundary. */
    std::fprintf(stderr, "[sleela_crypto] register=%s value_len=%zu\n",
                 registry_id, std::strlen(value));
    return 1;
}

const char *sleela_crypto_last_error(void) {
    return g_last_error.c_str();
}

} /* extern "C" */
