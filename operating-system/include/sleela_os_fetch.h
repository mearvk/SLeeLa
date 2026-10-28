/*
 * operating-system/include/sleela_os_fetch.h
 * SLeeLa OS fetch -- the C backing of lib/os/SLOSSensitivityFetch.sleela and
 * the SLDistroFetch download idiom (DOWNLOADING capability).
 *
 * This does NOT open sockets or run git itself -- transport is the VM's job
 * through the existing slos_run() bridge (impl/core/sleela_os.c). What this
 * provides is the pure, testable logic AROUND a fetch: composing a correct,
 * safe `git clone` command string from a warehouse reference, composing the
 * pin-checkout command, and resolving the on-disk path of a chosen shape and
 * component in the fetched tree. The SLeeLa layer (or a kernel/initramfs tool)
 * runs the composed command; this guarantees the command is well-formed and the
 * paths are correct, in one place, in C.
 *
 * Reference model mirrors SLOSSensitivitySource: owner/repo/url/branch/commit
 * plus the per-shape, per-component directory map.
 */
#ifndef SLEELA_OS_FETCH_H
#define SLEELA_OS_FETCH_H

#include <stddef.h>
#include "sleela_os_shape.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Clone depth/mode, matching SLOSSensitivityFetch.MODE_*. */
typedef enum sleela_os_fetch_mode {
    SLEELA_OS_FETCH_SHALLOW = 0,  /* --depth 1 --single-branch (default) */
    SLEELA_OS_FETCH_FULL    = 1   /* full history (allows a hard commit pin) */
} sleela_os_fetch_mode;

/* A zero-payload warehouse reference (mirrors SLOSSensitivitySource fields). */
typedef struct sleela_os_source {
    const char *owner;        /* e.g. "mearvk" */
    const char *repo;         /* e.g. "OS.Sensitivity.III" */
    const char *url;          /* https clone URL */
    const char *branch;       /* default branch, e.g. "main" */
    const char *pinned_commit;/* exact revision for reproducible fetches */
} sleela_os_source;

/* Fill `out` with the known OS.Sensitivity.III warehouse reference (the same
 * values SLOSSensitivitySource.configure() sets). */
void sleela_os_source_default(sleela_os_source *out);

/* The warehouse directory for a component ("uefi"/"bootloader"/.../"os-core"),
 * matching SLOSSensitivitySource.componentDir(). Returns NULL for an unknown
 * component. */
const char *sleela_os_component_dir(const char *component);

/* 1 if `component` is one this warehouse provides, else 0. */
int sleela_os_source_provides_component(const char *component);

/* Compose the `git clone` command into `buf` (NUL-terminated, truncated safely
 * if it does not fit). Mirrors SLOSSensitivityFetch.cloneCommand(): shallow mode
 * adds `--depth 1 --single-branch --branch <branch>`. Returns the number of
 * characters that WOULD be written (like snprintf), or a negative value on a
 * bad argument. */
int sleela_os_fetch_clone_command(const sleela_os_source *src,
                                  sleela_os_fetch_mode mode,
                                  const char *dest_dir,
                                  char *buf, size_t cap);

/* Compose the pin-checkout command (`git -C <dest> checkout <commit>`). Returns
 * 0 and writes "" for SHALLOW mode (a shallow clone cannot hard-pin); otherwise
 * writes the command and returns its would-be length. Negative on bad args. */
int sleela_os_fetch_pin_command(const sleela_os_source *src,
                                sleela_os_fetch_mode mode,
                                const char *dest_dir,
                                char *buf, size_t cap);

/* Compose the on-disk path of a chosen shape's folder inside a fetched tree:
 *   <dest_root>/<repo>/<shape-dir>
 * Mirrors SLOSSensitivityFetch.shapePath(). Negative on bad args. */
int sleela_os_fetch_shape_path(const sleela_os_source *src,
                               const char *dest_root,
                               sleela_os_shape shape,
                               char *buf, size_t cap);

/* Compose the on-disk path of a (shape, component) C/C++ folder:
 *   <dest_root>/<repo>/<shape-dir>/<component-dir>
 * Mirrors SLOSSensitivityFetch.componentPath(). Returns negative on bad args or
 * an unknown component. */
int sleela_os_fetch_component_path(const sleela_os_source *src,
                                   const char *dest_root,
                                   sleela_os_shape shape,
                                   const char *component,
                                   char *buf, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_FETCH_H */
