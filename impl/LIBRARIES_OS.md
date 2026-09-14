# Native Library Loading — OS Contract

SLeeLa extensions and subject implementations may require native shared libraries. This abstraction keeps the loader contract stable while selecting the platform-native mechanism.

## Backends

- **Linux/POSIX:** `dlopen`, `dlsym`, and `dlclose`.
- **Windows:** `LoadLibraryA`, `GetProcAddress`, and `FreeLibrary`.

## Contract

`impl/core/sleela_library.h` exposes:

- platform detection and availability;
- library open/close;
- symbol lookup;
- handle validation;
- platform loader error reporting.

The API deliberately avoids exposing `dlopen` or Win32 loader types to extension and subject code. This gives SLeeLa a single native-library boundary that can later be extended for additional operating systems without changing consumers.

## Security

Library loading is an execution boundary. Callers should resolve libraries from trusted locations and apply the repository's required SHA-256 verification policy before build or execution. The loader itself does not treat an arbitrary path as trusted.

## Smoke Test

`core/library_platform_smoke.c` loads a platform-native system library, resolves a known symbol, validates the handle, and closes it. It is included in the verified test graph.
