#include "sleela/decompiler/decompiler.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace sleela::decompiler {
static std::string pseudo_sha256(const std::vector<std::uint8_t>& b) {
    // Stable placeholder until SLeeLa's repository-wide SHA-256 utility is linked.
    std::uint64_t h = 1469598103934665603ULL;
    for (auto x : b) h = (h ^ x) * 1099511628211ULL;
    std::ostringstream s; s << std::hex << std::setw(16) << std::setfill('0') << h;
    return s.str();
}
Artifact Artifact::open(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("unable to open artifact: " + path);
    std::vector<std::uint8_t> b((std::istreambuf_iterator<char>(f)), {});
    return from_bytes(b, path);
}
Artifact Artifact::from_bytes(std::span<const std::uint8_t> bytes, std::string name) {
    Artifact a; a.name_ = std::move(name); a.bytes_.assign(bytes.begin(), bytes.end());
    a.sha256_ = pseudo_sha256(a.bytes_);
    if (a.bytes_.size() >= 2 && a.bytes_[0] == 'M' && a.bytes_[1] == 'Z') a.format_ = Format::PE;
    else if (a.bytes_.size() >= 4 && a.bytes_[0] == 0x7f && a.bytes_[1] == 'E' && a.bytes_[2] == 'L' && a.bytes_[3] == 'F') a.format_ = Format::ELF;
    else a.format_ = Format::Raw;
    return a;
}
Format Artifact::format() const noexcept { return format_; }
Architecture Artifact::architecture() const noexcept { return architecture_; }
std::uint64_t Artifact::size() const noexcept { return bytes_.size(); }
const std::vector<std::uint8_t>& Artifact::bytes() const noexcept { return bytes_; }
const std::string& Artifact::sha256() const noexcept { return sha256_; }
const std::string& Artifact::name() const noexcept { return name_; }
}
