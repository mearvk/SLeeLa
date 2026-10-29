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
sleela vm select 10         # a standard think-tank version (idea-vm-010)
sleela vm select premium 6  # a premium SLVM generation (slvm-6); or shorthand: p6
```

**Run a program on a premium VM** (premium generations are real runnable VMs):

```sh
sleela vm run premium 6 program.sleela   # boot slvm-6 and run program.sleela on it
sleela vm run p11 program.sleela         # shorthand for premium 11
sleela vm run native program.sleela      # same as plain `sleela run` (Native VM)
```

### Standard vs. premium VMs

- **Standard** (`1..10`) — the think-tank versions under
  `virtual-machine/vm-version/NNN`: a layered progression of one user-level
  improvement each.
- **Premium** (`premium 1..11`) — the SLVM generations under
  `sleela-virtual-machine/NNN`. Each is a **real, runnable virtual machine**: it
  links the shared native Core engine (archived as `impl/build/libslcore.a`) and
  runs compiled `.sleela` artifacts. The grades **differ in source**: each grade
  compiles and calls its own distinct feature validator at admission — e.g.
  grade 1 a security supervisor, 2 a crypto provider, 3 an isolation policy, 4
  authenticated distributed nodes, 5 a policy snapshot, 6 an execution lease, 7
  manager health, 8 the admission gate, 9 a filesystem substrate, 10 verified
  storage, 11 the TAC3 filesystem module. A program only boots once that grade's
  validator admits a well-formed instance (and `--selfcheck` also proves the
  validator rejects a malformed one). Each has its own `src/`, `include/`,
  `Makefile`, and `LANGUAGE.SPEC.md`, and builds to
  `sleela-virtual-machine/NNN/vm-build/slvm-NN`. Inspect one with
  `sleela vm info premium <n>`.

> **Selection vs. running.** `sleela vm select premium <n>` resolves and reports
> the choice (validated the same way the VM's `OP_VM_SELECT` opcode validates
> it, now `1..11`). `sleela vm run premium <n> <program.sleela>` goes further: it
> builds that generation's VM on demand (if needed), compiles your program to a
> Core artifact, and **actually boots the generation VM and runs the program on
> it**. The premium VMs are built by the master build (`make premium-vms`, part
> of `make all`). Standard think-tank versions (`1..10`) are not yet bootable
> this way — use the Native VM (plain `sleela run`) for those.

### Lock in a VM generation

If you want one generation used every time — typically the **advanced
(highest)** VM — you can pin it so the CLI refuses anything else:

```sh
sleela vm lock              # lock in the advanced VM (the highest generation)
sleela vm lock premium 11   # lock a specific generation (same as above)
sleela vm status            # show the current pin (version / locked / strict)
sleela vm unlock            # clear the pin; selection is free again
```

While locked, `sleela vm run premium <n>` uses the pinned generation and
**refuses** any other `<n>` (exit code 3) with a message telling you to run the
pinned version or `sleela vm unlock`. `sleela vm select premium <n>` still
reports the choice but notes that running it will be refused until you unlock.

The lock is your explicit choice and is **persisted** to the VM config
(`config/vm.conf`, or the path in `$SLEELA_VM_CONF`) as `vm.locked = true` with
`vm.version = <n>`. It is written through the existing VM-selection precedence
(CLI → `SLEELA_VM_VERSION` → `vm.conf` → compiled default 11), and the save
preserves everything else already in the file (e.g. the `java.*` block). Nothing
is locked by default.

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

**Scan everything, verify bytes exactly, or verify authenticity:**

```sh
sleela modium status            # summary across all ten versions
sleela modium scan 10 --exact   # SHA-256 + exact byte re-read
sleela modium scan 10 --seal    # verify each entry's ORIGINALITY SEAL
```

### Two kinds of check (why both matter)

- **Verify** (default / `--exact`) — does each file match the SHA recorded in
  `MODIUM.json`? Fast; the everyday "are my files intact" check.
- **Seal** (`--seal`) — is the *record itself* authentic? Each entry carries an
  originality seal = `SHA-256(vm | path | sha256 | author)`. If a record was
  tampered **and regenerated** under a different identity, `--seal` catches it
  (the seal won't recompute), where plain verify would wrongly say "ok". This is
  the same seal concept the registry uses.

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

## 3. `sleela registry` — settings, features, and provenance

A Windows-style registry: a tree of keys holding settings and feature flags,
with sealed provenance for tracked artifacts. A fresh install seeds a baseline;
the store is a portable `.sleelareg` file (`$SLEELA_REGISTRY`, default
`./sleela.sleelareg`).

```sh
sleela registry review                 # show the whole tree (seeds if empty)
sleela registry review Settings        # a subtree
sleela registry get Settings verbosity
sleela registry set Settings verbosity verbose
sleela registry set Settings threadCap 64 --int
sleela registry feature modium          # query a feature flag
sleela registry feature modium on       # toggle it
sleela registry export registry.sql     # portable MySQL script of the registry
```

The provenance of a tracked artifact records its file, date, author, version,
hex id, SHA-256 hash, and an **originality seal** — the same seal idea as
`modium --seal`. Review shows `[SEALED]` or `[BROKEN]` per entry.

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
- `sleela vm select` never executes anything; it only resolves and reports the
  choice. Running a program on a premium VM is the explicit, separate
  `sleela vm run premium <n> <program>` — which boots that VM and runs the
  program on it under the usual SHA-256 build gate.
- The Native VM remains the default and is the recommended starting point.
