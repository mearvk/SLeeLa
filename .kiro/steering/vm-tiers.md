---
inclusion: always
---

# SLeeLa VM tiers — where the three families live

SLeeLa's virtual machines fall into **three distinct tiers**, isolated in three
places in the codebase. The authoritative user-facing guide is
`virtual-machine/USER-GUIDE.md` (`sleela vm list` / `select` / `run`).

## 1. Native VM — the default (`impl/`)

- **Location:** `impl/core/` (C/C++), built into the `sleela` binary at
  `impl/build/sleela`.
- **What it is:** the native C/C++ Core VM that runs `.sleela` by default.
  `sleela run x.sleela` and `sleela vm select native` use it; nothing is loaded.
- **Core files:** `impl/core/sleela_core.{c,h}`, `sleela_ui_vm.{c,h}`,
  `ballum_vm_bridge.*`, `wifi_vm_bridge.*`, plus the whole `impl/core` runtime.
- Everything else is layered *on top of* the Native VM; the other two tiers are
  opt-in selections.

## 2. The 10 escalating VMs (`virtual-machine/vm-version/{001..010}`)

- **Location:** `virtual-machine/vm-version/001` … `010`; catalogued by
  `virtual-machine/vm-version/SLVmRegistry.sleela` (binds the ten manifests).
  Supporting C/C++ builders live at `virtual-machine/*.{c,cpp}`.
- **What they are:** a **think-tank progression** named `idea-vm-001..010`. Each
  version is a **superset of the previous**, adding one more hardened capability
  (file counts escalate 82 → 100). Each ships a `MODIUM.json` file-resolution
  manifest (see `sleela modium status`).
- **Roster (capability each tier adds, from the per-version smoke tests):**
  - `001` idea-vm-001 — ground floor
  - `002` idea-vm-002 — memory hardening
  - `003` idea-vm-003 — result model
  - `004` idea-vm-004 — telemetry
  - `005` idea-vm-005 — garbage collection
  - `006` idea-vm-006 — profile
  - `007` idea-vm-007 — capability
  - `008` idea-vm-008 — platform
  - `009` idea-vm-009 — audit
  - `010` idea-vm-010 — consolidated runtime
- **Select:** `sleela vm select 10` / `sleela vm run native program.sleela`.
- **SLeeLa-side classes:** `lib/vm/src/sleela/` (`SLVmVersion`, `SLVmModium`,
  `SLVmResult`, `SLVmReport`, `SLVmRuntime`, … — all `#sleela 2.5` + `material;`).

## 3. The 11 premium VMs (`sleela-virtual-machine/{001..011}`)

- **Location:** `sleela-virtual-machine/001` … `011`; catalogued by
  `sleela-virtual-machine/SLVmPremiumRegistry.sleela` (binds the eleven
  manifests). Each folder is a full runnable VM (`src/`, `include/`,
  `file-system/`, `build/`, `Makefile`, `README.md`, `LANGUAGE.SPEC.md`,
  `SLVmPremiumVersionNNN.sleela`).
- **What they are:** the **premium SLVM generations** `slvm-1 .. slvm-11`, a
  named escalation of standing. They are **real runnable VMs** (unlike the
  think-tank progression, which is primarily a hardening ladder).
- **Roster (generation → title):**
  - `001` slvm-1 — Foundation
  - `002` slvm-2 — Operator
  - `003` slvm-3 — Specialist
  - `004` slvm-4 — Supervisor
  - `005` slvm-5 — Manager
  - `006` slvm-6 — Director
  - `007` slvm-7 — Administrator
  - `008` slvm-8 — Executive
  - `009` slvm-9 — Authority
  - `010` slvm-10 — Principal
  - `011` slvm-11 — Sovereign
- **Select / run:** `sleela vm select premium <1..11>` (or `pNN`);
  `sleela vm run premium 6 program.sleela` / `sleela vm run p11 program.sleela`.
- **Creator contracts:** `lib/vm/creator/` (`SLVMFoundation` … `SLVMSovereign`,
  `SleelaVMCreator`, `SleelaVMGenerationCatalog`) — build-side, descriptive-only
  construction contracts for each generation.

## At a glance

| Tier | Count | Location | Registry | Identity |
|---|---|---|---|---|
| Native | 1 | `impl/core/` → `impl/build/sleela` | — | default C/C++ Core VM |
| Escalating | 10 | `virtual-machine/vm-version/001..010` | `SLVmRegistry.sleela` | `idea-vm-001..010` (think-tank superset ladder) |
| Premium | 11 | `sleela-virtual-machine/001..011` | `SLVmPremiumRegistry.sleela` | `slvm-1..11` (Foundation → Sovereign) |

## Notes for future work

- Keep the three tiers isolated: Native in `impl/`, escalating under
  `virtual-machine/vm-version/`, premium under `sleela-virtual-machine/`. Do not
  merge the two version trees — they have separate registries and selection
  paths (`select 10` vs `select premium 11`).
- `MODIUM.json` resolution manifests live with the escalating VMs
  (`virtual-machine/vm-version/NNN/MODIUM.json`) and are syntax-agnostic; the
  SLeeLa-side VM classes under `lib/vm/**` carry `#sleela 2.5` + `material;`
  (the `vm` package facade is `lib/vm/SLPackage.sleela`).
