/* =============================================================================
 * SleelaUI military command console -- a real, native OS window that renders
 * the DESIGNED SLeeLa GUI (lib/gui/version-military) with SleelaUI's own C/C++
 * toolkit and software rasterizer. This is the on-screen, reviewable form of
 * example/gui-military-command-console.sleela: the same scene (metal/blue
 * aesthetic, insignia + wordmark + tagline chrome, window buttons, a menu, and
 * the Actions / Cryptography Selector / Certificate Store cards) drawn by
 * SleelaUI -- NOT Java/Swing/JavaFX. The palette mirrors
 * lib/gui/version-military/preview/gui-military-preview.html.
 *
 * Build (Linux/Unix):  make military   ->   build/slui-military-console
 * Run:                 ./build/slui-military-console   (needs an X server)
 * Headless CI check:   make military-smoke
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"

#include <stdio.h>
#include <string.h>

/* The military "metal / blue-hue" palette from the version-military descriptors
 * and preview (accent #8FB7D9, deep steel surfaces, silver text). */
static void military_theme(SLUITheme* t) {
    slui_theme_preset(t, SLUI_THEME_CUSTOM);
    t->bg         = slui_rgb(0x16, 0x1C, 0x24); /* deep steel backdrop         */
    t->surface    = slui_rgb(0x2B, 0x33, 0x3D); /* card / panel surface        */
    t->surface_hi = slui_rgb(0x3A, 0x44, 0x50); /* raised / hovered surface    */
    t->chrome     = slui_rgb(0x39, 0x45, 0x55); /* header strip (brushed metal)*/
    t->border     = slui_rgba(0xC8, 0xD7, 0xE6, 0x2E); /* hairline              */
    t->fg         = slui_rgb(0xE8, 0xEE, 0xF5); /* silver text                 */
    t->fg_dim     = slui_rgb(0x9F, 0xB0, 0xC2); /* muted                       */
    t->accent     = slui_rgb(0x8F, 0xB7, 0xD9); /* the one blue accent         */
    t->accent_fg  = slui_rgb(0x12, 0x1A, 0x24);
    t->danger     = slui_rgb(0xE7, 0x86, 0x86);
    t->radius     = 9;
    t->control_height = 34;
    /* Keep the toolkit's resolved default family; just size it for a console. */
    t->font_size  = 13;
}

static SLUIApp* g_app;

static int on_window_event(const SLUIEvent* ev, void* user) {
    SLUIApp* app = (SLUIApp*)user;
    if (ev->type == SLUI_EVENT_CLOSE) { slui_app_quit(app, 0); return 1; }
    return 0;
}

/* One component card: a heading row (title + kind tag) over a vertical body. */
static SLUIWidget* card_with_heading(SLUIWidget* parent, const char* title,
                                     const char* kind) {
    SLUIWidget* card = slui_card(parent);
    slui_widget_set_expand(card, 1, 0);
    SLUIWidget* col = slui_box(card, SLUI_ORIENT_VERTICAL, 8);
    slui_widget_set_margin(col, 12, 14, 12, 14);
    slui_widget_set_expand(col, 1, 0);
    SLUIWidget* hrow = slui_box(col, SLUI_ORIENT_HORIZONTAL, 8);
    SLUIWidget* h = slui_label(hrow, title);
    slui_widget_set_align(h, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    slui_spacer(hrow);
    slui_badge(hrow, kind);
    return col; /* the body column to append rows into */
}

int main(void) {
    g_app = slui_app_create("com.mearvk.SleelaUI.MilitaryConsole");
    if (!g_app) {
        fprintf(stderr,
                "SleelaUI: could not open a display. On Linux/Unix a running\n"
                "X server (DISPLAY) is required to show the window.\n");
        return 1;
    }
    printf("SleelaUI %s -- backend: %s\n", slui_version_string(),
           slui_backend_name(slui_backend()));

    SLUITheme theme;
    military_theme(&theme);

    SLUIWindowConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.title = "SLeeLa Command Console \xe2\x80\x94 MILITARY 5.0";
    cfg.width = 1040;
    cfg.height = 620;
    cfg.resizable = 1;
    cfg.theme = &theme;

    SLUIWindow* win = slui_window_create(g_app, &cfg);
    slui_window_set_event_handler(win, on_window_event, g_app);
    SLUIWidget* root = slui_window_root(win);

    /* ---- Chrome: insignia + wordmark + tagline, then window buttons ---- */
    SLUIWidget* header = slui_header_bar(root);
    slui_widget_set_size_request(header, 0, 56);
    SLUIWidget* insignia = slui_label(header, "\xe2\x97\x88"); /* ◈ */
    slui_widget_set_margin(insignia, 0, 10, 0, 14);
    slui_widget_set_align(insignia, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    SLUIWidget* brandcol = slui_box(header, SLUI_ORIENT_VERTICAL, 0);
    SLUIWidget* wordmark = slui_label(brandcol, "SLeeLa Command");
    slui_widget_set_align(wordmark, SLUI_ALIGN_START, SLUI_ALIGN_END);
    SLUIWidget* tagline = slui_label(brandcol, "NEUTRAL \xc2\xb7 HARDENED \xc2\xb7 PRECISE");
    slui_widget_set_align(tagline, SLUI_ALIGN_START, SLUI_ALIGN_START);
    slui_spacer(header);
    SLUIWidget* bmin = slui_button(header, "\xe2\x80\x94");      /* — minimize   */
    SLUIWidget* bfull = slui_button(header, "\xe2\xa4\xa2");     /* ⤢ fullscreen */
    SLUIWidget* bfold = slui_button(header, "\xe2\x87\xa5");     /* ⇥ fold right */
    slui_widget_set_margin(bfold, 6, 12, 6, 0);
    (void)bmin; (void)bfull;

    /* ---- Content ---- */
    SLUIWidget* content = slui_box(root, SLUI_ORIENT_VERTICAL, 14);
    slui_widget_set_margin(content, 18, 18, 18, 18);
    slui_widget_set_expand(content, 1, 1);

    /* Menu row (File / Secure / View). */
    SLUIWidget* menu = slui_box(content, SLUI_ORIENT_HORIZONTAL, 4);
    SLUIWidget* mFile = slui_button(menu, "File");
    slui_widget_set_suggested(mFile, 1);
    slui_button(menu, "Secure");
    slui_button(menu, "View");
    slui_spacer(menu);

    /* Two-column grid of component cards. */
    SLUIWidget* grid = slui_box(content, SLUI_ORIENT_HORIZONTAL, 16);
    slui_widget_set_expand(grid, 1, 0);
    SLUIWidget* colL = slui_box(grid, SLUI_ORIENT_VERTICAL, 16);
    slui_widget_set_expand(colL, 1, 0);
    SLUIWidget* colR = slui_box(grid, SLUI_ORIENT_VERTICAL, 16);
    slui_widget_set_expand(colR, 1, 0);

    /* Actions toolbar card: metal buttons with leading icons. */
    SLUIWidget* actions = card_with_heading(colL, "Actions", "container \xc2\xb7 toolbar");
    SLUIWidget* arow = slui_box(actions, SLUI_ORIENT_HORIZONTAL, 8);
    SLUIWidget* authorize = slui_button(arow, "\xe2\x9b\x8a Authorize"); /* ⛊ */
    slui_widget_set_suggested(authorize, 1);                             /* the accent action */
    slui_button(arow, "\xe2\x96\xb9 Run");    /* ▹ */
    slui_button(arow, "\xe2\x9f\xb2 Reset");  /* ⟲ */
    slui_spacer(arow);
    SLUIWidget* iconnote = slui_label(actions,
        "icons: md/20 \xc2\xb7 leading \xc2\xb7 gap 8 \xc2\xb7 optically centered");
    slui_widget_set_align(iconnote, SLUI_ALIGN_START, SLUI_ALIGN_START);

    /* Cryptography Selector card. */
    SLUIWidget* crypto = card_with_heading(colR, "Cryptography Selector", "component");
    SLUIWidget* pick = slui_combo_box(crypto);
    slui_combo_box_add(pick, "AES-256-GCM + ML-KEM-768 + ML-DSA-65  [TOP-SECRET \xc2\xb7 PQC]");
    slui_combo_box_add(pick, "AES-256-GCM + X25519 (strong)");
    slui_widget_set_expand(pick, 1, 0);
    SLUIWidget* cnote = slui_label(crypto, "min strength: strong \xc2\xb7 legacy suites gated out");
    slui_widget_set_align(cnote, SLUI_ALIGN_START, SLUI_ALIGN_START);

    /* Public-key certificate store card (spans the width). */
    SLUIWidget* certs = card_with_heading(content, "Public Key Certificate Store", "component");
    slui_widget_set_expand(certs, 1, 0);
    SLUIWidget* r1 = slui_box(certs, SLUI_ORIENT_HORIZONTAL, 10);
    SLUIWidget* c1 = slui_label(r1, "SLeeLa Root CA");
    slui_widget_set_align(c1, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    SLUIWidget* c1m = slui_label(r1, "Ed25519 \xc2\xb7 ab12..root");
    slui_widget_set_align(c1m, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    slui_spacer(r1);
    slui_badge(r1, "trusted \xc2\xb7 CA");
    slui_separator(certs, SLUI_ORIENT_HORIZONTAL);
    SLUIWidget* r2 = slui_box(certs, SLUI_ORIENT_HORIZONTAL, 10);
    SLUIWidget* c2 = slui_label(r2, "cmd.sleela.mil");
    slui_widget_set_align(c2, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    SLUIWidget* c2m = slui_label(r2, "ECDSA-P256 \xc2\xb7 cd34..leaf");
    slui_widget_set_align(c2m, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    slui_spacer(r2);
    SLUIWidget* revoked = slui_badge(r2, "revoked");
    slui_widget_set_destructive(revoked, 1);

    /* Status bar. */
    SLUIWidget* status = slui_status_bar(root);
    SLUIWidget* sl = slui_label(status,
        "military line: metal/blue surface \xc2\xb7 strong branding \xc2\xb7 hardened crypto + certs");
    slui_widget_set_align(sl, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    slui_widget_set_margin(sl, 0, 12, 0, 12);
    (void)bfold;

    slui_window_show(win);
    int code = slui_app_run(g_app);
    slui_app_destroy(g_app);
    return code;
}
