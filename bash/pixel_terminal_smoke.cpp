#include "pixel_terminal.hpp"

#include <cassert>
#include <iostream>

int main() {
    using sleela::terminal::Handshake;
    using sleela::terminal::Pixel;
    using sleela::terminal::PixelTerminal;
    using sleela::terminal::Size;

    const Handshake handshake = PixelTerminal::handshake();
    assert(handshake.ready);
    assert(handshake.pixel_granularity);
    assert(handshake.native_frame);
    assert(handshake.resize_events);

    PixelTerminal terminal(Size{8, 4});
    assert((terminal.pixelSize() == Size{8, 4}));
    assert(terminal.center().x == 4);
    assert(terminal.center().y == 2);

    assert(terminal.setPixel(3, 2, Pixel{0x11223344u}));
    assert(!terminal.setPixel(8, 2, Pixel{0x55667788u}));
    assert(terminal.frame().at(2 * 8 + 3).value == 0x11223344u);

    assert(terminal.fill(Pixel{0xaabbccddu}));
    assert(terminal.frame().front().value == 0xaabbccddu);

    terminal.clear();
    assert(terminal.frame().front().value == 0u);

    assert((terminal.setSize(Size{2, 2})));
    assert(terminal.frame().size() == 4);
    assert(terminal.events().size() >= 2);

    std::cout << "native pixel terminal smoke: OK\n";
    return 0;
}
