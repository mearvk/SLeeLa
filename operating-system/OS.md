# `/os` — the native C/C++ subsystem backing `lib/os`

This directory holds the C and C++ that backs the **fine and executive
functions** of the SLeeLa `lib/os` package. It builds to one static library,
`libsleela_os.a`, following the same top-level-subsystem convention as
[`/compression`](../compression/) and [`/antivirus`](../antivirus/): public
headers in `include/`, implementation in `src/`, unit tests in `tests/`, a
`Makefile`, and this document.

`lib/operating-system/*.sleela` is the design and orchestration layer; it reaches the real
host OS through the VM's `os*` / file built-ins (serviced by `impl/core`
`slos_*` / `slio_*`). This `/os` subsystem sits one level in from that — at the
**OS-build concern** — providing pure, dependency-free, unit-testable logic that
the SLeeLa layer and the OS Creator rely on. It deliberately does **not**
reimplement the VM syscall bridge; transport (git clone, mkdir, file I/O) stays
in `impl/core`.

## The five executive capabilities

| Capability | Header | Source | Backs (SLeeLa) |
|---|---|---|---|
| **Downloading** | `include/sleela_os_fetch.h` | `src/sleela_os_fetch.c` | `SLOSSensitivityFetch`, `SLDistroFetch` |
| **Manipulating** | `include/sleela_os_translate.h` | `src/sleela_os_translate.cpp` | `SLExecutableTypes`, `SLExecutableTranslator` |
| **Ideology inspecting & conversion** | `include/sleela_os_ideology.h` | `src/sleela_os_ideology.c` | `SLCompatLayer`, `SLOSShape` |
| **Security review** | `include/sleela_os_security.h` | `src/sleela_os_security.c` | `SLPermissions`, the SHA-256 verify gate |
| **Aggregation** | `include/sleela_os_aggregate.h` | `src/sleela_os_aggregate.c` | `SLOSBuildReport` |
| **Theming** | `include/sleela_os_theme.h` | `src/sleela_os_theme.c` | `SLDesktopTheme` (in `SLDesktopSpec`) |

Two shared cores support the above:

- **Shape model** (`include/sleela_os_shape.h`, `src/sleela_os_shape.c`) — the
  Linux/Windows/macOS conventions, mirroring `lib/operating-system/SLOSShape.sleela`.
- **Minimum-boot driver series** (`base_drivers.{h,c,cpp}` + the default
  `src/base_drivers_table.c`) — the driver-admission policy, cross-OS bridge,
  and boot bring-up. The default table is reference data; a real OS build
  replaces `base_drivers_table.c` with the one the OS Creator emits
  (`lib/operating-system/os-creator/SLBaseDriverEmitter.sleela`).

### What each capability does

- **Downloading** composes a correct, hardened `git clone` command and the
  pin-checkout command from a warehouse reference, and resolves the on-disk path
  of a chosen shape + component in the fetched tree. It validates the command
  and paths in one place; the VM runs the command via `slos_run`. Mirrors
  `SLOSSensitivityFetch` against `mearvk/OS.Sensitivity.III`.
- **Manipulating** classifies which executables a shape runs natively and which
  guest shape a file belongs to, so a translator knows which compatibility layer
  to apply. (C++ TU, C ABI.)
- **Ideology inspecting & conversion** classifies an artifact's declared
  `metaOrigin` into the Metadata standard's ordered jurisdictional scope
  (National → International → Galactic; `SLEELA.syntax` §15.4) and compares two
  scopes by breadth — descriptive only, never a judgement of any nation or
  belief — and names the host→guest conversion runtime for the 3×3 shape matrix.
- **Security review** provides a self-contained SHA-256 (FIPS 180-4, no external
  dependency), a file digester, and a verify-before-execute gate, plus the
  driver-admission review re-exposed from `base_drivers.h`.
- **Aggregation** records each emitted artifact (kind, byte size, path) into a
  bounded build report with a printable index and a one-line summary, mirroring
  `SLOSBuildReport`.
- **Theming** carries the appearance a constructed OS's desktop adopts: a named
  set of role colors (background/foreground/accent, reusing the editor/SleelaUI
  role vocabulary) plus the desktop-asset identifiers (GTK theme, icon set,
  cursor, wallpaper) an OS rollout applies. Per-shape defaults and edition
  presets (including a `white` theme aligned with `ubuntu-white`); the asset
  bytes are fetched from the `desktop-loader` warehouse component. Backs
  `lib/operating-system/SLDesktopTheme.sleela`, composed into `SLDesktopSpec`.

## The SLeeLa handle

`lib/operating-system/SLOSNativeSubsystem.sleela` is the SLeeLa-facing façade: it names each
capability, the native header that backs it, and the `lib/os` class that uses
it, so the contract between the two halves is explicit.

## Build & test

```sh
cd os
make          # -> build/libsleela_os.a
make test     # builds and runs every tests/test_os_*.c; prints "os: all tests passed"
make clean
```

Pure C11/C++17, built with `-Wall -Wextra -Wpedantic`; no third-party
dependencies. The C test TUs are linked with the C++ driver because the library
embeds C++ objects (same as `/compression`).

## Relationship to the warehouse and the OS Creator

The OS-grade C/C++ for each shape is sourced per user from the warehouse
[`mearvk/OS.Sensitivity.III`](https://github.com/mearvk/OS.Sensitivity.III)
(folders `linux/`, `windows/`, `macos/`). SLeeLa drives the contract
(`SLOSSensitivitySource` / `SLOSSensitivityFetch`); this subsystem supplies the
native fetch-command composition, the security review of what is fetched, the
shape/conversion logic that tailors it, and the aggregation of the resulting
preliminary ISO source build. See [`lib/operating-system/WAREHOUSE.md`](../lib/operating-system/WAREHOUSE.md).
