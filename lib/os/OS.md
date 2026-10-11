# `lib/os` — Talking to the Host Operating System from SLeeLa

This package lets a SLeeLa program drive the **real host operating system** —
Windows, Linux, or macOS — through a single, portable surface. A developer
writes `os.run("make")`, `env.get("HOME")`, `fs.exists(path)`, or
`proc.start(cmd)`, and the SLeeLa VM makes the **genuine System API call** for
whatever OS the program is running on:

```
 SLeeLa source (lib/os/SL*.sleela)
   -> os* built-ins               osRun / osGetEnv / osSpawn / osExists / ...
     -> SLVM opcodes              OP_OS_* (sleela_core.c dispatch)
       -> sleela_os.c backend     #ifdef _WIN32  -> Win32 API
                                  #elif POSIX    -> Linux/macOS syscalls
```

The same `.sleela` program is portable: it calls one named built-in and the VM
selects the Win32, Linux, or macOS implementation at the native layer. Where the
platforms genuinely differ (shells, path separators, directory layout,
permission models), the **OS-specific flavor classes** make those differences
explicit instead of hiding them.

> **Boundary — honesty note.** These calls cross the explicit VM/OS bridge the
> same way the rest of the standard library does (sockets, files, time). A
> spawned child process is a **VM-local bounded handle**, never a raw PID or
> Win32 HANDLE — the same safety discipline used everywhere in the VM. The
> `os*` built-ins require `#sleela 1.5`.

## The SLVM OS system-call surface

The VM exposes these built-ins (lowered to `OP_OS_*` opcodes, serviced by
`impl/core/sleela_os.c`). All three OSes implement every one of them.

| Built-in | Effect | Host API (Windows / POSIX) |
|---|---|---|
| `osPlatform()` → String | Host OS name: `"Windows"`/`"Linux"`/`"macOS"` | compiled platform |
| `osCapability(cap)` → int | 1/0: does the host support a capability class | `slplatform_*` |
| `osGetEnv(name)` → String | Environment variable (`""` if unset) | `GetEnvironmentVariable` / `getenv` |
| `osSetEnv(name,val)` → int | Set/overwrite a variable (0 ok) | `SetEnvironmentVariable` / `setenv` |
| `osCurrentDir()` → String | Current working directory | `GetCurrentDirectory` / `getcwd` |
| `osChangeDir(path)` → int | Change working directory (0 ok) | `SetCurrentDirectory` / `chdir` |
| `osHostName()` → String | Machine host name | `GetComputerName` / `gethostname` |
| `osUserName()` → String | Current user name | `GetUserName` / `getpwuid`/`$USER` |
| `osTempDir()` → String | System temp directory | `GetTempPath` / `$TMPDIR`-or-`/tmp` |
| `osProcessId()` → int | This process' id | `GetCurrentProcessId` / `getpid` |
| `osExists(path)` → int | 1 if the path exists | `GetFileAttributes` / `stat` |
| `osIsDir(path)` → int | 1 if the path is a directory | `FILE_ATTRIBUTE_DIRECTORY` / `S_ISDIR` |
| `osFileSize(path)` → int | Size in bytes (`-1` on error) | `GetFileAttributesEx` / `stat` |
| `osMakeDir(path)` → int | Create a directory (0 ok, ok if exists) | `CreateDirectory` / `mkdir` |
| `osRemove(path)` → int | Delete a file or empty dir (0 ok) | `DeleteFile`/`RemoveDirectory` / `unlink`/`rmdir` |
| `osRename(from,to)` → int | Rename/move (0 ok) | `MoveFileEx` / `rename` |
| `osRun(cmd)` → int | Run a shell command synchronously; exit code | `system`/cmd.exe / `/bin/sh` |
| `osSpawn(cmd)` → int | Start a background command; VM-local handle | `CreateProcess` / `posix_spawn` |
| `osWait(h)` → int | Wait for a spawned process; exit code; frees `h` | `WaitForSingleObject` / `waitpid` |
| `osKill(h)` → int | Terminate a spawned process (0 ok) | `TerminateProcess` / `kill(SIGTERM)` |
| `osProcessClose(h)` | Release a spawned process without waiting | `CloseHandle` / reap `WNOHANG` |

## The classes

### General (cross-OS) — write once, run on all three

| Class | Role |
|---|---|
| `SLOperatingSystem` | The capstone facade: platform identity, capabilities, environment, working directory, and synchronous process execution. |
| `SLEnvironment` | Read/write environment variables, with portable `HOME`/`PATH`/`SHELL` resolution across Windows and POSIX. |
| `SLProcess` | A host process: `runSync()`, or `start()` → `wait()`/`kill()`/`close()` over a VM-local handle. |
| `SLFileSystem` | Metadata + namespace operations: exists / isDir / size / makeDir / remove / rename / move. |
| `SLFile` | A single named file: existence, size, delete, rename. |
| `SLDirectory` | A directory: create, test, remove, rename, `enter()` (chdir), plus current/temp dir. |
| `SLPath` | Host-aware path join/append using the correct separator (`\` vs `/`) and list separator (`;` vs `:`). |
| `SLPermissions` | Portable read/write/execute/exists view; the full ACL/mode model is per-OS. |
| `SLClock` | The host system clock: wall-clock millis, monotonic nanos, HTTP date, and a monotonic stopwatch. |
| `SLEventSignal` | Deliver an OS terminate request (SIGTERM / TerminateProcess) to a child process. |

### OS-specific flavors — show off each platform

| Class | Role |
|---|---|
| `SLWindowsOS` | Windows idioms: cmd.exe / PowerShell, `USERPROFILE`/`APPDATA`/`SystemRoot`, backslash paths and drive letters. |
| `SLLinuxOS` | Linux idioms: `/bin/sh`, HOME/PATH/XDG, the FHS roots (`/etc`,`/proc`,`/sys`,`/tmp`,`/var`), and reading `/proc/cmdline` + `/etc/os-release`. |
| `SLMacOS` | macOS idioms: `open(1)`/`open -a`, the `~/Library` layout, Homebrew prefix detection, and `sw_vers`/`defaults` queries. |

Each flavor class exposes `isHost()` so a program can branch safely:

```sleela
SLWindowsOS win = new SLWindowsOS(); win.configure();
if (win.isHost()) { win.powershell("Get-Process"); }
```

## Minimal usage

```sleela
SLOperatingSystem os = new SLOperatingSystem();
os.configure("host"); os.open();

os.platform();                 // "Linux" / "Windows" / "macOS"
os.userName();                 // the logged-in user
os.getEnv("PATH");             // the search path
os.changeDir(os.tempDir());    // cd into the temp dir
int rc = os.run("echo hello"); // run a command, get its exit code

// A background job, reaped for its exit code:
SLProcess job = new SLProcess();
job.configure("sleep 1");
job.start();
int code = job.wait();         // 0
```

## Relationship to the rest of SLeeLa

- `lib/cpu` models an operating system **in SLeeLa** (a teaching kernel on a
  simulated CPU). `lib/os` is the opposite direction: SLeeLa source that drives
  the **real** host OS through the VM. They are complementary — one builds an OS,
  the other uses one.
- File *content* I/O continues to use the existing `openFile`/`read`/`write`/
  `close` built-ins; `lib/os` adds the surrounding **namespace, process,
  environment, and identity** services an OS provides.
- Time is an OS service, so `SLClock` reuses the existing `time*` built-ins
  rather than duplicating the clock substrate.

## Note on fidelity

Every method here resolves to a genuine host System API call on Windows, Linux,
and macOS. Where an OS concept has no portable equivalent (POSIX mode bits vs.
Windows ACLs; signals vs. `TerminateProcess`), the general classes expose the
portable intersection and the flavor classes expose the native idiom, so the
differences are named rather than hidden.

---

# Building an OS by OOD — the OS-construction family

The classes above *use* the real host OS. The classes below *design and build a
new one*. This is the second half of `lib/os`: an object-oriented model for
**constructing an operating system**, able to assume the shape of Windows,
Linux, or macOS, where **compiling and running the SLeeLa description emits the
C and C++ code for the OS, assembles it into a bootable ISO, and generates a
per-OS-type installer**.

It is the complement to `lib/cpu` from the other direction. `lib/cpu` compiles a
small custom kernel onto a simulated CPU; this family describes a *whole
distribution* — firmware → boot → kernel → user space → desktop — as composed
SLeeLa objects, then lowers that object model to the native sources a real
toolchain builds toward an `.iso`.

> **Boundary — honesty note.** These classes are a **design and code-generation
> model**. They emit C/C++ source, an `xorriso`/`mkisofs` ISO recipe, and
> installer scripts; the actual compilation, image authoring, and disk writes
> are performed by the native toolchain the emitted recipes drive (`cc`, `c++`,
> `xorriso`, `grub-install`/`bcdboot`/`bless`, `qemu`). The SLeeLa layer is the
> single source of the OS *description* and of those recipes, not a replacement
> for the host toolchain.

## The layered model

An OS is described as one `SLOSModel` aggregate composing one object per layer,
in the firmware → desktop order a system actually boots:

```text
SLOSShape          which family: Linux / Windows / macOS (the OOD axis)
SLArchitectureSet  target CPU arch(es): amd64 / arm64 / riscv64 / ...
      ↓
SLBootloaderSpec   firmware + loader (UEFI+GRUB / UEFI+BOOTMGR / UEFI+boot.efi)
      ↓
SLKernelSpec       kernel family, version, subsystems, command line
      ↓
SLDriverSet        drivers (built-in vs module; storage must reach the rootfs)
      ↓
SLFilesystemLayout root filesystem format + directory namespace (FHS / Windows / Apple)
      ↓
SLPackageSet       the composed userland package seed (debootstrap-style)
      ↓
SLServiceSet       system services under the init manager (systemd / SCM / launchd)
      ↓
SLDesktopSpec      optional desktop/session (GNOME / Explorer / Aqua), or headless
```

## The classes

### Model (the OOD aggregate and its layers)

| Class | Role |
|---|---|
| `SLOSModel` | Root aggregate: composes every layer; `shapeAs(family, arch)` applies a shape's defaults; `isBuildable()` checks the model is self-consistent. |
| `SLOSShape` | The OOD extrapolation axis: Linux / Windows / macOS, and the per-family defaults (separators, init manager, boot style, rootfs, desktop). |
| `SLOSArchitecture` / `SLArchitectureSet` | A target CPU architecture (triple, package arch, word size, endianness) and the multi-arch set a model builds for. |
| `SLBootloaderSpec` | Firmware interface + boot loader + Secure Boot + initramfs decision + ESP. |
| `SLKernelSpec` | Kernel family/version, subsystem toggles, boot command line; `isBootable()`. |
| `SLDriverSet` | Device drivers, each built-in or module; `canReachRootfs()`. |
| `SLFilesystemLayout` | Root filesystem format and the directory manifest for the shape. |
| `SLPackageSet` | The base + layered package seed installed into the rootfs. |
| `SLServiceSet` | The services the init/service manager brings up at boot. |
| `SLDesktopSpec` | The desktop/session stack, or `headless()` for a server edition. |

### Shape presets — OOD extrapolation

| Class | Role |
|---|---|
| `SLLinuxOSModel` | `extends SLOSModel`; `build(name, ver)` yields a Linux-shaped OS (Linux + GRUB + ext4/FHS + systemd + GNOME). `buildMultiArch` adds arm64/riscv64. |
| `SLWindowsOSModel` | `extends SLOSModel`; a Windows-shaped OS (NT + BOOTMGR + NTFS + SCM + Explorer). |
| `SLMacOSModel` | `extends SLOSModel`; a macOS-shaped OS (XNU + boot.efi + APFS + launchd + Aqua). `buildUniversal` adds x86_64. |

### Compilation → C/C++ → ISO → installer

| Class | Role |
|---|---|
| `SLCEmitter` | Emits `os_config.h` + `os_boot.c` (the C early-boot path) from a model. |
| `SLCppEmitter` | Emits `os_image.cpp` (the C++ `OsImage` that composes the boot chain). |
| `SLOSCompiler` | Front door: validates the model, runs both emitters, and (via `compileToIso`) drives the ISO build and installer generation. |
| `SLISOBuilder` | Assembles the rootfs and emits the `xorriso`/`mkisofs` ISO recipe. |
| `SLInstallerGenerator` | Emits the installer in the shape's idiom: `install-linux.sh`, `install-windows.ps1`, or `install-macos.sh`, each with a QEMU `--vm` path. |
| `SLOSArtifact` / `SLOSBuildReport` | A produced artifact (source / script / ISO) and the enumerable index of a whole build. |

## Minimal usage

```sleela
// Design a Linux-shaped OS by OOD, then compile it to C/C++, an ISO, and an installer.
SLLinuxOSModel m = new SLLinuxOSModel();
m.build("Determinant", "1.0");          // assume the shape of Linux
m.packageSet().add("firefox", "latest"); // refine any layer afterwards

SLOSCompiler c = new SLOSCompiler();
c.configure();
SLOSBuildReport report = c.compileToIso(m);   // -> os_config.h, os_boot.c,
                                              //    os_image.cpp, the ISO recipe,
                                              //    and install-linux.sh
print(report.summary());                      // "OK: Determinant 1.0 (...) -> 5 artifacts"
```

Re-shaping the OS is a one-line change — `new SLWindowsOSModel()` or
`new SLMacOSModel()` — and every emitted artifact (header defines, the C++
`OsImage`, the ISO loader and rootfs format, the installer idiom) changes with
it. See `os-build.sleela` for a self-contained, runnable demonstrator that
prints each generated artifact.

## Relationship to the OS projects in this lineage

This model mirrors the structure of the real OS projects built with SLeeLa (an
Ubuntu-derived distribution): a compositional, `debootstrap`-style base plus
layered packages; a firmware → boot → kernel → user space → desktop boot chain;
multi-architecture targets; an `xorriso`-authored ISO; and an installer that is
a control plane over the established Make/ISO contracts with a QEMU VM path for
read-only verification before touching a real disk. The desktop is modeled as a
replaceable, optional layer — **GNOME is not the operating system** — exactly as
that lineage treats it.

---

# Named hardware parts → a Machine on the Sleela VM

Beyond describing an OS, `lib/os` describes the **physical machine** it runs on,
part by part. Because the real catalogue of device types is finite, each part is
a **named SLeeLa class** you request by manufacturer and variant — "Samsung
DDR5", "Samsung 990 PRO NVMe PCIe 5.0", "AMI Aptio UEFI", "NVIDIA GeForce RTX
4090" — and compose into a machine. Compiling and running that machine
description **produces an actual machine on the Sleela VM** (it sizes the VM's
RAM and block store from the real part capacities and composes the `lib/cpu`
stack), which can then boot and run programs.

## The part families (with spec documents)

Each family ships a base device class, a by-name manufacturer catalogue, and a
`SPECIFICATION.md` grounded in the real device standard.

| Directory | Class(es) | Variants modeled by name | Manufacturers |
|---|---|---|---|
| `memory-ddr/` | `SLDDRModule`, `SLDDRCatalog` | **DDR, DDR2, DDR3, DDR4, DDR5** (standard, data rate, voltage, ECC) | Samsung, SK Hynix, Micron/Crucial, Kingston, Corsair, G.Skill, Mushkin |
| `ssd/` | `SLSSD`, `SLSSDCatalog` | **SATA III, NVMe PCIe 3/4/5**; form factor; SLC/MLC/TLC/QLC | Samsung, WD/SanDisk, SK Hynix/Solidigm, Crucial, Kingston, Seagate, Sabrent |
| `hdd/` | `SLHardDisk`, `SLHardDiskCatalog` | **5400/7200/10k/15k RPM**; SATA/SAS; CMR/SMR | Seagate, Western Digital, Toshiba |
| `usb/` | `SLUSBDevice`, `SLUSBCatalog` | **USB 1.1, 2.0, 3.2 Gen 1/Gen 2/Gen 2x2, USB4**; HID/mass/hub/controller | Intel, AMD, ASMedia, VIA, Renesas, TI, Fresco Logic |
| `uefi/` | `SLUEFIFirmware` | UEFI revision, Secure Boot, CSM, runtime services | AMI, Insyde, Phoenix, TianoCore/EDK II (OVMF), coreboot |
| `video-cards/` | `SLVideoCard`, `SLVideoCardCatalog` | GPU vendor/arch, PCIe gen, GDDR6/6X/7/HBM, outputs, power, Vulkan/D3D | NVIDIA, AMD, Intel (+ board partners ASUS/MSI/Gigabyte/Sapphire/Zotac/…) |

All device classes extend **`SLHardwareComponent`** (shared vendor / model /
device-class identity), so every part has a uniform `identity()` and class tag.

## The registry, model, and builder

| Class | Role |
|---|---|
| `SLHardwareComponent` | Common base of every named part. |
| `SLHardwareRegistry` | One by-name front door composing all catalogues: `memory(mfr, gen, mb)`, `ssd(mfr, iface, gb)`, `hdd(mfr, rpm, gb)`, `usbController(mfr, gen, ports)`, `uefi(vendor)`, `video(vendor, model, vramMb)`. |
| `SLMachineModel` | The OOD aggregate of a machine: CPU identity + the named parts installed into it; `isRunnable()`, `identity()` (a build sheet). |
| `SLMachineBuilder` | Turns a model into a **running machine on the Sleela VM**: sizes `SLRAM`/the block store from the real part capacities and composes the `lib/cpu` `SLMachine` (`SLCPU` + `SLHardDrive`), then boots and runs programs on it. |

## Minimal usage

```sleela
// 1. Request named parts from the registry.
SLHardwareRegistry hw = new SLHardwareRegistry(); hw.configure();
SLDDRModule   ram = hw.memory("Samsung", "DDR5", 16384);         // 16 GB DDR5
SLSSD         ssd = hw.ssd("Samsung", "PCIe5", 2048);            // 2 TB NVMe PCIe 5
SLUEFIFirmware fw = hw.uefi("AMI");                              // AMI Aptio
SLVideoCard   gpu = hw.video("NVIDIA", "RTX 4090", 24576);      // 24 GB

// 2. Assemble the machine model.
SLMachineModel m = new SLMachineModel(); m.configure("Workstation");
m.cpu("AMD", "Ryzen 9", 16, 64, 4500);
m.installMemory(ram, 2);        // two modules -> 32 GB
m.installSsd(ssd);
m.installFirmware(fw);
m.installVideo(gpu);
print(m.identity());            // the build sheet

// 3. Produce a machine on the Sleela VM and run a program on it.
SLMachineBuilder b = new SLMachineBuilder(); b.configure();
int result = b.buildBootAndRun(m, 64);                 // boot + run the demo program
// or run a program from source on the composed machine:
// int r = b.buildAndRunSource(m, 1, "prog.c", "prog");   // 1 = C
// or boot an OS guest (ties back to SLOSModel's Linux shape):
// int n = b.buildAndBootGuest(m, "kernel.c", 1, 4096, 100000);
```

Swapping a part name re-sizes the produced machine: `hw.ssd(...)` → `hw.hdd(...)`
changes the boot media and block count, and `"DDR5"` → `"DDR4"` changes the
memory standard and bandwidth. See `machine-build.sleela` for a self-contained,
runnable demonstrator that prints a full build sheet and the VM sizing.

## How the two halves connect

- **`SLOSModel`** (above) describes the **software** OS by shape (Windows/Linux/
  macOS) and emits C/C++ → ISO → installer.
- **`SLMachineModel`** describes the **hardware** by named parts and builds a
  running machine on the Sleela VM.
- `SLUEFIFirmware.canLaunch(loader)` is the seam between them: the hardware
  firmware launches the OS model's `SLBootloaderSpec` loader, and
  `SLMachineBuilder.buildAndBootGuest(...)` boots an OS guest on the machine the
  hardware description produced — firmware → boot → kernel → userspace →
  desktop, from named silicon up.

---

# OS Creator™ — from an OS description to `/os-development`

OS Creator™ is the capstone provider of `lib/os` (its classes live under
`lib/os/os-creator/`; see `os-creator/ARCHITECTURE.md` for the full design). It
turns an `SLOSModel` into a complete, on-disk **OS development tree** at
`/os-development`: every piece of the OS, its C and C++ source, its configuration
file, the build tooling, and the SLeeLa source that regenerates it — written as
real files through the `osMakeDir` / `openFile` / `write` / `close` bridge.

Running the **compiled** OS Creator object (a SLeeLa program that composes an
`SLOSModel` and calls `SLOSCreator.create(model, "/os-development")`) is what
materialises the tree.

## What it writes

| Piece | Directory | Files |
|---|---|---|
| UEFI | `firmware/uefi/` | `uefi.c`, `uefi.cpp`, `uefi.conf`, `README.md` |
| BIOS / CSM | `firmware/bios/` | `bios.c`, `bios.cpp`, `bios.cfg`, `README.md` |
| Boot Loader | `boot/loader/` | `bootloader.c`, `bootloader.cpp`, `loader.conf`, `README.md` |
| Kernel Loader | `boot/kernel-loader/` | `kernel_loader.c`, `kernel_loader.cpp`, `kernel-loader.conf`, `README.md` |
| Driver Loader | `kernel/driver-loader/` | `driver_loader.c`, `driver_loader.cpp`, `drivers.conf`, `README.md` |
| OS Loader (PID 1) | `userspace/os-loader/` | `os_loader.c`, `os_loader.cpp`, `services.conf`, `README.md` |
| Desktop GUI Loader | `desktop/gui-loader/` | `gui_loader.c`, `gui_loader.cpp`, `session.conf`, `README.md` |
| The OS | `os/` | `os_config.h`, `os_boot.c`, `os_image.cpp` |

Plus the top-level `Makefile`, `build.sh`, `build.ps1`, `config/os.conf`,
`tools/gen-iso.sh`, `src/sleela/os-model.sleela` (the regenerating source), and
`MANIFEST.md`.

## Classes

| Class | Role |
|---|---|
| `SLOSCreator` | The OS Creator™ provider: `create(model, root)` writes the whole tree; `plan(model)` is a dry run. |
| `SLOSDevTree` | Owns `/os-development`: makes directories and writes files through the OS bridge; records a manifest. |
| `SLOSPiece` | One emitted piece (name, dir, C, C++, config, README). |
| `SLUEFIEmitter`, `SLBIOSEmitter`, `SLBootLoaderEmitter`, `SLKernelLoaderEmitter`, `SLDriverLoaderEmitter`, `SLOSLoaderEmitter`, `SLDesktopLoaderEmitter` | Per-piece C/C++ + config emitters. |
| `SLToolchainEmitter` | Emits `Makefile`, `build.sh`, `build.ps1`, `config/os.conf`. |
| `SLSourceDropper` | Writes the regenerating SLeeLa source + provenance into `src/`. |

The OS itself (`os/`) and the ISO recipe (`tools/gen-iso.sh`) reuse the existing
`SLCEmitter` / `SLCppEmitter` / `SLISOBuilder`, so there is one source of truth.

## Minimal usage

```sleela
// Describe the OS, then run OS Creator™ to write /os-development.
SLLinuxOSModel os = new SLLinuxOSModel();
os.build("Determinant", "1.0");

SLOSCreator creator = new SLOSCreator();
creator.configure();
SLOSBuildReport report = creator.create(os, "/os-development");   // writes the whole tree
print(report.summary());    // "OK: Determinant 1.0 (...) -> N artifacts"

// Then build the OS from the generated sources:
//   cd /os-development && sh build.sh      (make all -> every piece -> OS -> ISO)
```

Changing the model's shape (`SLWindowsOSModel` / `SLMacOSModel`) re-shapes every
emitted piece: a Windows tree gets a BOOTMGR `bootloader.c`, a Service Control
Manager `os_loader.c`, and `build.ps1` as the primary driver; macOS gets a
`boot.efi` loader and a launchd OS loader. See `os-creator/os-creator.sleela` for
a self-contained, runnable demonstrator that prints the layout, a sample emitted
piece, the Makefile, and the regenerating source.

## Varieties and scales

- **Variety** — `SLOSShape` selects each piece's content (GRUB vs BOOTMGR vs
  boot.efi; systemd vs SCM vs launchd; GNOME vs Explorer vs Aqua).
- **Scale** — `SLArchitectureSet` and the edition select how many targets and
  pieces: multi-arch models emit a per-arch build matrix; a server edition drops
  the Desktop GUI Loader; the memory/storage parts size `config/os.conf`.

## Integrity

OS Creator™ writes **into** `/os-development` (an output tree), never into the
repository's gated sources, so it does not perturb `security/sha256-manifest.json`.
Its own source files are ordinary `/lib` units recorded in
`lib/LIBRARY.SYMBOLS.md` and `SHA256-DIGESTS.md`.

---

# Upstream OS parts — reference and opt-in download

The OS-construction model above *generates* SLeeLa-shaped scaffolding (C/C++ →
ISO → installer) for a Linux/Windows/macOS shape. The **real, production OS
parts** — kernels, the installer, the GNOME desktop sources, filesystem tooling,
and the userland seed — live in a separate, large Ubuntu-lineage distribution:
[`mearvk/Ubuntu.Determinant.Beta.Restricted`](https://github.com/mearvk/Ubuntu.Determinant.Beta.Restricted).
Its architecture is the same firmware → boot → kernel → user space → desktop,
`debootstrap`-compositional, multi-architecture model `SLOSModel` describes (see
that repo's `UBUNTU_OS.md`), so the two are two views of one system: SLeeLa
*designs and emits* the shape; the distro *is* the sourced parts.

That distribution is multi-gigabyte and carries a GraalVM submodule, so SLeeLa
does **not** vendor it. Instead `lib/os` carries a zero-payload **reference** and
an **opt-in downloader**:

| Class | Role |
|---|---|
| `SLDistroSource` | The reference: owner/repo, clone URL, **pinned commit**, default branch, submodule flag, and the map from an OS layer (`kernel`, `installer`, `desktop`, `filesystem`, `userland`) to the distro directory that provides it. Carries no distro bytes. |
| `SLDistroFetch` | The opt-in downloader: composes a `git clone` from the reference and runs it through the `osRun` bridge. **Shallow + single-branch by default** (the safe mode for a large repo); `fetchComplete()` does a full, submodule-recursive clone and pins the exact commit. Nothing runs automatically and nothing is written into the SLeeLa repo — the destination is an external output root the developer names. |

```sleela
// Reference only (no network): know exactly where the parts come from.
SLDistroSource ref = new SLDistroSource(); ref.configure();
print(ref.identity());                 // ...@4918080dcf...
print(ref.dirFor("kernel"));           // kernels

// Opt-in download of the real OS parts, on demand:
SLDistroFetch f = new SLDistroFetch(); f.configure();
if (f.probe() == 0) {                   // cheap pre-flight (git ls-remote)
    f.fetchShallow("/os-development/upstream");   // or fetchComplete(root)
}
```

Run the self-contained demonstrator (prints the reference sheet, the exact
clone command as a dry run, and a live reachability probe — no download):

```sh
./impl/build/sleela run lib/os/distro-source.sleela
```

**How it ties into generation.** The OS Creator(TM) output tree
(`/os-development`) is the SLeeLa-emitted scaffolding; `SLDistroFetch` populates
`/os-development/upstream/` with the real Ubuntu parts next to it, so a developer
can generate the shape, source the upstream, and reconcile the two. The pinned
commit keeps a fetch reproducible; repin with `SLDistroSource.pin(commit)`.

---

# Generating the OS source to disk — `os-generate.sleela`

`os-build.sleela` and `os-creator.sleela` *print* the generated artifacts to make
the shape visible. **`os-generate.sleela` writes them** — compiling and running
it materialises a real, buildable Linux source tree on disk through the
`osMakeDir` / `openFile` / `write` / `close` bridge, and closes the loop to the
upstream OS parts.

```sh
# Linux OS source tree with defaults (-> /projects/sandbox/os-out):
./impl/build/sleela run lib/os/os-generate.sleela

# Parameterised by the environment:
OS_NAME=Resolute OS_VERSION=2.5 OS_ARCH=arm64 OS_OUT=/tmp/myos \
  ./impl/build/sleela run lib/os/os-generate.sleela
```

Build parameters (environment, with Linux defaults): `OS_NAME` (`Determinant`),
`OS_VERSION` (`1.0`), `OS_ARCH` (`amd64`; `arm64` flips the triple to
`aarch64-unknown-linux-gnu`), `OS_OUT` (`/projects/sandbox/os-out`).

It writes a self-describing tree:

```text
<OS_OUT>/
  os/os_config.h  os/os_boot.c  os/os_image.cpp     the Linux-level C/C++ sources
  userspace/os-loader/os_loader.c                   PID 1 / service manager
  tools/gen-iso.sh   Makefile   build.sh            build tooling + ISO recipe
  upstream/DISTRO.reference                          the pinned SLDistroSource sheet
  fetch-upstream.sh                                  opt-in shallow clone of the real parts
  MANIFEST.md                                        bytes/path index of everything written
```

**The emitted C/C++ compiles** with a stock toolchain (`cc -c os/os_boot.c`,
`c++ -c os/os_image.cpp`, `cc -c userspace/os-loader/os_loader.c` all build
clean). After generating, a developer:

```sh
cd <OS_OUT>
sh build.sh          # make all -> compile the generated OS sources (-> ISO recipe)
sh fetch-upstream.sh # opt-in: shallow-clone the real Ubuntu OS parts into upstream/
```

So the full path is one program: **SLeeLa source → real Linux C/C++ on disk →
(opt-in) the sourced upstream parts beside it**, reproducible by the pinned
commit in `upstream/DISTRO.reference`.

---

# Executable file types and kernel-version selection

Two cross-OS capabilities, modeled as `.sleela` objects and emitted into the
generated C/C++ set.

## File types as executables — `SLExecutableTypes`

Each OS family decides "runnable" differently, and this object names those rules
instead of hiding them:

| Shape | Executable types | Mechanism |
|---|---|---|
| **Linux** | no-ext ELF, `.sh` `.bin` `.run` `.AppImage` `.elf` `.so` `.ko` | the exec bit (`chmod +x`) + a recognised format |
| **Windows 10+** | `.exe` `.com` `.bat` `.cmd` `.ps1` `.msi` `.vbs` `.scr` | PATHEXT association (no exec bit) |
| **macOS** | no-ext Mach-O, `.command` `.sh` `.app` `.bin` `.dylib` | exec bit for scripts/Mach-O; `.app` via `open(1)` |

```sleela
SLExecutableTypes ex = new SLExecutableTypes(); ex.configureForHost();
ex.isExecutable(".sh");            // true on Linux/macOS; false on Windows
ex.markExecutable("/path/run.sh"); // chmod +x on POSIX; no-op on Windows
ex.isHostExecutable(path, ".sh");  // exist + (exec ext OR `test -x`)
```

The OS generator emits an **`exec_types.h`** into the working C/C++ set: a
NULL-terminated table of executable extensions plus an `os_is_executable_ext()`
helper a loader uses to decide whether a file is a candidate executable. The
emitted header compiles with a stock C toolchain.

## Choosing a kernel / version — `SLKernelCatalog` + `SLKernelSpec`

`SLKernelCatalog` is the catalogue of selectable kernels and versions per OS
family; `SLKernelSpec.selectVersion(catalog, family, selection)` applies one.

| Family | Kernel | Selectable (examples) |
|---|---|---|
| **Linux** | Linux | `5.15 LTS`, `6.1 LTS`, `6.6 LTS`, `6.12 LTS`, `6.18`, `latest`, or explicit `x.y.z` |
| **Windows 10+** | NT | `10` (NT 10.0.19045), `11` (10.0.26100), `Server 2019/2022/2025`, `latest` — **pre-10 is rejected** |
| **macOS** | XNU/Darwin | `Ventura`/`13`, `Sonoma`/`14`, `Sequoia`/`15`, `latest` |

```sleela
SLKernelCatalog cat = new SLKernelCatalog();
SLKernelSpec    k   = new SLKernelSpec(); k.forShape(shape);
k.selectVersion(cat, "Linux", "6.6 LTS");   // -> Linux 6.6.0  (true)
k.selectVersion(cat, "Windows", "11");      // -> NT 10.0.26100 (true)
k.selectVersion(cat, "Windows", "XP");      // false (Windows 10+ only)
```

`os-generate.sleela` honours an `OS_KERNEL_VER` build parameter and emits the
resolved kernel into `os_config.h` as `OS_KERNEL` / `OS_KERNEL_VER`:

```sh
OS_KERNEL_VER="6.6 LTS" ./impl/build/sleela run lib/os/os-generate.sleela
# -> os/os_config.h: #define OS_KERNEL_VER "6.6.0"
```

---

# Running foreign executables — `SLExecutableTranslator`

`SLExecutableTypes` (above) says which file types are *native* to a shape.
`SLExecutableTranslator` lets the builder declare any file type runnable on a
host even when it is **foreign** — the canonical case: a **Linux host running a
Windows `.exe`**. Running a foreign type needs a translation layer, of two kinds:

| Kind | Mechanism | Example |
|---|---|---|
| **binfmt** (FS-level interpreter) | the Linux `binfmt_misc` facility: register `extension/magic → interpreter` so the filesystem hands the file to an interpreter on `exec()` | Windows PE via Wine; cross-arch ELF via QEMU-user |
| **container / compat layer** | a user-space translator that houses the execution | Wine/CrossOver (Win32), Rosetta 2 (x86-64 on Apple Silicon), WSL2 (ELF on Windows), Docker/Lima/QEMU VMs |

```sleela
SLExecutableTranslator t = new SLExecutableTranslator(); t.configure("Linux");
t.isForeign(".exe");                      // true (a Windows world on a Linux host)
t.kind(".exe");                           // "binfmt"  (Linux FS-level route)
t.translatorFor(".exe");                  // "Wine (Win32 compatibility layer)"
t.binfmtLine(".exe", "/usr/bin/wine");    // :foreign:E::exe::/usr/bin/wine:OC
t.register(".exe", "/usr/bin/wine");      // register with binfmt_misc (0 ok; needs privilege)
t.runCommand("/opt/app.exe", ".exe", "/usr/bin/wine");  // how the host launches it
```

Host coverage: **Linux** exposes `binfmt_misc`, so foreign types register at the
filesystem level (Wine for PE, QEMU-user for cross-arch). **macOS** and
**Windows** have no FS-level interpreter, so a foreign type runs through a named
container/compat layer (Wine/CrossOver, Rosetta, WSL2, or a VM). The methods are
honest: naming a translator does not install it — `register()` returns a
non-zero code where the facility is absent, and `runnableNow()` checks the
interpreter binary exists, so a missing layer is reported rather than faked.

The OS generator emits an **`exec_translate.h`** into the working C/C++ set
(`OS_HAS_BINFMT`, `OS_FOREIGN_EXE`, the `binfmt_misc` registration path), so the
generated Linux image records that it can host `.exe` via Wine and cross-arch
ELF via QEMU-user. It compiles with a stock C toolchain.

---

# Notes: what of the OS can — and cannot — be downloaded

The upstream parts (`SLDistroSource` / `SLDistroFetch`, and the generated
`fetch-upstream.sh`) pull from `mearvk/Ubuntu.Determinant.Beta.Restricted`.
Not everything comes down with a clone — `SLDistroSource.downloadabilityNote()`
records the specifics:

| Part | Downloadable? | Note |
|---|---|---|
| **Source tree** (kernels, installer, gnome-source, file-systems, userland, …) | **YES** — fully | All git-tracked source is in the repo; a shallow clone retrieves every part directory's source. |
| **Large binaries / prebuilt images** (≥ 50 MB) | **NO** — not in the repo | The distro enforces a 50 MB cap (`LFS.check.sh`) and `.gitignore`s anything larger, so big blobs are excluded and cannot be fetched from the repo. Rebuild them from the sources, or obtain them from their own release channel. |
| **GraalVM** (`graal-latest`) | **SEPARATE** download | It is a Git submodule pointing at `oracle/graal` — fetched only with a `--recurse-submodules` clone (`SLDistroFetch` `MODE_RECURSE` / `fetchComplete()`), as an additional external download. |
| **Git LFS** | n/a | The repo uses no LFS filters, so no `git lfs` step is needed; its size is plain git objects. |

In short: **all of the OS *source* can be downloaded** (shallow is enough for the
distro's own tree); **the ≥50 MB prebuilt artifacts cannot** (excluded by policy —
build them from source); and **GraalVM is a separate, opt-in submodule fetch**.

---

# Foreign-OS execution components — load, run, unload (`SLCompatLayer` / `SLCompatSet`)

`SLExecutableTranslator` (above) *classifies* which translator a foreign type
needs. `SLCompatLayer` is the **lifecycle** layer on top of it: the OS components
a host must **load** to run a guest OS's executables (and the libraries those
executables call), how it **handles** a foreign executable once loaded, and how
to **unload** the components. `SLCompatSet` is the set of such layers a built OS
ships, and composes into the master `SLOSModel`.

## The 3×3 host → guest matrix (round)

Every host/guest pair is covered — same-family is native (no layer); each
cross-pair names a real runtime + the guest's shared libraries:

| Host ↓ \ Guest → | Linux | Windows | macOS |
|---|---|---|---|
| **Linux** | native | Wine + **binfmt_misc** + win32-libs | Darling + binfmt_misc + darwin-libs |
| **Windows** | WSL2 + linux-userland | native | QEMU VM + darwin-libs |
| **macOS** | Lima VM + linux-userland | Wine/CrossOver + win32-libs | native (+ Rosetta 2 for x86-64) |

A layer loads up to three components — an FS-level **interpreter**
(`binfmt_misc`, Linux only), the translation **runtime** (Wine/QEMU/WSL/Rosetta/
Darling), and the guest **library** set — and dispatches a foreign executable, or
a **library + executable** pair, to the runtime.

```sleela
// A Linux variant running a Windows .exe (the canonical case):
SLCompatLayer w = new SLCompatLayer(); w.configure("Linux", "Windows");
w.summary();                       // "Linux runs Windows via wine + binfmt_misc + win32-libs"
w.componentManifest();             // binfmt_misc interpreter / wine runtime / win32-libs library
w.load("/usr/bin/wine");           // register binfmt + bring the runtime up (0 ok; -2 if absent)
w.handle("/opt/app.exe");          // -> the .exe (binfmt routes it) or "wine /opt/app.exe"
w.handlePair("/opt/dlls", "/opt/app.exe");  // -> "WINEDLLPATH=/opt/dlls <handle>"  (lib + exe)
w.unload();                        // unregister binfmt + drop the runtime
```

The lifecycle mirrors the codec loader (`configure → load → handle → unload`,
idempotent), and is honest: `present()`/`load()` check the runtime binary and
the `binfmt_misc` control file actually exist, so a missing component is reported
(`-1`/`-2`) rather than faked.

## In the master Sleela OS document

`SLOSModel` composes an `SLCompatSet` as an (optional) layer. `shapeAs(family,…)`
seeds the family's foreign-exec defaults — a **Linux** build ships Windows +
macOS support, a **Windows** build ships Linux (WSL2), a **macOS** build ships
Windows + Linux — and a builder refines it:

```sleela
SLOSModel os = new SLOSModel();
os.configure("Determinant", "1.0");
os.shapeAs("Linux", "amd64");          // seeds compat: runs Windows + macOS
os.compatSet().support("Windows");     // (already seeded; explicit add/drop available)
os.compatSet().drop("macOS");          // e.g. Windows-only foreign support
```

## Moved to OS Creator for real components

`SLCompatLoaderEmitter` (an OS Creator piece emitter) turns the model's
`SLCompatSet` into a real **compat-loader** OS component: `compat_loader.c` /
`compat_loader.cpp` that register each route's binfmt interpreter, load its
runtime, provide its guest libraries, and dispatch foreign executables — plus a
`compat.conf` listing the routes. `SLOSCreator` writes it under
`userspace/compat-loader/` whenever the model declares any guest. The runnable
`os-generate.sleela` emits the same piece into its on-disk tree:

```text
<OS_OUT>/userspace/compat-loader/compat_loader.c   # load/unload/run the components
<OS_OUT>/userspace/compat-loader/compat.conf        # route = Linux runs Windows via wine + ...
```

The emitted `compat_loader.c` compiles with a stock C toolchain. So the path is
complete: declare foreign-exec support on the OS model → OS Creator writes the
loader component → the built OS loads binfmt + Wine (etc.) and runs the guest's
executables and libraries.

---

# Swap (paging) — kind and quality (`SLSwapConfig`)

`SLSwapConfig` models an OS's swap/paging store: its **kind** and its **quality**.

| Kind | Meaning | Family |
|---|---|---|
| `file` | a swap file (e.g. `/swapfile`) | Linux (also macOS) |
| `partition` | a dedicated swap partition | Linux |
| `zram` | a compressed RAM block device | Linux |
| `zswap` | a compressed in-RAM cache in front of a backing swap | Linux |
| `pagefile` | `pagefile.sys` (system-managed) | Windows |
| `dynamic` | kernel-managed swap files under `/private/var/vm` | macOS |
| `none` | swap disabled | any |

**Quality** is the tunable set: size (absolute MB or a RAM multiple), swappiness
(`0..100` reclaim aggressiveness), compression algorithm + level for zram/zswap
(`lzo`/`lz4`/`zstd`, with a level knob), and swap priority.

```sleela
SLSwapConfig s = new SLSwapConfig(); s.forShape(shape);   // Linux: file @ 1.0x RAM, swappiness 60
s.setSizeMb(8192); s.setSwappiness(10);                   // 8 GB, low reclaim
print(s.emitLinux());                                      // vm.swappiness + /etc/fstab entry

SLSwapConfig z = new SLSwapConfig();
z.configure("zram", 4096, 100); z.setCompression("zstd", 3);  // 4 GB zram, zstd level 3
z.isValid();                                                   // true (compressed kind names an algo)
```

`forShape()` seeds the family default (Linux swap file, Windows pagefile, macOS
dynamic); `validForShape()` rejects a kind the family does not use (e.g. `zram`
on Windows). `emitLinux()` writes the `vm.swappiness` sysctl plus the fstab /
zram-generator / zswap-cmdline line; `emitForShape()` writes the Windows/macOS
equivalent note.

# Filesystem (file-table) types — all still in use (`SLFilesystemCatalog`)

`SLFilesystemCatalog` is the catalogue of filesystem table types a builder can
choose and validate, covering every family still in use:

| Family | Types |
|---|---|
| **FAT** | `fat12` `fat16` `fat32` `vfat` `exfat` |
| **Windows** | `ntfs` `refs` |
| **ext** | `ext2` `ext3` `ext4` |
| **Linux** | `xfs` `btrfs` `f2fs` `jfs` `reiserfs` |
| **cross-platform** | `zfs` |
| **Apple** | `apfs` `hfs+` |
| **Unix** | `ufs` |
| **optical** | `iso9660` `udf` |
| **pseudo/compressed** | `squashfs` `tmpfs` `swap` `overlayfs` |
| **network** | `nfs` `smb` `cifs` |

Per type it answers: `family`, `nativeOs`, `journaling`, `copyOnWrite`,
`caseSensitivity`, `maxFileSize` (real limits), `rootable`,
`efiSystemPartition`, `mkfsTool`, `useCase`, and `describe` (a one-line spec).
`defaultRootFor(shape)` / `rootChoicesFor(shape)` drive a format picker.

```sleela
SLFilesystemCatalog c = new SLFilesystemCatalog();
c.describe("ext4");               // ext4 [ext] native=Linux journaling=yes cow=no maxfile=16 TB rootable=yes mkfs=mkfs.ext4
c.efiSystemPartition("fat32");    // true  (an ESP must be FAT)
c.rootable("iso9660");            // false (optical media is not a writable root)
c.mkfsTool("btrfs");              // mkfs.btrfs
c.defaultRootFor("macOS");        // apfs
```

## In the master document and OS Creator

`SLOSModel` composes an `SLSwapConfig` (`swapConfig()`), seeded per shape by
`shapeAs`, and exposes `fsCatalog()` for choosing/validating the root filesystem
that `SLFilesystemLayout` formats. The runnable `os-generate.sleela` writes both
into the generated tree:

```text
<OS_OUT>/config/swap.conf          # kind + size + vm.swappiness + fstab entry
<OS_OUT>/config/filesystems.txt    # chosen root/ESP + the full catalogue of table types
```

---

# Minimum-boot base drivers, driver sourcing, and the Creator doctrine

## Base driver series — `SLBaseDriverSeries`

The irreducible drivers an OS needs to come up and be usable at first boot,
one per class, each marked `boot` (built-in / initramfs) or `early`:

| Class | Required at boot? | Linux example |
|---|---|---|
| bus (motherboard/chipset) | **yes** | `pci + acpi` |
| storage (reach the rootfs) | **yes** | `ahci + nvme + virtio_blk` |
| display / console | yes | `efifb / drm fbdev` |
| keyboard | yes | `atkbd + usbhid` |
| mouse | early | `usbhid` |
| usb host | early | `xhci_hcd` |
| audio | early (never boot) | `snd_hda_intel` |

`forShape()` seeds the series per family (Windows `.sys` drivers, macOS IOKit
families); `canBoot()` requires the irreducible trio (bus + storage + keyboard),
and `SLOSModel.isBuildable()` now enforces it.

## Driver sourcing + the cross-OS driver bridge — `SLDriverSource`

Drivers come from four origins, with a trust policy:

| Origin | Default admission |
|---|---|
| in-tree | load |
| vendor-signed | load |
| **unknown-source** | permissive → load (kernel tainted); `allowUnknown(false)` → refuse |
| **foreign-OS** | bridge-only; `allowForeign(false)` → refuse |

The **cross-OS driver bridge** adapts a driver written for another family — and
is honest about the hard kernel-ABI limits:

| Host + foreign driver | Bridge |
|---|---|
| Linux + Windows **network** (NDIS) | `ndiswrapper` |
| Linux + Windows **filesystem** | `fuse-shim` |
| Linux + Windows gpu/storage/audio | **none** (no in-kernel bridge — reported, not faked) |
| any + a device for VM passthrough | `vm-passthrough` (hosts the driver in its own OS) |

## Generated C/C++ in `/os`

The native base-driver runtime lives in `/os`:

- `os/base_drivers.h` — the driver-class enum, record, bring-up, and the source
  policy + bridge C ABI.
- `os/base_drivers.c` — `sleela_base_drivers_bringup()` (walks the series in
  class order, binds boot drivers, honours the source policy) and the
  admission/bridge logic.
- `os/base_drivers.cpp` — a typed C++ view (`sleela_os::BaseDrivers`).

`SLBaseDriverEmitter` (OS Creator piece) + `os-generate.sleela` emit the per-OS
`base_drivers_table.c` (the ordered `sleela_driver[]` series) + `base-drivers.conf`
that plug into those files. All of it compiles with a stock C/C++ toolchain.

## The Creator schema and doctrine

How these parts compose is pinned, not arbitrary:

- [`os-creator/os-creator.xsd`](os-creator/os-creator.xsd) — the normative schema:
  the ordered `identity → shape → arch → bootloader → kernel → base-drivers →
  filesystem → [swap] → packages → services → [compat] → [desktop]` sequence,
  required vs. optional layers, and the closed value sets (shapes, kernel
  families, filesystems, swap kinds, driver phases/origins, compat guests).
- [`os-creator/CREATOR.DOCTRINE.md`](os-creator/CREATOR.DOCTRINE.md) — the
  rationale: why each layer follows the one before, what may not go together,
  and the versioning rules.

### The compiler recognises the binding — by version

A Creator source declares its schema with a pragma after `#sleela`:

```java
#sleela 1.11
#schema os-creator/os-creator.xsd 1.0.0
```

On every `check`/`run`/`compile` the compiler (`resolveSchemaReference`) does
**code recognition** of this binding: it resolves the schema path (relative to
the source, then the repo root) and **errors if the schema is absent**, reads
the declared **version** and **errors if it exceeds the compiler's supported
Creator schema version** (`maxSupportedSchema()` = `1.0.0`). A source with no
`#schema` is accepted — the binding is opt-in, but once declared it is enforced.
Full structural XSD validation of an exported Creator document is done by
tooling/CI against the same XSD; the compiler guarantees reference integrity and
version compatibility at build time.

---

# Downloads and settings — resolved before compile / ISO

Before an OS is compiled or the ISO is authored, the build needs two things in
hand: **the URLs for its related downloads**, and **the specific configuration
files (its settings)**. Two layers capture these and gate the build.

## Related download URLs — `SLDownloadSet`

The external sources a build pulls from, each a kind + URL + a phase (needed
before **compile** or before the final **ISO** conversion):

| Kind | Phase | Linux default |
|---|---|---|
| package-repo | compile | `http://archive.ubuntu.com/ubuntu` |
| kernel-src | compile | `https://cdn.kernel.org/pub/linux/kernel` |
| upstream (distro) | compile | `…/Ubuntu.Determinant.Beta.Restricted.git` |
| firmware-src | iso | `…/linux-firmware.git` |
| runtime-wine | iso | `https://dl.winehq.org/wine-builds` |
| runtime-qemu | iso | `https://www.qemu.org/download` |

`forShape()` seeds the family defaults; `setPackageRepo()`/`setKernelSrc()`/…
and `addExtra(kind,url,phase)` override/extend. `readyForCompile()` requires a
package repo + kernel source; `readyForIso()` additionally requires the firmware
source. `downloadManifest()` lists every URL; `fetchScript(phase)` emits a
phase-aware fetch (git clone for repos, curl for payloads) — an explicit,
opt-in step, never run during compile.

## Configuration files (OS settings) — `SLConfigSet`

The settings checklist: which config files must be present, their path/format,
and the phase they are needed:

| File | Phase |
|---|---|
| `config/os.conf` (global), `config/cmdline.conf` (kernel cmdline) | compile |
| `config/downloads.conf` (resolved URLs), `config/filesystems.txt` | compile |
| `kernel/base-drivers/base-drivers.conf` | compile |
| `config/swap.conf`, `userspace/os-loader/services.conf` | iso |
| `userspace/compat-loader/compat.conf` (when compat is shipped) | iso |

`forModel(hasCompat)` seeds the checklist; `addExtra(path,format,phase)` adds a
developer settings file; `readyForCompile()`/`readyForIso()` are the phase
gates; `compileFilesPresent(root)` checks the required files are actually on
disk (over the `osExists` bridge). `emitOsConf(...)` writes the global header.

## The build gate

`SLOSModel` composes both (`downloadSet()`, `configSet()`), seeded by `shapeAs`,
and exposes the combined readiness gates:

```sleela
os.readyToCompile();   // isBuildable() AND downloads+settings ready for compile
os.readyForIso();      // readyToCompile() AND iso-phase downloads+settings ready
```

**`SLOSCreator.create()` now refuses unless `readyToCompile()` holds** — it will
not emit/compile a tree whose download URLs or required settings are missing —
and it writes `config/downloads.conf`, `config/config-manifest.conf`, and
`fetch-downloads.sh` alongside `config/os.conf`. The runnable
`os-generate.sleela` emits the same into its tree:

```text
<OS_OUT>/config/os.conf          # global settings (+ kernel version)
<OS_OUT>/config/cmdline.conf     # kernel command line
<OS_OUT>/config/downloads.conf   # every related download URL, by phase
<OS_OUT>/fetch-downloads.sh      # opt-in fetch of the compile/iso sources
```

So the final `gen-iso.sh` / compile step runs only once the URLs are resolved
and the OS settings are laid down.
