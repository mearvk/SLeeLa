#include "pixel_terminal.hpp"

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace {
volatile std::sig_atomic_t resize_pending = 0;

void on_resize(int) noexcept {
    resize_pending = 1;
}

std::size_t safe_area(std::size_t width, std::size_t height) {
    if (width == 0 || height == 0) return 0;
    return width > static_cast<std::size_t>(-1) / height ? 0 : width * height;
}
}

namespace sleela::terminal {

PixelTerminal::PixelTerminal() {
    querySize();
}

PixelTerminal::PixelTerminal(Size size) : pixel_size_(size) {
    updateCenter();
    rebuildFrame();
    recordEvent(SizeEventType::Initial);
}

PixelTerminal::~PixelTerminal() {
    unwatchResize();
}

bool PixelTerminal::queryCellSize() {
    winsize ws{};
    if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_col == 0 || ws.ws_row == 0) {
        return false;
    }

    const Size next{ws.ws_col, ws.ws_row};
    if (next != cell_size_) {
        cell_size_ = next;
        recordEvent(SizeEventType::CellSizeChanged);
    }
    return true;
}

bool PixelTerminal::queryPixelSize() {
    // The terminal escape query is asynchronous: a terminal may answer with
    // CSI 4 ; height ; width t. We deliberately do not block waiting for it.
    // The cell-size query remains the portable baseline.
    return false;
}

bool PixelTerminal::querySize() {
    const Size previous = pixel_size_;
    const bool cells = queryCellSize();

    if (!cells) return false;

    // Native terminals commonly expose cells, not addressable physical pixels.
    // Until a pixel-capable bridge supplies a physical raster size, use the
    // logical cell dimensions as the frame dimensions.
    if (pixel_size_ == Size{} || pixel_size_ == previous) {
        pixel_size_ = cell_size_;
    }

    if (pixel_size_ != previous) {
        updateCenter();
        rebuildFrame();
        recordEvent(SizeEventType::Resize);
    }
    return true;
}

bool PixelTerminal::setSize(Size size) {
    if (size.width == 0 || size.height == 0) return false;
    if (size == pixel_size_) return true;

    pixel_size_ = size;
    updateCenter();
    rebuildFrame();
    recordEvent(SizeEventType::PixelSizeChanged);
    return true;
}

void PixelTerminal::updateCenter() {
    center_.x = pixel_size_.width / 2;
    center_.y = pixel_size_.height / 2;
}

void PixelTerminal::rebuildFrame() {
    const auto area = safe_area(pixel_size_.width, pixel_size_.height);
    frame_.assign(area, Pixel{});
}

void PixelTerminal::recordEvent(SizeEventType type) {
    events_.push_back(SizeEvent{type, pixel_size_, cell_size_, center_});
}

bool PixelTerminal::setPixel(std::size_t x, std::size_t y, Pixel pixel) {
    if (x >= pixel_size_.width || y >= pixel_size_.height) return false;
    const auto index = y * pixel_size_.width + x;
    if (index >= frame_.size()) return false;
    frame_[index] = pixel;
    return true;
}

bool PixelTerminal::fill(Pixel pixel) {
    std::fill(frame_.begin(), frame_.end(), pixel);
    return !frame_.empty();
}

void PixelTerminal::clear() {
    std::fill(frame_.begin(), frame_.end(), Pixel{});
}

bool PixelTerminal::show() {
    // The SLeeLa bridge owns actual raster presentation. This native layer
    // owns the frame and geometry; presentation is intentionally separate.
    return active_ && !frame_.empty();
}

bool PixelTerminal::begin() {
    if (!querySize()) return false;
    active_ = true;
    return true;
}

void PixelTerminal::end() {
    active_ = false;
}

void PixelTerminal::watchResize() {
    if (watching_resize_) return;
    struct sigaction action{};
    action.sa_handler = on_resize;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (::sigaction(SIGWINCH, &action, nullptr) == 0) {
        watching_resize_ = true;
    }
}

void PixelTerminal::unwatchResize() {
    if (!watching_resize_) return;
    struct sigaction action{};
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    ::sigaction(SIGWINCH, &action, nullptr);
    watching_resize_ = false;
    resize_pending = 0;
}

bool PixelTerminal::pollResize() {
    if (!resize_pending) return false;
    resize_pending = 0;
    return querySize();
}

} // namespace sleela::terminal
