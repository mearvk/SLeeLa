#ifndef SLEELA_UI_MOOD_H
#define SLEELA_UI_MOOD_H
/* =============================================================================
 * SleelaUI(TM) Mood, Wash, Calculus-8 & millimetre light placement.
 *
 * This layer gives the Lighting system (sleela_ui_light.h) three things the
 * design calls for:
 *
 *   1. MILLIMETRE PRECISION. A light/dark/shadow emitter can be placed in real
 *      millimetres relative to the text's font-paint location: a HEIGHT the
 *      light shines down from, plus left/right and up/down offsets. The mm are
 *      resolved to the toolkit's device pixels through a DPI, so the same light
 *      sits at the same physical place on any display. Height, left, right, up
 *      and down are all supported.
 *
 *   2. CALCULUS-8 with 100 DIFFERENTIABLES. A light carries 100 control points
 *      ("differentiables"). A careful 8-stage calculus evaluates them -- eight
 *      smooth, analytically-differentiable basis stages -- and maps the inputs
 *      directly into a MOOD. Because every stage is differentiable, nudging an
 *      input moves the mood smoothly (no jumps), and the API returns both the
 *      value and its derivative so an adjuster can follow the slope.
 *
 *   3. MOODS from EXCELLENT WASHES. A mood is a named, adjustable state whose
 *      colour comes from an Excellent Wash: a soft, multi-stop graded colour
 *      field. Light colours are known and adjustable; the wash is how a mood
 *      paints the light. A mood also carries a NATURAL TANOR -- a soft,
 *      portrait-quality ambient glow for "lightless bulbs" (light-of-effect with
 *      no hard source) -- which has its own REFRESH on the light pattern and a
 *      gentle "a little left" bias in its freshing pattern.
 *
 * Everything composes with the existing SLUILight / SLUILightScene: you build a
 * light, place it in mm, give it a mood (wash + tanor + a calculus), and add it
 * to a scene as a source or an emitter exactly as before.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "sleela_ui_light.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Millimetre placement.
 * ------------------------------------------------------------------------- */
/* A light position in millimetres relative to a reference point (the text's
 * font-paint origin). +right / +down / +height-above-the-plane. left and up are
 * negative right/down. `height_mm` is how far above the surface the light sits
 * (it "shines down" from there). */
typedef struct {
    double right_mm;  /* +right, -left                                        */
    double down_mm;   /* +down, -up                                           */
    double height_mm; /* above the plane the light shines down from           */
} SLUIMm;

/* Convenience builders. */
SLUIMm slui_mm(double right_mm, double down_mm, double height_mm);
SLUIMm slui_mm_left(double mm, double height_mm);  /* shift left by mm        */
SLUIMm slui_mm_right(double mm, double height_mm);
SLUIMm slui_mm_up(double mm, double height_mm);
SLUIMm slui_mm_down(double mm, double height_mm);

/* Convert millimetres to device pixels at `dpi` (dots per inch; 96 is the UI
 * default). 1 inch = 25.4 mm. */
double slui_mm_to_px(double mm, double dpi);

/* Place `light` at `mm` relative to the font-paint origin (ref_x, ref_y) in
 * device pixels, at `dpi`. Sets the light's x/y (the ground point directly
 * under it) and z (the height it shines down from), so a point light at
 * height_mm genuinely grazes the text from that elevation, offset left/right
 * and up/down by the mm. Returns the modified light (also writes through the
 * pointer if non-NULL). */
SLUILight slui_light_place_mm(SLUILight *light, double ref_x, double ref_y,
                              SLUIMm mm, double dpi);

/* --------------------------------------------------------------------------
 * Excellent Wash: a soft, multi-stop graded colour field that paints a mood.
 * Up to 8 stops (position 0..1 -> colour); the wash is sampled smoothly
 * (smoothstep between stops) so it reads as a gentle wash, not hard bands.
 * ------------------------------------------------------------------------- */
#define SLUI_WASH_MAX_STOPS 8

typedef struct {
    int count;
    double pos[SLUI_WASH_MAX_STOPS];     /* ascending in [0,1]                */
    SLUIColor color[SLUI_WASH_MAX_STOPS];
    double softness;                     /* 0..1 extra blur between stops     */
} SLUIWash;

SLUIWash slui_wash_begin(void);
/* Add a stop; returns the wash by value so calls can chain through a variable.
 * Stops may be added out of order; sampling sorts by position. */
SLUIWash slui_wash_stop(SLUIWash wash, double position, SLUIColor color);
/* Two- and three-stop shortcuts for the common washes. */
SLUIWash slui_wash_linear(SLUIColor a, SLUIColor b);
SLUIWash slui_wash_triad(SLUIColor a, SLUIColor b, SLUIColor c);
/* Sample the wash at t in [0,1] -> colour (smoothstep-interpolated). */
SLUIColor slui_wash_sample(const SLUIWash *wash, double t);

/* --------------------------------------------------------------------------
 * Natural Tanor: a soft portrait-quality ambient glow for "lightless bulbs"
 * -- a light-of-effect with no hard source. It has its own refresh on the light
 * pattern and a gentle leftward ("a little left") bias in the freshing pattern.
 * ------------------------------------------------------------------------- */
typedef struct {
    double warmth;        /* 0 cool .. 1 warm -- the glow's temperament       */
    double diffusion;     /* 0..1 how soft/portrait the falloff is            */
    double refresh_hz;    /* how often the pattern freshens (Hz); 0 = static  */
    double left_bias;     /* 0..1 "a little left" drift of the pattern        */
    double depth;         /* 0..1 how deep the lightless glow reaches         */
    SLUIColor tone;       /* the tanor's base tone                            */
} SLUITanor;

/* A sensible portrait tanor (warm, soft, gently refreshing, a little left). */
SLUITanor slui_tanor_default(void);

/* --------------------------------------------------------------------------
 * Calculus-8: 100 differentiables evaluated through 8 careful stages into a
 * mood scalar. The 100 inputs are an owned vector; each is a "differentiable"
 * the host can nudge. The 8 stages are smooth basis functions (polynomial,
 * trigonometric, logistic, gaussian, ...) summed with learned-feel weights, so
 * the mapping inputs -> mood is continuous and differentiable everywhere.
 * ------------------------------------------------------------------------- */
#define SLUI_CALC8_INPUTS 100
#define SLUI_CALC8_STAGES 8

typedef struct SLUICalculus8 SLUICalculus8;

SLUICalculus8 *slui_calc8_create(void);
void slui_calc8_destroy(SLUICalculus8 *c);

/* Set/get one of the 100 differentiable inputs (index 0..99), each in [0,1].
 * Out-of-range indices are ignored / return 0. */
void slui_calc8_set(SLUICalculus8 *c, int index, double value);
double slui_calc8_get(const SLUICalculus8 *c, int index);
/* Set all 100 inputs from an array (any count up to 100). */
void slui_calc8_set_all(SLUICalculus8 *c, const double *values, int count);

/* Evaluate the calculus: returns the mood scalar in [0,1]. If `derivative` is
 * non-NULL it receives d(mood)/d(input) for input `wrt_index` (the slope along
 * that differentiable), so an adjuster can follow the gradient into a mood. */
double slui_calc8_eval(const SLUICalculus8 *c, int wrt_index,
                       double *derivative);

/* The per-stage contributions (SLUI_CALC8_STAGES values) for inspection, so a
 * tool can see how the eight stages shaped the mood. Writes up to `cap`. */
int slui_calc8_stages(const SLUICalculus8 *c, double *out, int cap);

/* --------------------------------------------------------------------------
 * Mood: a named, adjustable state = a wash + a tanor + a calculus. Applying a
 * mood to a light sets its colour (sampled from the wash at the calculus's mood
 * scalar) and folds the tanor's warmth/depth into the light, so "inputs ->
 * direct moods" is one call.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_MOOD_CALM = 0,
    SLUI_MOOD_WARM = 1,
    SLUI_MOOD_COOL = 2,
    SLUI_MOOD_VIVID = 3,
    SLUI_MOOD_SOMBER = 4,
    SLUI_MOOD_TENDER = 5,
    SLUI_MOOD_RADIANT = 6,
    SLUI_MOOD_CUSTOM = 7
} SLUIMoodKind;

typedef struct SLUIMood SLUIMood;

/* Create a mood. A built-in kind pre-fills a tasteful wash + tanor; CUSTOM
 * starts empty for the host to set. */
SLUIMood *slui_mood_create(SLUIMoodKind kind);
void slui_mood_destroy(SLUIMood *mood);

void slui_mood_set_wash(SLUIMood *mood, SLUIWash wash);
void slui_mood_set_tanor(SLUIMood *mood, SLUITanor tanor);
/* The mood borrows the calculus (host keeps ownership); its eval drives where
 * the wash is sampled. Pass NULL to use a fixed midpoint. */
void slui_mood_set_calculus(SLUIMood *mood, const SLUICalculus8 *calc);

SLUIWash slui_mood_wash(const SLUIMood *mood);
SLUITanor slui_mood_tanor(const SLUIMood *mood);

/* The mood's current colour: the wash sampled at the calculus's mood scalar
 * (advanced by `time_s` through the tanor's refresh + left bias so a lightless
 * bulb's glow freshens over time). */
SLUIColor slui_mood_color(const SLUIMood *mood, double time_s);

/* Apply the mood to a light: set the light's colour from the mood (at time_s)
 * and fold the tanor's depth into the light's intensity/softness so the light
 * carries the mood's portrait quality. */
void slui_light_apply_mood(SLUILight *light, const SLUIMood *mood,
                           double time_s);

/* --------------------------------------------------------------------------
 * Convenience: build a mm-placed, mood-coloured light for text in one call.
 * The light shines down on text painted at (font_x, font_baseline) from
 * `mm` away, coloured by `mood` at `time_s`. role/polarity choose light vs
 * shadow and source vs emitter.
 * ------------------------------------------------------------------------- */
SLUILight slui_light_for_text(double font_x, double font_baseline, SLUIMm mm,
                              double dpi, const SLUIMood *mood, double time_s,
                              SLUILightRole role, SLUIPolarity polarity,
                              double reach_px, double intensity);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_MOOD_H */
