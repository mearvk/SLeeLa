# OS Creator™ — Architecture

OS Creator™ is the SLeeLa provider that turns an object-oriented OS description
(`SLOSModel`) into a complete, on-disk **OS development tree** at `/os-development`.
It is the capstone of `lib/os`: where `SLOSModel` *describes* an OS and
`SLOSCompiler`/`SLISOBuilder`/`SLInstallerGenerator` emit individual artifacts,
OS Creator™ **lays down every piece of the OS, its source, its configuration,
and its build tooling** as real files, so that running the compiled OS Creator
object produces a buildable OS project top-to-bottom.

> **Name & mark.** This component is **OS Creator™**. The trademark denotes the
> provider surface (`SLOSCreator`) and the `/os-development` layout contract
> described here.

## 1. The provider contract

```text
   SLeeLa source: an SLOSModel (shape = Windows / Linux / macOS, arch, parts)
        |
        v
   SLOSCreator.create(model, "/os-development")      ← the OS Creator™ provider
        |
        +-- osMakeDir(...)   lays out the /os-development directory tree
        +-- piece emitters   emit C + C++ source for every OS piece
        +-- config emitters  emit each piece's configuration file
        +-- toolchain emitter emits Makefile + build.sh/build.ps1
        +-- source dropper    copies the SLeeLa source + the emitted source in
        |
        v
   /os-development            a complete, compilable OS project on disk
        |
        +-- make / build.sh    ← run the emitted toolchain to build the OS -> ISO
```

Running the **compiled** OS Creator object (the SLeeLa program that composes an
`SLOSModel` and calls `SLOSCreator.create(...)`) is what materializes the tree.
Every file write crosses the explicit VM/OS bridge via the `openFile`/`write`/
`close` and `osMakeDir` built-ins — the same honest bridge the rest of `lib/os`
uses. No file is invented in memory only; the result is a real directory.

## 2. The OS pieces (what gets written)

OS Creator™ writes one subtree per piece of the firmware → boot → kernel →
userspace → desktop chain. Each piece ships its C and C++ source in the
varieties/scales the model implies, plus a configuration file:

| # | Piece | Directory | Role | Emitter |
|---|---|---|---|---|
| 1 | **UEFI** | `firmware/uefi/` | UEFI firmware entry + ESP handoff | `SLUEFIEmitter` |
| 2 | **BIOS** | `firmware/bios/` | legacy BIOS/CSM entry (16-bit stub + C) | `SLBIOSEmitter` |
| 3 | **Boot Loader** | `boot/loader/` | GRUB/BOOTMGR/boot.efi-shaped loader | `SLBootLoaderEmitter` |
| 4 | **Kernel Loader** | `boot/kernel-loader/` | loads + relocates the kernel image | `SLKernelLoaderEmitter` |
| 5 | **Driver Loader** | `kernel/driver-loader/` | enumerates + loads built-in/module drivers | `SLDriverLoaderEmitter` |
| 6 | **OS Loader** | `userspace/os-loader/` | brings up PID 1 / the service manager | `SLOSLoaderEmitter` |
| 7 | **Desktop GUI Loader** | `desktop/gui-loader/` | starts the display server + session shell | `SLDesktopLoaderEmitter` |
| 8 | **The OS** | `os/` | the composed OS image + boot-chain `assemble()` | `SLCEmitter` + `SLCppEmitter` |

Each piece directory contains:
- `*.c` — the C surface of the piece,
- `*.cpp` — the C++ surface of the piece,
- a config file (`*.conf` / `*.json` / `*.cfg` as appropriate for the piece),
- a per-piece `README.md` describing what it is and how it builds.

A **server edition** omits the Desktop GUI Loader (piece 7); a **Windows shape**
labels the BIOS piece a CSM and the OS loader the Service Control Manager, etc.
— the piece set is the same contract, the content is shape-specific.

## 3. The `/os-development` layout

```text
/os-development/
├── MANIFEST.md                 what was generated, from which model, when
├── Makefile                    top-level build: all pieces -> ISO
├── build.sh                    POSIX build driver (Linux/macOS shapes)
├── build.ps1                   PowerShell build driver (Windows shape)
├── config/
│   └── os.conf                 the model's identity + global config
├── firmware/
│   ├── uefi/     {uefi.c, uefi.cpp, uefi.conf, README.md}
│   └── bios/     {bios.c, bios.cpp, bios.cfg, README.md}
├── boot/
│   ├── loader/        {bootloader.c, bootloader.cpp, loader.conf, README.md}
│   └── kernel-loader/ {kernel_loader.c, kernel_loader.cpp, kernel-loader.conf, README.md}
├── kernel/
│   └── driver-loader/ {driver_loader.c, driver_loader.cpp, drivers.conf, README.md}
├── userspace/
│   └── os-loader/     {os_loader.c, os_loader.cpp, services.conf, README.md}
├── desktop/
│   └── gui-loader/    {gui_loader.c, gui_loader.cpp, session.conf, README.md}
├── os/
│   ├── os_config.h             (from SLCEmitter)
│   ├── os_boot.c               (from SLCEmitter)
│   └── os_image.cpp            (from SLCppEmitter)
├── src/
│   └── sleela/                 the SLeeLa source that produced this tree
└── tools/
    └── gen-iso.sh              the ISO recipe (from SLISOBuilder)
```

## 4. Classes

| Class | Role |
|---|---|
| `SLOSCreator` | The OS Creator™ provider. `create(model, root)` lays out `/os-development` and drives every emitter; `plan(model)` returns the file plan without writing. |
| `SLOSDevTree` | Owns the `/os-development` directory layout: makes directories and writes files through the OS bridge; records a manifest. |
| `SLOSPiece` | One emitted piece (name, directory, C source, C++ source, config, readme). |
| `SLUEFIEmitter`, `SLBIOSEmitter`, `SLBootLoaderEmitter`, `SLKernelLoaderEmitter`, `SLDriverLoaderEmitter`, `SLOSLoaderEmitter`, `SLDesktopLoaderEmitter` | Per-piece C/C++ + config emitters. |
| `SLToolchainEmitter` | Emits the `Makefile`, `build.sh`, and `build.ps1` that compile the written pieces into the OS and ISO. |
| `SLSourceDropper` | Writes the SLeeLa source (and the emitted C/C++) into `/os-development/src`. |

`SLOSCreator` reuses the existing `SLCEmitter`/`SLCppEmitter` for piece 8 (the OS
itself) and `SLISOBuilder` for `tools/gen-iso.sh`, so there is one source of
truth for the OS image and the ISO recipe.

## 5. Varieties and scales

"All their varieties and scales" is expressed two ways, both from the single
`SLOSModel`:
- **Variety** — the shape (`SLOSShape`) selects the piece *content*: a Linux
  shape emits a GRUB-shaped boot loader and a systemd OS loader; a Windows shape
  emits a BOOTMGR loader and a Service Control Manager OS loader; macOS emits
  boot.efi and launchd.
- **Scale** — the architecture set (`SLArchitectureSet`) and edition
  (Desktop/Server) select *how many* targets and pieces: a multi-arch model
  emits per-arch build matrices in the Makefile; a server edition drops the GUI
  loader; the memory/storage parts size the emitted config.

## 6. Integrity

OS Creator™ writes **into** `/os-development` (an output tree), never into the
repository's gated sources, so it does not perturb `security/sha256-manifest.json`
(the build gate). The OS Creator™ source files themselves are ordinary `/lib`
units recorded in `lib/LIBRARY.SYMBOLS.md` and `SHA256-DIGESTS.md` like any other
library source. See `lib/os/OS.md` for how OS Creator™ sits atop the OS model.
