/*
 * Sleelavirin(TM) C++ convenience library - implementation.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Thin, exception-safe C++ wrapper over the portable C core (sleelavirin_scan.c
 * / sleelavirin_native.c). It owns no crypto or matching logic of its own; it
 * delegates to the C functions so the C and C++ APIs always agree by
 * construction. Original Sleelavirin code; no third-party AV engine is used.
 */
#include "sleelavirin_cpp.hpp"

#include <cstring>
#include <fstream>
#include <vector>

#include "sleelavirin_native.h"

namespace sleelavirin {

namespace {

MatchKind kind_from_c(sleelavirin_match_kind_t k)
{
    switch (k) {
        case SLEELAVIRIN_CLEAN:       return MatchKind::kClean;
        case SLEELAVIRIN_EXACT_MATCH: return MatchKind::kExactMatch;
        case SLEELAVIRIN_SEMI_MATCH:  return MatchKind::kSemiMatch;
        case SLEELAVIRIN_SUSPICIOUS:  return MatchKind::kSuspicious;
        case SLEELAVIRIN_UNKNOWN:     return MatchKind::kUnknown;
        case SLEELAVIRIN_SCAN_ERROR:  return MatchKind::kScanError;
        case SLEELAVIRIN_UNSUPPORTED: return MatchKind::kUnsupported;
    }
    return MatchKind::kUnknown;
}

}  // namespace

std::string sha256_to_hex(const std::array<uint8_t, 32> &digest)
{
    static const char hex[] = "0123456789abcdef";
    std::string out;
    out.resize(64);
    for (size_t i = 0; i < 32; ++i) {
        out[2 * i] = hex[digest[i] >> 4];
        out[2 * i + 1] = hex[digest[i] & 0x0F];
    }
    return out;
}

std::optional<std::array<uint8_t, 32>> hex_to_sha256(const std::string &hex)
{
    if (hex.size() != 64) return std::nullopt;
    std::array<uint8_t, 32> out{};
    auto nib = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    for (size_t i = 0; i < 32; ++i) {
        int hi = nib(hex[2 * i]);
        int lo = nib(hex[2 * i + 1]);
        if (hi < 0 || lo < 0) return std::nullopt;
        out[i] = static_cast<uint8_t>((hi << 4) | lo);
    }
    return out;
}

std::string FileFingerprint::to_hex() const
{
    return sha256_to_hex(sha256);
}

std::optional<FileFingerprint> FileFingerprint::from_file(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)),
                              std::istreambuf_iterator<char>());
    if (in.bad()) return std::nullopt;

    sleelavirin_file_fingerprint_t c{};
    if (sleelavirin_fingerprint_buffer(data.data(), data.size(), &c) != 0)
        return std::nullopt;

    FileFingerprint fp;
    std::memcpy(fp.sha256.data(), c.sha256, 32);
    fp.feature_fp = c.feature_fp;
    fp.feature_mask = c.feature_mask;
    fp.file_size = c.file_size;
    return fp;
}

sleelavirin_known_file_t KnownFile::to_c() const
{
    sleelavirin_known_file_t k{};
    std::snprintf(k.name, sizeof(k.name), "%s", name.c_str());
    std::snprintf(k.provider, sizeof(k.provider), "%s", provider.c_str());
    std::snprintf(k.rule_id, sizeof(k.rule_id), "%s", rule_id.c_str());
    std::memcpy(k.fingerprint.sha256, fingerprint.sha256.data(), 32);
    k.fingerprint.feature_fp = fingerprint.feature_fp;
    k.fingerprint.feature_mask = fingerprint.feature_mask;
    k.fingerprint.file_size = fingerprint.file_size;
    k.severity = severity;
    return k;
}

HashMap::HashMap(size_t capacity)
    : map_(sleelavirin_hashmap_create(capacity))
{
}

HashMap::~HashMap()
{
    sleelavirin_hashmap_destroy(map_);
}

HashMap::HashMap(HashMap &&other) noexcept : map_(other.map_)
{
    other.map_ = nullptr;
}

HashMap &HashMap::operator=(HashMap &&other) noexcept
{
    if (this != &other) {
        sleelavirin_hashmap_destroy(map_);
        map_ = other.map_;
        other.map_ = nullptr;
    }
    return *this;
}

bool HashMap::add(const KnownFile &known)
{
    if (!map_) return false;
    sleelavirin_known_file_t c = known.to_c();
    return sleelavirin_hashmap_add(map_, &c) == 0;
}

std::optional<Match> HashMap::scan(const FileFingerprint &file,
                                   uint32_t max_feature_distance) const
{
    if (!map_) return std::nullopt;
    sleelavirin_file_fingerprint_t c{};
    std::memcpy(c.sha256, file.sha256.data(), 32);
    c.feature_fp = file.feature_fp;
    c.feature_mask = file.feature_mask;
    c.file_size = file.file_size;

    sleelavirin_match_t out{};
    if (sleelavirin_hashmap_scan(map_, &c, max_feature_distance, &out) != 0)
        return std::nullopt;

    Match m;
    m.kind = kind_from_c(out.kind);
    m.confidence = out.confidence;
    m.distance_bits = out.distance_bits;
    if (out.known) {
        m.known_name = out.known->name;
        m.provider = out.known->provider;
        m.rule_id = out.known->rule_id;
    }
    return m;
}

}  // namespace sleelavirin
