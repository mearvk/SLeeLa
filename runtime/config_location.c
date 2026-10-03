#include "config_location.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_text(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    if (!src) src = "";
    snprintf(dst, cap, "%s", src);
}
static int is_absolute_path(const char *path) {
    if (!path || !path[0]) return 0;
#ifdef _WIN32
    return path[0] == '/' || path[0] == '\\' ||
           (((path[0] >= 'A' && path[0] <= 'Z') ||
             (path[0] >= 'a' && path[0] <= 'z')) && path[1] == ':');
#else
    return path[0] == '/';
#endif
}
static int join_path(char *dst, size_t cap, const char *base, const char *leaf) {
    if (!dst || cap == 0 || !base || !base[0] || !leaf || !leaf[0]) return 0;
#ifdef _WIN32
    snprintf(dst, cap, "%s\\%s", base, leaf);
#else
    snprintf(dst, cap, "%s/%s", base, leaf);
#endif
    return dst[0] != '\0';
}
void sleela_config_location_defaults(SLEELA_CONFIG_LOCATION *location) {
    if (!location) return;
    memset(location, 0, sizeof(*location));
    copy_text(location->project_name, sizeof(location->project_name), "SLeeLa");
    copy_text(location->absolute_root, sizeof(location->absolute_root), "/etc/sleela");
}
int sleela_config_location_from_environment(SLEELA_CONFIG_LOCATION *location) {
    if (!location) return 0;
    const char *name = getenv("SLEELA_PROJECT_NAME");
    const char *project = getenv("SLEELA_PROJECT_ROOT");
    const char *install = getenv("SLEELA_INSTALL_ROOT");
    const char *root = getenv("SLEELA_CONFIG_ROOT");
    if (name && name[0]) copy_text(location->project_name, sizeof(location->project_name), name);
    if (project && project[0]) copy_text(location->project_root, sizeof(location->project_root), project);
    if (install && install[0]) copy_text(location->install_root, sizeof(location->install_root), install);
    if (root && root[0]) {
        if (!is_absolute_path(root)) return 0;
        copy_text(location->config_root, sizeof(location->config_root), root);
        location->source = 1;
    }
    return 1;
}
int sleela_config_location_resolve(SLEELA_CONFIG_LOCATION *location,
                                   const char *project_root,
                                   const char *install_root,
                                   const char *explicit_absolute_root) {
    if (!location) return 0;
    if (explicit_absolute_root && explicit_absolute_root[0]) {
        if (!is_absolute_path(explicit_absolute_root)) return 0;
        copy_text(location->config_root, sizeof(location->config_root), explicit_absolute_root);
        location->source = 1;
        return 1;
    }
    if (project_root && project_root[0]) {
        copy_text(location->project_root, sizeof(location->project_root), project_root);
        if (join_path(location->config_root, sizeof(location->config_root), project_root, "config")) {
            location->source = 2;
            return 1;
        }
    }
    if (install_root && install_root[0]) {
        copy_text(location->install_root, sizeof(location->install_root), install_root);
        if (join_path(location->config_root, sizeof(location->config_root), install_root, "Config")) {
            location->source = 3;
            return 1;
        }
    }
    if (location->absolute_root[0] && is_absolute_path(location->absolute_root)) {
        copy_text(location->config_root, sizeof(location->config_root), location->absolute_root);
        location->source = 4;
        return 1;
    }
    return 0;
}
const char *sleela_config_location_root(const SLEELA_CONFIG_LOCATION *location);
