#include "terminal_capability.hpp"

namespace sleela::runtime::terminal {

Capability Capability::unavailable() noexcept {
    return Capability{};
}

Capability Capability::fromNativePixel() noexcept {
    const auto native = sleela::terminal::PixelTerminal::handshake();

    Capability result;
    result.bash_protocol_major = static_cast<std::uint16_t>(native.bash_protocol_major);
    result.phraign_version = static_cast<std::uint16_t>(native.phraign_version);
    result.variant = Variant::NativePixel;
    result.ready = native.ready;
    result.pixel_granularity = native.pixel_granularity;
    result.native_frame = native.native_frame;
    result.resize_events = native.resize_events;
    return result;
}

bool Capability::usable() const noexcept {
    return ready && pixel_granularity && native_frame;
}

} // namespace sleela::runtime::terminal
