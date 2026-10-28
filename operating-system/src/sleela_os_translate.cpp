/*
 * operating-system/src/sleela_os_translate.cpp
 * C++ implementation of executable-type classification (see
 * sleela_os_translate.h). Exposes a C ABI. Mirrors lib/os SLExecutableTypes /
 * SLExecutableTranslator. Pure classification: runs nothing.
 */
#include "sleela_os_translate.h"
#include <string>
#include <algorithm>

namespace {

/* Normalise an extension to a lower-case leading-dot form ("exe" -> ".exe",
 * ".EXE" -> ".exe", "" -> ""). */
std::string normalize_ext(const char *ext) {
    if (ext == nullptr) return std::string();
    std::string e(ext);
    if (!e.empty() && e[0] != '.') e.insert(e.begin(), '.');
    std::transform(e.begin(), e.end(), e.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return e;
}

bool is_windows_ext(const std::string &e) {
    return e == ".exe" || e == ".com" || e == ".bat" || e == ".cmd"
        || e == ".ps1" || e == ".msi" || e == ".vbs" || e == ".scr";
}

bool is_macos_ext(const std::string &e) {
    return e == ".app" || e == ".command" || e == ".dylib";
}

/* Linux/POSIX-runnable families (and "" == a native ELF/Mach-O binary). */
bool is_posix_script_or_elf(const std::string &e) {
    return e.empty() || e == ".sh" || e == ".elf" || e == ".so"
        || e == ".run" || e == ".appimage" || e == ".ko" || e == ".bin";
}

} // namespace

extern "C" {

sleela_os_shape sleela_os_guest_shape_for_ext(const char *ext) {
    const std::string e = normalize_ext(ext);
    if (is_windows_ext(e)) return SLEELA_OS_SHAPE_WINDOWS;
    if (is_macos_ext(e))   return SLEELA_OS_SHAPE_MACOS;
    return SLEELA_OS_SHAPE_LINUX;
}

int sleela_os_is_native_executable(sleela_os_shape host, const char *ext) {
    const std::string e = normalize_ext(ext);
    switch (host) {
        case SLEELA_OS_SHAPE_WINDOWS:
            return is_windows_ext(e) ? 1 : 0;
        case SLEELA_OS_SHAPE_MACOS:
            /* macOS runs Mach-O (no ext), scripts, .app bundles, and .dylib. */
            return (is_macos_ext(e) || e.empty() || e == ".sh" || e == ".command") ? 1 : 0;
        case SLEELA_OS_SHAPE_LINUX:
        default:
            return is_posix_script_or_elf(e) ? 1 : 0;
    }
}

int sleela_os_needs_translation(sleela_os_shape host, const char *ext) {
    return sleela_os_guest_shape_for_ext(ext) != host ? 1 : 0;
}

} // extern "C"
