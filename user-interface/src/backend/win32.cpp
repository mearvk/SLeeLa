/* =============================================================================
 * SleelaUI Win32 backend (Windows 10+).
 *
 * The real host surface on Windows: a top-level HWND, a 32-bit top-down DIB
 * section we blit the toolkit's Canvas into with SetDIBitsToDevice, and GDI
 * font selection with a grayscale glyph bitmap (GetGlyphOutline / GGO_GRAY8)
 * for antialiased coverage. No GDI+, no Direct2D, no UWP -- SleelaUI owns its
 * pixels and only asks Win32 to show them, exactly as the X11 backend does.
 *
 * This backend is "correct, not styled": it translates native input into
 * SLUIEvent and presents the Canvas; all look-and-feel lives in the core.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_backend.hpp"
#include "slui_window.hpp"

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <windowsx.h> /* GET_X_LPARAM / GET_Y_LPARAM */

#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace slui {
namespace {

uint32_t map_vk(WPARAM vk) {
    switch (vk) {
    case VK_RETURN: return SLUI_KEY_ENTER;
    case VK_ESCAPE: return SLUI_KEY_ESCAPE;
    case VK_BACK: return SLUI_KEY_BACKSPACE;
    case VK_TAB: return SLUI_KEY_TAB;
    case VK_SPACE: return SLUI_KEY_SPACE;
    case VK_LEFT: return SLUI_KEY_LEFT;
    case VK_RIGHT: return SLUI_KEY_RIGHT;
    case VK_UP: return SLUI_KEY_UP;
    case VK_DOWN: return SLUI_KEY_DOWN;
    case VK_HOME: return SLUI_KEY_HOME;
    case VK_END: return SLUI_KEY_END;
    case VK_DELETE: return SLUI_KEY_DELETE;
    default: return SLUI_KEY_UNKNOWN;
    }
}

uint32_t current_mods() {
    uint32_t m = 0;
    if (GetKeyState(VK_SHIFT) & 0x8000) m |= SLUI_MOD_SHIFT;
    if (GetKeyState(VK_CONTROL) & 0x8000) m |= SLUI_MOD_CTRL;
    if (GetKeyState(VK_MENU) & 0x8000) m |= SLUI_MOD_ALT;
    if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) m |= SLUI_MOD_SUPER;
    return m;
}

std::wstring widen(const std::string& s) {
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}

class Win32Backend;

class Win32Window : public NativeWindow {
public:
    Win32Window(Win32Backend* backend, HWND hwnd) : backend_(backend), hwnd_(hwnd) {}
    ~Win32Window() override {
        if (dib_) DeleteObject(dib_);
        if (mem_dc_) DeleteDC(mem_dc_);
        if (hwnd_) DestroyWindow(hwnd_);
    }

    void set_title(const std::string& title) override {
        SetWindowTextW(hwnd_, widen(title).c_str());
    }
    void show() override {
        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);
    }
    void hide() override { ShowWindow(hwnd_, SW_HIDE); }
    int width() const override {
        RECT r;
        GetClientRect(hwnd_, &r);
        return r.right - r.left;
    }
    int height() const override {
        RECT r;
        GetClientRect(hwnd_, &r);
        return r.bottom - r.top;
    }
    void request_redraw() override { InvalidateRect(hwnd_, nullptr, FALSE); }

    void present(const Canvas& canvas) override {
        int w = canvas.width(), h = canvas.height();
        if (w <= 0 || h <= 0) return;
        HDC dc = GetDC(hwnd_);
        BITMAPINFO bmi;
        std::memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; /* top-down */
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        /* Canvas packs 0xAARRGGBB; Win32 BI_RGB 32bpp reads BGRA in memory,
         * which is the same byte order on little-endian Windows. */
        SetDIBitsToDevice(dc, 0, 0, (DWORD)w, (DWORD)h, 0, 0, 0, (UINT)h,
                          canvas.pixels(), &bmi, DIB_RGB_COLORS);
        ReleaseDC(hwnd_, dc);
    }

    HWND hwnd() const { return hwnd_; }

private:
    Win32Backend* backend_;
    HWND hwnd_;
    HBITMAP dib_ = nullptr;
    HDC mem_dc_ = nullptr;
};

class Win32Backend : public Backend {
public:
    explicit Win32Backend(const std::string& app_id) : app_id_(app_id) {
        hinst_ = GetModuleHandleW(nullptr);
        WNDCLASSEXW wc;
        std::memset(&wc, 0, sizeof(wc));
        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = &Win32Backend::wnd_proc;
        wc.hInstance = hinst_;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"SleelaUIWindow";
        RegisterClassExW(&wc);
        font_dc_ = CreateCompatibleDC(nullptr);
        SetBkMode(font_dc_, TRANSPARENT);
    }
    ~Win32Backend() override {
        if (font_) DeleteObject(font_);
        if (font_dc_) DeleteDC(font_dc_);
    }

    bool ok() const { return hinst_ != nullptr; }
    SLUIBackend id() const override { return SLUI_BACKEND_WIN32; }

    std::unique_ptr<NativeWindow> create_window(Window* owner,
                                                const std::string& title,
                                                int width, int height,
                                                bool resizable) override {
        DWORD style = resizable ? WS_OVERLAPPEDWINDOW
                                : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                                   WS_MINIMIZEBOX);
        RECT r{0, 0, width, height};
        AdjustWindowRect(&r, style, FALSE);
        HWND hwnd = CreateWindowExW(
            0, L"SleelaUIWindow", widen(title).c_str(), style, CW_USEDEFAULT,
            CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, nullptr, nullptr,
            hinst_, this);
        if (!hwnd) return nullptr;
        auto nw = std::make_unique<Win32Window>(this, hwnd);
        owners_[hwnd] = owner;
        windows_[hwnd] = nw.get();
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        return nw;
    }

    int pump(bool block) override {
        MSG msg;
        int dispatched = 0;
        if (block) {
            if (GetMessageW(&msg, nullptr, 0, 0) <= 0) {
                quit_ = true;
                return dispatched;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            ++dispatched;
        }
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                quit_ = true;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            ++dispatched;
        }
        return dispatched;
    }

    void quit(int code) override {
        quit_ = true;
        exit_code_ = code;
        PostQuitMessage(code);
    }
    bool should_quit(int* code) const override {
        if (quit_ && code) *code = exit_code_;
        return quit_;
    }

    /* ---- fonts (GDI + GGO_GRAY8 coverage) ------------------------------ */
    bool set_font(const std::string& family, int size_pt) override {
        std::string key = family + "@" + std::to_string(size_pt);
        if (key == font_key_ && font_) return true;
        if (font_) {
            DeleteObject(font_);
            font_ = nullptr;
        }
        int px = -MulDiv(size_pt, GetDeviceCaps(font_dc_, LOGPIXELSY), 72);
        std::wstring face = widen(family == "system" || family.empty()
                                      ? "Segoe UI"
                                      : family);
        font_ = CreateFontW(px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                            face.c_str());
        if (!font_) return false;
        SelectObject(font_dc_, font_);
        TEXTMETRICW tm;
        GetTextMetricsW(font_dc_, &tm);
        ascent_ = tm.tmAscent;
        descent_ = tm.tmDescent;
        line_height_ = tm.tmHeight + tm.tmExternalLeading;
        font_key_ = key;
        advance_cache_.clear();
        return true;
    }
    double font_ascent() const override { return ascent_; }
    double font_descent() const override { return descent_; }
    double font_line_height() const override { return line_height_; }

    bool rasterize_glyph(uint32_t cp, GlyphBitmap* out) override {
        if (!font_) return false;
        SelectObject(font_dc_, font_);
        GLYPHMETRICS gm;
        MAT2 mat{{0, 1}, {0, 0}, {0, 0}, {0, 1}}; /* identity */
        UINT fmt = GGO_GRAY8_BITMAP;
        DWORD size = GetGlyphOutlineW(font_dc_, cp, fmt, &gm, 0, nullptr, &mat);
        if (size == GDI_ERROR) return false;
        std::vector<unsigned char> buf(size ? size : 1, 0);
        if (size) GetGlyphOutlineW(font_dc_, cp, fmt, &gm, size, buf.data(), &mat);
        out->left = gm.gmptGlyphOrigin.x;
        out->top = gm.gmptGlyphOrigin.y;
        out->width = gm.gmBlackBoxX;
        out->height = gm.gmBlackBoxY;
        out->advance = gm.gmCellIncX;
        /* GGO_GRAY8 rows are DWORD-aligned and range 0..64; scale to 0..255. */
        int pitch = (out->width + 3) & ~3;
        out->coverage.assign((size_t)out->width * out->height, 0);
        for (int y = 0; y < out->height; ++y) {
            for (int x = 0; x < out->width; ++x) {
                unsigned v = size ? buf[(size_t)y * pitch + x] : 0;
                out->coverage[(size_t)y * out->width + x] =
                    (unsigned char)(v > 64 ? 255 : v * 255 / 64);
            }
        }
        return true;
    }

    double glyph_advance(uint32_t cp) override {
        if (!font_) return 8.0;
        auto it = advance_cache_.find(cp);
        if (it != advance_cache_.end()) return it->second;
        SelectObject(font_dc_, font_);
        ABC abc;
        double adv = 8.0;
        wchar_t wc = (wchar_t)cp;
        if (GetCharABCWidthsW(font_dc_, cp, cp, &abc))
            adv = abc.abcA + (int)abc.abcB + abc.abcC;
        else {
            SIZE sz;
            if (GetTextExtentPoint32W(font_dc_, &wc, 1, &sz)) adv = sz.cx;
        }
        advance_cache_[cp] = adv;
        return adv;
    }

private:
    static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        auto* self = reinterpret_cast<Win32Backend*>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        if (!self) {
            if (msg == WM_CREATE) {
                auto* cs = reinterpret_cast<CREATESTRUCTW*>(lp);
                SetWindowLongPtrW(hwnd, GWLP_USERDATA,
                                  (LONG_PTR)cs->lpCreateParams);
            }
            return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return self->handle(hwnd, msg, wp, lp);
    }

    LRESULT handle(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        auto oit = owners_.find(hwnd);
        Window* owner = oit == owners_.end() ? nullptr : oit->second;
        SLUIEvent ev{};
        if (owner) ev.window = reinterpret_cast<SLUIWindow*>(owner);

        switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (owner && windows_.count(hwnd))
                windows_[hwnd]->present(owner->render());
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SIZE:
            if (owner) {
                ev.type = SLUI_EVENT_RESIZE;
                ev.width = LOWORD(lp);
                ev.height = HIWORD(lp);
                owner->handle_event(ev);
            }
            return 0;
        case WM_CLOSE:
            if (owner) {
                ev.type = SLUI_EVENT_CLOSE;
                owner->handle_event(ev);
            }
            return 0;
        case WM_MOUSEMOVE:
            if (owner) {
                ev.type = SLUI_EVENT_POINTER_MOVE;
                ev.x = GET_X_LPARAM(lp);
                ev.y = GET_Y_LPARAM(lp);
                ev.modifiers = current_mods();
                owner->handle_event(ev);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            if (owner) {
                bool down = (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ||
                             msg == WM_MBUTTONDOWN);
                ev.type = down ? SLUI_EVENT_POINTER_DOWN : SLUI_EVENT_POINTER_UP;
                ev.x = GET_X_LPARAM(lp);
                ev.y = GET_Y_LPARAM(lp);
                ev.modifiers = current_mods();
                ev.button = (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
                                ? SLUI_BUTTON_LEFT
                            : (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
                                ? SLUI_BUTTON_RIGHT
                                : SLUI_BUTTON_MIDDLE;
                if (down) SetCapture(hwnd);
                else ReleaseCapture();
                owner->handle_event(ev);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
            if (owner) {
                ev.type = msg == WM_KEYDOWN ? SLUI_EVENT_KEY_DOWN
                                            : SLUI_EVENT_KEY_UP;
                ev.keysym = map_vk(wp);
                ev.modifiers = current_mods();
                owner->handle_event(ev);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_CHAR:
            if (owner && wp >= 0x20 && wp != 0x7F) {
                ev.type = SLUI_EVENT_TEXT;
                ev.codepoint = (uint32_t)wp;
                owner->handle_event(ev);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
        }
    }

    std::string app_id_;
    HINSTANCE hinst_ = nullptr;
    HDC font_dc_ = nullptr;
    HFONT font_ = nullptr;
    std::string font_key_;
    double ascent_ = 12, descent_ = 3, line_height_ = 16;
    bool quit_ = false;
    int exit_code_ = 0;
    std::map<HWND, Window*> owners_;
    std::map<HWND, Win32Window*> windows_;
    std::unordered_map<uint32_t, double> advance_cache_;
};

} // namespace

std::unique_ptr<Backend> create_backend(const std::string& app_id) {
    auto be = std::make_unique<Win32Backend>(app_id);
    if (!be->ok()) return nullptr;
    return be;
}

} // namespace slui
