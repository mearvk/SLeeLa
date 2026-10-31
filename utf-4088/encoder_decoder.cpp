#include "encoder_decoder.hpp"

namespace utf4088 {

std::optional<std::uint32_t> pack_input(const InputFields& f) {
    if (f.x > 7 || f.y > 11 || f.pressure < 1 || f.pressure > 50) {
        return std::nullopt;
    }
    return static_cast<std::uint32_t>(f.x)
        | (static_cast<std::uint32_t>(f.y) << 3)
        | (static_cast<std::uint32_t>(f.pressure - 1) << 7)
        | (static_cast<std::uint32_t>(f.voltage) << 13);
}

std::optional<InputFields> unpack_input(std::uint32_t packed) {
    if ((packed & ~kPackedInputMask) != 0) return std::nullopt;
    InputFields f{
        static_cast<std::uint8_t>(packed & 0x7U),
        static_cast<std::uint8_t>((packed >> 3) & 0xFU),
        static_cast<std::uint8_t>(((packed >> 7) & 0x3FU) + 1U),
        static_cast<std::uint8_t>((packed >> 13) & 0xFFU)
    };
    if (f.y > 11 || f.pressure > 50) return std::nullopt;
    return f;
}

std::optional<CodePoint> encode_input(const InputFields& fields) {
    const auto packed = pack_input(fields);
    if (!packed) return std::nullopt;
    return kInputIdentifierBase + *packed;
}

std::optional<InputFields> decode_input(CodePoint identifier) {
    if (identifier < kInputIdentifierBase) return std::nullopt;
    const auto delta = identifier - kInputIdentifierBase;
    if (delta > kPackedInputMask) return std::nullopt;
    return unpack_input(static_cast<std::uint32_t>(delta));
}

std::optional<CodePoint> symbol_from_input(InputState packed_input) {
    if (packed_input > kPackedInputMask) return std::nullopt;
    const auto fields = unpack_input(static_cast<std::uint32_t>(packed_input));
    if (!fields) return std::nullopt;
    return encode_input(*fields);
}

} // namespace utf4088
