#ifndef SLEELA_UI_FONT_H
#define SLEELA_UI_FONT_H
/* =============================================================================
 * SleelaUI(TM) Font API -- fonts and font effects for Sleela and her UI.
 *
 * A FONT bundles a family, size, weight, slant, and spacing with a stack of
 * EFFECTS -- shadows, light, glow, an emitter, outline, relief (emboss/engrave),
 * and a gradient fill -- so a developer or agent can render rich, lit text with
 * one handle. The effects compose with the rest of the toolkit: the Draw API
 * (sleela_ui_draw.h) owns the pixels, and the RELIEF / LIGHT / EMITTER effects
 * speak the same language as the Lighting layer (sleela_ui_light.h) -- light and
 * shadow are emissions, and an EMITTER radiates from the text WITHOUT reserving
 * the glyphs as "used" (so the same text can light its neighbours yet remain
 * free to carry more effects), exactly as elsewhere in SleelaUI.
 *
 * The look is produced by the toolkit's own rasterizer over an 8-bit glyph
 * coverage mask, so a styled string is pixel-identical on Windows, macOS, and
 * Linux/Unix. Every effect has a QUALITY knob (draft .. ultra) that trades
 * sample count for smoothness, so you can "choose many quality effects".
 *
 * Colours are the usual 0xRRGGBBAA words; geometry uses SLUIRect/SLUIPoint.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "sleela_ui_draw.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Weight, slant, and global rendering quality.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_FONT_THIN = 100,
    SLUI_FONT_LIGHT = 300,
    SLUI_FONT_REGULAR = 400,
    SLUI_FONT_MEDIUM = 500,
    SLUI_FONT_SEMIBOLD = 600,
    SLUI_FONT_BOLD = 700,
    SLUI_FONT_BLACK = 900
} SLUIFontWeight;

typedef enum {
    SLUI_SLANT_UPRIGHT = 0,
    SLUI_SLANT_ITALIC = 1,  /* a true italic face if present                 */
    SLUI_SLANT_OBLIQUE = 2  /* a synthetic shear when no italic face exists  */
} SLUIFontSlant;

/* Quality trades samples for smoothness. It governs shadow/glow blur taps,
 * outline/relief angular samples, and whether coverage is super-sampled.
 * "Choose many quality effects" -> pick ULTRA for the richest result. */
typedef enum {
    SLUI_QUALITY_DRAFT = 0,  /* cheapest: hard shadows, few taps             */
    SLUI_QUALITY_LOW = 1,
    SLUI_QUALITY_MEDIUM = 2, /* a sensible default                           */
    SLUI_QUALITY_HIGH = 3,
    SLUI_QUALITY_ULTRA = 4   /* the most samples / smoothest edges           */
} SLUIFontQuality;

/* --------------------------------------------------------------------------
 * Effects. A font carries an ordered stack; they are drawn back-to-front
 * (shadows under the glyph, glyph fill, then light/glow/outline on top), so the
 * stack order you add them in is the visual layering.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_FX_DROP_SHADOW = 0,  /* an offset, blurred, dark copy behind         */
    SLUI_FX_INNER_SHADOW = 1, /* shadow inside the glyph (a pressed-in look)  */
    SLUI_FX_GLOW = 2,         /* a soft coloured halo around the glyph        */
    SLUI_FX_LIGHT = 3,        /* a directional sheen lighting the glyph face  */
    SLUI_FX_EMITTER = 4,      /* the text radiates light into the scene       */
    SLUI_FX_OUTLINE = 5,      /* a stroked edge of a chosen width + colour    */
    SLUI_FX_RELIEF = 6,       /* emboss/engrave the glyph (lighting normals)  */
    SLUI_FX_GRADIENT_FILL = 7 /* fill the glyph with a vertical gradient      */
} SLUIFontEffectType;

/* Relief direction for SLUI_FX_RELIEF (mirrors the Lighting relief idea). */
typedef enum {
    SLUI_FONT_EMBOSSED = 0, /* the text stands OFF the surface               */
    SLUI_FONT_ENGRAVED = 1  /* the text is pressed INTO the surface          */
} SLUIFontRelief;

/* One effect. Only the fields relevant to `type` are read; use the builder
 * helpers below so you never have to fill the whole struct. */
typedef struct {
    SLUIFontEffectType type;
    SLUIColor color;    /* shadow/glow/light/outline/emitter colour           */
    SLUIColor color2;   /* second colour (gradient bottom)                    */
    double dx, dy;      /* shadow offset (px) / light+emitter direction       */
    double blur;        /* shadow/glow softness radius (px)                   */
    double size;        /* outline width / relief depth / emitter radius (px) */
    double intensity;   /* 0..N strength (glow/light/emitter/relief)          */
    double opacity;     /* 0..1 effect opacity                                */
    int relief;         /* SLUIFontRelief for SLUI_FX_RELIEF                  */
    int reserved_anchor;/* EMITTER: -1 => reserve nothing (an emitter);       */
                        /*          >=0 => bind as a SOURCE on that anchor     */
} SLUIFontEffect;

/* Effect builders (fill a sensible default, then tweak the returned struct). */
SLUIFontEffect slui_fx_drop_shadow(double dx, double dy, double blur,
                                   SLUIColor color);
SLUIFontEffect slui_fx_inner_shadow(double dx, double dy, double blur,
                                    SLUIColor color);
SLUIFontEffect slui_fx_glow(double radius, double intensity, SLUIColor color);
SLUIFontEffect slui_fx_light(double dx, double dy, double intensity,
                             SLUIColor color);
/* An emitter: the text radiates light into a scene. anchor<0 keeps it a pure
 * emitter (reserves nothing); anchor>=0 binds it as a light SOURCE on that
 * anchor when applied to a light scene. */
SLUIFontEffect slui_fx_emitter(double radius, double intensity, SLUIColor color,
                               int anchor);
SLUIFontEffect slui_fx_outline(double width, SLUIColor color);
SLUIFontEffect slui_fx_relief(SLUIFontRelief relief, double depth,
                              double intensity);
SLUIFontEffect slui_fx_gradient_fill(SLUIColor top, SLUIColor bottom);

/* --------------------------------------------------------------------------
 * Font handle.
 * ------------------------------------------------------------------------- */
typedef struct SLUIFont SLUIFont;

/* Create a font. `family` may be a real face name or "system"/"mono". */
SLUIFont *slui_font_create(const char *family, double size_pt);
void slui_font_destroy(SLUIFont *font);
SLUIFont *slui_font_clone(const SLUIFont *font); /* copy incl. effect stack   */

/* Face attributes. */
void slui_font_set_family(SLUIFont *font, const char *family);
void slui_font_set_size(SLUIFont *font, double size_pt);
void slui_font_set_weight(SLUIFont *font, SLUIFontWeight weight);
void slui_font_set_slant(SLUIFont *font, SLUIFontSlant slant);
void slui_font_set_letter_spacing(SLUIFont *font, double px);
void slui_font_set_line_height(SLUIFont *font, double multiple); /* x em      */
void slui_font_set_quality(SLUIFont *font, SLUIFontQuality quality);

double slui_font_size(const SLUIFont *font);
SLUIFontQuality slui_font_quality(const SLUIFont *font);

/* Effect stack. Effects render back-to-front in the order added. */
void slui_font_add_effect(SLUIFont *font, SLUIFontEffect effect);
void slui_font_clear_effects(SLUIFont *font);
int slui_font_effect_count(const SLUIFont *font);

/* --------------------------------------------------------------------------
 * Measuring and drawing.
 * ------------------------------------------------------------------------- */
/* Advance width of `utf8` in this font (letter spacing included), in pixels. */
double slui_font_measure(SLUIFont *font, const char *utf8);
/* Ascent / descent / line height (px) for the current face + size. */
double slui_font_ascent(SLUIFont *font);
double slui_font_descent(SLUIFont *font);
double slui_font_line_height(SLUIFont *font);

/* Draw `utf8` into `dc` with the full effect stack, pen origin at (x, baseline)
 * and the base fill colour `color` (a GRADIENT_FILL effect overrides it). */
void slui_font_draw(SLUIDrawContext *dc, SLUIFont *font, const char *utf8, int x,
                    int baseline, SLUIColor color);

/* Draw into a rect with horizontal alignment (0 start, 1 center, 2 end) and
 * vertical centering -- the usual label path. */
void slui_font_draw_in(SLUIDrawContext *dc, SLUIFont *font, const char *utf8,
                       SLUIRect area, int halign, SLUIColor color);

/* If the font carries an EMITTER effect, register that emission into `scene`
 * for the text drawn at (x, baseline). A pure emitter (anchor<0) adds an
 * emitter that reserves nothing; an anchored one adds a light source reserving
 * its anchor. Returns the light index, SLUI_ERR_* , or 1 if the font has no
 * emitter effect. The scene type is the opaque SLUILightScene from
 * sleela_ui_light.h (passed as void* to keep this header independent). */
int slui_font_emit_into_scene(SLUIFont *font, void *light_scene, int x,
                              int baseline, const char *utf8);

/* --------------------------------------------------------------------------
 * Widget integration. Give any text widget a styled font so its label renders
 * with the font's effects. The widget borrows the font (host keeps ownership);
 * pass NULL to revert to the theme's plain text.
 * ------------------------------------------------------------------------- */
void slui_widget_set_font(SLUIWidget *widget, SLUIFont *font);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_FONT_H */
