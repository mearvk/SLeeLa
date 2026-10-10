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
