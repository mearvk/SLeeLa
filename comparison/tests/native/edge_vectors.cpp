#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

static int utf8_codepoints(const std::string& s) {
    std::size_t i = 0;
    int count = 0;
    while (i < s.size()) {
        const auto c = static_cast<unsigned char>(s[i]);
        std::size_t width;
        if (c <= 0x7f) width = 1;
        else if (c >= 0xc2 && c <= 0xdf) width = 2;
        else if (c >= 0xe0 && c <= 0xef) width = 3;
        else if (c >= 0xf0 && c <= 0xf4) width = 4;
        else return -1;
        if (i + width > s.size()) return -1;
        for (std::size_t j = 1; j < width; ++j) {
            if ((static_cast<unsigned char>(s[i + j]) & 0xc0) != 0x80) return -1;
        }
        i += width;
        ++count;
    }
    return count;
}

int main() {
    const std::int32_t min = std::numeric_limits<std::int32_t>::min();
    const std::int32_t max = std::numeric_limits<std::int32_t>::max();
    std::uint32_t wrapped = std::numeric_limits<std::uint32_t>::max();
    const std::string text = u8"Aé😀";

    wrapped += UINT32_C(1);
    std::cout << "INT32_MIN=" << min << '\n';
    std::cout << "INT32_MAX=" << max << '\n';
    std::cout << "U32_WRAP=" << wrapped << '\n';
    std::cout << "SIGNED_DIV=" << (-7 / 3) << '\n';
    std::cout << "SIGNED_REM=" << (-7 % 3) << '\n';
    std::cout << "UTF8_BYTES=" << text.size() << '\n';
    std::cout << "UNICODE_CODEPOINTS=" << utf8_codepoints(text) << '\n';
    return 0;
}
