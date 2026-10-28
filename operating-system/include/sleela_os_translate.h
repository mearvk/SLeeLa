/*
 * operating-system/include/sleela_os_translate.h
 * SLeeLa OS executable translation -- the C ABI of the MANIPULATION capability
 * (classifying which executables a shape runs, and which guest shape a file
 * belongs to). Backs lib/operating-system/SLExecutableTypes + SLExecutableTranslator.
 *
 * The implementation is C++ (sleela_os_translate.cpp) but the surface is a C
 * ABI so it links cleanly with the rest of /os and is callable from the VM
 * bridge. It is pure classification over file extensions and host/guest shapes;
 * it runs nothing.
 */
#ifndef SLEELA_OS_TRANSLATE_H
#define SLEELA_OS_TRANSLATE_H

#include "sleela_os_shape.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 1 if an executable with the given extension (e.g. ".exe", ".sh", "", ".app")
 * is natively runnable on `host`, else 0. A leading dot is optional; "" (no
 * extension) is treated as a native binary (ELF/Mach-O) on POSIX shapes.
 * Mirrors SLExecutableTypes.isExecutable for the host shape. */
int sleela_os_is_native_executable(sleela_os_shape host, const char *ext);

/* Classify which guest SHAPE an executable extension belongs to, so a
 * translator knows which compatibility layer to apply:
 *   .exe/.com/.bat/.cmd/.ps1/.msi -> Windows
 *   .app/.command/.dylib          -> macOS
 *   .sh/.elf/.so/.run/.AppImage/"" -> Linux (default)
 * Returns the guest shape. */
sleela_os_shape sleela_os_guest_shape_for_ext(const char *ext);

/* 1 if running an executable of extension `ext` on `host` requires a
 * compatibility layer (its guest shape differs from the host), else 0. */
int sleela_os_needs_translation(sleela_os_shape host, const char *ext);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_TRANSLATE_H */
