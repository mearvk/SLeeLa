# Virtual Machines & Modium — a hands-on user guide

*Idea System(TM) / Idea(TM). From Author; distributive.*

A short, try-it guide to two terminal features:

- **`sleela vm`** — see and select which virtual machine you run on.
- **`sleela modium`** — scan a VM's files for resolution (that they exist, are
  where they belong, and are intact), with an exact-reconstitution database.

> You do not need either to use SLeeLa. By default every command runs on the
> **Native VM** (`impl/core`). These are here when you want to inspect or pick a
> different VM build, or verify a VM's files.

## Before you start

Build the toolchain once and point the integrity gate at the manifest:

```sh
make -C impl all
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
alias sleela="$PWD/impl/build/sleela"
```

(On a prebuilt tree you can use `working-binaries/sleela` instead.)

---

## 1. `sleela vm` — choose your virtual machine

The Native VM is the default and runs everything unless you pick another. The
ten versioned VMs (`idea-vm-001`..`010`) are a think-tank progression that adds
one user-level improvement each step (memory hardening, typed results,
telemetry, GC quality, a dispatch profiler, capability enforcement, platform
parity, an audit trail, and a consolidated runtime).

**List what's available:**

```sh
sleela vm list
```
```
Available virtual machines (the Native VM is the default):
  native   Native VM (impl/core) — used unless you select another   [default]
  1  idea-vm-001  [ground floor]        -> virtual-machine/vm-version/001/vm_ground_test
  2  idea-vm-002  [memory hardening]    -> virtual-machine/vm-version/002/vm_memory_test
  ...
  10 idea-vm-010  [consolidated]        -> virtual-machine/vm-version/010/vm_runtime
```

**Look at one in detail:**

```sh
sleela vm info 10
```

**Select one (or stay native):**

```sh
sleela vm select native     # the default; nothing to load
sleela vm select 10         # resolves idea-vm-010 and names its native main
```

> What selection does today: it **resolves and reports** the choice (validated
> the same way the VM's `OP_VM_SELECT` opcode validates it). Actually building
> and booting a non-native VM's compiled `main` is a host/build step beyond the
> shipped `sleela` binary — the command tells you so. Start on the Native VM.

---

## 2. `sleela modium` — scan a VM's files for resolution

Modium checks that each file of a VM version **exists**, sits **where it should**
(recorded relative to itself, its parent, and the whole folder path that led to
it), and is **intact** by SHA-256. For exact recovery, each version ships a
MySQL `rebuild.sql` carrying every file's content.

**Scan one version:**

```sh
sleela modium scan 1
```
```
idea-vm-1: 80/80 ok  [sha256]
```

**Scan everything, or verify bytes exactly:**

```sh
sleela modium status            # summary across all ten versions
sleela modium scan 10 --exact   # SHA-256 + exact byte re-read
```

**What a problem looks like** (if a file were changed or deleted):

```
  CORRUPT  VERSION.md (sha mismatch)
idea-vm-2: 81/82 ok corrupt=1  [sha256]
```

**Restore (explicit, never automatic):**

```sh
sleela modium restore 2
```
Modium **reports what is broken first** and never overwrites a file on its own.
To reconstitute, either:
- restore the file from git (git's object store is a hash-verified database):
  `git checkout -- <path>`; or
- load that version's exact content from its database:
  `mysql <db> < virtual-machine/vm-version/002/rebuild.sql`
  and write back the recorded content for the flagged files.

### The records (what modium reads)

Each `virtual-machine/vm-version/NNN/` folder carries two generated, idempotent
artifacts:

| File | Purpose |
|------|---------|
| `MODIUM.json` | the hash-scan manifest (path · name · parent · ancestry · depth · sha256 · size for **every** file) |
| `rebuild.sql` | a portable MySQL script carrying each file's content for **exact** reconstitution |

Regenerate them after you intentionally change a version's files:

```sh
python3 tools/generate-modium.py            # all versions
python3 tools/generate-modium.py --check    # verify only (CI)
```

---

## How this differs from the build gate

SLeeLa already has a SHA-256 **build gate** (`verify-before-execution.py`) that
protects the compiler's own sources before build/run. **Modium is different in
scope:** it is a *per-VM-version file-resolution* scan a VM runs for itself, with
an exact-reconstitution database. Use the build gate for "is the toolchain
trustworthy to run"; use modium for "are this VM version's files all present and
intact."

## Safety notes

- `sleela modium restore` **never** rewrites files automatically — it reports,
  then you act.
- `sleela vm select` on a non-native VM does not silently execute anything; it
  resolves the choice and tells you what a host load would entail.
- The Native VM remains the default and is the recommended starting point.
