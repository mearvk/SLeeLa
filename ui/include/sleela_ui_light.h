#ifndef SLEELA_UI_LIGHT_H
#define SLEELA_UI_LIGHT_H
/* =============================================================================
 * SleelaUI(TM) Lighting & Shadow API -- quality light, darkness, and relief for
 * Canvas-aware objects.
 *
 * This layer sits on top of the Draw API (sleela_ui_draw.h). It lets a developer
 * or agent light the pixels of a draw context the way a scene is lit: place
 * lights, give a drawn object a MATERIAL with a relief (height) profile, and the
 * renderer shades it -- directional highlights and the recessed/raised look of
 * genuine relief, plus soft CAST SHADOWS with real penumbrae and ambient
 * occlusion in the creases. We spend on the quality of both the LIGHT and the
 * DARKNESS: smooth physically-flavoured falloff, multi-sample soft shadows, a
 * specular sheen, and shadow that is itself emitted (not merely "absence of
 * light") so relief reads crisply even on the deep warm #2B1608 base.
 *
 * Two ways a light exists in a scene -- the key distinction you asked for:
 *
 *   * A SOURCE is bound to (reserves) an anchor object: that object IS the light
 *     and is marked USED as a source -- only one source may reserve a given
 *     anchor. Reach for a source when a specific widget/shape should literally
 *     be the thing glowing (or casting dark), e.g. a lit button is its own
 *     light source.
 *
 *   * An EMITTER is a source WITHOUT reserving the thing itself as used: it
 *     radiates light or shadow from a position/direction but owns no object and
 *     consumes nothing, so any number of emitters can share the scene and be
 *     reused freely. Reach for an emitter for ambient fills, a moving sun, a
 *     torch you pass around, or darkness poured into a region.
 *
 * BOTH light and shadow can come from either a source or an emitter: a light's
 * POLARITY decides whether it brightens (light) or darkens (shadow/umbra). A
 * shadow-emitter is simply an emitter with negative polarity -- darkness treated
 * as a first-class emission, which is what gives the relief its depth.
 *
 * Colours are the same 0xRRGGBBAA words as the rest of the toolkit.
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
 * Light kind, role, and polarity.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_LIGHT_POINT = 0,      /* radiates from (x,y) with a radius + falloff  */
    SLUI_LIGHT_DIRECTIONAL = 1,/* parallel rays from a direction (a "sun")     */
    SLUI_LIGHT_AMBIENT = 2     /* uniform fill, no position (base illumination)*/
} SLUILightKind;

typedef enum {
    SLUI_LIGHT_SOURCE = 0,  /* reserves its anchor object; the object is USED  */
    SLUI_LIGHT_EMITTER = 1  /* free-standing; reserves nothing, reusable       */
} SLUILightRole;

typedef enum {
    SLUI_POLARITY_LIGHT = 0,  /* brightens: a light                            */
    SLUI_POLARITY_SHADOW = 1  /* darkens: a shadow / umbra emission            */
} SLUIPolarity;

/* A single light. For a POINT light (x,y) is the position and `radius` its
 * reach; for DIRECTIONAL (dx,dy) is the unit direction the rays travel; for
 * AMBIENT only colour + intensity matter. `softness` (0..1) widens the penumbra
 * of cast shadows. `z` is the light's height above the canvas plane and shapes
 * how grazing the relief lighting is (small z = long dramatic relief). */
typedef struct {
    SLUILightKind kind;
    SLUILightRole role;
    SLUIPolarity polarity;
    double x, y;        /* point position (device px)                         */
    double z;           /* height above the plane (relief grazing)            */
    double dx, dy;      /* directional unit vector                            */
    double radius;      /* point reach in px (0 => infinite)                  */
    double intensity;   /* 0..N multiplier on the emission                    */
    double softness;    /* 0..1 penumbra width for cast shadows               */
    SLUIColor color;    /* emission colour (shadow colour when polarity SHADOW)*/
    int anchor;         /* for a SOURCE: the reserved anchor id (>=0); -1 none */
} SLUILight;

/* Convenience initialisers (fill a sensible default then set what you need). */
SLUILight slui_light_point(double x, double y, double radius, double intensity,
                           SLUIColor color);
SLUILight slui_light_directional(double dx, double dy, double intensity,
                                 SLUIColor color);
SLUILight slui_light_ambient(double intensity, SLUIColor color);
/* A shadow (negative-polarity) variant of a point emission. */
SLUILight slui_shadow_point(double x, double y, double radius, double intensity,
                            SLUIColor shadow_color);

/* --------------------------------------------------------------------------
 * Light scene: a collection of lights applied to a draw context. The scene also
 * owns the SOURCE RESERVATION table so an anchor can be used as a source at most
 * once; emitters never touch it.
 * ------------------------------------------------------------------------- */
typedef struct SLUILightScene SLUILightScene;

SLUILightScene *slui_light_scene_create(void);
void slui_light_scene_destroy(SLUILightScene *scene);
void slui_light_scene_clear(SLUILightScene *scene);

/* Add an EMITTER: reserves nothing, always succeeds, returns its index (>=0).
 * The same emitter struct may be added to many scenes and reused freely. */
int slui_light_scene_add_emitter(SLUILightScene *scene, SLUILight light);

/* Add a SOURCE bound to `anchor`: reserves that anchor so it is USED as a
 * source. Returns the light index (>=0) on success, or SLUI_ERR_BACKEND if the
 * anchor is already reserved by another source in this scene. `light.role` and
 * `light.anchor` are set for you. An anchor is any host-chosen id (e.g. a widget
 * handle cast to int) that names the object the light reserves. */
int slui_light_scene_add_source(SLUILightScene *scene, int anchor,
                                SLUILight light);

/* True if `anchor` is currently reserved as a source in this scene. */
int slui_light_scene_anchor_reserved(const SLUILightScene *scene, int anchor);
/* Release a source's reservation (and remove the light) so the anchor is free
 * to be used as a source again. Emitters are unaffected. */
void slui_light_scene_release_source(SLUILightScene *scene, int anchor);

int slui_light_scene_count(const SLUILightScene *scene);

/* The scene's base ambient illumination applied before any light (so even an
 * unlit object is not pure black). Defaults to a dim warm fill suited to the
 * #2B1608 base; set it to taste. */
void slui_light_scene_set_ambient(SLUILightScene *scene, double intensity,
                                  SLUIColor color);

/* --------------------------------------------------------------------------
 * Material: how a drawn object catches light. The RELIEF is a height field in
 * [0,1] the lighting uses to compute surface normals, which is what produces
 * the raised/recessed "quality relief". A material also carries how matte vs
 * glossy the surface is (specular sheen) and how much ambient occlusion to pool
 * in its creases.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_RELIEF_FLAT = 0,      /* no height: pure shading, no bevel            */
    SLUI_RELIEF_ROUNDED = 1,   /* a smooth dome/pillow -- soft raised look     */
    SLUI_RELIEF_BEVEL = 2,     /* a chamfered edge -- a crisp raised panel     */
    SLUI_RELIEF_ENGRAVED = 3,  /* inset: the shape reads as pressed INTO the floor*/
    SLUI_RELIEF_EMBOSSED = 4   /* raised: the shape reads as standing OFF the floor*/
} SLUIReliefProfile;

typedef struct {
    SLUIReliefProfile profile;
    double depth;        /* relief height in px (how pronounced the relief is) */
    double gloss;        /* 0 matte .. 1 mirror -- specular sheen strength     */
    double occlusion;    /* 0..1 ambient-occlusion darkening in the creases    */
    double edge_px;      /* width of the bevel/edge band in px                 */
} SLUIMaterial;

SLUIMaterial slui_material(SLUIReliefProfile profile, double depth);

/* --------------------------------------------------------------------------
 * Shading. These composite the scene's light and shadow over what is already
 * drawn in `dc`, within `area`, giving the pixels relief and cast shadows.
 *
 * Typical use for a Canvas-aware object:
 *   1. draw the object's base shape into dc (fill, text, ...);
 *   2. slui_light_apply_relief(dc, area, radius, material, scene) to light it
 *      and emboss/engrave it;
 *   3. optionally slui_light_cast_shadow(...) to drop a soft shadow beneath it.
 * ------------------------------------------------------------------------- */

/* Light and relieve a rounded-rect region already filled in `dc`. The renderer
 * builds a height field for `material.profile` over the rounded rect, derives
 * normals, and for every pixel accumulates each light's contribution (diffuse +
 * specular for light polarity; darkening for shadow polarity), plus ambient and
 * ambient-occlusion. The result is composited back, so the object gains genuine
 * directional relief and sheen. */
void slui_light_apply_relief(SLUIDrawContext *dc, SLUIRect area, double radius,
                             SLUIMaterial material,
                             const SLUILightScene *scene);

/* Cast a soft shadow of a rounded-rect occluder onto the area beneath it, using
 * every light in the scene (each light throws the occluder's shadow away from
 * itself, with a penumbra scaled by the light's softness and the occluder's
 * height `lift`). Shadow emitters (negative polarity) deepen it. Draw this
 * BEFORE the occluder's own fill so the object sits on top of its shadow. */
void slui_light_cast_shadow(SLUIDrawContext *dc, SLUIRect occluder,
                            double radius, double lift,
                            const SLUILightScene *scene);

/* A one-call convenience: fill a rounded rect with `base` colour, drop its cast
 * shadow, and apply relief lighting -- the usual "a lit, raised panel" in one
 * step. Returns nothing; everything lands in `dc`. */
void slui_light_panel(SLUIDrawContext *dc, SLUIRect area, double radius,
                      SLUIColor base, SLUIMaterial material,
                      const SLUILightScene *scene);

/* --------------------------------------------------------------------------
 * Widget integration. A Canvas View / Throbber (sleela_ui.h) can be told to run
 * its painting through a light scene so the whole widget is lit and relieved by
 * the toolkit automatically, each frame.
 * ------------------------------------------------------------------------- */
/* Attach a scene to a canvas-aware widget. The widget borrows the scene (the
 * host keeps ownership) and lights its surface each frame. Pass NULL to detach.
 * `material` describes the widget's own relief. */
void slui_widget_set_light_scene(SLUIWidget *widget, const SLUILightScene *scene,
                                 SLUIMaterial material);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_LIGHT_H */
