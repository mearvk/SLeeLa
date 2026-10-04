/*
 * lib/text/include/sleela_string.hpp
 * SLeeLa Standard Library - text family native boundary (C++ orchestration).
 * Max Rupplin - MEARVK LLC - 2026
 *
 * C++ facade over the stable C ABI in sleela_string.h. C provides the ABI; C++
 * may orchestrate string objects and pipelines but cannot bypass the ABI's
 * length-explicit, allocation-owning contract. Results are std::string, so
 * ownership is handled by the standard library rather than manual free().
 */
#ifndef SLEELA_STRING_HPP
#define SLEELA_STRING_HPP

#include "sleela_string.h"

#include <string>
#include <vector>

namespace sleela {
namespace text {

/* Sentinel mirroring SLEELA_STRING_NPOS at the C++ layer. */
static const std::size_t npos = SLEELA_STRING_NPOS;

/*
 * SleelaString: a value-semantic string manipulation facade. Every operation
 * delegates to the C ABI over an explicit (data, length) span, so embedded NUL
 * bytes survive. Methods that transform the string return a new SleelaString;
 * the receiver is never mutated unless documented otherwise.
 */
class SleelaString {
public:
    SleelaString() = default;
    SleelaString(const char *s, std::size_t len) : value_(s ? s : "", s ? len : 0) {}
    SleelaString(const std::string &s) : value_(s) {}       /* NOLINT: intentional implicit */
    SleelaString(const char *s) : value_(s ? s : "") {}     /* NOLINT: intentional implicit */

    /* ---- accessors ---- */
    const std::string &str() const { return value_; }
    const char *data() const { return value_.data(); }
    std::size_t length() const { return value_.size(); }
    bool empty() const { return sleela_string_is_empty(value_.data(), value_.size()) != 0; }

    /* ---- comparison ---- */
    int compare(const SleelaString &o) const {
        return sleela_string_compare(value_.data(), value_.size(),
                                     o.value_.data(), o.value_.size());
    }
    int compareIgnoreCase(const SleelaString &o) const {
        return sleela_string_compare_ci(value_.data(), value_.size(),
                                        o.value_.data(), o.value_.size());
    }
    bool equals(const SleelaString &o) const {
        return sleela_string_equals(value_.data(), value_.size(),
                                    o.value_.data(), o.value_.size()) != 0;
    }

    /* ---- search ---- */
    std::size_t find(const SleelaString &needle, std::size_t from = 0) const {
        return sleela_string_find(value_.data(), value_.size(),
                                  needle.value_.data(), needle.value_.size(), from);
    }
    std::size_t rfind(const SleelaString &needle) const {
        return sleela_string_rfind(value_.data(), value_.size(),
                                   needle.value_.data(), needle.value_.size());
    }
    std::size_t count(const SleelaString &needle) const {
        return sleela_string_count(value_.data(), value_.size(),
                                   needle.value_.data(), needle.value_.size());
    }
    bool contains(const SleelaString &needle) const {
        return sleela_string_contains(value_.data(), value_.size(),
                                      needle.value_.data(), needle.value_.size()) != 0;
    }
    bool startsWith(const SleelaString &needle) const {
        return sleela_string_starts_with(value_.data(), value_.size(),
                                         needle.value_.data(), needle.value_.size()) != 0;
    }
    bool endsWith(const SleelaString &needle) const {
        return sleela_string_ends_with(value_.data(), value_.size(),
                                       needle.value_.data(), needle.value_.size()) != 0;
    }

    /* ---- substring / manipulation ---- */
    SleelaString substring(std::size_t start, std::size_t count) const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_substring(value_.data(), value_.size(), start, count, o);
        });
    }
    SleelaString sliceFrom(std::size_t start) const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_slice_from(value_.data(), value_.size(), start, o);
        });
    }
    SleelaString concat(const SleelaString &o) const {
        return adopt([&](sleela_string_t *r) {
            return sleela_string_concat(value_.data(), value_.size(),
                                        o.value_.data(), o.value_.size(), r);
        });
    }
    SleelaString trim() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_trim(value_.data(), value_.size(), o);
        });
    }
    SleelaString trimLeft() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_trim_left(value_.data(), value_.size(), o);
        });
    }
    SleelaString trimRight() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_trim_right(value_.data(), value_.size(), o);
        });
    }
    SleelaString toUpper() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_to_upper(value_.data(), value_.size(), o);
        });
    }
    SleelaString toLower() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_to_lower(value_.data(), value_.size(), o);
        });
    }
    SleelaString reverse() const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_reverse(value_.data(), value_.size(), o);
        });
    }
    SleelaString replace(const SleelaString &from, const SleelaString &to) const {
        return adopt([&](sleela_string_t *o) {
            return sleela_string_replace(value_.data(), value_.size(),
                                         from.value_.data(), from.value_.size(),
                                         to.value_.data(), to.value_.size(), o);
        });
    }

    /* ---- split ---- */
    std::vector<SleelaString> split(const SleelaString &sep) const {
        std::vector<SleelaString> parts;
        sleela_string_t *fields = nullptr;
        std::size_t n = 0;
        if (sleela_string_split(value_.data(), value_.size(),
                                sep.value_.data(), sep.value_.size(),
                                &fields, &n) == SLEELA_STRING_OK) {
            parts.reserve(n);
            for (std::size_t i = 0; i < n; ++i)
                parts.emplace_back(fields[i].data, fields[i].length);
            sleela_string_split_free(fields, n);
        }
        return parts;
    }

private:
    /* Run a C ABI producer, wrap its owned buffer as std::string, and free it. */
    template <typename Fn>
    static SleelaString adopt(Fn producer) {
        sleela_string_t raw{};
        SleelaString result;
        if (producer(&raw) == SLEELA_STRING_OK && raw.data)
            result.value_.assign(raw.data, raw.length);
        sleela_string_free(&raw);
        return result;
    }

    std::string value_;
};

/* ---- free-function orchestration (defined in src/sleela_string.cpp) ---- */

/* Join fields with a separator (inverse of SleelaString::split). */
SleelaString join(const std::vector<SleelaString> &parts, const SleelaString &sep);

/* Trim ends, then collapse interior whitespace runs to a single space. */
SleelaString normalizeWhitespace(const SleelaString &s);

} /* namespace text */
} /* namespace sleela */

#endif /* SLEELA_STRING_HPP */
