#include <gtk/gtk.h>
#include <vte/vte.h>
#include <pango/pangocairo.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <regex>
#include <set>
#include <cmath>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {

constexpr const char *kApplicationId = "com.mearvk.SleelaTerminal";
constexpr const char *kWindowTitle = "SleelaTerminal™ — MEARVK LLC";
constexpr const char *kConfigName = "sleela-terminal.conf";
constexpr const char *kVersion = "1.0.0";

// One "child of wattage": a discrete excitation that flows along the throbber
// strip and locally modulates the base light-blue colour. Many of these,
// summed, make the living field. Motion is integrated to third order
// (jerk -> accel -> vel -> pos) so speed changes are smooth and reflexive.
struct ThrobberCell {
    bool alive = false;
    double x = 0.0;        // position in pixels
    double v = 0.0;        // velocity (px/s), mostly positive = left->right
    double a = 0.0;        // acceleration (px/s^2)
    double j = 0.0;        // jerk (px/s^3) -- the 3rd-order control term
    double base_intensity = 0.0; // the cell's inherent shade strength [0..1]
    double intensity = 0.0;      // live rendered contribution (base * fades)
    double energy = 1.0;         // life/decay budget
    double sigma = 0.0;    // spatial half-width of this cell's glow (px)
    double speed_mul = 1.0;// per-cell speed character (some glide, some race)
    double phase = 0.0;    // per-cell phase for subtle flicker
    bool retro = false;    // a rare child allowed to pause / go right->left
    double retro_px = 0.0; // pixels of retrograde/pause travel remaining
    // Optional colour tint, only expressed under strong amplification.
    // (tr,tg,tb) is a fully-saturated hue; tinted==false means pure base shade.
    bool tinted = false;
    double tr = 0.0, tg = 0.0, tb = 0.0;
};

struct AppConfig {
    std::string font = "Monospace 11";
    std::string foreground = "#FFFFFF";
    std::string input_language = "system";
    std::string output_language = "system";
    bool national_feeds = false;
    bool persons_of_interest = false;
    bool parametric_documents = false;
    bool national_identifiers = false;
    unsigned footer_scroll_delay_minutes = 5;
};

struct AppState {
    std::string shell_path;
    GtkWindow *window = nullptr;
    VteTerminal *terminal = nullptr;
    GtkWidget *footer_text = nullptr;
    std::string executable_path;
    GPid child_pid = 0;
    guint footer_tick = 0;
    guint footer_scroll_tick = 0;
    guint footer_position = 0;
    gint64 footer_text_changed_us = 0;
    std::string footer_full_text;
    std::string feed_topics_signature;
    bool feed_fetching = false;
    gint64 feed_last_fetch = 0;
    AppConfig config;

    // Full-width 4px pure-white throbber rail under the title bar. The existing
    // coloured cells move and sway within its pixels; only those coloured cells
    // may cast reflected light onto the terminal text pane.
    GtkWidget *throbber = nullptr;
    GtkWidget *terminal_glow = nullptr; // non-interactive overlay lit by throbber cells
    guint throbber_tick = 0; // GTK frame-clock callback ID (main thread only)
    gint64 throbber_last_frame_us = 0;
    double throbber_t = 0.0;        // monotonically advancing time (seconds)
    int    throbber_w = 0;          // last known strip width (px)
    double throbber_amp = 0.6;      // slow reflexive "amplification" field [0..1]
    double throbber_amp_v = 0.0;    // its rate of change (for smooth 2nd order)
    double throbber_spawn_accum = 0.0; // fractional spawn accumulator
    std::vector<ThrobberCell> throbber_cells;
};

std::filesystem::path config_path() {
    const char *config_dir = g_get_user_config_dir();
    return std::filesystem::path(config_dir) / "sleela" / kConfigName;
}

void save_config(const AppConfig &config) {
    const auto path = config_path();
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) return;

    std::ofstream out(path);
    if (!out) return;
    out << "# SleelaTerminal local presentation and data-orientation settings\n";
    out << "# Data source switches are declarative; they do not fetch or transmit data.\n";
    out << "font=" << config.font << '\n';
    out << "foreground=" << config.foreground << '\n';
    out << "input_language=" << config.input_language << '\n';
    out << "output_language=" << config.output_language << '\n';
    out << "national_feeds=" << (config.national_feeds ? "true" : "false") << '\n';
    out << "persons_of_interest=" << (config.persons_of_interest ? "true" : "false") << '\n';
    out << "parametric_documents=" << (config.parametric_documents ? "true" : "false") << '\n';
    out << "national_identifiers=" << (config.national_identifiers ? "true" : "false") << '\n';
    out << "footer_scroll_delay_minutes=" << config.footer_scroll_delay_minutes << '\n';
}

void load_config(AppConfig &config) {
    std::ifstream in(config_path());
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.front() == '#') continue;
        const auto equals = line.find('=');
        if (equals == std::string::npos) continue;
        const std::string key = line.substr(0, equals);
        const std::string value = line.substr(equals + 1);

        if (key == "font") config.font = value;
        else if (key == "foreground") config.foreground = value;
        else if (key == "input_language") config.input_language = value;
        else if (key == "output_language") config.output_language = value;
        else if (key == "national_feeds") config.national_feeds = value == "true";
        else if (key == "persons_of_interest") config.persons_of_interest = value == "true";
        else if (key == "parametric_documents") config.parametric_documents = value == "true";
        else if (key == "national_identifiers") config.national_identifiers = value == "true";
        else if (key == "footer_scroll_delay_minutes") {
            try { unsigned delay = static_cast<unsigned>(std::stoul(value)); if (delay >= 5 && delay <= 60) config.footer_scroll_delay_minutes = delay; }
            catch (...) { /* retain default */ }
        }
    }
}

std::string asset_path(const AppState *state, const char *name) {
    namespace fs = std::filesystem;
    fs::path executable = fs::absolute(state->executable_path);
    const std::vector<fs::path> candidates = {
        executable.parent_path() / "assets" / name,
        executable.parent_path() / ".." / "assets" / name,
        fs::path("sleela-terminal") / "assets" / name
    };
    std::error_code ec;
    for (const auto &candidate : candidates) {
        if (fs::exists(candidate, ec) && !ec) return candidate.lexically_normal().string();
    }
    return name;
}

// CMD is the Java native launcher associated with SecureJDK 28.
// The footer CMD image is a strict image control: no button outline or frame.
std::string installer_path(const AppState *state) {
    namespace fs = std::filesystem;
    fs::path executable = fs::absolute(state->executable_path);
    const std::vector<fs::path> candidates = {
        executable.parent_path() / ".." / "tools" / "install" / "sleela-software.sh",
        fs::path("sleela-terminal") / "tools" / "install" / "sleela-software.sh",
        fs::path("tools") / "install" / "sleela-software.sh"
    };
    std::error_code ec;
    for (const auto &candidate : candidates) {
        if (fs::exists(candidate, ec) && !ec) return candidate.lexically_normal().string();
    }
    return candidates.front().lexically_normal().string();
}

std::string sibling_path(const char *argv0, const char *name) {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path executable = fs::absolute(argv0, ec);
    if (ec) return name;
    fs::path candidate = executable.parent_path() / name;
    if (fs::exists(candidate, ec) && !ec) return candidate.string();
    return name;
}

bool parse_rgba(const std::string &value, GdkRGBA &rgba) {
    return gdk_rgba_parse(&rgba, value.c_str());
}

void apply_terminal_style(AppState *state) {
    if (state->terminal == nullptr) return;

    PangoFontDescription *font = pango_font_description_from_string(state->config.font.c_str());
    vte_terminal_set_font(state->terminal, font);
    pango_font_description_free(font);

    GdkRGBA foreground;
    GdkRGBA background;
    if (!parse_rgba(state->config.foreground, foreground)) {
        gdk_rgba_parse(&foreground, "#FFFFFF");
    }
    // Terminal background matches the window background (@sl_bg) so the terminal
    // reads as one surface with the chrome rather than a separate near-black
    // rectangle. See UI-PRINCIPLES.md (principle 2).
    gdk_rgba_parse(&background, "#15101c");

    // A 16-colour ANSI palette tuned to the dark-purple theme: slightly warmer
    // and less saturated than raw VTE defaults so output is legible and calm on
    // the @sl_bg background without clashing with the purple frame.
    static const char *const kPalette[16] = {
        "#1b1526", "#e06c75", "#8fcf8f", "#e5c07b", // black, red, green, yellow
        "#8f9ff0", "#c58af0", "#6fd0d8", "#cfcad8", // blue, magenta, cyan, white
        "#4a4160", "#ff8a93", "#b0e0b0", "#f2d79a", // bright black..yellow
        "#aab6ff", "#d9b0ff", "#9fe6ec", "#ffffff"  // bright blue..white
    };
    GdkRGBA palette[16];
    for (int i = 0; i < 16; ++i) gdk_rgba_parse(&palette[i], kPalette[i]);
    vte_terminal_set_colors(state->terminal, &foreground, &background, palette, 16);
}

void install_css(AppState *state) {
    // One palette, one source of truth (see UI-PRINCIPLES.md). Every colour
    // below is a named token; selectors never carry raw hex values. The window
    // and terminal share @sl_bg so the terminal does not float as a separate
    // near-black rectangle inside the purple chrome.
    //
    // NOTE: this is a C++ raw string literal; newlines here are real newlines.
    // Never write the two characters backslash-n inside it -- GTK's CSS parser
    // rejects the stray backslash and drops the rest of the rule.
    const char *css = R"CSS(
        @define-color sl_bg          #15101c;
        @define-color sl_surface     #21112f;
        @define-color sl_chrome      #24103f;
        @define-color sl_border      #7f56aa;
        @define-color sl_fg          #ffffff;
        @define-color sl_accent      #6fa5ff;
        @define-color sl_menu_bg     #16244f;
        @define-color sl_menu_mid    #203b78;
        @define-color sl_menu_edge   #4f74c8;
        @define-color sl_menu_hover  #31569a;
        @define-color sl_menu_active #426fc0;

        /* Title-bar gradient palette. The bar is no longer a flat chrome fill:
           the selected preset distributes this curated sequence of the project's
           dark-purple / blue family along the horizontal or vertical axis. These are the only places these shades are
           used, so they live here as named tokens (principle 2: one palette,
           one source of truth) and are composed into .sleela-titlebar below. */
        @define-color sl_tb_deep_purple  #2a0f45;   /* deep purple        */
        @define-color sl_tb_near_black   #0d0a12;   /* dark gray / black  */
        @define-color sl_tb_light_blue   #4f74c8;   /* light blue accent  */
        @define-color sl_tb_dark_blue    #16244f;   /* dark blue          */
        @define-color sl_tb_dark_purple  #1e0e33;   /* dark purple        */
        @define-color sl_tb_black        #060409;   /* black              */

        window { background: @sl_bg; }

        /* Title bar: an elegant preset-driven gradient (see assets/*.cfg). The bottom
           highlight is no longer a static border -- it is the living throbber
           strip drawn just below the bar (see .sleela-throbber and the 20Hz
           organic update). A transparent 1px border keeps the bar's height
           identical to before. */
        headerbar.sleela-titlebar {
            /* Deep purple -> dark gray/black -> deep purple -> light blue -> dark blue ->
               dark purple -> black. The active preset determines whether the stops
               are distributed left-to-right or top-to-bottom. Stops are eased unevenly
               so the light-blue highlight sits a little past centre and the ends settle
               into near black, giving the bar depth without competing with the terminal
               (principle 1). Colours are the named sl_tb_* tokens above. */
            background: linear-gradient(to right,
                @sl_tb_deep_purple  0%,
                @sl_tb_near_black   16%,
                @sl_tb_deep_purple  33%,
                @sl_tb_light_blue   52%,
                @sl_tb_dark_blue    68%,
                @sl_tb_dark_purple  85%,
                @sl_tb_black        100%);
            color: @sl_fg;
            min-height: 38px;
            border-bottom: 1px solid transparent;
        }
        /* The throbber occupies the thin seam between the title bar and the
           terminal; its colour is painted per-frame by the draw function. */
        drawingarea.sleela-throbber { background: #ffffff; min-height: 4px; }
        headerbar.sleela-titlebar label {
            color: #ffffff;
            font-weight: 600;
            /* GTK CSS has no portable text-stroke; eight 1px shadows create a crisp black outline. */
            text-shadow: -1px -1px 0 #000000, 0 -1px 0 #000000, 1px -1px 0 #000000,
                         -1px 0 0 #000000, 1px 0 0 #000000,
                         -1px 1px 0 #000000, 0 1px 0 #000000, 1px 1px 0 #000000;
        }
        image.sleela-titlebar-logo { margin-left: 8px; margin-right: 4px; }
        /* Window controls (close / minimize / maximize). In GTK 4 these are
           native title buttons under a `windowcontrols` node, and the symbolic
           glyph is recoloured by the `color` property ON THE BUTTON (not a
           child image). We therefore set `color` on the button itself, and also
           on the inner .icon/image node as belt-and-suspenders. The whole block
           is loaded at USER provider priority so it beats the system theme. */
        windowcontrols > button.titlebutton,
        headerbar.sleela-titlebar windowcontrols > button.titlebutton,
        headerbar.sleela-titlebar button.titlebutton {
            color: @sl_fg;                 /* recolours the symbolic glyph */
            background: alpha(@sl_fg, 0.16);
            border: 1px solid alpha(@sl_fg, 0.35);
            border-radius: 50%;
            min-width: 22px;
            min-height: 22px;
            margin: 0 3px;
            padding: 0;
            -gtk-icon-shadow: none;
            transition: background 120ms ease, border-color 120ms ease, color 120ms ease;
        }
        /* Belt-and-suspenders: force the inner glyph node bright and opaque. */
        windowcontrols > button.titlebutton > image,
        windowcontrols > button.titlebutton image,
        headerbar.sleela-titlebar button.titlebutton image {
            color: @sl_fg;
            opacity: 1;
            -gtk-icon-size: 16px;
        }
        windowcontrols > button.titlebutton:hover,
        headerbar.sleela-titlebar button.titlebutton:hover {
            background: alpha(@sl_fg, 0.30);
            border-color: alpha(@sl_fg, 0.65);
            color: @sl_fg;
        }
        windowcontrols > button.titlebutton:active,
        headerbar.sleela-titlebar button.titlebutton:active { background: alpha(@sl_fg, 0.42); }
        windowcontrols > button.titlebutton:focus,
        headerbar.sleela-titlebar button.titlebutton:focus { outline: 2px solid @sl_accent; outline-offset: -2px; }
        /* Per-control mouseover highlights make the three native window actions
           easy to distinguish. Keep the glyph white and use the shared transition
           defined above for a restrained, quick highlight rather than a flash. */
        windowcontrols > button.titlebutton.minimize:hover,
        headerbar.sleela-titlebar button.titlebutton.minimize:hover {
            background: #4f74c8;
            border-color: #8fb4ff;
            color: #ffffff;
        }
        windowcontrols > button.titlebutton.minimize:active,
        headerbar.sleela-titlebar button.titlebutton.minimize:active {
            background: #31569a;
            border-color: #8fb4ff;
        }
        windowcontrols > button.titlebutton.maximize:hover,
        headerbar.sleela-titlebar button.titlebutton.maximize:hover {
            background: #238636;
            border-color: #56d364;
            color: #ffffff;
        }
        windowcontrols > button.titlebutton.maximize:active,
        headerbar.sleela-titlebar button.titlebutton.maximize:active {
            background: #196c2e;
            border-color: #56d364;
        }

        /* Close: a clear red resting tint, solid red on hover, glyph stays white. */
        windowcontrols > button.titlebutton.close,
        headerbar.sleela-titlebar button.titlebutton.close {
            background: alpha(#ff5f57, 0.28);
            border-color: alpha(#ff5f57, 0.65);
            color: @sl_fg;
        }
        windowcontrols > button.titlebutton.close:hover,
        headerbar.sleela-titlebar button.titlebutton.close:hover {
            background: #ff5f57;
            border-color: #ff5f57;
            color: #ffffff;
        }

        /* Settings: compact blue hamburger control with a fine black outline and
           softly rounded corners (18% radius) to match the title-bar blue. */
        headerbar.sleela-titlebar button.sleela-settings-btn {
            color: #ffffff;
            background: #203b78;
            border: 1px solid #000000;
            border-radius: 18%;
            box-shadow: inset 0 1px 0 alpha(#ffffff, 0.28);
            min-width: 27px;
            min-height: 19px;
            padding: 1px 3px;
            margin-right: 4px;
            transition: background 120ms ease, box-shadow 120ms ease;
        }
        headerbar.sleela-titlebar button.sleela-settings-btn:hover {
            background: #31569a;
            box-shadow: inset 0 1px 0 alpha(#ffffff, 0.36), 0 0 0 1px alpha(#000000, 0.20);
        }
        headerbar.sleela-titlebar button.sleela-settings-btn:active {
            background: #16244f;
            box-shadow: inset 0 1px 2px alpha(#000000, 0.35);
        }
        headerbar.sleela-titlebar button.sleela-settings-btn:focus {
            outline: 1px solid @sl_accent;
            outline-offset: 1px;
        }

        /* Footer: flat chrome bar matching the title bar, one hairline on top. */
        box.sleela-footer {
            min-height: 38px;
            background: @sl_chrome;
            border-top: 1px solid @sl_border;
        }
        box.sleela-footer label { color: @sl_fg; font-weight: 600; }
        label.sleela-footer-brand { font-weight: 800; letter-spacing: 0.5px; }
        label.sleela-footer-separator { color: alpha(@sl_fg, 0.42); padding-left: 8px; padding-right: 8px; }
        label.sleela-footer-ticker, drawingarea.sleela-footer-ticker {
            color: #b8b8b8;
            text-shadow: -1px -1px 0 #000000, 0 -1px 0 #000000, 1px -1px 0 #000000,
                         -1px 0 0 #000000, 1px 0 0 #000000,
                         -1px 1px 0 #000000, 0 1px 0 #000000, 1px 1px 0 #000000;
            padding-left: 8px; padding-right: 8px;
        }
        button.sleela-footer-java {
            min-width: 32px; min-height: 32px; padding: 0; margin: 0 8px;
            background: transparent; border: none; box-shadow: none; border-radius: 0;
        }
        button.sleela-footer-java:hover,
        button.sleela-footer-java:active { background: transparent; border: none; box-shadow: none; }
        button.sleela-footer-java:focus { outline: 2px solid @sl_accent; outline-offset: -2px; }
        button.sleela-footer-java image { background: transparent; border: none; box-shadow: none; }

        /* All interactive menus and selection surfaces share the main frame's
           blue palette: deep-blue base, brighter blue hover, and light-blue focus.
           This covers the Settings subframe, software popover, and terminal
           right-click/click-select menu instead of letting GTK theme them gray. */
        window.sleela-installer {
            background: linear-gradient(to bottom, @sl_menu_mid, @sl_menu_bg);
            color: @sl_fg;
            border: 1px solid @sl_menu_edge;
        }
        window.sleela-installer label { color: @sl_fg; }

        popover contents {
            background: linear-gradient(to bottom, @sl_menu_mid, @sl_menu_bg);
            color: @sl_fg;
            border: 1px solid @sl_menu_edge;
        }
        popover.sleela-software contents,
        popover.sleela-context-menu contents {
            background: linear-gradient(to bottom, @sl_menu_mid, @sl_menu_bg);
            color: @sl_fg;
            border: 1px solid @sl_menu_edge;
        }
        popover label { color: @sl_fg; }

        /* Settings is a subframe window; its whole scrollable surface matches
           the same blue used by the main title frame. */
        window.sleela-settings {
            background: linear-gradient(to bottom, @sl_menu_mid, @sl_menu_bg);
            color: @sl_fg;
            border: 1px solid @sl_menu_edge;
        }
        window.sleela-settings scrolledwindow,
        window.sleela-settings viewport { background: @sl_menu_bg; color: @sl_fg; }

        popover.sleela-software label,
        window.sleela-settings label { color: @sl_fg; }
        window.sleela-settings label.section { font-weight: 800; margin-top: 8px; }

        popover button,
        window.sleela-settings button,
        window.sleela-installer button {
            color: @sl_fg;
            background: @sl_menu_bg;
            border: 1px solid alpha(@sl_menu_edge, 0.78);
            border-radius: 6px;
            min-width: 260px;
            min-height: 32px;
            text-align: left;
            transition: background 120ms ease, border-color 120ms ease;
        }
        popover button:hover,
        window.sleela-settings button:hover,
        window.sleela-installer button:hover {
            background: @sl_menu_hover;
            border-color: @sl_menu_edge;
        }
        popover button:active,
        window.sleela-settings button:active,
        window.sleela-installer button:active { background: @sl_menu_active; }
        popover button:focus,
        window.sleela-settings button:focus,
        window.sleela-installer button:focus {
            outline: 2px solid @sl_menu_edge;
            outline-offset: -2px;
        }
        popover checkbutton,
        window.sleela-settings checkbutton { color: @sl_fg; }
        /* Keep the Close suggested-action compact, with a solid frame-blue fill. */
        window.sleela-settings button.suggested-action { min-width: 88px; }
        button.suggested-action {
            background: @sl_menu_edge;
            border-color: @sl_menu_edge;
            color: @sl_fg;
        }
        button.suggested-action:hover { background: @sl_menu_hover; }

        textview.sleela-scan-output { color: @sl_fg; background: @sl_bg; }

        /* Secondary / explanatory notes: one dim colour, slightly smaller. */
        label.sleela-software-note,
        label.sleela-trust-note {
            color: alpha(@sl_fg, 0.70);
            font-size: 0.9em;
        }
    )CSS";

    // Load the active title-bar gradient preset. GRADIENT.cfg selects either
    // HORIZONTAL.cfg or VERTICAL.cfg; each preset carries the same palette and
    // stop positions, changing only the gradient axis.
    std::string active_preset = "VERTICAL.cfg";
    {
        std::ifstream selector(asset_path(state, "GRADIENT.cfg"));
        std::string line;
        while (std::getline(selector, line)) {
            if (line.rfind("active=", 0) == 0) {
                const std::string candidate = line.substr(7);
                if (candidate == "HORIZONTAL.cfg" || candidate == "VERTICAL.cfg")
                    active_preset = candidate;
            }
        }
    }
    std::string gradient_direction = "to bottom";
    std::string gradient_stops;
    {
        std::ifstream preset(asset_path(state, active_preset.c_str()));
        std::string line;
        while (std::getline(preset, line)) {
            if (line.rfind("direction=", 0) == 0) {
                const std::string candidate = line.substr(10);
                if (candidate == "to right" || candidate == "to bottom")
                    gradient_direction = candidate;
            } else if (line.rfind("stops=", 0) == 0) {
                gradient_stops = line.substr(6);
            }
        }
    }
    std::string css_text(css);
    // The preset's stop list is authoritative, not just its orientation.
    // This keeps the horizontal reference palette intact while allowing the
    // active vertical preset to use the new navy / silver bevel treatment.
    if (!gradient_stops.empty()) {
        std::string gradient = "linear-gradient(" + gradient_direction + ",";
        std::size_t start = 0;
        while (start < gradient_stops.size()) {
            const std::size_t comma = gradient_stops.find(',', start);
            const std::string stop = gradient_stops.substr(
                start, comma == std::string::npos ? std::string::npos : comma - start);
            const std::size_t colon = stop.find(':');
            if (colon != std::string::npos) {
                const std::string position = stop.substr(0, colon);
                const std::string color = stop.substr(colon + 1);
                try {
                    const int percent = std::stoi(position);
                    if (percent >= 0 && percent <= 100 &&
                        color.size() == 7 && color[0] == '#') {
                        gradient += " " + color + " " + std::to_string(percent) + "%,";
                    }
                } catch (...) {
                    // Ignore malformed stop entries and keep valid ones.
                }
            }
            if (comma == std::string::npos) break;
            start = comma + 1;
        }
        if (gradient.back() == ',') {
            gradient.back() = ')';
            const std::string gradient_anchor = "background: linear-gradient(to right,";
            const auto gradient_pos = css_text.find(gradient_anchor);
            if (gradient_pos != std::string::npos) {
                const auto end_pos = css_text.find(';', gradient_pos);
                if (end_pos != std::string::npos)
                    css_text.replace(gradient_pos, end_pos - gradient_pos,
                                     "background: " + gradient);
            }
        }
    }

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css_text.c_str());
    // Use USER priority (above the system theme) so our window-control and
    // chrome rules actually win over the installed GTK theme -- at APPLICATION
    // priority the theme's more specific `windowcontrols` selectors override
    // our colours and the control icons never changed.
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

void set_font(AppState *state, const char *font) {
    state->config.font = font;
    apply_terminal_style(state);
    save_config(state->config);
}

void set_foreground(AppState *state, const char *color) {
    state->config.foreground = color;
    apply_terminal_style(state);
    save_config(state->config);
}

void set_input_language(AppState *state, const char *language) {
    state->config.input_language = language;
    save_config(state->config);
}

void set_output_language(AppState *state, const char *language) {
    state->config.output_language = language;
    save_config(state->config);
}

void set_source(GtkCheckButton *button, bool &target, AppState *state) {
    target = gtk_check_button_get_active(button);
    save_config(state->config);
}

void make_settings_menu(AppState *state, GtkWidget *settings_button_widget) {
    (void) settings_button_widget;

    // Settings opens as a proper subframe window (not a transient popover), so
    // the adjustments stay on screen while the user works through them. It is a
    // modal child of the main window, consistent with the other SleelaTerminal
    // subframes (installer / software output).
    GtkWidget *frame = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(frame), "SleelaTerminal — Settings");
    gtk_window_set_default_size(GTK_WINDOW(frame), 420, 560);
    gtk_window_set_modal(GTK_WINDOW(frame), TRUE);
    if (state->window != nullptr) {
        gtk_window_set_transient_for(GTK_WINDOW(frame), state->window);
    }
    gtk_widget_add_css_class(frame, "sleela-settings");

    // The settings content can be taller than the frame, so make it scrollable.
    GtkWidget *scroller = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroller, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);

    GtkWidget *title = gtk_label_new("SleelaTerminal Settings");
    gtk_widget_add_css_class(title, "heading");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *footer_section = gtk_label_new("Footer ticker");
    gtk_widget_add_css_class(footer_section, "section");
    gtk_widget_set_halign(footer_section, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), footer_section);
    GtkWidget *footer_delay_note = gtk_label_new("Start rightward scrolling after:");
    gtk_widget_set_halign(footer_delay_note, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), footer_delay_note);
    const unsigned footer_delays[] = {5, 10, 15, 30, 45, 60};
    for (unsigned delay : footer_delays) {
        const std::string label = std::to_string(delay) + " minutes";
        GtkWidget *button = gtk_button_new_with_label(label.c_str());
        g_object_set_data(G_OBJECT(button), "sleela-delay", GUINT_TO_POINTER(delay));
        if (delay == state->config.footer_scroll_delay_minutes) gtk_widget_add_css_class(button, "suggested-action");
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            state->config.footer_scroll_delay_minutes = std::clamp(GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(button), "sleela-delay")), 5u, 60u);
            state->footer_position = 0; state->footer_text_changed_us = g_get_monotonic_time(); save_config(state->config);
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *appearance = gtk_label_new("Presentation");
    gtk_widget_add_css_class(appearance, "section");
    gtk_widget_set_halign(appearance, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), appearance);

    GtkWidget *font_status = gtk_label_new("Installed monospaced fonts");
    gtk_widget_set_halign(font_status, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), font_status);

    // Only offer font families reported by the active OS/Pango font backend.
    PangoFontMap *font_map = pango_cairo_font_map_get_default();
    PangoFontFamily **families = nullptr;
    int family_count = 0;
    pango_font_map_list_families(font_map, &families, &family_count);
    std::vector<std::string> installed_monospace;
    for (int i = 0; i < family_count; ++i) {
        if (pango_font_family_is_monospace(families[i])) {
            const char *name = pango_font_family_get_name(families[i]);
            if (name && *name) installed_monospace.emplace_back(name);
        }
    }
    g_free(families);
    std::sort(installed_monospace.begin(), installed_monospace.end());
    installed_monospace.erase(std::unique(installed_monospace.begin(), installed_monospace.end()),
                              installed_monospace.end());

    for (const auto &family : installed_monospace) {
        GtkWidget *button = gtk_button_new_with_label((family + " 11").c_str());
        const std::string value = family + " 11";
        g_object_set_data_full(G_OBJECT(button), "sleela-value", g_strdup(value.c_str()), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            set_font(state, static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-value")));
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }
    if (installed_monospace.empty()) {
        GtkWidget *button = gtk_button_new_with_label("Monospace 11 (system fallback)");
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
            set_font(static_cast<AppState *>(data), "Monospace 11");
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *size_label = gtk_label_new("Font size");
    gtk_widget_set_halign(size_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), size_label);
    const char *font_sizes[] = {"10", "11", "12", "14", "16"};
    for (const char *size : font_sizes) {
        GtkWidget *button = gtk_button_new_with_label(size);
        g_object_set_data_full(G_OBJECT(button), "sleela-size", g_strdup(size), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            const char *size = static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-size"));
            PangoFontDescription *current = pango_font_description_from_string(state->config.font.c_str());
            const char *family = pango_font_description_get_family(current);
            std::string selected = std::string(family && *family ? family : "Monospace") + " " + size;
            pango_font_description_free(current);
            set_font(state, selected.c_str());
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }
    struct ColorChoice { const char *label; const char *value; };
    static const ColorChoice colors[] = {
        {"Font: White", "#FFFFFF"},
        {"Font: Soft Violet", "#E8D8FF"},
        {"Font: Ice Blue", "#D8F0FF"},
        {"Font: Mint", "#D8FFE8"},
        {"Font: Amber", "#FFE8B0"}
    };
    for (const auto &choice : colors) {
        GtkWidget *button = gtk_button_new_with_label(choice.label);
        g_object_set_data_full(G_OBJECT(button), "sleela-value", g_strdup(choice.value), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            set_foreground(state, static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-value")));
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *language = gtk_label_new("Language Orientation");
    gtk_widget_add_css_class(language, "section");
    gtk_widget_set_halign(language, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), language);

    static const char *languages[] = {"System", "English", "Spanish", "French", "German", "Korean", "Chinese", "Japanese"};
    for (const char *name : languages) {
        GtkWidget *button = gtk_button_new_with_label(name);
        g_object_set_data_full(G_OBJECT(button), "sleela-language", g_strdup(name), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            const char *language = static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-language"));
            set_input_language(state, language);
            set_output_language(state, language);
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *orientation = gtk_label_new("Parametric Ring of Trust / Orientation");
    gtk_widget_add_css_class(orientation, "section");
    gtk_widget_set_halign(orientation, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), orientation);

    GtkWidget *note = gtk_label_new("Local declarations only. Sources remain off until explicitly enabled.");
    gtk_widget_add_css_class(note, "sleela-trust-note");
    gtk_label_set_wrap(GTK_LABEL(note), TRUE);
    gtk_widget_set_halign(note, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), note);

    struct SourceChoice { const char *label; bool *member; };
    SourceChoice sources[] = {
        {"National / channel feeds", &state->config.national_feeds},
        {"Persons of interest", &state->config.persons_of_interest},
        {"Parametric documents / resumes", &state->config.parametric_documents},
        {"National identifiers", &state->config.national_identifiers}
    };
    for (const auto &source : sources) {
        GtkWidget *check = gtk_check_button_new_with_label(source.label);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(check), *source.member);
        g_object_set_data(G_OBJECT(check), "sleela-member", source.member);
        g_signal_connect(check, "toggled", G_CALLBACK(+[](GtkCheckButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            auto *member = static_cast<bool *>(g_object_get_data(G_OBJECT(button), "sleela-member"));
            set_source(button, *member, state);
        }), state);
        gtk_box_append(GTK_BOX(box), check);
    }

    GtkWidget *trust = gtk_label_new(
        "The ring describes orientation and declared relationships; it does not itself establish identity, authority, truth, or legal status.\n"
        "Configuration is human-readable and stored locally under the user's configuration directory.");
    gtk_widget_add_css_class(trust, "sleela-trust-note");
    gtk_label_set_wrap(GTK_LABEL(trust), TRUE);
    gtk_widget_set_halign(trust, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), trust);

    GtkWidget *done = gtk_button_new_with_label("Close");
    gtk_widget_add_css_class(done, "suggested-action");
    gtk_widget_set_halign(done, GTK_ALIGN_END);
    gtk_widget_set_margin_top(done, 8);
    g_signal_connect_swapped(done, "clicked", G_CALLBACK(gtk_window_destroy), frame);
    gtk_box_append(GTK_BOX(box), done);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroller), box);
    gtk_window_set_child(GTK_WINDOW(frame), scroller);
    gtk_window_present(GTK_WINDOW(frame));
}

void open_settings(GtkButton *, gpointer user_data) {
    auto *button = static_cast<GtkWidget *>(user_data);
    auto *state = static_cast<AppState *>(g_object_get_data(G_OBJECT(button), "sleela-state"));
    make_settings_menu(state, button);
}

void software_done(GObject *source, GAsyncResult *result, gpointer user_data) {
    auto *buffer = GTK_TEXT_BUFFER(user_data);
    GError *error = nullptr;
    gchar *stdout_text = nullptr;
    gchar *stderr_text = nullptr;
    if (!g_subprocess_communicate_utf8_finish(G_SUBPROCESS(source), result, &stdout_text, &stderr_text, &error)) {
        gtk_text_buffer_set_text(buffer, error ? error->message : "Software operation failed", -1);
        if (error) g_error_free(error);
        g_free(stdout_text); g_free(stderr_text); g_object_unref(source); return;
    }
    std::string text = stdout_text ? stdout_text : "";
    if (stderr_text && *stderr_text) { text += "\n"; text += stderr_text; }
    gtk_text_buffer_set_text(buffer, text.c_str(), -1);
    g_free(stdout_text); g_free(stderr_text); g_object_unref(source);
}

void run_software_action(AppState *state, const char *action, const char *product, GtkTextBuffer *buffer) {
    const std::string script = installer_path(state);
    const char *argv[] = {"bash", script.c_str(), action, product, nullptr};
    GError *error = nullptr;
    GSubprocess *process = g_subprocess_newv(argv, static_cast<GSubprocessFlags>(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE), &error);
    if (!process) {
        gtk_text_buffer_set_text(buffer, error ? error->message : "Unable to start software operation", -1);
        if (error) {
            g_error_free(error);
        }
        return;
    }
    gtk_text_buffer_set_text(buffer, "Working…", -1);
    g_subprocess_communicate_utf8_async(process, nullptr, nullptr, software_done, buffer);
}

void make_software_menu(AppState *state, GtkWidget *anchor) {
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_widget_add_css_class(GTK_WIDGET(popover), "sleela-software");
    gtk_widget_set_parent(GTK_WIDGET(popover), anchor);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(box, 12); gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12); gtk_widget_set_margin_bottom(box, 12);
    GtkWidget *title = gtk_label_new("SecureJDK 28 / CMD Software Center");
    gtk_widget_add_css_class(title, "heading"); gtk_widget_set_halign(title, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), title);
    GtkWidget *note = gtk_label_new("Scan the configured GitHub source for release state, then explicitly install or prepare the latest source. Alpha/pre-release and final states are reported separately.");
    gtk_widget_add_css_class(note, "sleela-software-note"); gtk_label_set_wrap(GTK_LABEL(note), TRUE); gtk_widget_set_halign(note, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), note);
    GtkTextView *view = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_view_set_editable(view, FALSE); gtk_text_view_set_cursor_visible(view, FALSE);
    gtk_widget_add_css_class(GTK_WIDGET(view), "sleela-scan-output"); gtk_widget_set_size_request(GTK_WIDGET(view), 390, 130); gtk_box_append(GTK_BOX(box), GTK_WIDGET(view));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    struct Payload { AppState *state; std::string action; std::string product; GtkTextBuffer *buffer; };
    auto add = [&](const char *label, const char *action, const char *product) {
        GtkWidget *button = gtk_button_new_with_label(label);
        auto *payload = new Payload{state, action, product, buffer};
        g_signal_connect_data(button, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
            auto *p = static_cast<Payload *>(data);
            run_software_action(p->state, p->action.c_str(), p->product.c_str(), p->buffer);
        }), payload, [](gpointer data, GClosure *) {
            delete static_cast<Payload *>(data);
        }, G_CONNECT_AFTER);
        gtk_box_append(GTK_BOX(box), button);
    };
    add("Scan GitHub Releases", "scan", "all");
    add("Install / Prepare SecureJDK 28", "install", "securejdk28");
    add("Install / Prepare CMD", "install", "cmd");
    add("Install / Prepare Asysma", "install", "asysma");
    add("Install / Prepare All", "install", "all");
    GtkWidget *source = gtk_label_new("Source: github.com/mearvk/Ubuntu.Determinant.Beta.Restricted");
    gtk_widget_add_css_class(source, "sleela-software-note"); gtk_label_set_wrap(GTK_LABEL(source), TRUE); gtk_widget_set_halign(source, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), source);
    gtk_popover_set_child(popover, box); gtk_popover_set_has_arrow(popover, TRUE); gtk_popover_set_autohide(popover, FALSE); gtk_popover_popup(popover);
}


void show_install_prompt(AppState *state, GtkWidget *, const char *product, const char *display_name) {
    GtkWidget *dialog = gtk_window_new();
    gtk_widget_add_css_class(dialog, "sleela-installer");
    gtk_window_set_title(GTK_WINDOW(dialog), "SleelaTerminal — Java Installer");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 520, 300);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), state->window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);

    GtkWidget *title = gtk_label_new("CMD / Java Program Installer");
    gtk_widget_add_css_class(title, "heading");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), title);

    std::string prompt =
        "SleelaTerminal is ready to install the latest appropriate release of " +
        std::string(display_name) +
        ".\n\n"
        "The installer will inspect the configured source, distinguish FINAL from "
        "ALPHA/PRE-RELEASE where available, and prepare the selected program locally.\n\n"
        "Proceed with the installation?";

    GtkWidget *message = gtk_label_new(prompt.c_str());
    gtk_label_set_wrap(GTK_LABEL(message), TRUE);
    gtk_widget_set_halign(message, GTK_ALIGN_START);
    gtk_widget_set_valign(message, GTK_ALIGN_START);
    gtk_widget_set_vexpand(message, TRUE);
    gtk_box_append(GTK_BOX(box), message);

    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(buttons, GTK_ALIGN_END);
    GtkWidget *other = gtk_button_new_with_label("Other Java Programs…");
    GtkWidget *no = gtk_button_new_with_label("No");
    GtkWidget *yes = gtk_button_new_with_label("Yes — Install");
    gtk_widget_add_css_class(yes, "suggested-action");
    gtk_box_append(GTK_BOX(buttons), other);
    gtk_box_append(GTK_BOX(buttons), no);
    gtk_box_append(GTK_BOX(buttons), yes);
    gtk_box_append(GTK_BOX(box), buttons);

    struct InstallPayload { AppState *state; GtkWindow *dialog; std::string product; std::string display_name; };
    auto *payload = new InstallPayload{state, GTK_WINDOW(dialog), product, display_name};

    g_signal_connect_data(yes, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
        auto *p = static_cast<InstallPayload *>(data);
        GtkWidget *result_window = gtk_window_new();
        gtk_widget_add_css_class(result_window, "sleela-installer");
        gtk_window_set_title(GTK_WINDOW(result_window), "SleelaTerminal — Installer Output");
        gtk_window_set_default_size(GTK_WINDOW(result_window), 620, 360);
        gtk_window_set_modal(GTK_WINDOW(result_window), TRUE);
        gtk_window_set_transient_for(GTK_WINDOW(result_window), p->state->window);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
        gtk_widget_set_margin_start(box, 16);
        gtk_widget_set_margin_end(box, 16);
        gtk_widget_set_margin_top(box, 16);
        gtk_widget_set_margin_bottom(box, 16);

        GtkWidget *title = gtk_label_new(("Installing " + p->display_name).c_str());
        gtk_widget_set_halign(title, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(box), title);

        GtkTextView *view = GTK_TEXT_VIEW(gtk_text_view_new());
        gtk_text_view_set_editable(view, FALSE);
        gtk_text_view_set_cursor_visible(view, FALSE);
        gtk_widget_add_css_class(GTK_WIDGET(view), "sleela-scan-output");
        gtk_widget_set_vexpand(GTK_WIDGET(view), TRUE);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(view));

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
        run_software_action(p->state, "install", p->product.c_str(), buffer);
        gtk_window_set_child(GTK_WINDOW(result_window), box);
        gtk_window_present(GTK_WINDOW(result_window));
        gtk_window_destroy(p->dialog);
    }), payload, [](gpointer data, GClosure *) {
        delete static_cast<InstallPayload *>(data);
    }, G_CONNECT_AFTER);

    g_signal_connect_swapped(no, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
    g_signal_connect(other, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
        auto *p = static_cast<InstallPayload *>(data);
        make_software_menu(p->state, GTK_WIDGET(p->state->terminal));
        gtk_window_destroy(p->dialog);
    }), payload);
    gtk_window_set_child(GTK_WINDOW(dialog), box);
    gtk_window_present(GTK_WINDOW(dialog));
}

void open_software_menu(GtkButton *, gpointer user_data) {
    auto *button = static_cast<GtkWidget *>(user_data);
    auto *state = static_cast<AppState *>(g_object_get_data(G_OBJECT(button), "sleela-state"));
    show_install_prompt(state, button, "cmd", "CMD");
}
// Ctrl+C is copy when VTE has a selection. Without a selection it is left to
// the PTY so the shell/foreground program receives the normal interrupt key.
gboolean terminal_key_pressed(GtkEventControllerKey *, guint keyval, guint,
                              GdkModifierType state, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    const bool ctrl = (state & GDK_CONTROL_MASK) != 0;
    const bool shift = (state & GDK_SHIFT_MASK) != 0;

    if (ctrl && keyval == GDK_KEY_c) {
        if (shift || vte_terminal_get_has_selection(terminal)) {
            vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
            return TRUE;
        }
        return FALSE;
    }
    if (ctrl && keyval == GDK_KEY_v) {
        vte_terminal_paste_clipboard(terminal);
        return TRUE;
    }
    return FALSE;
}

void terminal_copy(GtkWidget *, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    if (vte_terminal_get_has_selection(terminal)) {
        vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
    }
}

void terminal_paste(GtkWidget *, gpointer user_data) { vte_terminal_paste_clipboard(VTE_TERMINAL(user_data)); }
void terminal_select_all(GtkWidget *, gpointer user_data) { vte_terminal_select_all(VTE_TERMINAL(user_data)); }
void terminal_clear_selection(GtkWidget *, gpointer user_data) { vte_terminal_unselect_all(VTE_TERMINAL(user_data)); }

void terminal_mouse_menu(GtkGestureClick *gesture, int, double, double, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_widget_add_css_class(GTK_WIDGET(popover), "sleela-context-menu");
    gtk_widget_set_parent(GTK_WIDGET(popover), GTK_WIDGET(terminal));
    gtk_popover_set_has_arrow(popover, TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 8);
    gtk_widget_set_margin_bottom(box, 8);

    GtkWidget *selection = gtk_label_new("Terminal Text Selection");
    gtk_widget_add_css_class(selection, "heading");
    gtk_widget_set_halign(selection, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), selection);
    GtkWidget *left = gtk_label_new("Left Mouse — Select Terminal Text");
    gtk_widget_set_halign(left, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), left);
    GtkWidget *right = gtk_label_new("Right Mouse — Open Terminal Menu");
    gtk_widget_set_halign(right, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), right);
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(box), separator);

    GtkWidget *copy = gtk_button_new_with_label("Copy Selected Text");
    g_signal_connect(copy, "clicked", G_CALLBACK(terminal_copy), terminal);
    g_signal_connect_swapped(copy, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), copy);
    GtkWidget *paste = gtk_button_new_with_label("Paste Text");
    g_signal_connect(paste, "clicked", G_CALLBACK(terminal_paste), terminal);
    g_signal_connect_swapped(paste, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), paste);
    GtkWidget *select_all = gtk_button_new_with_label("Select All Terminal Text");
    g_signal_connect(select_all, "clicked", G_CALLBACK(terminal_select_all), terminal);
    g_signal_connect_swapped(select_all, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), select_all);
    GtkWidget *clear = gtk_button_new_with_label("Clear Selection");
    g_signal_connect(clear, "clicked", G_CALLBACK(terminal_clear_selection), terminal);
    g_signal_connect_swapped(clear, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), clear);

    gtk_popover_set_child(popover, box);
    GdkRectangle rect;
    double x = 0.0, y = 0.0;
    gtk_gesture_get_point(GTK_GESTURE(gesture), nullptr, &x, &y);
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
    rect.width = 1;
    rect.height = 1;
    gtk_popover_set_pointing_to(popover, &rect);
    gtk_popover_popup(popover);
}


std::string readFooterbarTopics() {
    const char *xdg = std::getenv("XDG_CONFIG_HOME");
    const char *home = std::getenv("HOME");
    std::filesystem::path base = (xdg && *xdg) ? xdg :
        ((home && *home) ? (std::filesystem::path(home) / ".config").string() : ".config");
    std::ifstream in(base / "sleela" / "footerbar-topics.conf");
    std::string topic, joined;
    while (in >> topic) {
        if (!joined.empty()) joined += " OR ";
        joined += topic;
    }
    return joined;
}

void set_footer_text(AppState *state, const std::string &value) {
    if (!state || state->footer_full_text == value) return;
    state->footer_full_text = value; state->footer_position = 0; state->footer_text_changed_us = g_get_monotonic_time();
    state->footer_position = 0;
    if (state->footer_text) gtk_widget_queue_draw(state->footer_text);
}

std::string cleanFeedTitle(std::string value) {
    if (value.size() >= 12 && value.compare(0, 9, "<![CDATA[") == 0 &&
        value.compare(value.size() - 3, 3, "]]>") == 0)
        value = value.substr(9, value.size() - 12);
    const std::pair<const char*, const char*> entities[] = {
        {"&amp;", "&"}, {"&quot;", "\""}, {"&#39;", "'"}, {"&apos;", "'"},
        {"&lt;", "<"}, {"&gt;", ">"}
    };
    for (const auto &entity : entities) {
        std::size_t pos = 0;
        const auto length = std::char_traits<char>::length(entity.first);
        while ((pos = value.find(entity.first, pos)) != std::string::npos) {
            value.replace(pos, length, entity.second);
            pos += std::char_traits<char>::length(entity.second);
        }
    }
    std::replace(value.begin(), value.end(), '\n', ' ');
    std::replace(value.begin(), value.end(), '\r', ' ');
    return value;
}

void footer_feed_worker(GTask *task, gpointer, gpointer task_data, GCancellable *) {
    const std::string topics = static_cast<const char *>(task_data);
    const std::string fresh_query = topics + " when:1d";
    gchar *escaped = g_uri_escape_string(fresh_query.c_str(), nullptr, FALSE);
    const std::string query = "https://news.google.com/rss/search?q=" +
        std::string(escaped ? escaped : "") + "&hl=en-US&gl=US&ceid=US:en";
    g_free(escaped);
    gchar *argv[] = {const_cast<gchar *>("curl"), const_cast<gchar *>("-fsSL"),
        const_cast<gchar *>("--max-time"), const_cast<gchar *>("12"),
        const_cast<gchar *>(query.c_str()), nullptr};
    gchar *stdout_text = nullptr, *stderr_text = nullptr;
    gint wait_status = 0;
    GError *error = nullptr;
    const gboolean spawned = g_spawn_sync(nullptr, argv, nullptr, G_SPAWN_SEARCH_PATH,
        nullptr, nullptr, &stdout_text, &stderr_text, &wait_status, &error);
    std::string headline;
    if (spawned && g_spawn_check_wait_status(wait_status, nullptr) && stdout_text) {
        const std::string xml(stdout_text);
        const std::regex item_title("<item[^>]*>[\\s\\S]*?<title[^>]*>([\\s\\S]*?)</title>", std::regex::icase);
        unsigned headline_count = 0;
        for (std::sregex_iterator it(xml.begin(), xml.end(), item_title), end; it != end && headline_count < 8; ++it, ++headline_count) {
            const std::string title = cleanFeedTitle((*it)[1].str());
            if (title.empty()) continue;
            if (!headline.empty()) headline += "   •   ";
            headline += title;
        }
    }
    if (headline.empty()) headline = "No current headlines returned; check network access and curl";
    g_free(stdout_text);
    g_free(stderr_text);
    if (error) g_error_free(error);
    g_task_return_pointer(task, g_strdup(headline.c_str()), g_free);
}

void footer_feed_finished(GObject *source, GAsyncResult *result, gpointer user_data) {
    (void)source;
    auto *state = static_cast<AppState *>(user_data);
    GError *error = nullptr;
    gchar *headline = static_cast<gchar *>(g_task_propagate_pointer(G_TASK(result), &error));
    state->feed_fetching = false;
    if (state->footer_text != nullptr) {
        if (headline) {
            const std::string display = "RSS  •  " + state->feed_topics_signature + "  —  " + headline;
            set_footer_text(state, display);
        } else if (error) {
            set_footer_text(state, "RSS feed unavailable");
        }
    }
    if (headline) g_free(headline);
    if (error) g_error_free(error);
}

gboolean footer_feed_tick(gpointer user_data);

void footer_ticker_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (!state || state->footer_full_text.empty() || width <= 0 || height <= 0) return;
    PangoLayout *layout = pango_cairo_create_layout(cr);
    PangoFontDescription *font = pango_font_description_from_string("Sans Bold 10");
    pango_layout_set_font_description(layout, font);
    pango_layout_set_text(layout, state->footer_full_text.c_str(), -1);
    int text_width = 0, text_height = 0;
    pango_layout_get_pixel_size(layout, &text_width, &text_height);
    const gint64 now = g_get_monotonic_time();
    const gint64 delay = static_cast<gint64>(state->config.footer_scroll_delay_minutes) * 60 * G_USEC_PER_SEC;
    const bool scrolling = state->footer_text_changed_us != 0 &&
        now - state->footer_text_changed_us >= delay;
    // Show one copy at the left during the pause. Then it travels right and
    // fully exits; only after it has gone does that same single line re-enter
    // from the left. There is never a second, duplicated copy on screen.
    double x = 0.0;
    double alpha = 1.0;
    if (scrolling) {
        const double cycle = static_cast<double>(std::max(1, width + text_width));
        const double phase = std::fmod(static_cast<double>(state->footer_position), cycle);
        x = phase < width ? phase : -static_cast<double>(text_width) + (phase - width);
        // Fade only as the line leaves the right edge.
        const double fade_start = static_cast<double>(width) * 0.55;
        if (x > fade_start) {
            alpha = std::clamp((static_cast<double>(width) - x) /
                               std::max(1.0, static_cast<double>(width) - fade_start), 0.0, 1.0);
        }
    }
    const double y = std::max(0.0, (height - text_height) / 2.0);
    cairo_save(cr);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_clip(cr);
    // Draw a crisp 1px black outline around the grey ticker lettering.
    const double outline[][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, alpha);
    for (const auto &offset : outline) {
        cairo_move_to(cr, x + offset[0], y + offset[1]);
        pango_cairo_show_layout(cr, layout);
    }
    cairo_set_source_rgba(cr, 0.72, 0.72, 0.72, alpha);
    cairo_move_to(cr, x, y);
    pango_cairo_show_layout(cr, layout);
    cairo_restore(cr);
    pango_font_description_free(font);
    g_object_unref(layout);
    (void)area;
}

gboolean footer_scroll_tick(gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (!state->footer_text || state->footer_full_text.empty()) return G_SOURCE_CONTINUE;
    const gint64 now = g_get_monotonic_time();
    const gint64 delay = static_cast<gint64>(state->config.footer_scroll_delay_minutes) * 60 * G_USEC_PER_SEC;
    if (state->footer_text_changed_us != 0 && now - state->footer_text_changed_us >= delay) {
        // Approx. 13 px/s at the 150ms cadence. Keep advancing through one
        // complete cycle: the line exits right, then re-enters from the left.
        // Modulo positioning in the draw callback guarantees a single copy.
        state->footer_position += 2;
        gtk_widget_queue_draw(state->footer_text);
    }
    return G_SOURCE_CONTINUE;
}

gboolean footer_feed_tick(gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    const std::string topics = readFooterbarTopics();
    if (topics.empty()) {
        state->feed_topics_signature.clear();
        if (state->footer_text != nullptr)
            set_footer_text(state, "Secure Shell Interface  •  Interactive session");
        return G_SOURCE_CONTINUE;
    }
    const gint64 now = g_get_monotonic_time();
    if (topics != state->feed_topics_signature) {
        state->feed_topics_signature = topics;
        state->feed_last_fetch = 0;
    }
    if (!state->feed_fetching && (state->feed_last_fetch == 0 ||
        now - state->feed_last_fetch >= ([&]() {
            GDateTime *local = g_date_time_new_now_local(); const int hour = local ? g_date_time_get_hour(local) : 12;
            if (local) g_date_time_unref(local);
            return (hour >= 9 && hour < 17 ? 15LL : 60LL) * 60 * G_USEC_PER_SEC;
        })())) {
        state->feed_fetching = true;
        state->feed_last_fetch = now;
        GTask *task = g_task_new(nullptr, nullptr, footer_feed_finished, state);
        g_task_set_task_data(task, g_strdup(topics.c_str()), g_free);
        g_task_run_in_thread(task, footer_feed_worker);
        g_object_unref(task);
        if (state->footer_text != nullptr) {
            const std::string display = "RSS  •  " + topics + "  —  loading headlines…";
            set_footer_text(state, display);
        }
    }
    return G_SOURCE_CONTINUE;
}

const std::vector<std::string> &footer_messages() {
    // A single, static status line. The footer is a calm frame, not a ticker:
    // moving text competes with terminal output. See UI-PRINCIPLES.md (6).
    static const std::vector<std::string> messages = {
        "Secure Shell Interface  •  Interactive session"
    };
    return messages;
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->footer_tick != 0) {
        g_source_remove(state->footer_tick);
        state->footer_tick = 0;
    }
    if (state->footer_scroll_tick != 0) { g_source_remove(state->footer_scroll_tick); state->footer_scroll_tick = 0; }
    if (state->throbber_tick != 0 && state->throbber != nullptr) {
        gtk_widget_remove_tick_callback(state->throbber, state->throbber_tick);
        state->throbber_tick = 0;
    }
    state->throbber = nullptr;
    state->footer_text = nullptr;
    state->child_pid = 0;
    if (state->window != nullptr) {
        GtkApplication *application = gtk_window_get_application(state->window);
        GtkWindow *window = state->window;
        state->window = nullptr;
        gtk_window_destroy(window);
        if (application != nullptr) g_application_quit(G_APPLICATION(application));
    }
}

void shell_spawned(VteTerminal *, GPid child_pid, GError *error, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (error != nullptr || child_pid <= 0) {
        state->child_pid = 0;
        return;
    }
    state->child_pid = child_pid;
}

gboolean window_close_request(GtkWindow *window, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->child_pid > 0) {
        ::kill(static_cast<pid_t>(state->child_pid), SIGHUP);
        state->child_pid = 0;
    }
    if (state->footer_tick != 0) {
        g_source_remove(state->footer_tick);
        state->footer_tick = 0;
    }
    if (state->footer_scroll_tick != 0) { g_source_remove(state->footer_scroll_tick); state->footer_scroll_tick = 0; }
    if (state->throbber_tick != 0 && state->throbber != nullptr) {
        gtk_widget_remove_tick_callback(state->throbber, state->throbber_tick);
        state->throbber_tick = 0;
    }
    state->throbber = nullptr;
    state->footer_text = nullptr;
    gtk_window_destroy(window);
    g_application_quit(G_APPLICATION(gtk_window_get_application(window)));
    return TRUE;
}

// Render the living field. The strip is NOT one uniform level: it rests at the
// title-bar chrome colour and is modulated locally by every excitation ("child
// of wattage"), each adding a relative darker/lighter shade near its position.
// The sum, drawn per column, is a texture that flows left->right. Under strong
// amplification, latent per-cell hues (orange/green/blue/red) surface.
void throbber_draw(GtkDrawingArea *, cairo_t *cr, int width, int height, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (width <= 0 || height <= 0) return;
    state->throbber_w = width;

    // Keep the rail pure white edge-to-edge. Translucent glass-like colour
    // layers are composited above it; no opaque colour block replaces the rail.
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    const double amp = state->throbber_amp;
    const double colour_gate = std::clamp((amp - 0.80) / 0.15, 0.0, 1.0);
    const double t = state->throbber_t;
    const double tau = 6.283185307179586;
    auto clamp01 = [](double v) { return std::clamp(v, 0.0, 1.0); };
    auto mix = [](double a, double b, double f) { return a + (b - a) * f; };

    // Thin-film interference palette: cyan, sapphire, violet, rose and amber.
    auto spectrum = [&](double phase) {
        const double p = phase - std::floor(phase);
        const double segment = p * 5.0;
        const int i = std::min(4, static_cast<int>(segment));
        const double f = segment - i;
        static const double colours[6][3] = {
            {0.12, 0.92, 1.00}, {0.22, 0.43, 1.00},
            {0.66, 0.30, 1.00}, {1.00, 0.35, 0.72},
            {1.00, 0.72, 0.30}, {0.12, 0.92, 1.00}
        };
        return std::array<double, 3>{
            mix(colours[i][0], colours[i + 1][0], f),
            mix(colours[i][1], colours[i + 1][1], f),
            mix(colours[i][2], colours[i + 1][2], f)
        };
    };

    std::vector<double> field(static_cast<size_t>(width), 0.0);
    std::vector<double> hue_r(static_cast<size_t>(width), 0.0);
    std::vector<double> hue_g(static_cast<size_t>(width), 0.0);
    std::vector<double> hue_b(static_cast<size_t>(width), 0.0);
    std::vector<double> hue_w(static_cast<size_t>(width), 0.0);
    for (const auto &c : state->throbber_cells) {
        if (!c.alive || c.intensity < 0.001) continue;
        const int x0 = std::max(0, static_cast<int>(std::floor(c.x - c.sigma * 4.8)));
        const int x1 = std::min(width - 1, static_cast<int>(std::ceil(c.x + c.sigma * 2.4)));
        const double flick = 0.84 + 0.16 * std::sin(t * 6.0 + c.phase);
        for (int x = x0; x <= x1; ++x) {
            const double dx = (x + 0.5) - c.x;
            // Asymmetric falloff suggests a refracted edge and trailing caustic.
            const double sigma = std::max(0.5, dx < 0.0 ? c.sigma * 1.45 : c.sigma * 0.72);
            const double e = dx / sigma;
            const double contribution = c.intensity * std::exp(-0.5 * e * e) * flick;
            const size_t ix = static_cast<size_t>(x);
            field[ix] += contribution;
            if (c.tinted) {
                hue_r[ix] += c.tr * contribution;
                hue_g[ix] += c.tg * contribution;
                hue_b[ix] += c.tb * contribution;
                hue_w[ix] += contribution;
            }
        }
    }

    for (int x = 0; x < width; ++x) {
        const size_t ix = static_cast<size_t>(x);
        const double f = clamp01(field[ix] * (0.48 + 0.92 * amp));
        if (f < 0.003) continue;

        const double xf = static_cast<double>(x) / std::max(1, width);
        const double sway = 0.76 + 0.24 * (0.5 + 0.5 *
            std::sin(xf * tau * 1.15 - t * 1.25 + 0.35 * std::sin(t * 0.31)));
        const double band_phase = xf * 1.65 - t * 0.075 + 0.07 * std::sin(t * 0.19 + xf * tau);
        const auto prism = spectrum(band_phase);

        double tr = 0.10, tg = 0.22, tb = 0.42;
        if (hue_w[ix] > 0.0001 && colour_gate > 0.0) {
            const double hue_mix = colour_gate * std::min(1.0, f);
            tr = mix(tr, hue_r[ix] / hue_w[ix], hue_mix);
            tg = mix(tg, hue_g[ix] / hue_w[ix], hue_mix);
            tb = mix(tb, hue_b[ix] / hue_w[ix], hue_mix);
        }
        const double iridescence = 0.16 + 0.62 * colour_gate;
        tr = mix(tr, prism[0], iridescence);
        tg = mix(tg, prism[1], iridescence);
        tb = mix(tb, prism[2], iridescence);

        // Transparent coloured body and a moving specular line create the glass edge.
        const double alpha = std::clamp(f * sway * (0.28 + 0.38 * amp), 0.0, 0.78);
        cairo_set_source_rgba(cr, tr, tg, tb, alpha);
        cairo_rectangle(cr, x, 0, 1, height);
        cairo_fill(cr);

        const double glint_phase = std::sin(xf * tau * 2.3 - t * 2.2 + std::sin(t * 0.7));
        if (f > 0.12 && glint_phase > 0.80) {
            const double glint_alpha = (glint_phase - 0.80) * 0.42 * f;
            cairo_set_source_rgba(cr, 0.82, 0.98, 1.0, glint_alpha);
            cairo_rectangle(cr, x, 0, 1, height);
            cairo_fill(cr);
        }

        // A restrained spectral edge gives the cell a holographic rim.
        if (x > 0 && (static_cast<int>(x + t * 13.0) % 17) < 2 && f > 0.20) {
            cairo_set_source_rgba(cr, prism[0], prism[1], prism[2], 0.24 * f);
            cairo_rectangle(cr, x, 0, 1, height);
            cairo_fill(cr);
        }
    }
}

// Spawn a fresh excitation. Birth position spans the whole 0-100% of the strip
// width: most children enter at the left edge (preserving the dominant
// left->right feed), but a share are born anywhere across 0-100% and simply
// fade in at that spot (intensity eases up from 0). Either way the pulse then
// travels left->right all the way through to 100%.
// Paint a restrained, diffuse reflection of the title-bar throbber into the
// terminal pane. The overlay is non-interactive, so terminal input and selection
// remain owned by VTE. Keep alpha low so text contrast remains the priority.
void terminal_glow_draw(GtkDrawingArea *, cairo_t *cr, int width, int height, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (width <= 0 || height <= 0 || state->throbber_w <= 0) return;

    const double pane_scale = static_cast<double>(width) / state->throbber_w;
    for (const auto &c : state->throbber_cells) {
        // The white rail is deliberately not reflected onto the text pane.
        // Only explicitly coloured cells cast light into the terminal.
        if (!c.alive || !c.tinted || c.intensity < 0.008) continue;
        const double x = c.x * pane_scale;
        const double sigma = std::max(14.0, c.sigma * 2.8 * pane_scale);
        const double visual_width = std::max(1.0, c.sigma * 2.4);
        const double width_energy = std::clamp(visual_width / 50.0, 0.35, 2.8);
        const double colour_gate = std::clamp((state->throbber_amp - 0.80) / 0.15, 0.0, 1.0);
        const double strength = std::clamp(
            c.intensity * colour_gate * 0.12 * width_energy, 0.0, 0.12);
        if (strength < 0.001) continue;

        cairo_pattern_t *glow = cairo_pattern_create_radial(x, 0.0, 1.0, x, 0.0, sigma);
        cairo_pattern_add_color_stop_rgba(glow, 0.0, c.tr, c.tg, c.tb, strength);
        cairo_pattern_add_color_stop_rgba(glow, 1.0, c.tr, c.tg, c.tb, 0.0);
        cairo_rectangle(cr, 0, 0, width,
                        std::min(height, static_cast<int>(std::ceil(sigma))));
        cairo_set_source(cr, glow);
        cairo_fill(cr);
        cairo_pattern_destroy(glow);
    }
}

void throbber_spawn(AppState *state, int W) {
    ThrobberCell c;
    c.alive = true;

    // Birth position: odds favour starting around 35% of the width, so the
    // flow then does most of its pulsing through the central sweet spot before
    // fading by ~65%. A minority still enter from the left edge (keeping the
    // left->right feed) or appear elsewhere, but the bulk cluster near 0.35W.
    const double roll = g_random_double();
    if (roll < 0.65) {
        // cluster around 35% with a modest spread (approx normal via two rolls)
        double gauss = (g_random_double() + g_random_double() - 1.0); // ~[-1,1], centre-weighted
        double frac = 0.35 + gauss * 0.12;                            // mostly 0.23..0.47
        if (frac < 0.0) frac = 0.0;
        if (frac > 1.0) frac = 1.0;
        c.x = frac * W;
    } else if (roll < 0.85) {
        c.x = -4.0 + g_random_double() * 6.0;        // enter from just off the left
    } else {
        c.x = g_random_double() * W;                 // occasional anywhere in 0-100%
    }

    // Varied speed: some children glide, some race. Wider spread so the field
    // reads as many independent reflexes rather than a uniform conveyor.
    c.v = 14.0 + g_random_double() * g_random_double() * 150.0; // skewed: mostly slow, a few fast
    c.a = 0.0;
    c.j = 0.0;
    c.base_intensity = 0.35 + g_random_double() * 0.6; // relative shade strength
    c.intensity = 0.0;                                  // eases up from birth (fade-in)
    c.energy = 1.0;
    c.sigma = 7.0 + g_random_double() * 25.0;        // glow half-width; broad cells can exceed 50px overall
    c.speed_mul = 0.45 + g_random_double() * g_random_double() * 2.6; // 0.45..~3x, skewed slow
    c.phase = g_random_double() * 6.2831853;
    // 1 in 30 children may pause or run right->left for 20-50 px.
    c.retro = (g_random_int_range(0, 30) == 0);
    c.retro_px = c.retro ? (20.0 + g_random_double() * 30.0) : 0.0;

    // Each child carries a latent hue (orange / green / blue / red). It stays
    // dormant at normal wattage and only surfaces under strong amplification
    // (see throbber_draw). Give most children a hue so colour can appear when
    // the field is driven hard.
    static const double hues[4][3] = {
        {1.00, 0.55, 0.10},  // orange
        {0.20, 0.85, 0.35},  // green
        {0.30, 0.60, 1.00},  // blue
        {1.00, 0.25, 0.25},  // red
    };
    c.tinted = (g_random_double() < 0.75);
    if (c.tinted) {
        const int h = g_random_int_range(0, 4);
        c.tr = hues[h][0]; c.tg = hues[h][1]; c.tb = hues[h][2];
    }

    state->throbber_cells.push_back(c);
}

// Frame-clock-driven update: GTK calls this on the UI thread at display cadence.
// Simulate at up to ~30 updates/second, skip redundant refreshes on 60/120/144 Hz
// displays, and clamp long pauses (minimize, debugger stop, scheduling delay).
gboolean throbber_tick(GtkWidget *, GdkFrameClock *frame_clock, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->throbber == nullptr) return G_SOURCE_REMOVE;

    const gint64 frame_us = gdk_frame_clock_get_frame_time(frame_clock);
    if (state->throbber_last_frame_us == 0) {
        state->throbber_last_frame_us = frame_us;
        return G_SOURCE_CONTINUE;
    }
    const gint64 elapsed_us = frame_us - state->throbber_last_frame_us;
    if (elapsed_us < 33333) return G_SOURCE_CONTINUE; // cap simulation to ~30 FPS
    state->throbber_last_frame_us = frame_us;
    const double dt = std::clamp(elapsed_us / 1000000.0, 0.001, 0.050);
    state->throbber_t += dt;
    const int W = state->throbber_w > 0 ? state->throbber_w : 600;

    // --- Reflexive amplification field (smooth, 2nd order) --------------------
    // A slowly varying global gain that makes the whole field speed up / glow
    // more at "pertinent" moments, then relax. Driven to a wandering target
    // through a critically-damped-ish spring so it never snaps.
    double amp_target = 0.55
        + 0.30 * std::sin(state->throbber_t * 0.21)
        + 0.12 * std::sin(state->throbber_t * 0.53 + 1.3);
    if (amp_target < 0.1) amp_target = 0.1;
    if (amp_target > 1.0) amp_target = 1.0;
    double amp_acc = (amp_target - state->throbber_amp) * 2.2 - state->throbber_amp_v * 1.6;
    state->throbber_amp_v += amp_acc * dt;
    state->throbber_amp   += state->throbber_amp_v * dt;
    if (state->throbber_amp < 0.0) state->throbber_amp = 0.0;
    if (state->throbber_amp > 1.0) state->throbber_amp = 1.0;

    // --- Spawn rate scales with amplification (more wattage -> more children) -
    double rate = 6.0 + 10.0 * state->throbber_amp;   // children per second
    state->throbber_spawn_accum += rate * dt;
    while (state->throbber_spawn_accum >= 1.0) {
        state->throbber_spawn_accum -= 1.0;
        if (state->throbber_cells.size() < 64) throbber_spawn(state, W);
    }

    // --- Advance each child with 3rd-order (jerk-driven) motion ---------------
    for (auto &c : state->throbber_cells) {
        if (!c.alive) continue;

        // Reflexive, amplified desired speed, scaled by this cell's own speed
        // character so some pulses glide and others race across the sweet spot.
        double desired_v = (18.0 + 70.0 * state->throbber_amp) * c.speed_mul;

        // Size-dependent pacing: broad light bars (>50px overall) glide more
        // slowly; medium bars gain a little pace; narrow sparks move faster.
        // Width is estimated from the visible Gaussian footprint (about 2.4σ).
        const double visual_width = c.sigma * 2.4;
        if (visual_width > 50.0) desired_v /= 1.7;
        else if (visual_width >= 25.0) desired_v *= 1.12;
        else desired_v *= 1.32;

        if (c.retro && c.retro_px > 0.0) {
            // This rare child pauses or glides right->left for its allotment.
            desired_v = -(10.0 + 25.0 * state->throbber_amp);
            c.retro_px -= std::fabs(c.v) * dt;
        }

        // 3rd order: steer JERK toward closing the velocity error, integrate
        // jerk -> accel -> vel -> pos. This keeps the *rate of change of
        // acceleration* bounded, so speed-ups/slow-downs are gentle, not steppy.
        double v_err = desired_v - c.v;
        double jerk_target = v_err * 6.0;                       // reflexive gain
        c.j += (jerk_target - c.j) * 0.35;                      // ease the jerk
        c.a += c.j * dt;
        if (c.a >  900.0) c.a =  900.0;
        if (c.a < -900.0) c.a = -900.0;
        c.v += c.a * dt;
        c.x += c.v * dt;

        // --- Centre sweet spot (40-60%), soft shoulders to ~35/65% ----------
        // The pulsing concentrates in the middle of the strip. A column's
        // "centre weight" is ~1 across 40-60% and falls off outside it; pulses
        // that drift away from centre both glow less AND decay faster, so the
        // visible life of a pulse is spent mainly in the 35-65% band.
        const double frac = (W > 0) ? (c.x / W) : 0.5;
        double dist = std::fabs(frac - 0.50);
        double center_weight;
        if (dist <= 0.10) {
            center_weight = 1.0;                         // flat top across 40-60%
        } else {
            // Gaussian shoulders beyond +/-10%: ~0.37 at the 35/65% edges, less
            // further out, so off-centre pulses clearly recede.
            double e = (dist - 0.10) / 0.07;
            center_weight = std::exp(-0.5 * e * e);
        }

        // Energy (the "amplification series") decays faster the farther the
        // pulse is from the sweet spot: in-band it lingers, out-of-band it dies
        // off quickly because it "wasn't of the sweet spot".
        double decay = 0.10 + 0.05 * state->throbber_amp
                     + 0.9 * (1.0 - center_weight);       // off-centre penalty
        c.energy -= dt * decay;
        if (c.energy < 0.0) c.energy = 0.0;

        double edge_fade = 1.0;
        if (c.x > W * 0.90) edge_fade = std::max(0.0, (W - c.x) / (W * 0.10));
        double breath = 0.9 + 0.1 * std::sin(state->throbber_t * 1.7 + c.phase);
        // Brightness is gated by centre proximity, so the pulsing lives in the
        // middle and less-centred pulses are already fading relative to it.
        double target_i = c.base_intensity * center_weight * edge_fade * c.energy * breath;
        // Ease the live intensity toward its target so fade-in / fade-out smooth.
        c.intensity += (target_i - c.intensity) * 0.42;

        // The pulse still travels left->right across the whole width; it only
        // dies on exit at the right edge (or once it has faded to nothing well
        // past the sweet spot, so spent pulses don't linger invisibly).
        if (c.x > W + 6.0) c.alive = false;
        else if (frac > 0.70 && c.energy <= 0.0 && c.intensity < 0.01) c.alive = false;
    }

    // Compact the pool: drop dead cells.
    state->throbber_cells.erase(
        std::remove_if(state->throbber_cells.begin(), state->throbber_cells.end(),
                       [](const ThrobberCell &c) { return !c.alive; }),
        state->throbber_cells.end());

    gtk_widget_queue_draw(state->throbber);
    if (state->terminal_glow != nullptr) gtk_widget_queue_draw(state->terminal_glow);
    return G_SOURCE_CONTINUE;
}

void activate(GtkApplication *application, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    install_css(state);
    load_config(state->config);

    GtkWidget *window = gtk_application_window_new(application);
    state->window = GTK_WINDOW(window);
    gtk_window_set_title(state->window, kWindowTitle);
    gtk_window_set_default_size(state->window, 1100, 700);

    GtkWidget *header = gtk_header_bar_new();
    gtk_widget_add_css_class(header, "sleela-titlebar");
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE);
    // Guarantee all three window controls are present (and on the right) so the
    // minimize / maximize / close icons are shown and can be lit by the CSS.
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header), ":minimize,maximize,close");

    // Brand logo in the upper-left of the title bar. The asset is pre-trimmed
    // to the logo's minimum 2D content box with a transparent background (see
    // tools/logo/Trim.java), so it sits flush at the left with no surrounding
    // whitespace. Packed first so it is the leftmost title-bar element.
    GtkWidget *logo = gtk_image_new_from_file(asset_path(state, "titlebar-logo.png").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(logo), 22);
    gtk_widget_add_css_class(logo, "sleela-titlebar-logo");
    gtk_widget_set_valign(logo, GTK_ALIGN_CENTER);
    gtk_widget_set_tooltip_text(logo, "SleelaTerminal™ — Debian / Windows Terminal");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), logo);

    // Settings control on the RIGHT of the title bar: a custom three-bar
    // hamburger glyph, filled title-bar blue with a 1px black outline. Clicking
    // it opens the Settings subframe window. Packed at the end so it sits right.
    GtkWidget *settings = gtk_button_new();
    GtkWidget *settings_glyph = gtk_drawing_area_new();
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(settings_glyph), 15);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(settings_glyph), 10); // ~30% shorter vertically
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(settings_glyph),
        [](GtkDrawingArea *, cairo_t *cr, int width, int height, gpointer) {
            const double bar_height = 1.8;
            const double left = 1.0;
            const double right = static_cast<double>(width) - 1.0;
            const double inset = 1.0;
            const double bars_y[] = {inset, inset + 2.2, inset + 4.4};
            for (double y : bars_y) {
                const double radius = 0.9;
                cairo_new_path(cr);
                cairo_move_to(cr, left + radius, y);
                cairo_line_to(cr, right - radius, y);
                cairo_arc(cr, right - radius, y + radius, radius, -M_PI_2, 0.0);
                cairo_line_to(cr, right, y + bar_height - radius);
                cairo_arc(cr, right - radius, y + bar_height - radius, radius, 0.0, M_PI_2);
                cairo_line_to(cr, left + radius, y + bar_height);
                cairo_arc(cr, left + radius, y + bar_height - radius, radius, M_PI_2, M_PI);
                cairo_line_to(cr, left, y + radius);
                cairo_arc(cr, left + radius, y + radius, radius, M_PI, 3.0 * M_PI_2);
                cairo_close_path(cr);

                // Black 1px outline, with a subtle vertical blue bevel for depth.
                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_set_line_width(cr, 1.0);
                cairo_stroke_preserve(cr);
                cairo_pattern_t *bevel = cairo_pattern_create_linear(0.0, y, 0.0, y + bar_height);
                cairo_pattern_add_color_stop_rgb(bevel, 0.0, 0.16, 0.29, 0.62); // dark-blue top highlight
                cairo_pattern_add_color_stop_rgb(bevel, 0.45, 0.09, 0.19, 0.44);
                cairo_pattern_add_color_stop_rgb(bevel, 1.0, 0.04, 0.10, 0.27); // deeper shaded lower edge
                cairo_set_source(cr, bevel);
                cairo_fill(cr);
                cairo_pattern_destroy(bevel);

                // Fine top glint gives the bars a restrained 3D lighting effect.
                cairo_new_path(cr);
                cairo_move_to(cr, left + radius + 0.4, y + 0.35);
                cairo_line_to(cr, right - radius - 0.4, y + 0.35);
                cairo_set_source_rgba(cr, 0.58, 0.73, 1.0, 0.65);
                cairo_set_line_width(cr, 0.45);
                cairo_stroke(cr);
            }
        }, nullptr, nullptr);
    gtk_button_set_child(GTK_BUTTON(settings), settings_glyph);
    gtk_widget_set_tooltip_text(settings, "SleelaTerminal Settings");
    gtk_widget_add_css_class(settings, "sleela-settings-btn");
    gtk_widget_set_valign(settings, GTK_ALIGN_CENTER);
    g_object_set_data(G_OBJECT(settings), "sleela-state", state);
    g_signal_connect(settings, "clicked", G_CALLBACK(open_settings), settings);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), settings);

    GtkWidget *title = gtk_label_new(kWindowTitle);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title);
    gtk_window_set_titlebar(state->window, header);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Full-width 4px pure-white throbber rail below the title bar. The moving
    // coloured field is composited inside the white pixels; the frame clock
    // animates the cells and their slow travelling sway.
    GtkWidget *throbber = gtk_drawing_area_new();
    state->throbber = throbber;
    gtk_widget_add_css_class(throbber, "sleela-throbber");
    gtk_widget_set_hexpand(throbber, TRUE);
    gtk_widget_set_size_request(throbber, -1, 4);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(throbber), throbber_draw, state, nullptr);
    gtk_box_append(GTK_BOX(root), throbber);

    GtkWidget *terminal = vte_terminal_new();
    state->terminal = VTE_TERMINAL(terminal);
    gtk_widget_set_hexpand(terminal, TRUE);
    gtk_widget_set_vexpand(terminal, TRUE);
    vte_terminal_set_scrollback_lines(state->terminal, 10000);
    apply_terminal_style(state);

    GtkEventController *keys = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(keys, GTK_PHASE_CAPTURE);
    g_signal_connect(keys, "key-pressed", G_CALLBACK(terminal_key_pressed), terminal);
    gtk_widget_add_controller(terminal, keys);

    GtkGesture *right_click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click), GDK_BUTTON_SECONDARY);
    g_signal_connect(right_click, "pressed", G_CALLBACK(terminal_mouse_menu), terminal);
    gtk_widget_add_controller(terminal, GTK_EVENT_CONTROLLER(right_click));
    // Let the throbber cast a faint, moving light field over the terminal
    // surface without intercepting clicks, text selection, or keyboard input.
    GtkWidget *terminal_overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(terminal_overlay, TRUE);
    gtk_widget_set_vexpand(terminal_overlay, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(terminal_overlay), terminal);
    GtkWidget *glow = gtk_drawing_area_new();
    state->terminal_glow = glow;
    gtk_widget_add_css_class(glow, "sleela-terminal-glow");
    gtk_widget_set_hexpand(glow, TRUE);
    gtk_widget_set_vexpand(glow, TRUE);
    gtk_widget_set_can_target(glow, FALSE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(glow), terminal_glow_draw, state, nullptr);
    gtk_overlay_add_overlay(GTK_OVERLAY(terminal_overlay), glow);
    gtk_overlay_set_measure_overlay(GTK_OVERLAY(terminal_overlay), glow, FALSE);
    gtk_box_append(GTK_BOX(root), terminal_overlay);

    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class(footer, "sleela-footer");
    gtk_widget_set_hexpand(footer, TRUE);

    GtkWidget *brand = gtk_label_new("SLeeLa");
    gtk_widget_add_css_class(brand, "sleela-footer-brand");
    gtk_widget_set_margin_start(brand, 12);
    gtk_widget_set_margin_end(brand, 4);
    gtk_widget_set_valign(brand, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), brand);

    GtkWidget *separator = gtk_label_new("│");
    gtk_widget_add_css_class(separator, "sleela-footer-separator");
    gtk_widget_set_valign(separator, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), separator);

    GtkWidget *footer_text = gtk_drawing_area_new();
    state->footer_text = footer_text;
    gtk_widget_add_css_class(footer_text, "sleela-footer-ticker");
    gtk_widget_set_hexpand(footer_text, TRUE);
    gtk_widget_set_halign(footer_text, GTK_ALIGN_FILL);
    gtk_widget_set_valign(footer_text, GTK_ALIGN_CENTER);
    // DrawingArea measures independently of headline length. Pango renders the
    // entire RSS string inside the fixed allocated viewport, without ellipses
    // and without allowing the headline to resize the Terminal window.
    gtk_widget_set_size_request(footer_text, 160, 30);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(footer_text), footer_ticker_draw, state, nullptr);
    gtk_box_append(GTK_BOX(footer), footer_text);

    // CMD footer control: strict image only; the image itself is the clickable surface.
    GtkWidget *java_button = gtk_button_new();
    gtk_widget_add_css_class(java_button, "sleela-footer-java");
    gtk_widget_set_tooltip_text(java_button, "CMD — Java native launcher / SecureJDK 28 software center");
    GtkWidget *java_image = gtk_image_new_from_file(asset_path(state, "cmd.svg").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(java_image), 24); gtk_button_set_child(GTK_BUTTON(java_button), java_image);
    g_object_set_data(G_OBJECT(java_button), "sleela-state", state); g_signal_connect(java_button, "clicked", G_CALLBACK(open_software_menu), java_button); gtk_box_append(GTK_BOX(footer), java_button);

    GtkWidget *status = gtk_label_new(kVersion);
    gtk_widget_set_margin_start(status, 8);
    gtk_widget_set_margin_end(status, 12);
    gtk_widget_set_valign(status, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), status);

    gtk_box_append(GTK_BOX(root), footer);
    gtk_window_set_child(state->window, root);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(child_exited), state);
    g_signal_connect(state->window, "close-request", G_CALLBACK(window_close_request), state);

    // Keep the status stable for the configured delay, then run a complete
    // rightward marquee pass and refresh the RSS feed after the line exits.
    state->footer_position = 0;
    state->footer_text_changed_us = g_get_monotonic_time();
    state->footer_full_text = footer_messages().front();
    state->footer_tick = g_timeout_add_seconds(30, footer_feed_tick, state);
    state->footer_scroll_tick = g_timeout_add(150, footer_scroll_tick, state);
    footer_feed_tick(state);

    // GTK's frame clock schedules updates with display frames and naturally
    // pauses callbacks while the drawing widget is unmapped. This avoids a
    // free-running timer and keeps all GTK/Cairo work on the UI thread.
    state->throbber_last_frame_us = 0;
    state->throbber_tick = gtk_widget_add_tick_callback(
        throbber, throbber_tick, state, nullptr);

    std::vector<char *> shell_argv;
    shell_argv.push_back(const_cast<char *>(state->shell_path.c_str()));
    shell_argv.push_back(nullptr);
    vte_terminal_spawn_async(
        state->terminal,
        VTE_PTY_DEFAULT,
        nullptr,
        shell_argv.data(),
        nullptr,
        G_SPAWN_DEFAULT,
        nullptr,
        nullptr,
        nullptr,
        -1,
        nullptr,
        shell_spawned,
        state);

    gtk_window_present(state->window);
}

} // namespace

int main(int argc, char **argv) {
    AppState state;
    if (argc > 1) state.shell_path = argv[1];
    else state.shell_path = sibling_path(argv[0], "slsh");
    state.executable_path = argv[0];

    GtkApplication *application = gtk_application_new(
        kApplicationId, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(activate), &state);
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return status;
}