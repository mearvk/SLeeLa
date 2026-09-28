#pragma once

#include <cstdint>

#include "pixel_terminal.hpp"

namespace sleela::runtime::terminal {

enum class Variant : std::uint8_t {
    None = 0,
    Cell = 1,
    NativePixel = 2
};

struct Capability {
    std::uint16_t bash_protocol_major = 0;
    std::uint16_t phraign_version = 0;
    Variant variant = Variant::None;
    bool ready = false;
    bool pixel_granularity = false;
    bool native_frame = false;
    bool resize_events = false;

    static Capability unavailable() noexcept;
    static Capability fromNativePixel() noexcept;
    bool usable() const noexcept;
};

} // namespace sleela::runtime::terminal
