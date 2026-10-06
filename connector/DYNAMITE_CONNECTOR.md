# DynamiteConnector — intelligent loader of native shared libraries

`DynamiteConnector`
([`connector/java/.../DynamiteConnector.java`](java/com/mearvk/sleela/connector/DynamiteConnector.java))
is SLeeLa's facility for taking in a **native library** — a `.so` on Linux, a
`.dll` on Windows, or a `.dylib` on macOS — and loading it or binding its
symbols. It is the native-library counterpart to
[`SleelaClassLoader`](CLASS_LOADER.md): same hardened posture (explicit roots,
allow-listed names, cached, fail-closed), but for raw native code instead of
raw Java classes.

## Why SLeeLa needs this

SLeeLa already loads one native library the bare way
(`System.loadLibrary("sleela_java28")`), but `System.loadLibrary` only searches
`java.library.path` and gives no control over which file a logical name maps
to, no allow-list, and no symbol-level access. Driver families such as
`eprom-corrado` (USB → EPROM via libusb) need to load a backend library from a
known location, across three operating systems, without each caller re-deriving
the per-OS filename. `DynamiteConnector` is that single, reusable function.

## What "intelligent" means

| Property | Behavior |
|---|---|
| **Cross-platform naming** | A logical name (`sleela_java28`) maps to the right file per OS: `libsleela_java28.so` / `sleela_java28.dll` / `libsleela_java28.dylib`. Uses the JVM's own `System.mapLibraryName` plus the common conventions, so both `foo` and an already-decorated `libfoo.so` resolve. |
| **Explicit roots** | `addRoot(dir)` lists directories to search; no reliance on ambient `java.library.path`. |
| **Allow-listed** | A name loads only if `allowLibrary(name)` permitted it (or `allowAny()` in a trusted context). Default is fail-closed. |
| **Two load modes** | `load(name)` does a JNI-style `System.load`; `lookup(name)` opens an FFM `SymbolLookup` (no JNI) so individual symbols can be found/bound. |
| **Cached** | Resolutions and symbol lookups are memoised. |
| **Fail closed** | A disallowed name, a name with path separators / `..`, or a name that resolves to no file throws rather than guessing. |

Thread-safe; `AutoCloseable` (closing releases any FFM `Arena` opened for symbol
lookups).

## Usage

```java
try (DynamiteConnector dc = new DynamiteConnector()) {        // detects the OS
    dc.addRoot(Path.of("build/linux"))                        // where the libs are
      .allowLibrary("sleela_java28");                         // allow-list a name

    // JNI-style: load so registered natives become callable
    dc.load("sleela_java28");

    // FFM-style: bind individual symbols without JNI
    if (dc.hasSymbol("sleela_java28", "sleela_entry")) {
        var addr = dc.find("sleela_java28", "sleela_entry").orElseThrow();
        // ... bind with java.lang.foreign.Linker.downcallHandle(addr, ...) ...
    }
}
```

Running an FFM lookup requires `--enable-native-access` for the module/jar that
owns the connector (e.g. `--enable-native-access=ALL-UNNAMED` on the classpath).

## Errors

- `DynamiteConnector.LibraryResolutionException` — not allow-listed, resolved to
  no file on any root, or failed to load / open a symbol lookup.
- `IllegalArgumentException` — a blank name, a name containing a path separator,
  or a name containing `..` (guards against path-traversal input).

## Relationship to driver families

A family like [`eprom-corrado`](../eprom-corrado/README.md) can use
`DynamiteConnector` to load its backend native library (e.g. a libusb-backed
EPROM driver) from the family's own build output, then reach it either through
JNI (`load`) or FFM (`lookup`). The connector handles *finding and loading* the
library; the family's contract (`EpromControl` / `EpromConnector`) handles
*using* it.

## Security

Loading native code runs with full process privilege — there is no sandbox once
a `.so`/`.dll`/`.dylib` is loaded. Treat the allow-list and roots as a trust
boundary: only permit names and directories you control. Do not derive a root or
a library name from untrusted input. This is why the default is fail-closed and
`allowAny()` must be called explicitly.
