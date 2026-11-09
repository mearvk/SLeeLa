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

## Legacy plugin entry

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

## Typed method ABI (version 1)

A plugin that supports typed calls exports the following functions in addition
to the required legacy entry points:

```c
#include <string.h>
#include "native_loader.h"

uint32_t sl_native_typed_abi_version(void) {
    return SL_NATIVE_TYPED_ABI_VERSION;
}

int sl_native_call(const char *method, uint32_t argc,
                   const SLNativeValue *argv, SLNativeValue *out_result) {
    if (strcmp(method, "twice") == 0 && argc == 1 &&
        argv[0].tag == SL_NATIVE_VALUE_I64) {
        out_result->tag = SL_NATIVE_VALUE_I64;
        out_result->as.i64 = argv[0].as.i64 * 2;
        return SL_NATIVE_CALL_OK;
    }
    return SL_NATIVE_CALL_NOT_HANDLED;
}
```

The typed ABI is independently versioned from the legacy module ABI. When a
plugin exports `sl_native_call`, the loader requires
`sl_native_typed_abi_version` and checks it against
`SL_NATIVE_TYPED_ABI_VERSION`. A missing or mismatched typed version fails
closed with `SL_NATIVE_ABI_MISMATCH`. A legacy run-only plugin remains loadable. Calling its typed API returns
`SL_NATIVE_TYPED_ENTRY_NOT_FOUND`. A typed plugin that does not implement a
particular method returns `SL_NATIVE_CALL_NOT_HANDLED`, mapped to the distinct
host status `SL_NATIVE_METHOD_NOT_HANDLED`.

The value contract uses fixed-width tags and fields:

| Tag | Payload |
|---|---|
| `SL_NATIVE_VALUE_NULL` | no payload |
| `SL_NATIVE_VALUE_BOOL` | `int32_t`, exactly 0 or 1 |
| `SL_NATIVE_VALUE_I64` | `int64_t` |
| `SL_NATIVE_VALUE_F64` | `double` |
| `SL_NATIVE_VALUE_UTF8` | borrowed pointer plus explicit byte length |

The loader validates tags, reserved fields, boolean values, and nonempty string
pointers. Plugin methods return `SL_NATIVE_CALL_OK` on success,
`SL_NATIVE_CALL_NOT_HANDLED` when the named method is not implemented, or
`SL_NATIVE_CALL_ERROR` on a handled-method failure. Only NOT_HANDLED enables
SLeeLa interpreter fallback; plugin errors and ABI errors are surfaced and
never silently fall back.

UTF-8 pointers are borrowed byte spans, not NUL-terminated strings. Input spans
remain valid only for the duration of the call. A plugin-returned UTF-8 span
must remain valid until the next call to that module or module close; the VM
copies the result before releasing its module-call lock. SLeeLa's current String
representation is NUL-terminated, so returned spans containing embedded NUL
are rejected rather than truncated. Composite values (arrays, structs,
objects, closures), VM-local handles, and raw pointers are not part of this
ABI. Such values stay in the VM and may be passed to the bytecode fallback.

## SLeeLa bytecode and source dispatch

The core appends three opcodes without renumbering existing instructions:

- `OP_NATIVE_OPEN`: pops a module path String, asks the immutable host
  authorization callback, and pushes an opaque VM-owned module handle.
- `OP_NATIVE_CALL`: uses stack layout
  `[handle, method String, arg0, ..., argN-1]`; operand `a` is the argument
  count. It converts only null, bool, int64, double, and UTF-8 String values.
- `OP_NATIVE_CLOSE`: closes the module handle and pushes null.

SLeeLa syntax 3.0 exposes these as `nativeOpen(path)`,
`nativeCall(handle, method, ...args)`, and `nativeClose(handle)`. The host
must install `slvm_set_native_authorizer` before source execution; without it,
every open is denied. A path identifies a candidate and never grants authority.

When the plugin reports NOT_HANDLED, or an argument is a VM-owned composite
that cannot cross the scalar ABI, the interpreter searches for a SLeeLa bytecode
method with the same name and arity and dispatches it using the original VM
values. If no matching method exists, execution fails with a diagnostic.
Plugin execution errors, stale handles, and ABI mismatches do not trigger
fallback. Module calls are serialized against close/unload. Native code runs in
the host process and is not sandboxed.

### Optional host callback table

A plugin may export `sl_native_bind_host(const SLNativeHostAPI*)`. The loader
passes a size-checked `SL_NATIVE_HOST_CALLBACK_ABI_VERSION` table after the
host has authorized and opened the module. The plugin must copy the table and
may call `invoke(context, function, argc, argv, result)` synchronously from a typed method. The plugin must not retain or invoke the table asynchronously.
The host resolves an exact, unique compiled SLeeLa function name and executes
it on a separate nested interpreter stack. Only the typed scalar/UTF-8 values
cross the callback boundary; VM pointers and raw `SLValue` objects never do.

The suspended interpreter stack remains a GC root during callback execution.
Recursive native-module API operations from callback bytecode fail with
`SL_NATIVE_REENTRANT_CALL` to prevent mutex deadlocks and module-unload races.
Callback failure is a handled plugin error and never requests bytecode
fallback. The binder is optional, so existing plugins remain compatible.

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

The tests exercise the C interface and C++ wrapper against fixtures, move-only
ownership, ABI mismatch and missing-entry rejection, authorization denial,
bounded-table exhaustion, stale-handle rejection, and typed-call success,
argument/result validation, and compatibility with a legacy run-only module.
On POSIX, CMake links the system dynamic-loader library as needed. On Windows,
use a compiler/toolchain that produces the plugin DLL and exports the ABI
symbols.

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
