/* ballum/src/ballum_host_profiles.c -- starter profiles (see the header). */
#include "sleela_ballum/ballum_host_profiles.h"
#include <string.h>

#define XTERM_LIGHT "xterm", "-fa", "Monospace", "-fs", "12", "-bg", "white", "-fg", "black", "+sb"
#define XTERM_DARK  "xterm", "-fa", "Monospace", "-fs", "12", "-bg", "#101418", "-fg", "#d8dee9", "+sb"

static const char *const k_sleela_terminal[] = { "SleelaTerminal", NULL };
static const char *const k_slsh[]            = { XTERM_LIGHT, "-e", "slsh", NULL };
static const char *const k_xterm[]           = { XTERM_LIGHT, NULL };
static const char *const k_xterm_dark[]      = { XTERM_DARK, NULL };
static const char *const k_top[]             = { XTERM_DARK, "-e", "top", NULL };

static const ballum_profile_t k_profiles[] = {
    { "sleela-terminal", "SleelaTerminal",
      "The SleelaTerminal GTK/VTE terminal running slsh, pixel-for-pixel.",
      k_sleela_terminal, "disintegrate", "fade" },
    { "slsh", "slsh",
      "The SLeeLa shell in a plain xterm (needs xterm and slsh).",
      k_slsh, "slide-up", "slide-down" },
    { "xterm", "xterm",
      "A light xterm on the white Ballum background.",
      k_xterm, "pixelize", "fade" },
    { "xterm-dark", "xterm",
      "A dark xterm; contrasts with the white background.",
      k_xterm_dark, "fade", "fade" },
    { "top", "top",
      "A live process monitor in a dark xterm: a small admin view.",
      k_top, "iris-in", "iris-out" },
};

size_t ballum_profile_count(void) { return sizeof(k_profiles) / sizeof(k_profiles[0]); }

const ballum_profile_t *ballum_profile_at(size_t i) {
    return i < ballum_profile_count() ? &k_profiles[i] : NULL;
}

const ballum_profile_t *ballum_profile_find(const char *name) {
    if (!name) return NULL;
    for (size_t i = 0; i < ballum_profile_count(); ++i)
        if (strcmp(k_profiles[i].name, name) == 0) return &k_profiles[i];
    return NULL;
}
