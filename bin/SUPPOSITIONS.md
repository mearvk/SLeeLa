# `bin/` — Suppositions

> **Status: suppositions, not a contract.** This document records a *working
> model* of what the SLeeLa `bin/` directory is supposed to hold and how it is
> supposed to behave. Items marked **(shipped)** exist today; items marked
> **(supposed)** are the intended direction and are not yet all wired. Treat the
> build system and `bin/README.md` as authoritative where they disagree.

## 1. The premise

Historically `bin/` held only the POSIX launcher scripts. The premise here
widens it: **`bin/` is the collection point for SLeeLa's runnable products** —
both

1. the **launcher/entry scripts** (shipped today), and
2. the **compiled binaries** produced when SLeeLa's C and C++ sources are built
   — whether an **individual module** is compiled, or the **whole main SLeeLa
   program** is built.

This is **in addition to**, not instead of, the normal binary-install locations
a conventional install already uses:

| Scope | Location (example) | Owner |
|---|---|---|
| User-local | `~/.local/bin`, `$XDG_BIN_HOME` | the user |
| System | `/usr/local/bin`, `/usr/bin` | root/admin |
| Repo profile | this `bin/` | the SLeeLa tree |

So a product can land in the repo's `bin/` (for in-tree use and as the launcher's
resolution target) *and* be installed to a user/system/local `bin` by the
installer. The two are complementary: `bin/` is the build/staging profile; the
profile locations are the deployment targets.

## 2. How `bin/` accepts C/C++ compilation output (supposed)

When a C/C++ build runs — a single module or the whole program — its linked
executable is **staged into `bin/`** under a stable name, so the launcher and
the OS shell can find it next to the scripts:

- **Per-module build** — building one native module (e.g. the audio bridge, the
  Ballum demo, a premium VM generation) stages that module's executable into
  `bin/` (e.g. `bin/ballum-demo`), alongside its `.pc`/headers installed
  elsewhere.
- **Whole-program build** — building the main SLeeLa program (`make -C impl`)
  produces `sleela` (and `nordshrift`, `sleelvac`); these are staged/resolved so
  `bin/SLeeLa` finds them (today via `impl/build/sleela`; supposed: a copy or
  symlink in `bin/`).
- The **launcher already resolves** a binary named `sleela` sitting alongside
  the script (`bin/sleela`), so staging compiled output into `bin/` is the
  natural, already-supported mechanism — see `bin/SLeeLa` and `bin/OSsupport`,
  which both try `"$BIN_DIR/sleela"` first.

Objects (`.o`) and static archives (`.a`) are **not** staged into `bin/` — only
final linked executables. Libraries/headers go to `lib/`/`include/` (or the
pkg-config path), per the Ballum install pattern.

## 3. Supposed binaries in `bin/`

Grounded in what the build actually produces (see each Makefile). "Source"
names the build that emits it.

### 3.1 Launcher / entry scripts — (shipped)

| Name | Kind | Source | Purpose |
|---|---|---|---|
| `SLeeLa` | POSIX `sh` | in-tree | OS-terminal launcher: run a `.sleela` object/artifact, forward toolchain subcommands, set the catalog + SHA-256 gate |
| `OSsupport` | POSIX `sh` | in-tree | OS Defender / native-support provisioning front end (`sleela defender ...`) |

### 3.2 Core toolchain binaries (whole-program C/C++ build) — (shipped as `impl/build/*`; supposed to stage into `bin/`)

| Supposed `bin/` name | Built as | Source | Purpose |
|---|---|---|---|
| `sleela` | `impl/build/sleela` | `make -C impl` | the authoritative compiler/runtime (Sleelvac™) — `check`/`run`/`compile`/`native`/`vm`/… |
| `nordshrift` | `impl/build/nordshrift` | `make -C impl` | the Nordshrift `.sst` round-trip tool |
| `sleelvac` | `impl/build/sleelvac` (`SLEELAVC_BIN`) | `make -C impl` | the Sleelvac driver variant |
| `sleela.exe` / `nordshrift.exe` | MinGW build | `build-windows.ps1` | the Windows variants (the launcher already probes `.exe`) |

### 3.3 Per-module native products — (supposed to stage into `bin/` on build)

| Supposed `bin/` name | Built as | Source | Purpose |
|---|---|---|---|
| `ballum-demo` | `ballum/build/ballum-demo` | `make -C ballum demo` | linked Ballum display demo (links the OS display libs — X11/Xrandr, Cocoa/CoreGraphics, or gdi32) |
| `slvm-01` … `slvm-11` | `sleela-virtual-machine/0NN/vm-build/slvm-NN` | `make premium-vms` | the eleven premium SLVM generation runners |
| `sleela-audio-native` | audio build | `make -C audio` (native) | native audio bridge runner |
| `sleela-gov-native` | gov build | governance native build | native governance runner |
| `regex-*` | `regex/build/*` | `make -C regex` | regex engine C/C++ self-test/demo runners |

### 3.4 OS Defender native backends (compiled C/C++, per host OS) — (supposed; provisioned by `OSsupport`)

These are the privileged native-support artifacts `OSsupport`/`sleela defender`
fetches, builds, and installs. They are OS-native driver/module products, so
they install into the OS's own privileged location rather than a plain `bin/`,
but they are catalogued here as SLeeLa products of a C/C++ build.

| Host OS | Backend product | Build | Install target |
|---|---|---|---|
| Windows 10+ | `Windows.Admin.Defender` (signed WDK driver) | WDK/MSVC | driver store (elevated) |
| Linux | `Linux.Admin.Defender` (kernel module) | `make install` | kernel module path (privileged) |
| macOS | `MacOS.Admin.Defender` | — | **detect only**; no privileged backend |

## 4. Resolution order (how the launcher finds a product) — (shipped)

`bin/SLeeLa` and `bin/OSsupport` resolve the `sleela` binary in this order:

1. `SLEELA_BIN` (explicit override), if executable;
2. `bin/sleela` / `bin/sleela.exe` (a staged copy next to the script);
3. `impl/build/sleela` / `impl/build/sleela.exe` (the in-tree build).

This is exactly why staging compiled output into `bin/` (step 2) is the
supposed mechanism: it makes a freshly built product the one the launcher runs,
without touching the user/system install.

## 5. Per-OS linking note

Compiled products that touch the display or the OS integrate the OS-centric
libraries at link time (see `ballum/` and `GROFF.md`-adjacent build notes):

- **Linux** — `-lX11 -lXrandr -lm` (and friends), dev packages via each
  module's `deps` target.
- **macOS** — `-framework CoreGraphics -framework ApplicationServices -framework
  Foundation` (and Cocoa/CoreText for the UI), shipping with the Xcode CLT.
- **Windows** — `-lgdi32 -luser32` (and `-lshell32`), shipping with the SDK/MinGW.

The `.pc` (pkg-config) manifests that modules install record these in
`Libs.private`, so a downstream link of a staged `bin/` product picks them up.

## 6. Open questions (to resolve before promoting any of this to a contract)

- Does `make install` **copy** core binaries into the repo `bin/`, or does `bin/`
  only ever hold scripts + a symlink to `impl/build/`? (Today: resolution, not
  copy.)
- Should per-module `demo`/runner products auto-stage into `bin/`, or stay in
  their module `build/` and only be staged on an explicit `make stage-bin`?
- Naming: do Windows `.exe` products and POSIX products share one catalog entry
  (as above) or separate ones?

---

*Suppositions recorded for the `bin/` profile. See `bin/README.md` for the
shipped launcher behavior and each module's Makefile for what it actually
builds.*
