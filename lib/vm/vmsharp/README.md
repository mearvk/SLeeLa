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
