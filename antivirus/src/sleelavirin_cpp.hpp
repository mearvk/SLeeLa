#ifndef SLEELAVIRIN_CPP_HPP
#define SLEELAVIRIN_CPP_HPP

#include <array>
#include <optional>
#include <string>

#include "sleelavirin_scan.h"

namespace sleelavirin {

enum class MatchKind {
    kClean,
    kExactMatch,
    kSemiMatch,
    kSuspicious,
    kUnknown,
    kScanError,
    kUnsupported
};

struct FileFingerprint {
    std::array<uint8_t, 32> sha256{};
    uint64_t feature_fp = 0;
    uint64_t feature_mask = 0;
    uint64_t file_size = 0;

    static std::optional<FileFingerprint> from_file(const std::string &path);
    std::string to_hex() const;
};

struct KnownFile {
    std::string name;
    std::string provider;
    std::string rule_id;
    FileFingerprint fingerprint{};
    uint8_t severity = 0;

    sleelavirin_known_file_t to_c() const;
};

struct Match {
    MatchKind kind = MatchKind::kUnknown;
    uint8_t confidence = 0;
    uint32_t distance_bits = 0;
    std::string known_name;
    std::string provider;
    std::string rule_id;
};

class HashMap {
public:
    explicit HashMap(size_t capacity = 128);
    ~HashMap();

    HashMap(const HashMap &) = delete;
    HashMap &operator=(const HashMap &) = delete;
    HashMap(HashMap &&other) noexcept;
    HashMap &operator=(HashMap &&other) noexcept;

    bool add(const KnownFile &known);
    std::optional<Match> scan(const FileFingerprint &file,
                             uint32_t max_feature_distance = 64) const;

private:
    sleelavirin_hashmap_t *map_ = nullptr;
};

std::string sha256_to_hex(const std::array<uint8_t, 32> &digest);
std::optional<std::array<uint8_t, 32>> hex_to_sha256(const std::string &hex);

}  // namespace sleelavirin

#endif
