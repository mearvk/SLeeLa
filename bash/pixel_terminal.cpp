// SleelaTerminal(TM) -- SLeeLa's terminal product, built on GNU Bash
// (GPL, (C) Free Software Foundation). SLeeLa-authored addition; the vendored
// GNU Bash sources keep their own GPL headers and copyright.
//
// Phraign(TM) -- SleelaTerminal(TM)'s frame-based, pixel-accurate terminal
// control system. Native pixel-terminal layer implementation. See PHRAIGN.md.

#include "pixel_terminal.hpp"

#include <algorithm>
#include <csignal>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace {
#ifndef _WIN32
volatile std::sig_atomic_t resize_pending = 0;

void on_resize(int) noexcept {
    resize_pending = 1;
}
#endif

std::size_t safe_area(std::size_t width, std::size_t height) {
    if (width == 0 || height == 0) return 0;
    return width > static_cast<std::size_t>(-1) / height ? 0 : width * height;
}
}

namespace sleela::terminal {

Handshake PixelTerminal::handshake() noexcept {
    Handshake result;
    result.ready = true;
    result.pixel_granularity = true;
    result.native_frame = true;
    result.resize_events = true;
    return result;
}

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
#ifdef _WIN32
    // Windows console reports size in character cells via the screen buffer's
    // visible window rectangle. There is no pixel-granularity query, so
    // queryPixelSize() returns false and the cell size is used as the fallback.
    CONSOLE_SCREEN_BUFFER_INFO info{};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(out, &info)) {
        return false;
    }
    const unsigned cols = (unsigned)(info.srWindow.Right - info.srWindow.Left + 1);
    const unsigned rows = (unsigned)(info.srWindow.Bottom - info.srWindow.Top + 1);
    if (cols == 0 || rows == 0) return false;
    const Size next{cols, rows};
    if (next != cell_size_) {
        cell_size_ = next;
        recordEvent(SizeEventType::CellSizeChanged);
    }
    return true;
#else
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
#endif
}

bool PixelTerminal::queryPixelSize() {
#ifdef _WIN32
    // The Windows console API does not expose a physical pixel size; report
    // failure so querySize() falls back to the cell dimensions.
    return false;
#else
    winsize ws{};
    if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 ||
        ws.ws_xpixel == 0 || ws.ws_ypixel == 0) {
        return false;
    }

    const Size next{ws.ws_xpixel, ws.ws_ypixel};
    if (next != pixel_size_) {
        pixel_size_ = next;
        updateCenter();
        rebuildFrame();
        recordEvent(SizeEventType::PixelSizeChanged);
    }
    return true;
#endif
}

bool PixelTerminal::querySize() {
    const Size previous = pixel_size_;
    const bool cells = queryCellSize();
    const bool pixels = queryPixelSize();

    if (!cells && !pixels) return false;

    // Prefer physical pixel dimensions when the terminal exposes them.
    // Otherwise retain the logical cell dimensions as the safe fallback.
    if (!pixels) {
        const Size fallback = cell_size_;
        if (fallback.width == 0 || fallback.height == 0) return false;
        if (pixel_size_ != fallback) {
            pixel_size_ = fallback;
            updateCenter();
            rebuildFrame();
        }
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
#ifdef _WIN32
    // Windows has no SIGWINCH. Resize is detected by polling querySize()
    // (pollResize() below), so simply mark the watcher active.
    watching_resize_ = true;
#else
    struct sigaction action{};
    action.sa_handler = on_resize;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (::sigaction(SIGWINCH, &action, nullptr) == 0) {
        watching_resize_ = true;
    }
#endif
}

void PixelTerminal::unwatchResize() {
    if (!watching_resize_) return;
#ifdef _WIN32
    watching_resize_ = false;
#else
    struct sigaction action{};
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    ::sigaction(SIGWINCH, &action, nullptr);
    watching_resize_ = false;
    resize_pending = 0;
#endif
}

bool PixelTerminal::pollResize() {
#ifdef _WIN32
    // No signal-driven flag on Windows; re-query the console each poll and
    // report whether the size changed.
    if (!watching_resize_) return false;
    return querySize();
#else
    if (!resize_pending) return false;
    resize_pending = 0;
    return querySize();
#endif
}

} // namespace sleela::terminal
