/*
 * Ballum backend selection.
 *
 * Exactly one OS backend is compiled per build. The Makefile selects the right
 * backend source by platform and defines one of BALLUM_BACKEND_X11 /
 * BALLUM_BACKEND_MACOS / BALLUM_BACKEND_WINDOWS; with none defined (or an
 * unknown OS) we fall back to the always-present "unavailable" backend so calls
 * fail closed instead of failing to link.
 *
 * Each backend file defines `const ballum_backend_t *ballum_backend_<os>(void)`;
 * this file maps the compile-time selection to ballum_backend_default().
 */

#include "sleela_ballum/ballum_manager.h"

const ballum_backend_t *ballum_backend_unavailable(void);

#if defined(BALLUM_BACKEND_X11)
const ballum_backend_t *ballum_backend_x11(void);
#elif defined(BALLUM_BACKEND_MACOS)
const ballum_backend_t *ballum_backend_macos(void);
#elif defined(BALLUM_BACKEND_WINDOWS)
const ballum_backend_t *ballum_backend_windows(void);
#endif

const ballum_backend_t *ballum_backend_default(void) {
#if defined(BALLUM_BACKEND_X11)
    return ballum_backend_x11();
#elif defined(BALLUM_BACKEND_MACOS)
    return ballum_backend_macos();
#elif defined(BALLUM_BACKEND_WINDOWS)
    return ballum_backend_windows();
#else
    return ballum_backend_unavailable();
#endif
}

int ballum_backend_supported(void) {
#if defined(BALLUM_BACKEND_X11) || defined(BALLUM_BACKEND_MACOS) || defined(BALLUM_BACKEND_WINDOWS)
    return 1;
#else
    return 0;
#endif
}
