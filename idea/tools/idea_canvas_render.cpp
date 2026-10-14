/*
 * idea/tools/idea_canvas_render.cpp
 * Offscreen renderer for the Idea System(TM) Sleela Canvas.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * This reproduces the exact layout of lib/idea/IdeaCanvas.sleela using the
 * genuine SleelaUI Draw API (slui_draw_*), renders it to an offscreen RGBA
 * surface (no window / no X11), reads the pixels back, and writes a PPM image
 * so the GUI can be previewed headlessly. PPM is used to avoid any external
 * image-library dependency; convert to PNG with ImageMagick if desired.
 */
#include "sleela_ui_draw.h"

#include <cstdint>
#include <cstdio>
#include <vector>

static const int W = 720;
static const int H = 480;
static const int CHAT_H = 150;
static const double RADIUS = 18.0;

static void chat_pane(SLUIDrawContext *dc, int x, int y, int w, int h,
                      SLUIColor card, SLUIColor accent, SLUIColor panel,
                      SLUIColor rail, SLUIColor railFill, SLUIColor ink,
                      SLUIColor sub, SLUIColor bg) {
    slui_draw_fill_round_rect(dc, (SLUIRect){x, y, w, h}, RADIUS, card);
    slui_draw_stroke_round_rect(dc, (SLUIRect){x, y, w, h}, RADIUS, 1.0, accent);

    /* title with an accent dot */
    slui_draw_fill_circle(dc, x + 22, y + 20, 4.0, railFill);
    slui_draw_text(dc, "Chat  -  talk to the Idea(TM) System", x + 34, y + 24, ink);

    /* System bubble (left, panel), You bubble (right, accent-tinted) */
    int bw = w - 220;
    slui_draw_fill_round_rect(dc, (SLUIRect){x + 16, y + 38, bw, 30}, 14.0, panel);
    slui_draw_text(dc, "System: the State is the System; ask and I present.",
                   x + 30, y + 58, sub);
    slui_draw_fill_round_rect(dc, (SLUIRect){x + w - bw - 16, y + 74, bw, 30}, 14.0, rail);
    slui_draw_text(dc, "You: show me the Series.", x + w - bw - 2, y + 94, ink);

    /* rounded input + Send button, with comfortable padding */
    int iy = y + h - 42;
    slui_draw_fill_round_rect(dc, (SLUIRect){x + 16, iy, w - 118, 32}, 16.0, panel);
    slui_draw_stroke_round_rect(dc, (SLUIRect){x + 16, iy, w - 118, 32}, 16.0, 1.0, rail);
    slui_draw_text(dc, "Type a message...", x + 34, iy + 21, sub);
    slui_draw_fill_round_rect(dc, (SLUIRect){x + w - 90, iy, 74, 32}, 16.0, railFill);
    slui_draw_text(dc, "Send", x + w - 68, iy + 21, bg);
}

int main(int argc, char **argv) {
    const char *out = argc > 1 ? argv[1] : "idea-canvas.ppm";

    SLUIColor bg       = slui_rgb(13, 14, 18);
    SLUIColor panel    = slui_rgb(22, 24, 31);
    SLUIColor card     = slui_rgb(28, 31, 40);
    SLUIColor rail     = slui_rgb(40, 44, 56);
    SLUIColor railFill = slui_rgb(94, 156, 255);
    SLUIColor accent   = slui_rgb(120, 200, 255);
    SLUIColor ink      = slui_rgb(236, 240, 248);
    SLUIColor sub      = slui_rgb(150, 160, 178);

    SLUIDrawContext *dc = slui_draw_create(W, H, SLUI_BUFFER_SINGLE);
    if (!dc) { std::fprintf(stderr, "draw_create failed\n"); return 1; }

    /* State mid-Series, so the rail is partly filled and some decisions set. */
    int totalSecs = 82800;       /* 23h, from the Author payload */
    double seconds = totalSecs * 0.46;
    int decisions = 3, maxDec = 6;

    slui_draw_clear(dc, bg);

    /* header band + an accent tick on the left */
    slui_draw_fill_round_rect(dc, (SLUIRect){16, 16, W - 32, 60}, RADIUS, panel);
    slui_draw_fill_round_rect(dc, (SLUIRect){28, 30, 4, 32}, 2.0, railFill);
    slui_draw_text(dc, "Idea System(TM)  -  From Author; distributive", 44, 42, ink);
    slui_draw_text(dc, "resource: github.com/mearvk/Senior.Senate.Attorney.E44Hrs",
                   44, 62, sub);

    /* live Series panel */
    int py = 92;
    int ph = H - CHAT_H - py - 16;
    slui_draw_fill_round_rect(dc, (SLUIRect){16, py, W - 32, ph}, RADIUS, panel);

    /* state line with an accent dot */
    slui_draw_fill_circle(dc, 36, py + 22, 4.0, railFill);
    slui_draw_text(dc, "state: SOLVING", 48, py + 26, ink);

    /* progress rail: rounded track + rounded fill, roomy vertical placement */
    double railX = 32.0, railY = py + 56, railW = W - 64;
    slui_draw_fill_round_rect(dc, (SLUIRect){(int)railX, (int)railY, (int)railW, 14},
                              7.0, rail);
    double frac = seconds / totalSecs;
    int fillW = (int)(railW * frac);
    if (fillW > 0)
        slui_draw_fill_round_rect(dc, (SLUIRect){(int)railX, (int)railY, fillW, 14},
                                  7.0, railFill);
    /* decision markers: filled accent if reached, hollow track-colour if ahead */
    for (int i = 1; i <= maxDec; ++i) {
        double mx = railX + (railW * i) / (maxDec + 1);
        bool reached = (frac * (maxDec + 1)) >= i;
        slui_draw_fill_circle(dc, mx, railY + 7, 6.0, panel);
        slui_draw_fill_circle(dc, mx, railY + 7, 4.5, reached ? accent : rail);
    }

    char line[160];
    std::snprintf(line, sizeof line, "Series %d%%        decisions %d / %d",
                  (int)(frac * 100), decisions, maxDec);
    slui_draw_text(dc, line, 32, py + 96, sub);
    slui_draw_text(dc, "canvas: present 60 Hz    simulate 120 Hz (fixed step)",
                   32, py + 116, sub);

    /* chat pane */
    chat_pane(dc, 16, H - CHAT_H, W - 32, CHAT_H - 12,
              card, accent, panel, rail, railFill, ink, sub, bg);

    /* read back and write PPM */
    std::vector<uint8_t> rgba((size_t)W * H * 4);
    slui_draw_read_rgba(dc, rgba.data(), rgba.size());
    slui_draw_destroy(dc);

    FILE *f = std::fopen(out, "wb");
    if (!f) { std::fprintf(stderr, "cannot open %s\n", out); return 1; }
    std::fprintf(f, "P6\n%d %d\n255\n", W, H);
    for (size_t i = 0; i < (size_t)W * H; ++i) {
        std::fputc(rgba[i * 4 + 0], f);
        std::fputc(rgba[i * 4 + 1], f);
        std::fputc(rgba[i * 4 + 2], f);
    }
    std::fclose(f);
    std::printf("wrote %s (%dx%d)\n", out, W, H);
    return 0;
}
