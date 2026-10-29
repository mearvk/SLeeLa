/*
 * operating-system/src/sleela_os_ideology.c
 * Implementation of origin-scope inspection and shape-conversion naming
 * (see sleela_os_ideology.h). Descriptive and neutral by construction.
 */
#include "sleela_os_ideology.h"
#include <string.h>

static int eq(const char *a, const char *b) {
    return a != NULL && b != NULL && strcmp(a, b) == 0;
}

int sleela_os_origin_is_known(const char *meta_origin) {
    if (meta_origin == NULL || meta_origin[0] == '\0') return 0;
    if (eq(meta_origin, "none") || eq(meta_origin, "unknown")) return 0;
    return 1;
}

sleela_os_origin_scope sleela_os_origin_classify(const char *meta_origin) {
    if (!sleela_os_origin_is_known(meta_origin)) return SLEELA_OS_SCOPE_UNKNOWN;
    if (eq(meta_origin, "International")) return SLEELA_OS_SCOPE_INTERNATIONAL;
    if (eq(meta_origin, "Galactic"))     return SLEELA_OS_SCOPE_GALACTIC;
    /* "National", or a specific nation name, is a National-scope value. */
    return SLEELA_OS_SCOPE_NATIONAL;
}

const char *sleela_os_origin_scope_name(sleela_os_origin_scope scope) {
    switch (scope) {
        case SLEELA_OS_SCOPE_NATIONAL:      return "National";
        case SLEELA_OS_SCOPE_INTERNATIONAL: return "International";
        case SLEELA_OS_SCOPE_GALACTIC:      return "Galactic";
        default:                            return "Unknown";
    }
}

int sleela_os_origin_scope_cmp(sleela_os_origin_scope a, sleela_os_origin_scope b) {
    return (int)a - (int)b;
}

int sleela_os_convert_is_native(sleela_os_shape host, sleela_os_shape guest) {
    return host == guest ? 1 : 0;
}

const char *sleela_os_convert_runtime(sleela_os_shape host, sleela_os_shape guest) {
    if (host == guest) return "";
    switch (host) {
        case SLEELA_OS_SHAPE_LINUX:
            if (guest == SLEELA_OS_SHAPE_WINDOWS) return "wine";
            if (guest == SLEELA_OS_SHAPE_MACOS)   return "darling";
            return "qemu-user";
        case SLEELA_OS_SHAPE_MACOS:
            if (guest == SLEELA_OS_SHAPE_WINDOWS) return "wine";
            if (guest == SLEELA_OS_SHAPE_LINUX)   return "lima";
            return "rosetta2";
        case SLEELA_OS_SHAPE_WINDOWS:
            if (guest == SLEELA_OS_SHAPE_LINUX)   return "wsl2";
            return "qemu-system";
        default:
            return "qemu-system";
    }
}

int sleela_os_convert_component_count(sleela_os_shape host, sleela_os_shape guest) {
    if (host == guest) return 0;
    /* Only a Linux host contributes a filesystem interpreter (binfmt_misc),
     * which adds a third component on top of runtime + guest libraries. */
    if (host == SLEELA_OS_SHAPE_LINUX) return 3;
    return 2;
}
