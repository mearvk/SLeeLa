#pragma once

// Phraign(TM) City 3D -- renderer.
//
// Projects the City block grid into an oblique/dimetric view (seen from the top
// and slightly to the side) and draws it onto a Phraign native pixel frame
// (sleela::terminal::PixelTerminal), one pixel at a time, using a Palette
// derived from the chosen Theme.

#include "city_model.hpp"
#include "cityscape_model.hpp"

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

    // Render the city plus the general cityscape graph: the mating cylinder
    // pairs (columns in the 3rd dimension), the spheres of moral symmetry they
    // grace, and the noted neighbor links. Returns pixels written.
    std::size_t render(const City& city, const CityscapeModel& model,
                       sleela::terminal::PixelTerminal& terminal) const;

    const RenderOptions& options() const noexcept { return options_; }

private:
    RenderOptions options_;
};

} // namespace sleela::city
