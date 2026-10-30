#include "config_location.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_text(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || cap == 0 || !src) return 0;
    n = strlen(src);
    if (n >= cap) { dst[0] = '\0'; return 0; }
    memcpy(dst, src, n + 1);
    return 1;
}
static int is_absolute_path(const char *path) {
    if (!path || !path[0]) return 0;
#ifdef _WIN32
    return (((path[0] >= 'A' && path[0] <= 'Z') ||
             (path[0] >= 'a' && path[0] <= 'z')) && path[1] == ':' &&
            (path[2] == '\\' || path[2] == '/'));
#else
    return path[0] == '/';
#endif
}
static int join_path(char *dst, size_t cap, const char *base, const char *leaf) {
    if (!dst || cap == 0 || !base || !base[0] || !leaf || !leaf[0]) return 0;
#ifdef _WIN32
    int w = snprintf(dst, cap, "%s\\%s", base, leaf);
#else
    int w = snprintf(dst, cap, "%s/%s", base, leaf);
#endif
    /* Normalize separators in relative policy paths on Windows. */
#ifdef _WIN32
    { char *p; for (p = dst; *p; ++p) if (*p == '/') *p = '\\'; }
#endif
    /* snprintf returns the length it *would* have written; a value >= cap means
       the joined path was truncated and must not be treated as a valid path. */
    if (w < 0 || (size_t)w >= cap) { dst[0] = '\0'; return 0; }
    return dst[0] != '\0';
}
void sleela_config_location_defaults(SLEELA_CONFIG_LOCATION *location) {
    if (!location) return;
    memset(location, 0, sizeof(*location));
    copy_text(location->project_name, sizeof(location->project_name), "SLeeLa");
    copy_text(location->relative_path, sizeof(location->relative_path), ".sleela/config");
    copy_text(location->install_relative, sizeof(location->install_relative), "Config");
#ifdef _WIN32
    copy_text(location->absolute_root, sizeof(location->absolute_root), "C:\\ProgramData\\SLeeLa\\Config");
#else
    copy_text(location->absolute_root, sizeof(location->absolute_root), "/etc/sleela");
#endif
}
int sleela_config_location_load_policy(const char *path, SLEELA_CONFIG_LOCATION *location) {
    FILE *f; char line[512];
    if (!path || !location || !(f = fopen(path, "rb"))) return 0;
    while (fgets(line, sizeof(line), f)) {
        char *p = line, *eq; size_t n = strlen(line);
        if (n && line[n - 1] != '\n') {
            /* No trailing newline: peek one byte (same robust test as
               sleela_vm_config_load). More data -> the line was longer than
               the buffer (truncation) -> reject. EOF -> this was the final
               line without a newline -> accept. feof() alone is subtle here
               because it is only set after a read actually hits end-of-file. */
            int peek = fgetc(f);
            if (peek != EOF) { fclose(f); return 0; }
            if (ferror(f)) { fclose(f); return 0; }
        }
        while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) ++p;
        if (!*p || *p == '#') continue;
        eq = strchr(p, '='); if (!eq) { fclose(f); return 0; }
        *eq++ = '\0';
        while (*eq == ' ' || *eq == '\t') ++eq;
        { char *e = eq + strlen(eq); while (e > eq && (e[-1] == ' ' || e[-1] == '\t' || e[-1] == '\r' || e[-1] == '\n')) *--e = '\0'; }
        { char *k = p + strlen(p); while (k > p && (k[-1] == ' ' || k[-1] == '\t')) *--k = '\0'; }
        if (!strcmp(p, "config.project.name")) {
            if (!copy_text(location->project_name, sizeof(location->project_name), eq)) { fclose(f); return 0; }
        } else if (!strcmp(p, "config.location.relative")) {
            if (!eq[0] || is_absolute_path(eq) || eq[0] == '/' || eq[0] == '\\' || strchr(eq, ':') || strstr(eq, "..") || !copy_text(location->relative_path, sizeof(location->relative_path), eq)) { fclose(f); return 0; }
        } else if (!strcmp(p, "config.location.install_relative")) {
            if (!eq[0] || is_absolute_path(eq) || eq[0] == '/' || eq[0] == '\\' || strchr(eq, ':') || strstr(eq, "..") || !copy_text(location->install_relative, sizeof(location->install_relative), eq)) { fclose(f); return 0; }
        } else if (!strcmp(p, "config.location.absolute")) {
#ifdef _WIN32
            /* The checked-in /etc/sleela value is the Unix fallback. Keep the
               Windows default for that value; accept only absolute Windows
               paths when an explicit platform-specific fallback is supplied. */
            if (eq[0] != '/') {
                if (!is_absolute_path(eq) || !copy_text(location->absolute_root, sizeof(location->absolute_root), eq)) { fclose(f); return 0; }
            }
#else
            if (!is_absolute_path(eq) || !copy_text(location->absolute_root, sizeof(location->absolute_root), eq)) { fclose(f); return 0; }
#endif
        }
    }
    if (ferror(f)) { fclose(f); return 0; }
    fclose(f); return 1;
}
int sleela_config_location_from_environment(SLEELA_CONFIG_LOCATION *location) {
    if (!location) return 0;
    const char *name = getenv("SLEELA_PROJECT_NAME");
    const char *project = getenv("SLEELA_PROJECT_ROOT");
    const char *install = getenv("SLEELA_INSTALL_ROOT");
    const char *root = getenv("SLEELA_CONFIG_ROOT");
    if (name && name[0] && !copy_text(location->project_name, sizeof(location->project_name), name)) return 0;
    if (project && project[0] && (!is_absolute_path(project) || !copy_text(location->project_root, sizeof(location->project_root), project))) return 0;
    if (install && install[0] && (!is_absolute_path(install) || !copy_text(location->install_root, sizeof(location->install_root), install))) return 0;
    if (root && root[0]) {
        if (!is_absolute_path(root)) return 0;
        if (!copy_text(location->config_root, sizeof(location->config_root), root)) return 0;
        location->source = 1;
    }
    return 1;
}
int sleela_config_location_resolve(SLEELA_CONFIG_LOCATION *location,
                                   const char *project_root,
                                   const char *install_root,
                                   const char *explicit_absolute_root) {
    const char *effective_project;
    const char *effective_install;
    if (!location) return 0;
    if (explicit_absolute_root && explicit_absolute_root[0]) {
        if (!is_absolute_path(explicit_absolute_root) || !copy_text(location->config_root, sizeof(location->config_root), explicit_absolute_root)) return 0;
        location->source = 1;
        return 1;
    }
    if (location->config_root[0] && location->source == 1) return 1;
    effective_project = (project_root && project_root[0]) ? project_root : location->project_root;
    effective_install = (install_root && install_root[0]) ? install_root : location->install_root;
    if (effective_project && effective_project[0]) {
        if (!is_absolute_path(effective_project) || !copy_text(location->project_root, sizeof(location->project_root), effective_project)) return 0;
        if (!join_path(location->config_root, sizeof(location->config_root), effective_project, location->relative_path)) return 0;
        location->source = 2;
        return 1;
    }
    if (effective_install && effective_install[0]) {
        if (!is_absolute_path(effective_install) || !copy_text(location->install_root, sizeof(location->install_root), effective_install)) return 0;
        if (!join_path(location->config_root, sizeof(location->config_root), effective_install, location->install_relative)) return 0;
        location->source = 3;
        return 1;
    }
    if (location->absolute_root[0] && is_absolute_path(location->absolute_root)) {
        if (!copy_text(location->config_root, sizeof(location->config_root), location->absolute_root)) return 0;
        location->source = 4;
        return 1;
    }
    return 0;
}
const char *sleela_config_location_root(const SLEELA_CONFIG_LOCATION *location) {
    return location ? location->config_root : NULL;
}

