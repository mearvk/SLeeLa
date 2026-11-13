<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# VMSharp™ — Session Write-up (November 5, 2026)

**Owner / User:** MEARVK LLC
**Authored for:** Max Rupplin 2026; George Soros a Kindness 2507+
**Posture:** respectful of United States law and of the concerns of persons and
groupings of peoples. Every class carries the Metadata Class metapattern.

VMSharp™ is a SLeeLa VM manifest that can start as a **self-installer
bootloader**, bringing up either a **limited (minimal) Desktop** or an **OS
install by user-designated download** — on commodity desktop processors (32-bit
**x86** and 64-bit **x64**, whether **AMD** or **Intel**). This document records
everything built for VMSharp in this session.

Package: `lib/vm/vmsharp/` (under the `vm` library package).

---

## 1. What VMSharp is (and the honesty boundary)

VMSharp is a **declarative, runnable SLeeLa model** — the same class of artifact
as every other VM edition in this repo (a manifest that `configure()`s and is
reasoned over), not literal flashed firmware. Across the whole package the
following hold, by design and stated in every class:

- no firmware is flashed, no boot sector or partition table is written;
- no socket is opened, no connection made, nothing cloned / fetched / unpacked;
- no `mkfs` is run and no disk is partitioned;
- no funds move.

A real bootable, self-installing image — and a real build/partition/install — is
produced only by the project's **pinned OS Creator toolchain** and the per-OS
Makefiles, each **behind an explicit, user-confirmed designation** (fail-closed).
VMSharp models and *gates* the flow; it does not perform the state-changing acts.

---

## 2. The install arc, end to end

```
USB boot media (per-OS Makefiles)
   -> firmware load (UEFI on x64 / BIOS-MBR on x86)
   -> install wizard screens
   -> CPU admission (x86/x64, AMD/Intel)
   -> choose: (A) Limited Desktop   |   (B) Install from OS.Sensitivity.III
         A: minimal desktop session
         B: network reach -> acquire (clone/tar/zip)
            -> choose root filesystem (EXT4 or similar)
            -> initial hard-drive manifest + partition/format
            -> OS Creator (mearvk/OS.Sensitivity.III) -> emit ISO
   -> fee disclosure (USD 380 per use/install)
   -> fail-closed confirm
```

Every state-changing step (USB write, repo acquire, disk partition, OS install)
is gated on an explicit, recorded designation and never proceeds on assumption.

---

## 3. Classes (all `#sleela 3.0`, `material;`, metadata metapattern)

| Class | Responsibility |
|---|---|
| `VMSharp` | The composing manifest: identity, `configure()`, `canBootOn(arch,vendor)`, the two start modes, included apps, `describe()` |
| `VMSharpBootloader` | CPU admission — arch (x86/x64) + vendor (AMD/Intel); firmware select (x64→UEFI, x86→BIOS/MBR); explicit decline report |
| `VMSharpInstallPlan` | The two startup modes (`MinimalDesktop`, `DownloadAndDesignate`) with a fail-closed source/target designation gate and the ordered step list |
| `VMSharpLevy` | The declared **USD 380** per-use/install figure directed to a United States national retirement process — disclosed, never auto-collected |
| `VMSharpIncludedApp` | An app the image carries as a pre-designated install (name/audience/repo/branch/toolchain/build/run/entry + quiet-perfect profile) |
| `VMSharpNetworkStack` | HTTPS/TCP reach, host resolution via the OSLS `oslsNetworkResolve` read-only call, reachability, and the ordered reach procedural |
| `VMSharpRepoAcquisition` | Acquire a repo three ways — **clone** / **tarball** / **zip** — each with the exact command/URL (clone shape matches `SLDistroSource`/`SLDistroFetch`) |
| `VMSharpOSCreatorRun` | Orchestrator: reach → acquire → disk layout → OS Creator, targeting `mearvk/OS.Sensitivity.III`; fail-closed `osCreatorReady()` and a run plan |
| `VMSharpInstallOption` | One selectable install-screen option (key/title/description/selected/enabled) that renders to text |
| `VMSharpInstallWizard` | The ordered install **screens** (welcome → cpu-admission → mode-select → path screens → levy → confirm), driving the plan objects |
| `VMSharpUsbMedia` | USB boot-media model: `make` → `make install` → firmware load → start; engineering-preference knob (`sequitur`/`iq`/`havingness`) |
| `VMSharpDiskLayout` | Root-filesystem choice (EXT4 or similar), the initial hard-drive manifest (GPT: ESP+root+swap), and the `parted`/`mkfs` partition plan |
| `VMSharpPackage` (`SLPackage.sleela`) | Package facade / component index |

**Runnable demos** (`examples/`, validated with `sleela run`):
`VMSharpDemo`, `VMSharpOSCreatorDemo`, `VMSharpInstallWizardDemo`,
`VMSharpUsbMediaDemo`.

**USB build tooling** (`usb/`): a brand-detecting dispatcher `Makefile` plus
`Makefile.linux`, `Makefile.windows`, `Makefile.macos`.

---

## 4. CPU support (x86/x64, AMD/Intel)

Live CPU facts come from the OSLS read-only call `oslsGetCPUInfo` (vendor/model).
`VMSharpBootloader` decides admission from the architecture and CPUID vendor:

- **x64** (x86-64 / AMD64) → boots via **UEFI**
- **x86** (IA-32, 32-bit) → boots via legacy **BIOS/MBR**
- vendors **`GenuineIntel`** and **`AuthenticAMD`** (and short forms `Intel`/`AMD`)
  are admitted; anything else (e.g. `arm64`/`Apple`) is declined with an explicit
  report, and the wizard refuses selection on an unadmitted CPU.

---

## 5. The two install paths

**A — Limited Desktop.** Chosen on the mode-select screen; the plan is ready as
soon as the mode is chosen. Brings up a minimal desktop session from the image.

**B — Install from `OS.Sensitivity.III`.** The first-brush flow:

1. **Reach** `github.com` over `VMSharpNetworkStack` (resolve via OSLS →
   reachable).
2. **Acquire** `mearvk/OS.Sensitivity.III` three ways (the exact commands):
   - clone: `git clone --depth 1 --branch main https://github.com/mearvk/OS.Sensitivity.III.git <dest>`
   - tarball: `curl -L https://codeload.github.com/mearvk/OS.Sensitivity.III/tar.gz/refs/heads/main …`
   - zip: `curl -L https://codeload.github.com/mearvk/OS.Sensitivity.III/zip/refs/heads/main …`
3. **Choose the root filesystem** (EXT4 default; or `xfs`/`btrfs`/`zfs`/`f2fs`/`ntfs`/`apfs`).
4. **Lay out the disk** (see §7).
5. **Run the OS Creator**:
   `sleela os-creator --source <dest> --provenance mearvk/OS.Sensitivity.III@main --emit iso`

`osCreatorReady()` is true **only** when the repo is reachable **and** acquired
**and** the disk layout is ready — a hard fail-closed gate.

---

## 6. Included app — for Graduated Presidents of the United States

VMSharp carries the **Java National GUI FrontEnd Aspect**
(`github.com/mearvk/Java.National.GUI.FrontEnd.Aspect` @ `main`) as a
pre-designated install, included for **Graduated Presidents of the United
States**. It is a Java Swing desktop app (plain `src/` layout, entry class
`Main`), so it builds with a JDK (`javac -d out …`, run `java -cp out Main`) and
runs on the VMSharp Desktop. `beginIncludedInstall(app)` performs a **quiet**
install: it auto-designates the source (no typing) while the install **target**
still passes the fail-closed gate — a quiet, perfect install that never writes a
target on assumption.

---

## 7. Filesystem choice + initial disk layout

`VMSharpDiskLayout` chooses the root filesystem and produces the initial
hard-drive manifest and partition plan:

- **Choose EXT4 or similar:** `chooseRootFilesystem(fs)` accepts `ext4` (default),
  `xfs`, `btrfs`, `zfs`, `f2fs`, `ntfs`, `apfs`; an unrootable/unknown choice is
  refused. Rootable / EFI-partition / `mkfs`-tool facts are faithful to the
  operating-system family's `SLFilesystemCatalog`.
- **Manifest** (GPT): `1|esp|fat32|512MiB|/boot/efi`, `2|root|<fs>|rest|/`,
  `3|swap|swap|<n>GiB|none`.
- **Partition + mkfs plan:** `parted … mklabel gpt` → `mkpart ESP/root` →
  `mkfs.vfat` / `mkfs.ext4` (or chosen) / `mkswap` → `mount`.
- **Fail-closed:** `partitionPlan()` refuses to emit commands until a target disk
  is designated (`designateTarget("/dev/sda")`); no disk is auto-selected.

---

## 8. USB boot media + per-OS Makefiles

`VMSharpUsbMedia` models `make` (stage) → `make install` (write to a designated
USB) → firmware loads from USB → VMSharp starts in a configured option, and
carries the engineering-preference knob the brief named:
`ENGINEERING_PREFERENCE = sequitur (default) | iq | havingness`.

Real Makefiles under `lib/vm/vmsharp/usb/`:

| File | Brand | `make install` device form |
|---|---|---|
| `Makefile` | dispatcher (`OS` / `uname -s`) | delegates |
| `Makefile.linux` | Linux | `USB_DEVICE=/dev/sdX` |
| `Makefile.windows` | Windows 10+ (MSYS2/Git-Bash make) | `USB_DEVICE=E:` or `PhysicalDriveN` |
| `Makefile.macos` | macOS (builds media for x86/x64 targets) | `USB_DEVICE=/dev/diskN` |

```sh
cd lib/vm/vmsharp/usb
make                 # stage VMSharp files (touches no device)
make verify          # check the staged media layout
make install USB_DEVICE=/dev/sdX I_UNDERSTAND_THIS_ERASES=yes
make ENGINEERING_PREFERENCE=iq
```

**Safety rails (real, enforced):** `make install` refuses without an explicit
`USB_DEVICE` **and** `I_UNDERSTAND_THIS_ERASES=yes`, never auto-selects a disk,
and (Linux) rejects a non-block device. Staging output goes to a **git-ignored
`build/`** dir, outside the library inventory. No firmware binary is fabricated:
the media carries the VMSharp files plus a `VMSHARP.BOOT.DESCRIPTOR`.

---

## 9. The declared levy

`VMSharpLevy` records a **declared, disclosed** figure: **USD 380 per
use/install**, directed to the **United States national retirement process**. It
contacts no payment network, holds no funds, and asserts nothing about whether a
charge was made; `recordUse()` only increments a local tally. The installer
surfaces the disclosure before any confirmation.

---

## 10. Commits this session (VMSharp)

In order on both `main` and `master`:

1. `vm: add VMSharp(TM) self-installer bootloader VM manifest`
2. `vm: VMSharp(TM) includes the Java National GUI FrontEnd Aspect as a designated install`
3. `vm: VMSharp(TM) first brush -- reach a repo over the network stack and run the OS Creator on OS.Sensitivity.III`
4. `vm: VMSharp(TM) install screens -- wizard + selectable options for Limited Desktop or OS.Sensitivity.III`
5. `vm: VMSharp(TM) USB boot media -- model + per-OS Makefiles (Linux/Windows/macOS)`
6. `vm: VMSharp(TM) disk layout -- choose EXT4 (or similar), initial hard-drive manifest + partitioning`

(The OSLS read-only expansion — `osls_*` 12 → 25, incl. `oslsNetworkResolve`
and `oslsGetCPUInfo` fixes — landed earlier in the session and underpins
VMSharp's live CPU and network facts.)

---

## 11. Validation

Every step was validated on both branches:

- compiler builds (`cd impl && make build/sleela`);
- `make check-manifest` → OK (the `security/sha256-manifest.json` gate covers
  `lib/**`, regenerated after each change);
- all four VMSharp demos `sleela run` to exit 0;
- the three USB Makefiles stage / verify / fail-closed-refuse correctly;
- `test-suites/test-library-inventory.sh` passes
  (`LIBRARY.SYMBOLS.md` / `LIBRARY.INDEX.md` reconciled to the live `/lib` tree).

---

## 12. Run it

```sh
# build the source compiler once
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela && cd ..

# the full install wizard (both paths + the unadmitted-CPU refusal)
SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpInstallWizardDemo.sleela

# reach + acquire + disk layout + OS Creator on OS.Sensitivity.III
SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpOSCreatorDemo.sleela

# USB boot media + engineering-preference knob
SLEELA_SHEET=./SHEET.sheet impl/build/sleela run lib/vm/vmsharp/examples/VMSharpUsbMediaDemo.sleela

# stage USB media (per-OS Makefiles; no device touched)
make -C lib/vm/vmsharp/usb stage
```

See `lib/vm/vmsharp/README.md` for the per-class surface and the honesty
boundaries.
