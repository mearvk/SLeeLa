#pragma once

// Phraign(TM) City 3D -- renderer.
//
// Projects the City block grid into an oblique/dimetric view (seen from the top
// and slightly to the side) and draws it onto a Phraign native pixel frame
// (sleela::terminal::PixelTerminal), one pixel at a time, using a Palette
// derived from the chosen Theme.

#include "city_model.hpp"

// Phraign native pixel-terminal layer (sibling in bash/).
#include "pixel_terminal.hpp"

namespace sleela::city {

class Renderer {
public:
    explicit Renderer(const RenderOptions& options) : options_(options) {}

    // Render the city into the given Phraign terminal's frame. The terminal is
    // sized to the configured frame dimensions. Returns the number of pixels
    // written (roofs + walls + ground), useful for smoke checks.
    std::size_t render(const City& city,
                       sleela::terminal::PixelTerminal& terminal) const;

    const RenderOptions& options() const noexcept { return options_; }

private:
    RenderOptions options_;
};

} // namespace sleela::city
