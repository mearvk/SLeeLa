#ifndef SLEELA_UNSIGNED_INTEGER_H
#define SLEELA_UNSIGNED_INTEGER_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace sleela {

/*
 * Width-parameterized unsigned integer value foundation for compiler/runtime integration.
 * Bytes are little-endian. Every value is normalized to exactly ceil(width/8)
 * bytes and unused high bits are always cleared.
 */
class UnsignedInteger {
public:
    static constexpr std::size_t kMaximumWidth = 1048576;

    static bool isValidTypeName(const std::string& name) noexcept {
        if (name.size() < 2 || name[0] != 'U' || (name.size() > 2 && name[1] == '0')) return false;
        std::size_t width = 0;
        for (std::size_t i = 1; i < name.size(); ++i) {
            if (name[i] < '0' || name[i] > '9') return false;
            const unsigned digit = static_cast<unsigned>(name[i] - '0');
            if (width > 104857 || (width == 104857 && digit > 6)) return false;
            width = width * 10 + digit;
        }
        return width >= 1 && width <= kMaximumWidth;
    }

    explicit UnsignedInteger(std::size_t width, std::uint8_t fill = 0)
        : width_(width), bytes_(byteCount(width), fill) {
        normalize();
    }

    static UnsignedInteger fromDecimal(std::size_t width, const std::string& text) {
        if (text.empty()) throw std::invalid_argument("empty unsigned integer");
        UnsignedInteger value(width);
        for (char c : text) {
            if (c < '0' || c > '9') throw std::invalid_argument("invalid unsigned decimal");
            value.multiplySmall(10);
            value.addSmall(static_cast<unsigned>(c - '0'));
        }
        return value;
    }

    static UnsignedInteger fromBytes(std::size_t width, const std::vector<std::uint8_t>& bytes) {
        UnsignedInteger value(width);
        if (bytes.size() != value.bytes_.size())
            throw std::invalid_argument("serialized unsigned integer has incorrect byte length");
        value.bytes_ = bytes;
        if (value.exceedsWidth())
            throw std::out_of_range("serialized unsigned integer exceeds declared width");
        return value;
    }

    std::size_t width() const noexcept { return width_; }
    const std::vector<std::uint8_t>& bytes() const noexcept { return bytes_; }
    std::vector<std::uint8_t> toBytes() const { return bytes_; }

    bool isZero() const noexcept {
        return std::all_of(bytes_.begin(), bytes_.end(), [](std::uint8_t b) { return b == 0; });
    }

    int compare(const UnsignedInteger& other) const {
        requireSameWidth(other);
        for (std::size_t i = bytes_.size(); i-- > 0;) {
            if (bytes_[i] < other.bytes_[i]) return -1;
            if (bytes_[i] > other.bytes_[i]) return 1;
        }
        return 0;
    }

    UnsignedInteger add(const UnsignedInteger& other) const {
        requireSameWidth(other);
        if (addWouldOverflow(other)) throw std::overflow_error("unsigned integer addition overflow");
        UnsignedInteger out(width_);
        unsigned carry = 0;
        for (std::size_t i = 0; i < bytes_.size(); ++i) {
            unsigned sum = static_cast<unsigned>(bytes_[i]) + other.bytes_[i] + carry;
            out.bytes_[i] = static_cast<std::uint8_t>(sum & 0xffu);
            carry = sum >> 8;
        }
        out.normalize();
        return out;
    }

    // Subtraction is checked: unsigned underflow is an error, never wraparound.
    UnsignedInteger subtract(const UnsignedInteger& other) const {
        requireSameWidth(other);
        if (compare(other) < 0) throw std::underflow_error("unsigned integer underflow");
        UnsignedInteger out(width_);
        int borrow = 0;
        for (std::size_t i = 0; i < bytes_.size(); ++i) {
            int d = static_cast<int>(bytes_[i]) - static_cast<int>(other.bytes_[i]) - borrow;
            if (d < 0) { d += 256; borrow = 1; } else borrow = 0;
            out.bytes_[i] = static_cast<std::uint8_t>(d);
        }
        out.normalize();
        return out;
    }

    bool addWouldOverflow(const UnsignedInteger& other) const {
        requireSameWidth(other);
        unsigned carry = 0;
        for (std::size_t i = 0; i < bytes_.size(); ++i) {
            unsigned sum = static_cast<unsigned>(bytes_[i]) + other.bytes_[i] + carry;
            carry = sum >> 8;
            if (i + 1 == bytes_.size() && (width_ % 8) != 0) {
                const unsigned mask = (1u << (width_ % 8)) - 1u;
                if ((sum & ~mask) != 0) return true;
            }
        }
        return carry != 0;
    }

    std::string toDecimal() const {
        if (isZero()) return "0";
        std::vector<std::uint8_t> work = bytes_;
        std::string result;
        while (std::any_of(work.begin(), work.end(), [](std::uint8_t b) { return b != 0; })) {
            unsigned remainder = 0;
            for (std::size_t i = work.size(); i-- > 0;) {
                unsigned current = (remainder << 8) | work[i];
                work[i] = static_cast<std::uint8_t>(current / 10);
                remainder = current % 10;
            }
            result.push_back(static_cast<char>('0' + remainder));
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    static std::size_t byteCount(std::size_t width) {
        if (width == 0 || width > kMaximumWidth)
            throw std::out_of_range("unsigned integer width must be in 1..1048576");
        return (width + 7) / 8;
    }

    void requireSameWidth(const UnsignedInteger& other) const {
        if (width_ != other.width_) throw std::invalid_argument("unsigned integer width mismatch");
    }

    void normalize() noexcept {
        if (bytes_.empty() || width_ % 8 == 0) return;
        bytes_.back() &= static_cast<std::uint8_t>((1u << (width_ % 8)) - 1u);
    }

    void multiplySmall(unsigned factor) {
        unsigned carry = 0;
        for (std::size_t i = 0; i < bytes_.size(); ++i) {
            unsigned product = static_cast<unsigned>(bytes_[i]) * factor + carry;
            bytes_[i] = static_cast<std::uint8_t>(product & 0xffu);
            carry = product >> 8;
        }
        if (carry != 0 || exceedsWidth()) throw std::out_of_range("unsigned integer value exceeds declared width");
        normalize();
    }

    void addSmall(unsigned value) {
        unsigned carry = value;
        for (std::size_t i = 0; i < bytes_.size() && carry; ++i) {
            unsigned sum = static_cast<unsigned>(bytes_[i]) + carry;
            bytes_[i] = static_cast<std::uint8_t>(sum & 0xffu);
            carry = sum >> 8;
        }
        if (carry != 0 || exceedsWidth()) throw std::out_of_range("unsigned integer value exceeds declared width");
        normalize();
    }

    bool exceedsWidth() const noexcept {
        if (width_ % 8 == 0) return false;
        const unsigned mask = (1u << (width_ % 8)) - 1u;
        return (static_cast<unsigned>(bytes_.back()) & ~mask) != 0;
    }

    std::size_t width_;
    std::vector<std::uint8_t> bytes_;
};

} // namespace sleela
#endif
