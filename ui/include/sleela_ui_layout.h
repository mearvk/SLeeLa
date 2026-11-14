#ifndef SLEELA_UI_LAYOUT_H
#define SLEELA_UI_LAYOUT_H
/* =============================================================================
 * SleelaUI(TM) Layout & Flow manager -- named items, named groups, standardized
 * units, and several flow solutions.
 *
 * This layer sits above the widget tree (sleela_ui.h). It treats a UI as an
 * ORTHOGONAL CONFLATION of two independent concerns:
 *
 *   * UNIT PARTS -- what each item measures: a size/mass expressed in a single
 *     standardized unit vocabulary that spans US customary, Eurasian metric, and
 *     device pixels.
 *   * ORGANIZATIONS -- how those parts are arranged: items are collected into
 *     named GROUPS, and a group is placed by a chosen FLOW (several solutions,
 *     below). The same item can be re-measured without changing its
 *     organization, and re-organized without changing its measure.
 *
 * Everything is NAMEABLE: items have names, groups have names, and you adjust a
 * single named item, a whole named group, or an N-ARY selection (any subset of
 * a group of some size) in one call.
 *
 * Named ergonomics ("the ergonomics of publics shall be Named"): alignment and
 * packing have public, human names -- notably `MEDIUM` means CENTER, and
 * `CENTRAL` means weight/mass (a centre-of-mass gravity packing). A set of named
 * ergonomic presets (reading, gallery, dashboard, console, portrait, ...) tunes
 * spacing, flow, and alignment for a common public use at once.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Standardized units. One vocabulary across US customary, Eurasian metric, and
 * device pixels, plus relative units. A measurement is a (value, unit) pair; it
 * resolves to device pixels through a DPI and, for relative units, a reference
 * length (the available extent along the axis).
 * ------------------------------------------------------------------------- */
typedef enum {
    /* device */
    SLUI_UNIT_PX = 0,     /* device pixels                                   */
    /* Eurasian / metric (SI) */
    SLUI_UNIT_MM = 1,     /* millimetre                                      */
    SLUI_UNIT_CM = 2,     /* centimetre = 10 mm                              */
    SLUI_UNIT_M = 3,      /* metre = 1000 mm                                 */
    /* US customary */
    SLUI_UNIT_IN = 4,     /* inch = 25.4 mm                                  */
    SLUI_UNIT_FT = 5,     /* foot = 12 in                                    */
    SLUI_UNIT_PT = 6,     /* typographic point = 1/72 in                     */
    SLUI_UNIT_PICA = 7,   /* pica = 12 pt = 1/6 in                           */
    /* relative */
    SLUI_UNIT_PCT = 8,    /* percent of the reference extent                 */
    SLUI_UNIT_FR = 9,     /* flexible fraction (a share of leftover space)   */
    SLUI_UNIT_EM = 10     /* multiples of the base font size (em)            */
} SLUIUnitKind;

typedef struct {
    double value;
    SLUIUnitKind unit;
} SLUIMeasure;

/* Builders -- one per unit, so a call site reads like a measurement. */
SLUIMeasure slui_px(double v);
SLUIMeasure slui_mm_u(double v);   /* _u to avoid clashing with mood's slui_mm */
SLUIMeasure slui_cm(double v);
SLUIMeasure slui_m(double v);
SLUIMeasure slui_in(double v);
SLUIMeasure slui_ft(double v);
SLUIMeasure slui_ptm(double v);    /* points as a measure                    */
SLUIMeasure slui_pica(double v);
SLUIMeasure slui_pct(double v);
SLUIMeasure slui_fr(double v);     /* flexible fraction / weight share       */
SLUIMeasure slui_em(double v);

/* Resolve a measure to device pixels. `dpi` (default 96) converts physical
 * units; `reference_px` is the axis extent for PCT; `em_px` is the base font
 * size in px for EM. FR resolves to 0 here (fractions are distributed by the
 * flow from leftover space, not fixed) -- query it with slui_measure_is_flex. */
double slui_measure_to_px(SLUIMeasure m, double dpi, double reference_px,
                          double em_px);
/* True if the measure is a flexible fraction (FR) the flow should distribute. */
int slui_measure_is_flex(SLUIMeasure m);
/* The fraction weight of an FR measure (its `value`), or 0 for non-FR. */
double slui_measure_flex(SLUIMeasure m);

/* Parse a string like "12px", "3mm", "0.5in", "2fr", "50%", "1.5em" into a
 * measure. Returns 1 on success (writes *out), 0 on a parse error. Accepts US
 * ("in","ft","pt","pc"), metric ("mm","cm","m"), and relative units. */
int slui_measure_parse(const char *text, SLUIMeasure *out);

/* --------------------------------------------------------------------------
 * Named ergonomic alignment. MEDIUM is the public name for CENTER; the other
 * names are the public ergonomics of placement.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_PLACE_START = 0,  /* leading edge                                   */
    SLUI_PLACE_MEDIUM = 1, /* CENTER -- "medium implies center"              */
    SLUI_PLACE_END = 2,    /* trailing edge                                  */
    SLUI_PLACE_FILL = 3,   /* stretch to fill                                */
    SLUI_PLACE_CENTRAL = 4 /* weight/mass -- pack toward the centre of mass  */
} SLUIPlacement;

/* --------------------------------------------------------------------------
 * Flow solutions -- several ways a group organizes its items.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_FLOW_STACK = 0,  /* a single row/column (the box model)            */
    SLUI_FLOW_WRAP = 1,   /* a flow that wraps to new lines when full       */
    SLUI_FLOW_GRID = 2,   /* a fixed-column grid                            */
    SLUI_FLOW_DOCK = 3,   /* items docked to edges, the rest fills centre   */
    SLUI_FLOW_CENTRAL = 4 /* weight/mass packing -- heavier items pull in    */
} SLUIFlowKind;

typedef enum {
    SLUI_AXIS_HORIZONTAL = 0,
    SLUI_AXIS_VERTICAL = 1
} SLUIAxis;

/* --------------------------------------------------------------------------
 * The layout manager, its groups, and its items.
 * ------------------------------------------------------------------------- */
typedef struct SLUILayout SLUILayout;

SLUILayout *slui_layout_create(void);
void slui_layout_destroy(SLUILayout *layout);

/* The DPI and base em used to resolve physical/relative units. */
void slui_layout_set_dpi(SLUILayout *layout, double dpi);
void slui_layout_set_em(SLUILayout *layout, double em_px);

/* ---- groups (named) ----------------------------------------------------- */
/* Create a named group with a flow and axis. Returns a group id (>=0), or
 * SLUI_ERR_* . A duplicate name returns the existing group's id. */
int slui_layout_group(SLUILayout *layout, const char *group_name,
                      SLUIFlowKind flow, SLUIAxis axis);
int slui_layout_group_by_name(const SLUILayout *layout, const char *group_name);
int slui_layout_group_count(const SLUILayout *layout);

/* Per-group organization knobs. */
void slui_group_set_flow(SLUILayout *layout, const char *group_name,
                         SLUIFlowKind flow, SLUIAxis axis);
void slui_group_set_spacing(SLUILayout *layout, const char *group_name,
                            SLUIMeasure spacing);
void slui_group_set_padding(SLUILayout *layout, const char *group_name,
                            SLUIMeasure padding);
void slui_group_set_align(SLUILayout *layout, const char *group_name,
                          SLUIPlacement main_align, SLUIPlacement cross_align);
void slui_group_set_columns(SLUILayout *layout, const char *group_name,
                            int columns); /* for GRID / WRAP                 */

/* ---- items (named) ------------------------------------------------------ */
/* Add a named item carrying an optional widget, into a named group. `widget`
 * may be NULL for a pure spacer/placeholder. The item's main-axis size is
 * `size` (a measure; FR makes it flexible). Returns an item id (>=0). */
int slui_layout_item(SLUILayout *layout, const char *group_name,
                     const char *item_name, SLUIWidget *widget,
                     SLUIMeasure size);
int slui_layout_item_by_name(const SLUILayout *layout, const char *item_name);
int slui_group_item_count(const SLUILayout *layout, const char *group_name);

/* --------------------------------------------------------------------------
 * Adjustment -- a single item, a whole group, or an N-ARY selection.
 *
 * The same adjustment verbs work at three granularities, so you can resize one
 * item, re-weight a whole group, or nudge an arbitrary subset of a group of
 * some size. "size" is the main-axis measure; "mass" is the CENTRAL weight that
 * a SLUI_FLOW_CENTRAL / SLUI_PLACE_CENTRAL packing uses.
 * ------------------------------------------------------------------------- */
/* single */
void slui_item_set_size(SLUILayout *layout, const char *item_name,
                        SLUIMeasure size);
void slui_item_set_mass(SLUILayout *layout, const char *item_name, double mass);
void slui_item_set_align(SLUILayout *layout, const char *item_name,
                         SLUIPlacement cross_align);
void slui_item_set_visible(SLUILayout *layout, const char *item_name, int on);

/* group (every item in the named group) */
void slui_group_set_size(SLUILayout *layout, const char *group_name,
                         SLUIMeasure size);
void slui_group_set_mass(SLUILayout *layout, const char *group_name,
                         double mass);
void slui_group_set_item_align(SLUILayout *layout, const char *group_name,
                               SLUIPlacement cross_align);

/* n-ary (a chosen subset of a group) -- pass an array of `count` item names.
 * Adjusts exactly those items, leaving the rest of the group untouched. */
void slui_nary_set_size(SLUILayout *layout, const char *const *item_names,
                        int count, SLUIMeasure size);
void slui_nary_set_mass(SLUILayout *layout, const char *const *item_names,
                        int count, double mass);
void slui_nary_set_align(SLUILayout *layout, const char *const *item_names,
                         int count, SLUIPlacement cross_align);

/* --------------------------------------------------------------------------
 * Named ergonomic presets -- "the ergonomics of publics shall be Named". Each
 * applies a tasteful flow + spacing + alignment to a named group at once, for a
 * common public use.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_ERGO_READING = 0,   /* a calm single column, medium (centered) runs */
    SLUI_ERGO_GALLERY = 1,   /* a wrapping flow of equal tiles               */
    SLUI_ERGO_DASHBOARD = 2, /* a multi-column grid                          */
    SLUI_ERGO_CONSOLE = 3,   /* a dense top-aligned stack                    */
    SLUI_ERGO_PORTRAIT = 4,  /* central (weight/mass) packing, generous space*/
    SLUI_ERGO_TOOLBAR = 5    /* a tight horizontal stack, start-aligned      */
} SLUIErgonomics;

void slui_group_set_ergonomics(SLUILayout *layout, const char *group_name,
                               SLUIErgonomics ergo);

/* --------------------------------------------------------------------------
 * Resolution -- lay the groups out within a device-pixel rectangle and read
 * back each item's resolved rect, so a host can place real widgets. Call after
 * any adjustment. A group with no explicit area is laid out within `area`;
 * nested groups (an item whose name matches a group) recurse.
 * ------------------------------------------------------------------------- */
void slui_layout_arrange(SLUILayout *layout, const char *group_name,
                         SLUIRect area);
/* The resolved rect of a named item (valid after arrange). Returns 1 on
 * success (writes *out), 0 if the item is unknown or not yet arranged. */
int slui_layout_item_rect(const SLUILayout *layout, const char *item_name,
                          SLUIRect *out);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_LAYOUT_H */
