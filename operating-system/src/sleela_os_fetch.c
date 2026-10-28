/*
 * operating-system/src/sleela_os_fetch.c
 * Implementation of the OS fetch command/path composer (see sleela_os_fetch.h).
 * Values mirror SLOSSensitivitySource / SLOSSensitivityFetch.
 */
#include "sleela_os_fetch.h"
#include <stdio.h>
#include <string.h>

void sleela_os_source_default(sleela_os_source *out) {
    if (out == NULL) return;
    out->owner         = "mearvk";
    out->repo          = "OS.Sensitivity.III";
    out->url           = "https://github.com/mearvk/OS.Sensitivity.III.git";
    out->branch        = "main";
    out->pinned_commit = "f619c8f95acc4eecb734c4ddbb72cc1a954a36e9";
}

const char *sleela_os_component_dir(const char *component) {
    if (component == NULL) return 0;
    if (strcmp(component, "uefi")           == 0) return "uefi";
    if (strcmp(component, "bios")           == 0) return "bios";
    if (strcmp(component, "bootloader")     == 0) return "bootloader";
    if (strcmp(component, "kernel-loader")  == 0) return "kernel-loader";
    if (strcmp(component, "driver-loader")  == 0) return "driver-loader";
    if (strcmp(component, "base-drivers")   == 0) return "base-drivers";
    if (strcmp(component, "os-loader")      == 0) return "os-loader";
    if (strcmp(component, "desktop-loader") == 0) return "desktop-loader";
    if (strcmp(component, "os-core")        == 0) return "os-core";
    return 0;
}

int sleela_os_source_provides_component(const char *component) {
    return sleela_os_component_dir(component) != 0 ? 1 : 0;
}

/* The transport hardening used by the SLeeLa fetch idiom (large postBuffer,
 * forced HTTP/1.1) so a clone of a big tree does not stall. */
#define SLEELA_OS_GIT_BASE \
    "git -c http.postBuffer=524288000 -c http.version=HTTP/1.1 clone"

int sleela_os_fetch_clone_command(const sleela_os_source *src,
                                  sleela_os_fetch_mode mode,
                                  const char *dest_dir,
                                  char *buf, size_t cap) {
    if (src == NULL || dest_dir == NULL || buf == NULL || cap == 0) return -1;
    if (mode == SLEELA_OS_FETCH_SHALLOW) {
        return snprintf(buf, cap, "%s --depth 1 --single-branch --branch %s %s %s",
                        SLEELA_OS_GIT_BASE, src->branch, src->url, dest_dir);
    }
    return snprintf(buf, cap, "%s %s %s", SLEELA_OS_GIT_BASE, src->url, dest_dir);
}

int sleela_os_fetch_pin_command(const sleela_os_source *src,
                                sleela_os_fetch_mode mode,
                                const char *dest_dir,
                                char *buf, size_t cap) {
    if (src == NULL || dest_dir == NULL || buf == NULL || cap == 0) return -1;
    if (mode == SLEELA_OS_FETCH_SHALLOW) {
        buf[0] = '\0';
        return 0;
    }
    return snprintf(buf, cap, "git -C %s checkout %s", dest_dir, src->pinned_commit);
}

int sleela_os_fetch_shape_path(const sleela_os_source *src,
                               const char *dest_root,
                               sleela_os_shape shape,
                               char *buf, size_t cap) {
    const char *sd;
    if (src == NULL || dest_root == NULL || buf == NULL || cap == 0) return -1;
    sd = sleela_os_shape_warehouse_dir(shape);
    if (sd == 0) return -1;
    return snprintf(buf, cap, "%s/%s/%s", dest_root, src->repo, sd);
}

int sleela_os_fetch_component_path(const sleela_os_source *src,
                                   const char *dest_root,
                                   sleela_os_shape shape,
                                   const char *component,
                                   char *buf, size_t cap) {
    const char *sd;
    const char *cd;
    if (src == NULL || dest_root == NULL || buf == NULL || cap == 0) return -1;
    sd = sleela_os_shape_warehouse_dir(shape);
    cd = sleela_os_component_dir(component);
    if (sd == 0 || cd == 0) return -1;
    return snprintf(buf, cap, "%s/%s/%s/%s", dest_root, src->repo, sd, cd);
}
