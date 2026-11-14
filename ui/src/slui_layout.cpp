/* =============================================================================
 * SleelaUI Layout & Flow manager implementation.
 *
 * Backs sleela_ui_layout.h. A layout is a set of named GROUPS, each holding
 * named ITEMS. A unit part (an item's measure/mass) is orthogonal to its
 * organization (which group, which flow), so the two are stored and adjusted
 * independently. Five flow solutions arrange a group's items:
 *
 *   STACK   -- one row/column (the box model).
 *   WRAP    -- a flow that wraps to new lines when the extent is exceeded.
 *   GRID    -- a fixed-column grid of equal cells.
 *   DOCK    -- the first/last items dock to the leading/trailing edges, the
 *              rest fills the centre.
 *   CENTRAL -- weight/mass packing: items are sized, then the block is placed
 *              about its centre of mass so heavier items pull toward centre.
 *
 * Units standardize across US customary (in/ft/pt/pica), Eurasian metric
 * (mm/cm/m), device px, and relative (pct/fr/em). Everything resolves to pixels
 * through a DPI (and a reference extent for pct, the em size for em). FR is a
 * flexible fraction the flow distributes from leftover space.
 *
 * Named ergonomics: MEDIUM == center; CENTRAL == weight/mass. The preset enum
 * names common public arrangements.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui_layout.h"

#include "slui_geometry.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using slui::Rect;

/* ==========================================================================
 * Units
 * ========================================================================== */
extern "C" {

static SLUIMeasure M(double v, SLUIUnitKind u) {
    SLUIMeasure m;
    m.value = v;
    m.unit = u;
    return m;
}
SLUIMeasure slui_px(double v) { return M(v, SLUI_UNIT_PX); }
SLUIMeasure slui_mm_u(double v) { return M(v, SLUI_UNIT_MM); }
SLUIMeasure slui_cm(double v) { return M(v, SLUI_UNIT_CM); }
SLUIMeasure slui_m(double v) { return M(v, SLUI_UNIT_M); }
SLUIMeasure slui_in(double v) { return M(v, SLUI_UNIT_IN); }
SLUIMeasure slui_ft(double v) { return M(v, SLUI_UNIT_FT); }
SLUIMeasure slui_ptm(double v) { return M(v, SLUI_UNIT_PT); }
SLUIMeasure slui_pica(double v) { return M(v, SLUI_UNIT_PICA); }
SLUIMeasure slui_pct(double v) { return M(v, SLUI_UNIT_PCT); }
SLUIMeasure slui_fr(double v) { return M(v, SLUI_UNIT_FR); }
SLUIMeasure slui_em(double v) { return M(v, SLUI_UNIT_EM); }

double slui_measure_to_px(SLUIMeasure m, double dpi, double reference_px,
                          double em_px) {
    if (dpi <= 0) dpi = 96.0;
    const double mm_per_in = 25.4;
    double px_per_mm = dpi / mm_per_in;
    switch (m.unit) {
    case SLUI_UNIT_PX: return m.value;
    case SLUI_UNIT_MM: return m.value * px_per_mm;
    case SLUI_UNIT_CM: return m.value * 10.0 * px_per_mm;
    case SLUI_UNIT_M: return m.value * 1000.0 * px_per_mm;
    case SLUI_UNIT_IN: return m.value * dpi;
    case SLUI_UNIT_FT: return m.value * 12.0 * dpi;
    case SLUI_UNIT_PT: return m.value * dpi / 72.0;
    case SLUI_UNIT_PICA: return m.value * dpi / 6.0;
    case SLUI_UNIT_PCT: return m.value / 100.0 * reference_px;
    case SLUI_UNIT_EM: return m.value * (em_px > 0 ? em_px : 16.0);
    case SLUI_UNIT_FR: return 0.0; /* distributed by the flow, not fixed */
    default: return m.value;
    }
}
int slui_measure_is_flex(SLUIMeasure m) { return m.unit == SLUI_UNIT_FR ? 1 : 0; }
double slui_measure_flex(SLUIMeasure m) {
    return m.unit == SLUI_UNIT_FR ? m.value : 0.0;
}

int slui_measure_parse(const char* text, SLUIMeasure* out) {
    if (!text || !out) return 0;
    char* end = nullptr;
    double v = std::strtod(text, &end);
    if (end == text) return 0;
    while (*end && std::isspace(static_cast<unsigned char>(*end))) ++end;
    std::string u(end);
    for (auto& c : u) c = static_cast<char>(std::tolower((unsigned char)c));
    SLUIUnitKind k;
    if (u.empty() || u == "px") k = SLUI_UNIT_PX;
    else if (u == "mm") k = SLUI_UNIT_MM;
    else if (u == "cm") k = SLUI_UNIT_CM;
    else if (u == "m") k = SLUI_UNIT_M;
    else if (u == "in") k = SLUI_UNIT_IN;
    else if (u == "ft") k = SLUI_UNIT_FT;
    else if (u == "pt") k = SLUI_UNIT_PT;
    else if (u == "pc" || u == "pica") k = SLUI_UNIT_PICA;
    else if (u == "%" || u == "pct") k = SLUI_UNIT_PCT;
    else if (u == "fr") k = SLUI_UNIT_FR;
    else if (u == "em") k = SLUI_UNIT_EM;
    else return 0;
    out->value = v;
    out->unit = k;
    return 1;
}

} // extern "C"

/* ==========================================================================
 * Model
 * ========================================================================== */
struct LayoutItem {
    std::string name;
    SLUIWidget* widget = nullptr;
    SLUIMeasure size{0, SLUI_UNIT_FR}; /* default: flexible */
    double mass = 1.0;                 /* CENTRAL weight */
    SLUIPlacement cross_align = SLUI_PLACE_FILL;
    bool visible = true;
    Rect rect{};
    bool arranged = false;
};

struct LayoutGroup {
    std::string name;
    SLUIFlowKind flow = SLUI_FLOW_STACK;
    SLUIAxis axis = SLUI_AXIS_VERTICAL;
    SLUIMeasure spacing{8, SLUI_UNIT_PX};
    SLUIMeasure padding{0, SLUI_UNIT_PX};
    SLUIPlacement main_align = SLUI_PLACE_START;
    SLUIPlacement cross_align = SLUI_PLACE_FILL;
    int columns = 3;
    std::vector<LayoutItem> items;
};

struct SLUILayout {
    double dpi = 96.0;
    double em_px = 16.0;
    std::vector<LayoutGroup> groups;

    LayoutGroup* find_group(const std::string& n) {
        for (auto& g : groups)
            if (g.name == n) return &g;
        return nullptr;
    }
    /* find an item by name across all groups */
    LayoutItem* find_item(const std::string& n, LayoutGroup** owner = nullptr) {
        for (auto& g : groups)
            for (auto& it : g.items)
                if (it.name == n) {
                    if (owner) *owner = &g;
                    return &it;
                }
        return nullptr;
    }
};

/* ==========================================================================
 * Lifecycle, groups, items
 * ========================================================================== */
extern "C" {

SLUILayout* slui_layout_create(void) { return new SLUILayout(); }
void slui_layout_destroy(SLUILayout* l) { delete l; }
void slui_layout_set_dpi(SLUILayout* l, double dpi) {
    if (l && dpi > 0) l->dpi = dpi;
}
void slui_layout_set_em(SLUILayout* l, double em_px) {
    if (l && em_px > 0) l->em_px = em_px;
}

int slui_layout_group(SLUILayout* l, const char* name, SLUIFlowKind flow,
                      SLUIAxis axis) {
    if (!l || !name) return SLUI_ERR_INVALID;
    std::string n(name);
    for (size_t i = 0; i < l->groups.size(); ++i)
        if (l->groups[i].name == n) return static_cast<int>(i);
    LayoutGroup g;
    g.name = n;
    g.flow = flow;
    g.axis = axis;
    l->groups.push_back(std::move(g));
    return static_cast<int>(l->groups.size() - 1);
}
int slui_layout_group_by_name(const SLUILayout* l, const char* name) {
    if (!l || !name) return SLUI_ERR_INVALID;
    std::string n(name);
    for (size_t i = 0; i < l->groups.size(); ++i)
        if (l->groups[i].name == n) return static_cast<int>(i);
    return SLUI_ERR_INVALID;
}
int slui_layout_group_count(const SLUILayout* l) {
    return l ? static_cast<int>(l->groups.size()) : 0;
}

void slui_group_set_flow(SLUILayout* l, const char* name, SLUIFlowKind flow,
                         SLUIAxis axis) {
    if (!l) return;
    if (auto* g = l->find_group(name)) {
        g->flow = flow;
        g->axis = axis;
    }
}
void slui_group_set_spacing(SLUILayout* l, const char* name, SLUIMeasure sp) {
    if (!l) return;
    if (auto* g = l->find_group(name)) g->spacing = sp;
}
void slui_group_set_padding(SLUILayout* l, const char* name, SLUIMeasure pad) {
    if (!l) return;
    if (auto* g = l->find_group(name)) g->padding = pad;
}
void slui_group_set_align(SLUILayout* l, const char* name, SLUIPlacement main_a,
                          SLUIPlacement cross_a) {
    if (!l) return;
    if (auto* g = l->find_group(name)) {
        g->main_align = main_a;
        g->cross_align = cross_a;
    }
}
void slui_group_set_columns(SLUILayout* l, const char* name, int columns) {
    if (!l) return;
    if (auto* g = l->find_group(name)) g->columns = columns < 1 ? 1 : columns;
}

int slui_layout_item(SLUILayout* l, const char* group_name,
                     const char* item_name, SLUIWidget* widget,
                     SLUIMeasure size) {
    if (!l || !group_name || !item_name) return SLUI_ERR_INVALID;
    LayoutGroup* g = l->find_group(group_name);
    if (!g) {
        int id = slui_layout_group(l, group_name, SLUI_FLOW_STACK,
                                   SLUI_AXIS_VERTICAL);
        if (id < 0) return id;
        g = &l->groups[id];
    }
    LayoutItem it;
    it.name = item_name;
    it.widget = widget;
    it.size = size;
    g->items.push_back(std::move(it));
    return static_cast<int>(g->items.size() - 1);
}
int slui_layout_item_by_name(const SLUILayout* l, const char* item_name) {
    if (!l || !item_name) return SLUI_ERR_INVALID;
    std::string n(item_name);
    for (const auto& g : l->groups)
        for (size_t i = 0; i < g.items.size(); ++i)
            if (g.items[i].name == n) return static_cast<int>(i);
    return SLUI_ERR_INVALID;
}
int slui_group_item_count(const SLUILayout* l, const char* group_name) {
    if (!l) return 0;
    for (const auto& g : l->groups)
        if (g.name == group_name) return static_cast<int>(g.items.size());
    return 0;
}

/* ==========================================================================
 * Adjustment -- single / group / n-ary
 * ========================================================================== */
void slui_item_set_size(SLUILayout* l, const char* name, SLUIMeasure size) {
    if (!l) return;
    if (auto* it = l->find_item(name)) it->size = size;
}
void slui_item_set_mass(SLUILayout* l, const char* name, double mass) {
    if (!l) return;
    if (auto* it = l->find_item(name)) it->mass = mass < 0 ? 0 : mass;
}
void slui_item_set_align(SLUILayout* l, const char* name, SLUIPlacement a) {
    if (!l) return;
    if (auto* it = l->find_item(name)) it->cross_align = a;
}
void slui_item_set_visible(SLUILayout* l, const char* name, int on) {
    if (!l) return;
    if (auto* it = l->find_item(name)) it->visible = on != 0;
}

void slui_group_set_size(SLUILayout* l, const char* group_name, SLUIMeasure s) {
    if (!l) return;
    if (auto* g = l->find_group(group_name))
        for (auto& it : g->items) it.size = s;
}
void slui_group_set_mass(SLUILayout* l, const char* group_name, double mass) {
    if (!l) return;
    if (auto* g = l->find_group(group_name))
        for (auto& it : g->items) it.mass = mass < 0 ? 0 : mass;
}
void slui_group_set_item_align(SLUILayout* l, const char* group_name,
                               SLUIPlacement a) {
    if (!l) return;
    if (auto* g = l->find_group(group_name))
        for (auto& it : g->items) it.cross_align = a;
}

void slui_nary_set_size(SLUILayout* l, const char* const* names, int count,
                        SLUIMeasure s) {
    if (!l || !names) return;
    for (int i = 0; i < count; ++i)
        if (names[i])
            if (auto* it = l->find_item(names[i])) it->size = s;
}
void slui_nary_set_mass(SLUILayout* l, const char* const* names, int count,
                        double mass) {
    if (!l || !names) return;
    for (int i = 0; i < count; ++i)
        if (names[i])
            if (auto* it = l->find_item(names[i])) it->mass = mass < 0 ? 0 : mass;
}
void slui_nary_set_align(SLUILayout* l, const char* const* names, int count,
                         SLUIPlacement a) {
    if (!l || !names) return;
    for (int i = 0; i < count; ++i)
        if (names[i])
            if (auto* it = l->find_item(names[i])) it->cross_align = a;
}

/* ==========================================================================
 * Ergonomics -- named public presets
 * ========================================================================== */
void slui_group_set_ergonomics(SLUILayout* l, const char* group_name,
                               SLUIErgonomics ergo) {
    if (!l) return;
    LayoutGroup* g = l->find_group(group_name);
    if (!g) return;
    switch (ergo) {
    case SLUI_ERGO_READING:
        g->flow = SLUI_FLOW_STACK;
        g->axis = SLUI_AXIS_VERTICAL;
        g->spacing = slui_px(12);
        g->padding = slui_px(16);
        g->main_align = SLUI_PLACE_START;
        g->cross_align = SLUI_PLACE_MEDIUM; /* centered runs */
        break;
    case SLUI_ERGO_GALLERY:
        g->flow = SLUI_FLOW_WRAP;
        g->axis = SLUI_AXIS_HORIZONTAL;
        g->spacing = slui_px(10);
        g->padding = slui_px(12);
        g->main_align = SLUI_PLACE_START;
        g->cross_align = SLUI_PLACE_START;
        break;
    case SLUI_ERGO_DASHBOARD:
        g->flow = SLUI_FLOW_GRID;
        g->axis = SLUI_AXIS_HORIZONTAL;
        g->columns = 3;
        g->spacing = slui_px(12);
        g->padding = slui_px(12);
        break;
    case SLUI_ERGO_CONSOLE:
        g->flow = SLUI_FLOW_STACK;
        g->axis = SLUI_AXIS_VERTICAL;
        g->spacing = slui_px(2);
        g->padding = slui_px(6);
        g->main_align = SLUI_PLACE_START;
        g->cross_align = SLUI_PLACE_FILL;
        break;
    case SLUI_ERGO_PORTRAIT:
        g->flow = SLUI_FLOW_CENTRAL; /* weight/mass packing */
        g->axis = SLUI_AXIS_VERTICAL;
        g->spacing = slui_px(16);
        g->padding = slui_px(24);
        g->main_align = SLUI_PLACE_CENTRAL;
        g->cross_align = SLUI_PLACE_MEDIUM;
        break;
    case SLUI_ERGO_TOOLBAR:
        g->flow = SLUI_FLOW_STACK;
        g->axis = SLUI_AXIS_HORIZONTAL;
        g->spacing = slui_px(6);
        g->padding = slui_px(6);
        g->main_align = SLUI_PLACE_START;
        g->cross_align = SLUI_PLACE_MEDIUM;
        break;
    }
}

} // extern "C"

/* ==========================================================================
 * Flow arrangement
 * ========================================================================== */
namespace {

/* Resolve a measure along an axis extent. */
double resolve(const SLUILayout* l, SLUIMeasure m, double extent) {
    return slui_measure_to_px(m, l->dpi, extent, l->em_px);
}

/* Place `n` sized blocks of total `used` within `avail` per main_align, writing
 * the start offset of the run and the per-item gap (for FILL/justify). */
double main_start(SLUIPlacement a, double avail, double used, double spacing,
                  int n, double& extra_gap) {
    extra_gap = 0.0;
    double slack = avail - used - spacing * (n > 1 ? n - 1 : 0);
    if (slack < 0) slack = 0;
    switch (a) {
    case SLUI_PLACE_START: return 0.0;
    case SLUI_PLACE_MEDIUM: return slack / 2.0;       /* center */
    case SLUI_PLACE_CENTRAL: return slack / 2.0;      /* centre of mass base */
    case SLUI_PLACE_END: return slack;
    case SLUI_PLACE_FILL:
        if (n > 1) extra_gap = slack / (n - 1);
        return 0.0;
    default: return 0.0;
    }
}

double cross_pos(SLUIPlacement a, double avail, double size, double& out_size) {
    out_size = size;
    switch (a) {
    case SLUI_PLACE_FILL:
        out_size = avail;
        return 0.0;
    case SLUI_PLACE_MEDIUM:
    case SLUI_PLACE_CENTRAL:
        return (avail - size) / 2.0;
    case SLUI_PLACE_END:
        return avail - size;
    case SLUI_PLACE_START:
    default:
        return 0.0;
    }
}

void arrange_group(SLUILayout* l, LayoutGroup& g, Rect area);

/* STACK: a single row/column. FR items share leftover space. */
void flow_stack(SLUILayout* l, LayoutGroup& g, Rect box) {
    bool horiz = (g.axis == SLUI_AXIS_HORIZONTAL);
    double main_extent = horiz ? box.w : box.h;
    double cross_extent = horiz ? box.h : box.w;
    double spacing = resolve(l, g.spacing, main_extent);

    std::vector<LayoutItem*> vis;
    for (auto& it : g.items)
        if (it.visible) vis.push_back(&it);
    int n = static_cast<int>(vis.size());
    if (n == 0) return;

    double fixed = 0, frsum = 0;
    for (auto* it : vis) {
        if (slui_measure_is_flex(it->size)) frsum += slui_measure_flex(it->size);
        else fixed += resolve(l, it->size, main_extent);
    }
    double leftover = main_extent - fixed - spacing * (n - 1);
    if (leftover < 0) leftover = 0;

    double used = fixed + (frsum > 0 ? leftover : 0);
    double extra_gap = 0;
    double pos = main_start(g.main_align, main_extent, used, spacing, n, extra_gap);

    for (auto* it : vis) {
        double msize = slui_measure_is_flex(it->size)
                           ? (frsum > 0 ? leftover * slui_measure_flex(it->size) / frsum : 0)
                           : resolve(l, it->size, main_extent);
        double cs;
        double cp = cross_pos(it->cross_align, cross_extent, msize, cs);
        if (it->cross_align != SLUI_PLACE_FILL &&
            it->cross_align != SLUI_PLACE_MEDIUM &&
            it->cross_align != SLUI_PLACE_END && it->cross_align != SLUI_PLACE_CENTRAL) {
            cs = cross_extent; /* START default fills cross for a stack */
            cp = 0;
        }
        if (horiz) {
            it->rect = Rect{box.x + static_cast<int>(pos),
                            box.y + static_cast<int>(cp),
                            static_cast<int>(msize), static_cast<int>(cs)};
        } else {
            it->rect = Rect{box.x + static_cast<int>(cp),
                            box.y + static_cast<int>(pos),
                            static_cast<int>(cs), static_cast<int>(msize)};
        }
        it->arranged = true;
        pos += msize + spacing + extra_gap;
        /* nested group? recurse */
        if (LayoutGroup* sub = l->find_group(it->name))
            if (sub != &g) arrange_group(l, *sub, it->rect);
    }
}

/* WRAP: fill along the main axis, wrap to a new line when the next item would
 * overflow. Cross lines advance by the tallest/widest item seen. */
void flow_wrap(SLUILayout* l, LayoutGroup& g, Rect box) {
    bool horiz = (g.axis == SLUI_AXIS_HORIZONTAL);
    double main_extent = horiz ? box.w : box.h;
    double spacing = resolve(l, g.spacing, main_extent);
    double default_cell = main_extent / std::max(1, g.columns);

    double main = 0, cross = 0, line_cross = 0;
    for (auto& it : g.items) {
        if (!it.visible) continue;
        double msize = slui_measure_is_flex(it.size)
                           ? default_cell
                           : resolve(l, it.size, main_extent);
        if (msize <= 0) msize = default_cell;
        if (main > 0 && main + msize > main_extent) {
            main = 0;
            cross += line_cross + spacing;
            line_cross = 0;
        }
        double cell_cross = default_cell * 0.66; /* a pleasant tile aspect */
        if (horiz) {
            it.rect = Rect{box.x + static_cast<int>(main),
                           box.y + static_cast<int>(cross),
                           static_cast<int>(msize),
                           static_cast<int>(cell_cross)};
        } else {
            it.rect = Rect{box.x + static_cast<int>(cross),
                           box.y + static_cast<int>(main),
                           static_cast<int>(cell_cross),
                           static_cast<int>(msize)};
        }
        it.arranged = true;
        main += msize + spacing;
        line_cross = std::max(line_cross, cell_cross);
        if (LayoutGroup* sub = l->find_group(it.name))
            if (sub != &g) arrange_group(l, *sub, it.rect);
    }
}

/* GRID: fixed columns, equal cells, row-major. */
void flow_grid(SLUILayout* l, LayoutGroup& g, Rect box) {
    double spacing = resolve(l, g.spacing, box.w);
    int cols = std::max(1, g.columns);
    std::vector<LayoutItem*> vis;
    for (auto& it : g.items)
        if (it.visible) vis.push_back(&it);
    int n = static_cast<int>(vis.size());
    if (n == 0) return;
    int rows = (n + cols - 1) / cols;
    double cw = (box.w - spacing * (cols - 1)) / cols;
    double ch = (box.h - spacing * (rows - 1)) / std::max(1, rows);
    for (int i = 0; i < n; ++i) {
        int c = i % cols, r = i / cols;
        vis[i]->rect = Rect{box.x + static_cast<int>(c * (cw + spacing)),
                            box.y + static_cast<int>(r * (ch + spacing)),
                            static_cast<int>(cw), static_cast<int>(ch)};
        vis[i]->arranged = true;
        if (LayoutGroup* sub = l->find_group(vis[i]->name))
            if (sub != &g) arrange_group(l, *sub, vis[i]->rect);
    }
}

/* DOCK: first item docks to the leading edge, last to the trailing edge, and
 * everything between fills the centre (as a nested stack along the axis). */
void flow_dock(SLUILayout* l, LayoutGroup& g, Rect box) {
    bool horiz = (g.axis == SLUI_AXIS_HORIZONTAL);
    double main_extent = horiz ? box.w : box.h;
    std::vector<LayoutItem*> vis;
    for (auto& it : g.items)
        if (it.visible) vis.push_back(&it);
    int n = static_cast<int>(vis.size());
    if (n == 0) return;

    double lead = (n >= 1 && !slui_measure_is_flex(vis.front()->size))
                      ? resolve(l, vis.front()->size, main_extent)
                      : 0;
    double trail = (n >= 2 && !slui_measure_is_flex(vis.back()->size))
                       ? resolve(l, vis.back()->size, main_extent)
                       : 0;
    double centre = main_extent - lead - trail;
    if (centre < 0) centre = 0;

    auto put = [&](LayoutItem* it, double start, double size) {
        if (horiz)
            it->rect = Rect{box.x + static_cast<int>(start), box.y,
                            static_cast<int>(size), box.h};
        else
            it->rect = Rect{box.x, box.y + static_cast<int>(start), box.w,
                            static_cast<int>(size)};
        it->arranged = true;
        if (LayoutGroup* sub = l->find_group(it->name))
            if (sub != &g) arrange_group(l, *sub, it->rect);
    };
    if (n >= 1) put(vis.front(), 0, lead > 0 ? lead : centre);
    if (n >= 2) put(vis.back(), main_extent - trail, trail > 0 ? trail : centre);
    /* middle items share the centre band evenly */
    int mid = n - 2;
    if (mid > 0) {
        double each = centre / mid;
        for (int i = 1; i < n - 1; ++i) put(vis[i], lead + (i - 1) * each, each);
    }
}

/* CENTRAL: weight/mass packing. Items are sized (FR falls back to a nominal
 * share), then the whole block is centred and each item is additionally pulled
 * toward the group's centre of mass in proportion to its mass -- heavier items
 * migrate inward. */
void flow_central(SLUILayout* l, LayoutGroup& g, Rect box) {
    bool horiz = (g.axis == SLUI_AXIS_HORIZONTAL);
    double main_extent = horiz ? box.w : box.h;
    double cross_extent = horiz ? box.h : box.w;
    double spacing = resolve(l, g.spacing, main_extent);
    std::vector<LayoutItem*> vis;
    for (auto& it : g.items)
        if (it.visible) vis.push_back(&it);
    int n = static_cast<int>(vis.size());
    if (n == 0) return;

    std::vector<double> sz(n);
    double total = 0, masssum = 0;
    for (int i = 0; i < n; ++i) {
        sz[i] = slui_measure_is_flex(vis[i]->size)
                    ? main_extent / (n + 1)
                    : resolve(l, vis[i]->size, main_extent);
        total += sz[i];
        masssum += vis[i]->mass;
    }
    double used = total + spacing * (n - 1);
    double start = (main_extent - used) / 2.0; /* centre the block */
    if (start < 0) start = 0;

    /* centre of mass index (weighted mean position 0..n-1) */
    double com = 0;
    if (masssum > 0) {
        double acc = 0;
        for (int i = 0; i < n; ++i) acc += i * vis[i]->mass;
        com = acc / masssum;
    } else {
        com = (n - 1) / 2.0;
    }

    double pos = start;
    for (int i = 0; i < n; ++i) {
        /* a gentle inward pull toward the centre of mass, scaled by this item's
         * mass share -- the heavier it is, the more it migrates toward com. */
        double share = masssum > 0 ? vis[i]->mass / masssum : 1.0 / n;
        double pull = (com - i) * spacing * 0.5 * share * n;
        double p = pos + pull;
        double cs;
        double cp = cross_pos(SLUI_PLACE_MEDIUM, cross_extent, sz[i] * 0.0, cs);
        (void)cp;
        double block_cross = cross_extent * 0.6; /* portrait-ish block */
        double ccp = (cross_extent - block_cross) / 2.0;
        if (horiz)
            vis[i]->rect = Rect{box.x + static_cast<int>(p),
                                box.y + static_cast<int>(ccp),
                                static_cast<int>(sz[i]),
                                static_cast<int>(block_cross)};
        else
            vis[i]->rect = Rect{box.x + static_cast<int>(ccp),
                                box.y + static_cast<int>(p),
                                static_cast<int>(block_cross),
                                static_cast<int>(sz[i])};
        vis[i]->arranged = true;
        pos += sz[i] + spacing;
        if (LayoutGroup* sub = l->find_group(vis[i]->name))
            if (sub != &g) arrange_group(l, *sub, vis[i]->rect);
    }
}

void arrange_group(SLUILayout* l, LayoutGroup& g, Rect area) {
    double pad = resolve(l, g.padding, std::min(area.w, area.h));
    Rect box{area.x + static_cast<int>(pad), area.y + static_cast<int>(pad),
             area.w - static_cast<int>(pad * 2), area.h - static_cast<int>(pad * 2)};
    if (box.w < 0) box.w = 0;
    if (box.h < 0) box.h = 0;
    switch (g.flow) {
    case SLUI_FLOW_WRAP: flow_wrap(l, g, box); break;
    case SLUI_FLOW_GRID: flow_grid(l, g, box); break;
    case SLUI_FLOW_DOCK: flow_dock(l, g, box); break;
    case SLUI_FLOW_CENTRAL: flow_central(l, g, box); break;
    case SLUI_FLOW_STACK:
    default: flow_stack(l, g, box); break;
    }
}

} // namespace

extern "C" {

void slui_layout_arrange(SLUILayout* l, const char* group_name, SLUIRect area) {
    if (!l) return;
    LayoutGroup* g = l->find_group(group_name);
    if (!g) return;
    arrange_group(l, *g, slui::to_rect(area));
}

int slui_layout_item_rect(const SLUILayout* l, const char* item_name,
                          SLUIRect* out) {
    if (!l || !item_name || !out) return 0;
    for (const auto& g : l->groups)
        for (const auto& it : g.items)
            if (it.name == item_name && it.arranged) {
                out->x = it.rect.x;
                out->y = it.rect.y;
                out->w = it.rect.w;
                out->h = it.rect.h;
                return 1;
            }
    return 0;
}

} // extern "C"
