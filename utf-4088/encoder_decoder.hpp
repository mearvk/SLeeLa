#pragma once

#include "utf4088.hpp"
#include <cstdint>
#include <optional>

namespace utf4088 {

// Version-1 input fields. All values are logical digital data, not raw hardware
// voltage or pressure measurements.
struct InputFields {
    std::uint8_t x{};          // 0..7
    std::uint8_t y{};          // 0..11
    std::uint8_t pressure{};   // 1..50
    std::uint8_t voltage{};    // normalized 0..255
};

// V1 reserves 0x110000..0x1140DD for the 16,606 front-end registry records.
// Encoded input-state identifiers occupy a separate, collision-free interval.
inline constexpr CodePoint kInputIdentifierBase = 0x200000ULL;
inline constexpr std::uint32_t kPackedInputMask = 0x001FFFFFU;

std::optional<std::uint32_t> pack_input(const InputFields& fields);
std::optional<InputFields> unpack_input(std::uint32_t packed);
std::optional<CodePoint> encode_input(const InputFields& fields);
std::optional<InputFields> decode_input(CodePoint identifier);
std::optional<CodePoint> symbol_from_input(InputState packed_input);

} // namespace utf4088
