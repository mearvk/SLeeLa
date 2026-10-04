# SLeeLa Text Library Build

Max Rupplin - MEARVK LLC - 2026

The text package has an explicit native build path for its string-processing
boundary. The `.sleela` definitions remain the source-level contract; the C/C++
layer below is the implementation boundary.

## Layout

```text
lib/text/
  include/
    sleela_string.h      C ABI: processing, manipulation, substring, split
    sleela_string.hpp    C++ orchestration facade (sleela::text::SleelaString)
  src/
    sleela_string.c      C implementation (C11, stdlib only)
    sleela_string.cpp    C++ free-function orchestration (join, normalizeWhitespace)
  Makefile               package build
  *.sleela               text front-end object definitions (source authority)
```

## Build order

```text
lib/text/*.sleela                 (source-level contract)
        |
        v
include/sleela_string.h           (stable C ABI)
        |
        +--> src/sleela_string.c  --> build/sleela_string.c.o
        |
        v
include/sleela_string.hpp         (C++ orchestration)
        |
        +--> src/sleela_string.cpp --> build/sleela_string.cpp.o
```

Run from the repository root:

```sh
make text
```

or:

```sh
make -C lib/text all
```

Objects are produced under `lib/text/build/` (git-ignored).

## Targets

| Target | Effect |
|---|---|
| `all` | Build native objects, then run the sanity check (default) |
| `native` | Compile the C and C++ string-processing objects |
| `sanity` | Print the text package contract |
| `clean` | Remove `lib/text/build/` |
| `help` | List targets |

## Toolchain

| Variable | Default | Purpose |
|---|---|---|
| `CC` | `cc` | C compiler |
| `CXX` | `c++` | C++ compiler |
| `CFLAGS` | `-std=c11 -Wall -Wextra -Wpedantic` | C flags |
| `CXXFLAGS` | `-std=c++17 -Wall -Wextra -Wpedantic` | C++ flags |
| `BUILD_DIR` | `build` | Object output directory |

The C and C++ translation units compile cleanly under `-Wall -Wextra
-Wpedantic`. They use distinct `.c.o` / `.cpp.o` object stems so the two units
never collide on one `build/<stem>.o` target — both languages are always built.
