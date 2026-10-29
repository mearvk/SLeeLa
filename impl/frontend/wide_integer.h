#ifndef SLEELA_WIDE_INTEGER_H
#define SLEELA_WIDE_INTEGER_H

#include <cstddef>
#include <string>

namespace sleela {

/*
 * SignedWideInteger -- recognizer for the extended wide *signed* integer
 * primitive type names introduced in syntax 2.7:
 *
 *     int256  int512  int1024  int2048  ...  int16777216
 *
 * The family is the type name "int" immediately followed by a decimal bit
 * width that is a power of two in the inclusive range 2^8 (256) .. 2^24
 * (16777216). These names are NOT lexer keywords: like the arbitrary-width
 * unsigned `U<width>` family (see unsigned_integer.h) they lex as ordinary
 * identifiers and are recognized here by the front end wherever a primitive
 * type may appear. The three narrower signed primitives `int`/`long`/`int64`
 * (64-bit) and `int128` (128-bit) stay keyword-spelled and are handled
 * separately; this recognizer deliberately rejects widths <= 128 so it never
 * shadows them.
 *
 * NOTE on arithmetic backing: like `int128`, these names type-check as integers
 * and participate in integer arithmetic. The VM carries their values in the
 * widest native integer slot it has (SL_INT128 -- a true 128-bit `__int128` on
 * GCC/Clang); arithmetic beyond 128 bits is a documented follow-up. The names
 * are accepted by the compiler and handled by the native VM today so that
 * declarations, stores, parameters, returns, casts and arrays all work.
 */
class SignedWideInteger {
public:
    // Smallest and largest declared bit widths of the family (powers of two).
    static constexpr std::size_t kMinimumWidth = 256;        // 2^8
    static constexpr std::size_t kMaximumWidth = 16777216;   // 2^24

    // True when `name` is exactly "int" + <decimal width>, the width is a power
    // of two, and kMinimumWidth <= width <= kMaximumWidth.
    static bool isValidTypeName(const std::string& name) noexcept {
        std::size_t width = 0;
        if (!parseWidth(name, width)) return false;
        if (width < kMinimumWidth || width > kMaximumWidth) return false;
        return isPowerOfTwo(width);
    }

    // Parse the declared bit width out of a valid name (0 when `name` is not a
    // member of the family). Useful for callers that need the width directly.
    static std::size_t widthOf(const std::string& name) noexcept {
        std::size_t width = 0;
        if (!parseWidth(name, width)) return 0;
        if (width < kMinimumWidth || width > kMaximumWidth || !isPowerOfTwo(width)) return 0;
        return width;
    }

private:
    // Match "int" followed by a non-empty run of decimal digits with no leading
    // zero. Fills `width` with the decimal value; returns false on any other
    // shape (overflow-guarded against absurd input).
    static bool parseWidth(const std::string& name, std::size_t& width) noexcept {
        const char prefix[] = "int";
        const std::size_t plen = 3;
        if (name.size() <= plen) return false;
        for (std::size_t i = 0; i < plen; ++i)
            if (name[i] != prefix[i]) return false;
        if (name[plen] == '0') return false;  // no leading zero / bare "int0"
        std::size_t w = 0;
        for (std::size_t i = plen; i < name.size(); ++i) {
            const char c = name[i];
            if (c < '0' || c > '9') return false;
            if (w > (kMaximumWidth / 10) + 1) return false;  // overflow guard
            w = w * 10 + static_cast<std::size_t>(c - '0');
        }
        width = w;
        return true;
    }

    static bool isPowerOfTwo(std::size_t n) noexcept {
        return n != 0 && (n & (n - 1)) == 0;
    }
};

}  // namespace sleela
#endif  // SLEELA_WIDE_INTEGER_H
