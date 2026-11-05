# VMSharp™

VMSharp™ is a SLeeLa VM manifest that can start as a **self-installer
bootloader**, bringing up either a **Minimal Desktop** or an **OS install by
user-designated download**. It targets commodity desktop processors — 32-bit
**x86** and 64-bit **x64**, whether **AMD** or **Intel**.

Authored for the User **MEARVK LLC** (Max Rupplin 2026; George Soros a Kindness
2507+). Respectful of United States law and of the concerns of persons and
groupings of peoples.

## Classes

| Class | Responsibility |
|---|---|
| `VMSharp` | The composing manifest: identity, `configure()`, `canBootOn()`, the two start modes, `describe()` |
| `VMSharpBootloader` | CPU admission — `supportsArch` (x86/x64), `supportsVendor` (AMD/Intel), `admits`, and `firmwareFor` (x64 → UEFI, x86 → BIOS/MBR) |
| `VMSharpInstallPlan` | The two startup modes (`MinimalDesktop`, `DownloadAndDesignate`), the fail-closed designation gate, and the ordered step list |
| `VMSharpLevy` | The declared **USD 380** per-use/install figure directed to a United States national retirement process |
| `VMSharpPackage` | Package facade / component index |
| `examples/VMSharpDemo` | Runnable demonstration (`sleela run`) |

## CPU support

Live CPU facts come from the OSLS read-only call `oslsGetCPUInfo` (vendor/model).
`VMSharpBootloader` decides admission from the architecture and CPUID vendor:

- **x64** (x86-64 / AMD64) → boots via **UEFI**
- **x86** (IA-32, 32-bit) → boots via legacy **BIOS/MBR**
- vendors **`GenuineIntel`** and **`AuthenticAMD`** (and the short forms `Intel`/`AMD`) are admitted; anything else is declined with an explicit report.

## Startup modes

1. **Minimal Desktop** — ready as soon as the mode is chosen; brings up a
   minimal desktop session from the image.
2. **Download and Designate** — fetches an OS image the user **designates** by
   URL, then installs it to a target the user **designates**. The plan is
   `isReady()` **only** after both the source and target are explicitly recorded
   and confirmed. An install never proceeds on assumption.

## The declared levy

`VMSharpLevy` records a **declared** figure: **USD 380 per use/install**,
directed to the **United States national retirement process**. It is a disclosure,
not a collection: the class contacts no payment network, holds no funds, and
asserts nothing about whether any charge was actually made. `recordUse()` only
increments a local tally a caller keeps for its own reporting.

## Included apps

VMSharp can carry applications as **pre-designated installs** so they come up on
the VMSharp Desktop without the user typing a source. Each is a
`VMSharpIncludedApp` record faithful to its real upstream project — repo, branch,
toolchain, build command, run command, entry point, and audience — plus a
**quiet, perfect install** profile (unattended once confirmed; deterministic,
all-or-nothing, fails closed).

Included today:

| App | Audience | Source | Build / Run |
|---|---|---|---|
| **Java National GUI FrontEnd Aspect** | Graduated Presidents of the United States | [`mearvk/Java.National.GUI.FrontEnd.Aspect`](https://github.com/mearvk/Java.National.GUI.FrontEnd.Aspect) @ `main` | JDK: `javac -d out $(find src -name *.java)` → `java -cp out Main` |

`VMSharp.beginIncludedInstall(app)` chooses `DownloadAndDesignate` mode and
auto-records the app's source designation (the quiet part). The install **target**
is still designated and confirmed through the plan's fail-closed gate, so even a
quiet install never writes a target on assumption. The Java National GUI app is a
Swing desktop app, so it builds with a JDK and runs on the VMSharp Desktop.

## Reaching a repo and running the OS Creator (first brush)

VMSharp can reach a GitHub repository over its networking stack, acquire it, and
run the project's **OS Creator** pointed at that repo for installation
instructions, data, and code. Three classes model this path:

| Class | Responsibility |
|---|---|
| `VMSharpNetworkStack` | The transport (HTTPS/TCP/IP, port 443), host resolution (via the OSLS `oslsNetworkResolve` read-only call), reachability, and the ordered reach procedural (resolve → connect → TLS → request → receive) |
| `VMSharpRepoAcquisition` | Acquire a repo three ways — **clone** (`git clone --depth 1`), **tarball** (`codeload .../tar.gz/refs/heads/<branch>`), **zip** (`codeload .../zip/refs/heads/<branch>`); produces the exact command/URL. Clone shape matches the operating-system family's `SLDistroSource`/`SLDistroFetch` |
| `VMSharpOSCreatorRun` | Wires network reach → acquisition → OS Creator, targeting [`mearvk/OS.Sensitivity.III`](https://github.com/mearvk/OS.Sensitivity.III). Emits the acquisition command, a provenance line, the `sleela os-creator --source … --emit iso` invocation, and a 9-step run plan |

**Fail-closed gate:** `VMSharpOSCreatorRun.osCreatorReady()` is true only after the
repo is **reachable** *and* **acquired** — the OS Creator never runs on an
unreached or unacquired source. `markAcquired()` itself requires reachability
first.

Run the first-brush demo:

    SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpOSCreatorDemo.sleela

## Install screens (the wizard)

`VMSharpInstallWizard` is the finish-level set of install **screens** that lead
the user to either install VMSharp as a **limited Desktop** or install an OS
**from `OS.Sensitivity.III`**. Each screen renders to text; the two primary
choices are selectable `VMSharpInstallOption` components.

| Screen | Content |
|---|---|
| 1. welcome | Title + owner |
| 2. cpu-admission | x86/x64 · AMD/Intel admission report — **gates the rest** |
| 3. mode-select | `(A)` Limited Desktop · `(B)` Install from OS.Sensitivity.III |
| 4a. desktop | (path A) mode, step count, readiness |
| 4b. os-sensitivity | (path B) repo, reachability, clone command, OS Creator command + provenance |
| 5. levy-disclosure | the USD 380 fee disclosure |
| 6. confirm | fail-closed point of user confirmation |

**Fail-closed throughout:** `choose()` refuses any selection unless the CPU is
admitted; `canConfirm()` is true only when the CPU is admitted **and** the chosen
path's plan is ready — Limited Desktop once chosen, and the OS.Sensitivity.III
path only when the repo is reachable **and** acquired **and** the install target
is designated. Choosing path B pre-designates the source (the repo itself); only
the target remains for the user.

Run the wizard demo (walks both paths + the unadmitted-CPU refusal):

    SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpInstallWizardDemo.sleela

## USB boot media (`make` / `make install`)

VMSharp can initialize from a **USB device**. `VMSharpUsbMedia` models the flow
— `make` stages the VMSharp files, `make install` writes them to a USB you
designate, the computer's firmware (UEFI on x64, BIOS/MBR on x86) loads them,
and VMSharp starts in either configured option — and carries the
engineering-preference knob (`sequitur` | `iq` | `havingness`).

The real build tooling lives in `usb/`, with a Makefile for each OS brand:

| File | Brand | `make install` device form |
|---|---|---|
| `usb/Makefile` | dispatcher (detects host via `OS`/`uname -s`) | delegates |
| `usb/Makefile.linux` | Linux | `USB_DEVICE=/dev/sdX` |
| `usb/Makefile.windows` | Windows 10+ (MSYS2/Git-Bash `make`) | `USB_DEVICE=E:` or `PhysicalDriveN` |
| `usb/Makefile.macos` | macOS (builds media for x86/x64 targets) | `USB_DEVICE=/dev/diskN` |

```sh
cd lib/vm/vmsharp/usb
make                 # stage VMSharp files (touches no device)
make verify          # check the staged media layout
make install USB_DEVICE=/dev/sdX I_UNDERSTAND_THIS_ERASES=yes
make ENGINEERING_PREFERENCE=iq     # sequitur (default) | iq | havingness
```

**Safety rails (real, enforced):** `make install` **refuses** to run without an
explicit `USB_DEVICE` and the `I_UNDERSTAND_THIS_ERASES=yes` confirmation, never
auto-selects a disk, and (on Linux) rejects a non-block device — so the
destructive write is only ever reached deliberately. The staging output goes to
`build/` (git-ignored) and is **not** part of the library inventory.

**No fabricated firmware:** VMSharp is a SLeeLa model, so the media carries the
VMSharp files plus a `VMSHARP.BOOT.DESCRIPTOR`, not an invented kernel/bootloader
binary. A real bootable VMSharp image is produced only by the pinned OS Creator
toolchain.

## Honesty boundary

Every class here is a **declarative model**. VMSharp source:

- flashes **no** firmware and writes **no** boot sector or partition table;
- performs **no** download, disk write, partitioning, or install;
- opens **no** socket, makes **no** connection, and clones/fetches/unpacks **nothing**;
- moves **no** funds.

A real bootable, self-installing image is produced only by the project's pinned
**OS Creator** toolchain, not by these source classes. OS installation is a
state-changing operation, kept behind an explicit user designation — mirroring
the OSLS boundary that keeps state-changing work off the read-only path.

## Run it

    SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpDemo.sleela
