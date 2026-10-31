#include "utf4088.hpp"
#include "encoder_decoder.hpp"

namespace utf4088 {
namespace {
// Generic experimental namespace bound. This is not a Unicode scalar range.
constexpr CodePoint kMaxExperimentalCodePoint = 0x1FFFFFFFFULL;
}

bool is_valid_code_point(CodePoint value) {
    return value > 0x10FFFFULL && value <= kMaxExperimentalCodePoint;
}

// InputState now means the packed V1 digital input word. Conversion is
// implemented by the central encoder and uses a namespace disjoint from the
// 16,606-record front-end registry.
std::optional<CodePoint> symbol_from_input(InputState input) {
    if (input > UINT32_MAX) return std::nullopt;
    return encode_input(static_cast<std::uint32_t>(input));
}

} // namespace utf4088
