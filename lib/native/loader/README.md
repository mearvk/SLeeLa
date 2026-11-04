# SLeeLa Native Module Loader (C / C++)

A small runtime loader for a trusted native plugin built from **at most nine
linked C/C++ source or object units**. C and C++ source is compiled and linked
into one platform shared library (`.dll`, `.dylib`, or `.so`) first; the loader
then opens that library and calls its explicit entry point. Native C/C++ is not
interpreted, and a relocatable `.o`/`.obj` cannot be loaded directly.

This is a runtime execution boundary, not a sandbox: load only modules from a
trusted path whose integrity has been checked under the host application's
policy. Do not load untrusted plugins.

## Plugin ABI

Export both C-linkage functions, including from C++:

```c
#include "native_loader.h"

uint32_t sl_native_abi_version(void) { return SL_NATIVE_ABI_VERSION; }
int sl_native_run(int argc, const char *const argv[]) {
    /* Run the plugin operation; return its result. */
    return argc;
}
```

The loader checks the ABI version before exposing the entry point. The C API is
in `native_loader.h` / `native_loader.c`; the move-only RAII facade is in
`native_loader.hpp` / `native_loader.cpp`.

## Build and test

```sh
cmake -S lib/native/loader -B build/native-loader
cmake --build build/native-loader
ctest --test-dir build/native-loader --output-on-failure
```

The test fixture exercises both the C interface and C++ wrapper. On POSIX,
CMake links the system dynamic-loader library as needed. On Windows, use a
compiler/toolchain that produces the plugin DLL and exports the two ABI symbols.

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

Then pass the resulting shared-library path to `sl_native_open` or
`NativeModule::open`. The application controls the compiler, linker options,
trusted plugin path, and integrity verification; this library never invokes a
shell or compiles caller-provided source at runtime.
