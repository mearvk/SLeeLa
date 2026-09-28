#include "sleela/decompiler/archive.hpp"
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

namespace sleela::decompiler {
namespace {
std::uint64_t decimal_field(const std::uint8_t* p, std::size_t n) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n; ++i)
        if (p[i] >= '0' && p[i] <= '9') v = v * 10 + (p[i] - '0');
    return v;
}
std::string trim_name(std::string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '/')) s.pop_back();
    return s;
}
}
void analyze_archive(std::span<const std::uint8_t> in, NativeInterfaces& out) {
    std::vector<std::uint8_t> b(in.begin(), in.end());
    if (b.size() < 8 || std::memcmp(b.data(), "!<arch>\n", 8) != 0) return;
    std::uint64_t p = 8;
    std::string longnames;
    while (p + 60 <= b.size()) {
        const auto header = p;
        const auto size = decimal_field(b.data() + p + 48, 10);
        if (b[p + 58] != '`' || b[p + 59] != '\n' || p + 60 + size > b.size()) break;
        const auto data = p + 60;
        std::string name = trim_name(std::string(reinterpret_cast<const char*>(b.data() + p), 16));
        if (name == "//") {
            longnames.assign(reinterpret_cast<const char*>(b.data() + data), static_cast<std::size_t>(size));
        } else if (name != "/") {
            if (name.size() > 1 && name[0] == '/') {
                std::size_t q = 1;
                while (q < name.size() && std::isdigit(static_cast<unsigned char>(name[q]))) ++q;
                if (q > 1 && q == name.size() && !longnames.empty()) {
                    const auto off = std::stoull(name.substr(1));
                    if (off < longnames.size()) {
                        auto e = longnames.find("/\n", off);
                        if (e == std::string::npos) e = longnames.find('\n', off);
                        name = longnames.substr(off, e == std::string::npos ? longnames.size() - off : e - off);
                    }
                }
            }
            Format f = Format::Raw;
            if (size >= 4 && b[data] == 0x7f && b[data + 1] == 'E' && b[data + 2] == 'L' && b[data + 3] == 'F') f = Format::ELF;
            else if (size >= 2 && b[data] == 'M' && b[data + 1] == 'Z') f = Format::PE;
            out.archive_members.push_back({name, header, data, size, f});
        }
        p = data + size + (size & 1u);
    }
}
}
