# SLeeLa Native Module Loader (C / C++)

This component provides a small C ABI and C++ RAII wrapper for loading and
calling a trusted native plugin built from **at most nine linked C/C++ source
or object units**. Compile and link those units into one platform shared library
(`.dll`, `.dylib`, or `.so`) first; the loader then opens that library and calls
its explicit entry point. Native C/C++ is compiled, not interpreted, and a
relocatable `.o`/`.obj` cannot be loaded directly.

Loading native code executes it in the current process; this is not a sandbox.
Only load modules from trusted locations after any integrity checks required by
your application's policy. This API never invokes a shell or compiles
caller-provided source at runtime.

## Plugin ABI

Export these two C-linkage functions, including when the implementation is
written in C++:

```c
#include "native_loader.h"

uint32_t sl_native_abi_version(void) { return SL_NATIVE_ABI_VERSION; }
int sl_native_run(int argc, const char *const argv[]) {
    /* Run the plugin operation; return its result. */
    return argc;
}
```

The loader checks the ABI version before exposing the entry point. The C API is
in `native_loader.h` / `native_loader.c`; the move-only C++ wrapper is in
`native_loader.hpp` / `native_loader.cpp`.

## Runtime-owned handle table

`native_module_table.h` / `native_module_table.c` add a bounded C runtime
handle table over the loader:

- At most `SL_NATIVE_TABLE_CAPACITY` (nine) modules are open at once.
- The host must provide an authorization callback before any library is loaded.
  A source path or module reference never authorizes execution by itself.
- VM-facing handles are opaque 32-bit slot/generation values, not raw pointers.
  Closing a module invalidates its handle; stale handles are rejected.
- The table supports explicit open, run, close, and close-all operations.
- The caller must serialize concurrent access to a table instance. This small
  table is a runtime integration seam, not yet a SLeeLa bytecode opcode or
  automatic interpreter/native dispatcher.

The authorization callback should enforce the host's trusted roots, manifest or
signature policy, and any deployment-specific module allowlist. Do not approve
a path merely because it exists or is syntactically valid.

## Build and test

From the repository root:

```sh
cmake -S os-level-support -B build/os-level-support
cmake --build build/os-level-support
ctest --test-dir build/os-level-support --output-on-failure
```

The tests exercise the C interface and C++ wrapper against a small fixture,
move-only ownership, ABI mismatch and missing-entry rejection, authorization
denial, bounded-table exhaustion, and stale-handle rejection. On POSIX, CMake
links the system dynamic-loader library as needed. On Windows, use a
compiler/toolchain that produces the plugin DLL and exports both ABI symbols.

## Nine-unit plugin build

Use CMake to compile/link the translation units before loading the result. A
mixed C/C++ target should be linked by the C++ driver. Object files may be
included as target sources when produced for the same platform, architecture,
and ABI:

```cmake
set(plugin_units
  src/entry.c
  src/common.c
  src/feature.cpp
  build/extra.o
)
list(LENGTH plugin_units unit_count)
if(unit_count GREATER 9)
  message(FATAL_ERROR "A native plugin may contain at most 9 linked units")
endif()
add_library(example_plugin SHARED ${plugin_units})
```

Pass the output shared-library path to `sl_native_open` or
`NativeModule::open`. The application controls the compiler, linker options,
trusted plugin path, and integrity verification.
