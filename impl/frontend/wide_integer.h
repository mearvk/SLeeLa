#ifndef SLEELA_WIDE_INTEGER_H
#define SLEELA_WIDE_INTEGER_H

#include <cstddef>
#include <string>

namespace sleela {

/*
 * SignedWideInteger -- recognizer for the base-2 series of named *signed*
 * integer primitive type names (syntax 2.7):
 *
 *     int32  int64  int128  int256  int512  int1024  ...  int16777216
 *
 * The family is the type name "int" immediately followed by a decimal bit
 * width that is a power of two in the inclusive range 2^5 (32) .. 2^24
 * (16777216) -- the full binary base-2 series of widths: 32, 64, 128, 256,
 * 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
 * 1048576, 2097152, 4194304, 8388608, 16777216 (twenty widths, 2^5..2^24).
 *
 * These names are NOT lexer keywords: like the arbitrary-width unsigned
 * `U<width>` family (see unsigned_integer.h) they lex as ordinary identifiers
 * and are recognized here by the front end wherever a primitive type may
 * appear. Two series members, `int64` and `int128`, are ALSO spelled as lexer
 * keywords for source convenience (and `int`/`long` are 64-bit too); the
 * keyword and pattern paths are deliberately kept consistent -- a width is
 * routed to the same runtime backing however it was spelled.
 *
 * RUNTIME BACKING. The native VM carries each value in the narrowest native
 * integer slot that holds its declared width:
 *   - widths <= 64 bits  (int32, int64)       -> SL_INT    (64-bit int64_t)
 *   - widths >= 128 bits (int128 .. 2^24)      -> SL_INT128 (true 128-bit
 *                                                 __int128 on GCC/Clang)
 * Arithmetic exact up to 128 bits today; true arbitrary-width arithmetic beyond
 * 128 bits is a documented follow-up. The compiler and VM accept and handle
 * every member so declarations, stores, parameters, returns, casts and arrays
 * all work across the whole series.
 */
class SignedWideInteger {
public:
    // Smallest and largest declared bit widths of the series (powers of two).
    static constexpr std::size_t kMinimumWidth = 32;         // 2^5
    static constexpr std::size_t kMaximumWidth = 16777216;   // 2^24
    // Widths at or below this ride the 64-bit native slot; wider values use the
    // 128-bit slot. (Matches the VM's SL_INT / SL_INT128 split.)
    static constexpr std::size_t kNativeNarrowWidth = 64;

    // True when `name` is exactly "int" + <decimal width>, the width is a power
    // of two, and kMinimumWidth <= width <= kMaximumWidth.
    static bool isValidTypeName(const std::string& name) noexcept {
        return widthOf(name) != 0;
    }

    // Parse the declared bit width out of a valid name (0 when `name` is not a
    // member of the series). Useful for callers that need the width directly.
    static std::size_t widthOf(const std::string& name) noexcept {
        std::size_t width = 0;
        if (!parseWidth(name, width)) return 0;
        if (width < kMinimumWidth || width > kMaximumWidth || !isPowerOfTwo(width)) return 0;
        return width;
    }

    // True when a valid family member fits the VM's 64-bit native integer slot
    // (int32, int64). Wider members (int128 and up) use the 128-bit slot.
    static bool fitsNativeNarrow(const std::string& name) noexcept {
        const std::size_t w = widthOf(name);
        return w != 0 && w <= kNativeNarrowWidth;
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

/*
 * UnsignedWideInteger -- recognizer for the base-2 series of named *unsigned*
 * (positive-only) integer primitive type names (syntax 2.8):
 *
 *     uint32  uint64  uint128  uint256  uint512  uint1024  ...  uint16777216
 *
 * The exact unsigned mirror of SignedWideInteger: the type name "uint"
 * immediately followed by a decimal bit width that is a power of two in the
 * inclusive range 2^5 (32) .. 2^24 (16777216) -- the same twenty widths. The
 * names lex as ordinary identifiers (no keyword spelling) and are recognized by
 * the front end wherever a primitive type may appear.
 *
 * POSITIVE-ONLY semantics. A uint<width> value type-checks as the unsigned kind
 * (Kind::Unsigned), so unary minus is rejected and arithmetic / comparison
 * require operands of the same unsigned width -- the same positive-only,
 * width-strict discipline as the arbitrary-width U<width> family.
 *
 * RUNTIME BACKING. Identical to the signed series: widths <= 64 (uint32,
 * uint64) ride the 64-bit native slot (SL_INT); widths >= 128 (uint128 and up)
 * ride the 128-bit slot (SL_INT128, true __int128 on GCC/Clang). Exact up to
 * 128 bits; true arbitrary-width arithmetic beyond 128 bits is a documented
 * follow-up.
 */
class UnsignedWideInteger {
public:
    static constexpr std::size_t kMinimumWidth = 32;         // 2^5
    static constexpr std::size_t kMaximumWidth = 16777216;   // 2^24
    static constexpr std::size_t kNativeNarrowWidth = 64;

    static bool isValidTypeName(const std::string& name) noexcept {
        return widthOf(name) != 0;
    }

    static std::size_t widthOf(const std::string& name) noexcept {
        std::size_t width = 0;
        if (!parseWidth(name, width)) return 0;
        if (width < kMinimumWidth || width > kMaximumWidth || !isPowerOfTwo(width)) return 0;
        return width;
    }

    static bool fitsNativeNarrow(const std::string& name) noexcept {
        const std::size_t w = widthOf(name);
        return w != 0 && w <= kNativeNarrowWidth;
    }

private:
    // Match "uint" followed by a non-empty run of decimal digits (no leading
    // zero). Fills `width`; returns false on any other shape (overflow-guarded).
    static bool parseWidth(const std::string& name, std::size_t& width) noexcept {
        const char prefix[] = "uint";
        const std::size_t plen = 4;
        if (name.size() <= plen) return false;
        for (std::size_t i = 0; i < plen; ++i)
            if (name[i] != prefix[i]) return false;
        if (name[plen] == '0') return false;  // no leading zero / bare "uint0"
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
