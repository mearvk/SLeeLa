/*
 * operating-system/src/sleela_os_shape.c
 * Implementation of the SLeeLa OS shape model (see sleela_os_shape.h).
 * Values are faithful to SLOSShape.sleela; keep the two in lockstep.
 */
#include "sleela_os_shape.h"
#include <string.h>

sleela_os_shape sleela_os_shape_by_name(const char *name) {
    if (name != NULL) {
        if (strcmp(name, "Windows") == 0) return SLEELA_OS_SHAPE_WINDOWS;
        if (strcmp(name, "macOS")   == 0) return SLEELA_OS_SHAPE_MACOS;
    }
    return SLEELA_OS_SHAPE_LINUX;
}

const char *sleela_os_shape_name(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "Windows";
        case SLEELA_OS_SHAPE_MACOS:   return "macOS";
        default:                      return "Linux";
    }
}

const char *sleela_os_shape_path_separator(sleela_os_shape shape) {
    return shape == SLEELA_OS_SHAPE_WINDOWS ? "\\" : "/";
}

const char *sleela_os_shape_list_separator(sleela_os_shape shape) {
    return shape == SLEELA_OS_SHAPE_WINDOWS ? ";" : ":";
}

const char *sleela_os_shape_init_manager(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "smss+ServiceControlManager";
        case SLEELA_OS_SHAPE_MACOS:   return "launchd";
        default:                      return "systemd";
    }
}

const char *sleela_os_shape_boot_style(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "UEFI+BOOTMGR";
        case SLEELA_OS_SHAPE_MACOS:   return "UEFI+boot.efi";
        default:                      return "UEFI+GRUB";
    }
}

const char *sleela_os_shape_root_filesystem(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "NTFS";
        case SLEELA_OS_SHAPE_MACOS:   return "APFS";
        default:                      return "ext4";
    }
}

const char *sleela_os_shape_desktop_session(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "Explorer/DWM";
        case SLEELA_OS_SHAPE_MACOS:   return "Aqua/WindowServer";
        default:                      return "GNOME/Wayland";
    }
}

const char *sleela_os_shape_system_root(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return "C:\\Windows";
        case SLEELA_OS_SHAPE_MACOS:   return "/System";
        default:                      return "/usr";
    }
}

const char *sleela_os_shape_executable_extension(sleela_os_shape shape) {
    return shape == SLEELA_OS_SHAPE_WINDOWS ? ".exe" : "";
}

const char *sleela_os_shape_warehouse_dir(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_LINUX:   return "linux";
        case SLEELA_OS_SHAPE_WINDOWS: return "windows";
        case SLEELA_OS_SHAPE_MACOS:   return "macos";
        default:                      return 0;
    }
}
