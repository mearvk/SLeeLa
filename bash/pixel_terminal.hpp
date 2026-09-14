#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::terminal {

struct Pixel {
    std::uint32_t value = 0;
};

struct Size {
    std::size_t width = 0;
    std::size_t height = 0;

    bool operator==(const Size& other) const noexcept {
        return width == other.width && height == other.height;
    }

    bool operator!=(const Size& other) const noexcept {
        return !(*this == other);
    }
};

struct Point {
    std::size_t x = 0;
    std::size_t y = 0;
};

enum class SizeEventType {
    Initial,
    Resize,
    PixelSizeChanged,
    CellSizeChanged
};

struct SizeEvent {
    SizeEventType type = SizeEventType::Initial;
    Size pixel_size{};
    Size cell_size{};
    Point center{};
};

struct Handshake {
    static constexpr unsigned int bash_protocol_major = 1;
    static constexpr unsigned int pixel_terminal_version = 1;
    static constexpr const char* variant = "NATIVE";

    bool ready = false;
    bool pixel_granularity = false;
    bool native_frame = false;
    bool resize_events = false;
};

class PixelTerminal {
public:
    PixelTerminal();
    explicit PixelTerminal(Size size);
    ~PixelTerminal();

    PixelTerminal(const PixelTerminal&) = delete;
    PixelTerminal& operator=(const PixelTerminal&) = delete;

    // Returns the capability contract SLeeLa expects from the Bash pixel
    // endpoint. The bridge should complete its Bash HELLO/READY exchange
    // before sending frame commands.
    static Handshake handshake() noexcept;

    bool querySize();
    bool setSize(Size size);

    Size pixelSize() const noexcept { return pixel_size_; }
    Size cellSize() const noexcept { return cell_size_; }
    Point center() const noexcept { return center_; }

    bool setPixel(std::size_t x, std::size_t y, Pixel pixel);
    bool fill(Pixel pixel);
    void clear();
    bool show();

    bool begin();
    void end();

    void watchResize();
    void unwatchResize();
    bool pollResize();

    const std::vector<Pixel>& frame() const noexcept { return frame_; }
    const std::vector<SizeEvent>& events() const noexcept { return events_; }

private:
    void rebuildFrame();
    void updateCenter();
    void recordEvent(SizeEventType type);
    bool queryCellSize();
    bool queryPixelSize();

    Size pixel_size_{};
    Size cell_size_{};
    Point center_{};
    std::vector<Pixel> frame_{};
    std::vector<SizeEvent> events_{};
    bool watching_resize_ = false;
    bool active_ = false;
};

} // namespace sleela::terminal
