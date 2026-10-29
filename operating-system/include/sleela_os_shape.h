/*
 * operating-system/include/sleela_os_shape.h
 * SLeeLa OS shape model -- the C backing of lib/operating-system/SLOSShape.sleela.
 *
 * A "shape" is the family of conventions a target OS adopts: Linux, Windows, or
 * macOS. This header gives the native, dependency-free view of those per-family
 * conventions (path separators, init manager, boot style, root filesystem,
 * desktop session, system root, executable extension) so the C/C++ half of the
 * OS Creator can reshape and query a model without re-deriving the facts the
 * SLeeLa layer already encodes. It mirrors SLOSShape exactly; both are the same
 * source of truth expressed once in each language.
 *
 * Pure value logic: no syscalls, no allocation. Safe to use from any backend.
 */
#ifndef SLEELA_OS_SHAPE_H
#define SLEELA_OS_SHAPE_H

#ifdef __cplusplus
extern "C" {
#endif

/* The three canonical OS shapes, matching SLOSShape.SHAPE_*. */
typedef enum sleela_os_shape {
    SLEELA_OS_SHAPE_LINUX   = 0,
    SLEELA_OS_SHAPE_WINDOWS = 1,
    SLEELA_OS_SHAPE_MACOS   = 2
} sleela_os_shape;

/* Resolve a shape by name ("Linux"/"Windows"/"macOS", case-sensitive as the
 * SLeeLa layer uses them). An unrecognised name defaults to Linux, matching
 * SLOSShape.configure(). */
sleela_os_shape sleela_os_shape_by_name(const char *name);

/* The canonical label for a shape ("Linux"/"Windows"/"macOS"). */
const char *sleela_os_shape_name(sleela_os_shape shape);

/* Per-family conventions. Each returns a static string owned by the library;
 * callers must not free it. These mirror SLOSShape's accessors one-for-one. */
const char *sleela_os_shape_path_separator(sleela_os_shape shape);  /* "/" or "\\" */
const char *sleela_os_shape_list_separator(sleela_os_shape shape);  /* ":" or ";"  */
const char *sleela_os_shape_init_manager(sleela_os_shape shape);
const char *sleela_os_shape_boot_style(sleela_os_shape shape);
const char *sleela_os_shape_root_filesystem(sleela_os_shape shape);
const char *sleela_os_shape_desktop_session(sleela_os_shape shape);
const char *sleela_os_shape_system_root(sleela_os_shape shape);
const char *sleela_os_shape_executable_extension(sleela_os_shape shape); /* ".exe" or "" */

/* The warehouse folder for a shape ("linux"/"windows"/"macos"), matching
 * SLOSSensitivitySource.shapeDir(). Returns NULL for an invalid shape. */
const char *sleela_os_shape_warehouse_dir(sleela_os_shape shape);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_SHAPE_H */
