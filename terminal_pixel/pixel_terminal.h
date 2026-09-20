/*
 * pixel_terminal.h - Single-pixel precision terminal renderer
 * Supports 60Hz refresh rate with framebuffer-based rendering
 * 
 * This module provides a hardware-independent interface for pixel-perfect
 * terminal rendering at 60 frames per second.
 */

#ifndef PIXEL_TERMINAL_H
#define PIXEL_TERMINAL_H

#include <stdint.h>
#include <time.h>
#include <pthread.h>

/* Color representation: 32-bit ARGB */
typedef uint32_t pixel_color_t;

#define PIXEL_ARGB(a, r, g, b) \
    (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

#define PIXEL_RGB(r, g, b) PIXEL_ARGB(0xFF, r, g, b)
#define PIXEL_BLACK PIXEL_RGB(0, 0, 0)
#define PIXEL_WHITE PIXEL_RGB(255, 255, 255)
#define PIXEL_RED PIXEL_RGB(255, 0, 0)
#define PIXEL_GREEN PIXEL_RGB(0, 255, 0)
#define PIXEL_BLUE PIXEL_RGB(0, 0, 255)

/* Framebuffer structure */
typedef struct {
    uint32_t *pixels;           /* Pixel data: width * height */
    uint32_t *back_buffer;      /* Double-buffer for smooth rendering */
    int width;                  /* Framebuffer width in pixels */
    int height;                 /* Framebuffer height in pixels */
    int pitch;                  /* Bytes per scanline */
    int bpp;                    /* Bits per pixel (32) */
} pixel_framebuffer_t;

/* Renderer context: manages timing and refresh */
typedef struct {
    pixel_framebuffer_t *fb;
    pthread_t render_thread;
    pthread_mutex_t fb_lock;
    pthread_cond_t frame_ready;
    volatile int running;
    volatile int frame_count;
    double frame_time;          /* Milliseconds per frame (16.667 for 60Hz) */
    struct timespec last_frame;
    uint64_t total_frames;
} pixel_renderer_t;

/* Font glyph bitmap cache */
typedef struct {
    uint8_t *bitmap;            /* Glyph bitmap data */
    int width;                  /* Glyph width in pixels */
    int height;                 /* Glyph height in pixels */
    int advance;                /* Horizontal advance width */
} pixel_glyph_t;

typedef struct {
    pixel_glyph_t *glyphs;      /* ASCII 0-255 glyphs */
    int glyph_width;
    int glyph_height;
} pixel_font_t;

/* ============================================================================
 * Core Framebuffer Operations
 * ============================================================================ */

/**
 * pixel_fb_create - Create a framebuffer at specified resolution
 * @width: Framebuffer width in pixels
 * @height: Framebuffer height in pixels
 * Returns: Allocated framebuffer, or NULL on error
 */
pixel_framebuffer_t* pixel_fb_create(int width, int height);

/**
 * pixel_fb_destroy - Free framebuffer resources
 * @fb: Framebuffer to destroy
 */
void pixel_fb_destroy(pixel_framebuffer_t *fb);

/**
 * pixel_fb_clear - Fill entire framebuffer with color
 * @fb: Target framebuffer
 * @color: Color to fill (ARGB)
 */
void pixel_fb_clear(pixel_framebuffer_t *fb, pixel_color_t color);

/**
 * pixel_fb_swap - Swap front and back buffers (double-buffering)
 * @fb: Framebuffer
 */
void pixel_fb_swap(pixel_framebuffer_t *fb);

/* ============================================================================
 * Single-Pixel Operations
 * ============================================================================ */

/**
 * pixel_set - Write a single pixel
 * @fb: Target framebuffer
 * @x, @y: Pixel coordinates
 * @color: Color (ARGB)
 * Returns: 0 on success, -1 if out of bounds
 */
int pixel_set(pixel_framebuffer_t *fb, int x, int y, pixel_color_t color);

/**
 * pixel_get - Read a single pixel
 * @fb: Source framebuffer
 * @x, @y: Pixel coordinates
 * Returns: Color at (x,y), or 0 if out of bounds
 */
pixel_color_t pixel_get(pixel_framebuffer_t *fb, int x, int y);

/**
 * pixel_blend - Alpha-blend a pixel onto target
 * @fb: Target framebuffer
 * @x, @y: Pixel coordinates
 * @color: Source color (ARGB)
 * Returns: 0 on success, -1 if out of bounds
 */
int pixel_blend(pixel_framebuffer_t *fb, int x, int y, pixel_color_t color);

/* ============================================================================
 * Primitive Drawing Operations
 * ============================================================================ */

/**
 * pixel_line - Draw line using Bresenham algorithm
 * @fb: Target framebuffer
 * @x0, @y0, @x1, @y1: Line endpoints
 * @color: Line color
 */
void pixel_line(pixel_framebuffer_t *fb, int x0, int y0, int x1, int y1, 
                pixel_color_t color);

/**
 * pixel_rect - Draw filled rectangle
 * @fb: Target framebuffer
 * @x, @y, @w, @h: Rectangle bounds
 * @color: Fill color
 */
void pixel_rect(pixel_framebuffer_t *fb, int x, int y, int w, int h, 
                pixel_color_t color);

/**
 * pixel_rect_outline - Draw rectangle outline
 * @fb: Target framebuffer
 * @x, @y, @w, @h: Rectangle bounds
 * @color: Outline color
 * @thickness: Outline thickness in pixels
 */
void pixel_rect_outline(pixel_framebuffer_t *fb, int x, int y, int w, int h,
                        pixel_color_t color, int thickness);

/**
 * pixel_circle - Draw filled circle
 * @fb: Target framebuffer
 * @cx, @cy, @r: Center coordinates and radius
 * @color: Fill color
 */
void pixel_circle(pixel_framebuffer_t *fb, int cx, int cy, int r, 
                  pixel_color_t color);

/**
 * pixel_circle_outline - Draw circle outline
 * @fb: Target framebuffer
 * @cx, @cy, @r: Center coordinates and radius
 * @color: Outline color
 * @thickness: Outline thickness in pixels
 */
void pixel_circle_outline(pixel_framebuffer_t *fb, int cx, int cy, int r,
                          pixel_color_t color, int thickness);

/* ============================================================================
 * Text Rendering
 * ============================================================================ */

/**
 * pixel_font_create_default - Create default monospace font
 * @glyph_width, @glyph_height: Size of each glyph in pixels
 * Returns: Allocated font, or NULL on error
 */
pixel_font_t* pixel_font_create_default(int glyph_width, int glyph_height);

/**
 * pixel_font_destroy - Free font resources
 * @font: Font to destroy
 */
void pixel_font_destroy(pixel_font_t *font);

/**
 * pixel_putchar - Render single character at pixel coordinates
 * @fb: Target framebuffer
 * @x, @y: Pixel coordinates (top-left)
 * @c: Character to render (ASCII 0-127)
 * @fg, @bg: Foreground/background colors
 * @font: Font to use for rendering
 * Returns: 0 on success, -1 on error
 */
int pixel_putchar(pixel_framebuffer_t *fb, int x, int y, char c,
                  pixel_color_t fg, pixel_color_t bg, pixel_font_t *font);

/**
 * pixel_puts - Render null-terminated string
 * @fb: Target framebuffer
 * @x, @y: Pixel coordinates (top-left)
 * @str: String to render
 * @fg, @bg: Foreground/background colors
 * @font: Font to use
 * Returns: X coordinate after last character, or -1 on error
 */
int pixel_puts(pixel_framebuffer_t *fb, int x, int y, const char *str,
               pixel_color_t fg, pixel_color_t bg, pixel_font_t *font);

/* ============================================================================
 * 60Hz Renderer Thread
 * ============================================================================ */

/**
 * pixel_renderer_create - Create and start 60Hz renderer thread
 * @fb: Framebuffer to render
 * Returns: Renderer context, or NULL on error
 * 
 * Starts a background thread that:
 * - Maintains 60Hz frame rate (16.667ms per frame)
 * - Swaps double-buffer each frame
 * - Pushes pixels to display device
 */
pixel_renderer_t* pixel_renderer_create(pixel_framebuffer_t *fb);

/**
 * pixel_renderer_destroy - Stop and clean up renderer thread
 * @renderer: Renderer to destroy
 */
void pixel_renderer_destroy(pixel_renderer_t *renderer);

/**
 * pixel_renderer_lock - Acquire lock before framebuffer modification
 * @renderer: Renderer context
 */
void pixel_renderer_lock(pixel_renderer_t *renderer);

/**
 * pixel_renderer_unlock - Release framebuffer lock
 * @renderer: Renderer context
 */
void pixel_renderer_unlock(pixel_renderer_t *renderer);

/**
 * pixel_renderer_wait_frame - Synchronize with 60Hz frame boundary
 * @renderer: Renderer context
 * Returns: Frame number (0-based)
 */
uint64_t pixel_renderer_wait_frame(pixel_renderer_t *renderer);

/**
 * pixel_renderer_get_fps - Get current frames per second
 * @renderer: Renderer context
 * Returns: Instantaneous FPS estimate
 */
double pixel_renderer_get_fps(pixel_renderer_t *renderer);

/* ============================================================================
 * Display Backend (Implementation-specific)
 * ============================================================================ */

/**
 * pixel_display_init - Initialize display backend
 * @width, @height: Display resolution in pixels
 * Returns: 0 on success, -1 on error
 * 
 * Implementation varies:
 * - Linux: /dev/fb0 framebuffer
 * - X11: XCreateImage + XPutImage
 * - Wayland: wl_surface + wl_shm
 * - Fallback: ASCII art simulation
 */
int pixel_display_init(int width, int height);

/**
 * pixel_display_blit - Push framebuffer pixels to display
 * @fb: Framebuffer to display
 * Returns: 0 on success, -1 on error
 */
int pixel_display_blit(pixel_framebuffer_t *fb);

/**
 * pixel_display_cleanup - Shut down display backend
 */
void pixel_display_cleanup(void);

#endif /* PIXEL_TERMINAL_H */
