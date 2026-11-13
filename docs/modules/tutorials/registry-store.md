<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# registry-store: Tutorial

| | |
|---|---|
| Nature | datasource (`bin/sleelactl help registry-store`) |
| Registry summary | Native object/subject registry store |
| Lives in | `lib/registry/` (10 `.sleela` files, the SLeeLa mirror); native core in `registry/` (C/C++, Makefile) |
| Writeup type | Tutorial, because you can run the SLeeLa registry program and build and use the native registry tools, and I ran every step below. |
| Verified | 2026-11-13 against `master` e048707fd: `sleela run lib/registry/SLRegistry.sleela` exits 0; `make`, `make run` and `make test` in `registry/` succeed (`SLeeLa Registry 3.0 record tests: PASS`); the SLREG 1 and 3.0 CLIs ran. Three defects were found (listed in Step 3 and Step 4). |

## What it is

The registry is a Windows-style tree of keys (`System`, `Settings`, `Features`, ...) that holds typed settings and feature flags, plus a provenance record with an "originality seal" for tracked files ([`registry/README.md`](../../../registry/README.md)). There are two layers:

- The **native core** in `registry/`: a C ABI, a C++ facade, a terminal tool `sleela-registry` (`seed`, `review`) and the newer **Registry 3.0 record engine** in `registry/v3/` with its own CLI `sleela-registry3`.
- The **SLeeLa mirror** in `lib/registry/`: `SLRegistry`, `SLRegistryKey`, `SLRegistryValue`, `SLRegistryProvenance`, plus 3.0 model classes (Record, Identity, Relationship, Lifecycle, Resolver, Review). Its README says the "native record engine owns the full record set".

The registry entry maps `registry-store` to `dir=lib/registry` ([`registry.conf`](../../../bin/sleelactl.d/registry.conf) line 44).

## Before you start

You need `cc`, `g++`, `ar`, and the prebuilt `impl/build/sleela`. Native outputs go to the git-ignored `registry/build/`.

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

A note on `sleelactl`: `bin/sleelactl status registry-store` exits 0 and prints `✓ registry-store library present at lib/registry`. `open` and `close` only print a hint to run `SLDataSource ds = new SLDataSource(); ds.configure("registry"); ds.open();`. `SLRegistry` has no `open`, `close` or `active` method at all (a grep of `lib/registry/*.sleela` finds none), so that hint does not apply to the registry classes. Use the steps below instead.

## Step 1 - Run the SLeeLa registry

```sh
timeout 120 impl/build/sleela run lib/registry/SLRegistry.sleela
```

```text
Sleela Registry 3.0.0  (keys=6)
[System]
    name = "Idea System(TM) / SLeeLa"
    registryVersion = "3.0.0"
[System/VM]
    default = "native"
[Settings]
    memoryLimitMiB = 2048
    threadCap = 128
    verbosity = "normal"
[Features]
    modium = true
    vmSelect = true
    premiumVms = true
[Artifacts]
[Math]
    irrationalDigitCarry = 2048
    ...
feature modium enabled: true
```

This is `SLRegistry.main`: `seedDefaults()`, `show()`, `featureEnabled("modium")`. It is built in memory on every run and saved nowhere. The README text says `seedDefaults()` creates five keys; the output shows six, because `Math` was added later.

## Step 2 - Build the native registry

```sh
cd registry
timeout 300 make
timeout 100 make run
```

`make run` runs `./build/sleela-registry seed build/demo.sleelareg` and prints a review that starts:

```text
seeded + saved build/demo.sleelareg
Sleela Registry 1.0.0  (keys=7 values=16 provenance=1)
[Artifacts]
[Artifacts/registry-core]
    <provenance> file=sleela_registry.c author=MEARVK LLC / the Author version=1.0.0 hexid=1dea0005ec0de001
                 hash=0db65e63... seal=3816ea8f... [SEALED]
```

`build/demo.sleelareg` is plain text (`SLREG 1`, then `[Key]`, `= i name 123`, `@ file | date | author | ...`).

```sh
timeout 100 make test
```

```text
cd build && ./test_registry_v3
SLeeLa Registry 3.0 record tests: PASS
cd build && ./test_registry_v3_cpp
```

The C++ test prints nothing and returned success.

## Step 3 - Review the saved file (a defect shows up)

```sh
./build/sleela-registry review build/demo.sleelareg
```

The output is the same as the seed review, except the seal flag:

```text
    <provenance> file=sleela_registry.c author=MEARVK LLC / the Author  version=1.0.0  hexid=1dea0005ec0de001
                 hash=0db65e63... seal=3816ea8f... [BROKEN]
```

The same registry that printed `[SEALED]` when seeded prints `[BROKEN]` after a save and reload. The likely cause (I did not prove it by patching) is visible in `registry/sleela_registry.c` lines 287-295: the loader reads the `@` line with `sscanf` and then trims trailing spaces from `name`, `hash` and `seal` only. `date`, `author` and `version` keep the space before `|`, as the doubled spaces in the output show, and the seal is computed over `hexId | version | hash | author`. I did not change the code. The README's example review shows `[SEALED]` for a reloaded file, so the documented behaviour and the code disagree.

## Step 4 - Use the Registry 3.0 CLI

```sh
R=build/sleela-registry3
D=/tmp/slreg-demo && mkdir -p $D
$R init $D/r3.slreg 0123456789abcdef0123456789abcdef
$R add $D/r3.slreg 00000000000000000000000000000001 demo app Widget
$R identify $D/r3.slreg demo Widget
$R review $D/r3.slreg
```

```text
registry saved: /tmp/slreg-demo/r3.slreg
registry saved: /tmp/slreg-demo/r3.slreg
00000000000000000000000000000001 app draft Widget\n
SLeeLa Registry 3.0 id=0123456789abcdef0123456789abcdef records=1
[demo] Widget kind=app state=draft rev=1 integrity=unverified
```

Defect: `identify` prints a literal backslash and `n` at the end instead of a newline. `registry/v3/sleela-registry3.c` line 70 has `printf("%s %s %s %s\\n", ...)` (and line 69 does the same for the error text). Everything else worked.

Migrate the SLREG 1 file from Step 2 into a 3.0 file:

```sh
$R migrate-v1 build/demo.sleelareg $D/v2.slreg 0123456789abcdef0123456789abcdef
$R review $D/v2.slreg | head -4
```

```text
migrated build/demo.sleelareg -> /tmp/slreg-demo/v2.slreg (SLREG 1 -> SLREG 2)
SLeeLa Registry 3.0 id=0123456789abcdef0123456789abcdef records=24
[sleela.compat] Math/repeatingDigitCarry kind=setting state=registered rev=2 integrity=unverified
[sleela.compat] System/VM kind=legacy_key state=registered rev=1 integrity=unverified
```

Clean up: `rm -rf /tmp/slreg-demo`.

## What you did

You ran the in-memory SLeeLa registry, built the native registry, seeded and reviewed an SLREG 1 file, ran the 3.0 test suite, created and queried a 3.0 record file, and migrated the old file to the new format. The native core, not `lib/registry`, is the store: the `.sleela` classes model it in memory.

## Next steps

- [`lib/registry/README.md`](../../../lib/registry/README.md) and [`registry/README.md`](../../../registry/README.md).
- [`registry/REGISTRY-3.0-ARCHITECTURE.md`](../../../registry/REGISTRY-3.0-ARCHITECTURE.md), [`registry/v3/README.md`](../../../registry/v3/README.md).
- Related datasources: [database](../explanations/database.md), [ledger](../explanations/ledger.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `lib/registry/README.md`, `lib/registry/SLRegistry.sleela` (lines 150-196), `registry/README.md`, `registry/Makefile`, `registry/sleela_registry.c` (lines 283-297), `registry/v3/sleela-registry3.c` (lines 42, 64-72).

Commands run (with `timeout`): `bin/sleelactl help|available|status|open|close registry-store`; `impl/build/sleela run` and `check` on `lib/registry/SLRegistry.sleela`; `make`, `make run`, `make test` in `registry/`; `sleela-registry review`; `sleela-registry3 init|add|identify|review|migrate-v1`. Built here: `registry/build/` (git-ignored, including `registry/build/demo.sleelareg`).
