#include "utf4088.hpp"

namespace utf4088 {
namespace {
// InputState and CodePoint are uint64_t. This implementation intentionally
// exposes only a 33-bit experimental range, not a literal 4088-bit integer.
constexpr CodePoint kMaxExperimentalCodePoint = 0x1FFFFFFFFULL;
}

bool is_valid_code_point(CodePoint value) {
    // Keep this namespace separate from Unicode scalar values. Values above
    // Unicode's maximum are experimental identifiers, not Unicode characters.
    return value > 0x10FFFFULL && value <= kMaxExperimentalCodePoint;
}

std::optional<CodePoint> symbol_from_input(InputState input) {
    // The driver accepts documented digital values only. Electrical sampling
    // belongs in a hardware adapter with explicit voltage/safety constraints.
    if (!is_valid_code_point(input)) return std::nullopt;
    return static_cast<CodePoint>(input);
}

} // namespace utf4088
