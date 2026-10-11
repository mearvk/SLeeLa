<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# OS Creator examples

Compilable, runnable examples that demonstrate the SLeeLa OS Creator for the
three major OS families. Each example shapes an OS, checks the pre-build
readiness gate (download URLs + settings in place), and prints the key artifacts
the OS Creator writes — `os_config.h`, the minimum-boot base-driver series, the
`downloads.conf` URL set, the swap/filesystem settings, and the foreign-exec
(compat) routes.

| Example | Family | Highlights |
|---|---|---|
| [`linux/linux-creator.sleela`](linux/linux-creator.sleela) | Linux | GRUB · Linux 6.12 · ext4 · systemd · GNOME · swap file · runs Windows `.exe` (Wine+binfmt) |
| [`windows/windows-creator.sleela`](windows/windows-creator.sleela) | Windows 10+ | BOOTMGR · NT 10.0.26100 · NTFS · SCM · Explorer · pagefile · runs Linux (WSL2) |
| [`macos/macos-creator.sleela`](macos/macos-creator.sleela) | macOS | boot.efi · XNU/Darwin 24 · APFS · launchd · Aqua · dynamic swap · runs Windows (Wine) + Linux (Lima) |

## Running

Each is a single, self-contained file — run it directly from the repo root:

```sh
./impl/build/sleela run lib/os/examples/linux/linux-creator.sleela
./impl/build/sleela run lib/os/examples/windows/windows-creator.sleela
./impl/build/sleela run lib/os/examples/macos/macos-creator.sleela

# validate without running:
./impl/build/sleela check lib/os/examples/linux/linux-creator.sleela
```

Each prints `readyToCompile = true`, then the emitted config header, the
base-driver series, the resolved download URLs, the settings, and the compat
routes — the inputs the OS Creator turns into a `/os-development` tree.

## Issues and notes (read before extending)

These are deliberate design choices forced by the current SLeeLa toolchain, and
the reasons the examples look the way they do:

1. **The examples are self-contained aggregates, not direct `new SLOSCreator()`
   usage — and this is required, not a shortcut.** `sleela run`/`check` compiles
   **one file as a single compilation unit**. The split `lib/os` provider
   classes (`SLOSModel`, `SLOSShape`, the emitters, …) are **not** visible to a
   program that only `import`s them, so `SLOSModel m = new SLOSModel();` in a
   standalone example fails with `unknown type 'SLOSShape'`. Each example
   therefore inlines a compact model + the emit logic in the one file it runs,
   exactly like `lib/os/os-build.sleela` and `os-generate.sleela` do. This is
   the established runnable pattern for this package.

2. **Several split provider classes do not compile standalone at all**, because
   they call the `native sleela_str_*` string intrinsics (e.g. `SLCEmitter`,
   `SLISOBuilder`, `SLOSLoaderEmitter`), which are accepted by the library
   *index* but rejected by the single-file compiler (`unexpected token 'native'
   in expression`). They are design contracts consumed through the toolchain,
   not runnable units. The examples avoid `native` entirely and build their
   strings with ordinary `+` concatenation.

3. **These examples PRINT the artifacts; they do not write files.** They mirror
   the shape and content the OS Creator produces so the flow is visible and the
   example stays side-effect-free and fast. To actually **write** a Linux tree
   to disk (and fetch the upstream parts), use the writes-to-disk generator:
   `./impl/build/sleela run lib/os/os-generate.sleela` (honours `OS_NAME`,
   `OS_ARCH`, `OS_KERNEL_VER`, `OS_OUT`). Only Linux currently has a
   writes-to-disk generator; the Windows/macOS examples are print-only
   demonstrations of their shapes.

4. **Values are faithful to each family** (kernels, loaders, rootfs, init, swap
   kind, driver names, compat runtimes) but the model is **compact** — it is a
   teaching slice of the full `SLOSModel`/layer set, not every field. For the
   complete layer model and the real emitters, see `../OS.md` and the
   `../os-creator/` provider classes; for the normative composition order and
   the schema the compiler recognises, see `../os-creator/CREATOR.DOCTRINE.md`
   and `../os-creator/os-creator.xsd`.

5. **`#sleela 1.11`** is required: the examples use features added through 1.11
   (nested-friendly dispatch, the current surface). Running them on an older
   compiler is rejected by the version gate.

## Relationship to the real provider

| Example piece | Real `lib/os` object(s) |
|---|---|
| the inlined `Model` + `linux()`/`windows()`/`macos()` | `SLOSModel` + `SLOSShape` (`shapeAs`) |
| `configHeader()` | `SLCEmitter.emitConfigHeader` |
| `baseDrivers()` | `SLBaseDriverSeries` + `SLBaseDriverEmitter` |
| `downloads()` | `SLDownloadSet` |
| `settings()` | `SLSwapConfig` + `SLFilesystemCatalog` + `SLConfigSet` |
| `compat()` | `SLCompatSet` + `SLCompatLayer` + `SLCompatLoaderEmitter` |
| `readyToCompile()` | `SLOSModel.readyToCompile()` |