# Binary version identities

**Registry edition:** 1.0121  
**Effective date:** 2026-11-19  
**Repository:** `mearvk/SLeeLa`

This registry separates a shipped binary's artifact identity from the product
version printed by the executable, the language syntax edition, the VM ABI,
and the bytecode/artifact format. These numbers are not interchangeable.

## Version policy

- Every shipped binary must carry an explicit artifact identity.
- A binary that already has a product/version identity must have an artifact
  identity of at least **1.012**.
- A binary with no prior version identity is assigned **1.0121** as its first
  explicit artifact identity.
- Artifact identity is monotonically increased when the binary payload changes.
- A source-only commit does not change the identity of an already-built binary.
- A binary may claim Syntax 3.2 support only after it is rebuilt from the
  Syntax 3.2 source tree and passes the version, syntax-3.2, VM-opcode, and
  runtime regression checks.
- SHA-256 values must describe the exact committed binary payload, never a
  source revision or a previous build.

## Known prebuilt toolchain

| Artifact | Product identity already present | Artifact identity | Status |
|---|---|---:|---|
| `working-binaries/sleela` | SLeeLa Language/Runtime 1.3.2-pro | 1.012 | Rebuild required before claiming Syntax 3.2 |
| `working-binaries/nordshrift` | Nordshrift 2.7.1-dev | 1.012 | Rebuild required before claiming Syntax 3.2 |

The product identities above describe the currently committed payloads; the artifact identity is an additional monotonically versioned binary-build identity. The source-built `sleela` and `sleelvac` release identity is now **1.0121**, and the source compiler accepts Syntax 3.2. No committed binary payload is relabelled by that source change. The prebuilt executables must be rebuilt and replaced, then their SHA-256 values regenerated, before the checked-in binaries can claim the new product identity or Syntax 3.2 support.

## Additional tracked binary artifacts

The repository tree inventory also finds these non-toolchain binary payloads. The authoritative per-path identity map is [binary-identities.json](binary-identities.json):

| Artifact | Identity | Classification / action |
|---|---:|---|
| `ide/sleela-intellij/.../sleela-intellij-0.3.0-dev.jar` | 1.012 | IntelliJ plugin; build metadata is 1.012, but the checked-in JAR remains the old 0.3.0-dev payload and must be rebuilt/replaced |
| `tests/nordshrift` | 1.0121 | Previously unversioned executable test artifact; assigned a stable identity pending owner/build provenance |
| `modules/AE6E66/source/EmailDistributor.class` | 1.0121 | Previously unversioned Java class artifact |
| `telephony-skya/javafx/target/classes/com/mearvk/sleela/skya/*.class` (9 class files) | 1.0121 | Previously unversioned Java build artifacts |
| `api/sleela/2.8/generic.dna.bin` | 1.0121 | Binary data artifact; identity assigned without changing its data format |
| `utf-4088/tools/utf4088-million.bin` | 1.0121 | Generated binary data artifact; identity assigned without changing its data format |

The 1.0121 assignments are registry identities. They do not alter byte payloads. The inventory covers 17 binary payloads: 3 toolchain executables (`javaconv` added 2026-11-20), 1 plugin JAR, 1 native test executable, 9 Java class files, and 2 binary data files.
The class and data artifacts should be regenerated from their owning sources when
those projects next build. The old IntelliJ JAR is explicitly not represented as
rebuilt: its source build identity is 1.012, while the committed JAR still needs
replacement. The two SLeeLa toolchain binaries were rebuilt from the Syntax 3.2 source tree on
2026-11-19 and now carry `SLEELA-BIN-1.0121` and `NORDSHRIFT-BIN-1.0121` (see below).

## New identities

The concrete identities for every inventoried payload are recorded in `binary-identities.json`; this section names the primary toolchain identities:

- `SLEELA-BIN-1.012` — the previous `working-binaries/sleela` payload (`1.3.2-pro`, syntax up to 3.1); superseded.
- `NORDSHRIFT-BIN-1.012` — the previous `working-binaries/nordshrift` payload; superseded.
- `SLEELA-BIN-1.0122` — current `working-binaries/sleela`, rebuilt from `impl/` on 2026-11-20 (product `1.0121`, syntax 1.0 – 3.2), SHA-256 `e259db8fc79f9977c6c6e2a7259532e90434839ff2466fcc045b02daf23d917a`.
- `SLEELA-BIN-1.0121` — the 2026-11-19 `working-binaries/sleela` payload (`1.0121`, syntax 1.0 – 3.2), SHA-256 `7608bd9cc3a85628ec28c81f490c38c876003349447281c286cc19788ae70483`; superseded.
- `SLEELAVC-BIN-1.0121` — next source-built Sleelvac compiler artifact (`impl/build/sleelvac`).
- `NORDSHRIFT-BIN-1.0121` — current `working-binaries/nordshrift`, built from `impl/` (`2.7.1-dev`, drives syntax up to 3.2), SHA-256 `21f3740dd6d4a18f4e5506346a62a4e23418063a8f617c96b96607f2d4199507`.
- `JAVACONV-BIN-1.0122` — current `working-binaries/javaconv`, rebuilt 2026-11-20 (later) for conversion model 1.7.0, SHA-256 `87dae9cbff1411c5e04f894f949461a2a09c3c0dc8388215f3539fcebcf7d45d`.
- `JAVACONV-BIN-1.0121` — superseded `working-binaries/javaconv`, the Java to SLeeLa converter (`sleelactl/convert`), added 2026-11-20 from `3679be471`; it had no prior version identity, so it takes the initial identity 1.0121. SHA-256 `66db79f3345137d57527d21a7dfb29392e9ee7e25a179b54ded9bbb2e12aaf22`.
- `SLEELA-TERMINAL-GUI-BIN-1.0121` — next GUI executable (`sleela-terminal/build/SleelaTerminal`).
- `SLEELA-TERMINAL-SHELL-BIN-1.0121` — shell executable (`sleela-terminal/build/slsh`), exposed by `slsh --version`.
- `SLEELA-UNVERSIONED-BIN-1.0121` — reserved initial identity for any
  previously unversioned SLeeLa binary discovered during inventory.
- `NORDSHRIFT-UNVERSIONED-BIN-1.0121` — reserved initial identity for any
  previously unversioned Nordshrift binary discovered during inventory.

The 1.0121 identities are assigned to the inventoried unversioned artifacts. Any newly discovered unversioned binary must receive a concrete name, path, platform, architecture, SHA-256, and 1.0121 identity before publication. SHA-256 values must be computed from the actual payload at build time; Git blob SHA-1 values are not substitutes.

## Rebuild record (2026-11-19)

Both toolchain payloads were rebuilt on Linux x86-64 (GCC 11.5) from `master` after the integrity manifest was
regenerated and `make check-manifest` printed OK. The checks this policy requires before a Syntax 3.2 claim:
`make test-version`, `impl/tests/syntax32/run_syntax32_tests.sh` (and `syntax31`), `make test-opcode-registry`,
`make test-runtime`, `make test-osls-vm` and `make test-sleela` all pass, as do `test-nordshrift`
(+ `-compiler`, `-semantic`, `-compat`, `-e2e`) and the full regression set (214 tests, with mutation testing).

## Rebuild record (2026-11-20)

`working-binaries/sleela` only, rebuilt on Linux x86-64 (GCC 11.5) from `master` at `cd43c1db`, after
`make regen-manifest` and `make check-manifest` printed OK. The payload changed, so the artifact identity rises
to `SLEELA-BIN-1.0122`; the product version it prints is still `1.0121`. The reason is the `impl/frontend/driver.cpp`
change in `833b3a44`: a multi-file `sleela run/compile a.sleela b.sleela` now merges the user files first and
resolves `/lib` units once. The 2026-11-19 payload resolved them per file, so a library class used by two files
(`String`, or a project `Box` beside `javax.swing.Box`) was declared twice and the semantic pass rejected the
program. With the old payload, the six multi-file project checks of `tests/javaconv_differential_test.sh` failed.

`nordshrift` does not link `driver.o` and was not rebuilt; it stays `NORDSHRIFT-BIN-1.0121`.

Checks run with the new payload: `make test-version`, `test-opcode-registry`, `test-runtime`, `test-osls-vm`,
`test-sleela`; `impl/tests/syntax31` and `syntax32` fixture scripts; the regression set (214 tests, with mutation
testing) with `SLEELA_BIN=$PWD/working-binaries/sleela`; and `tests/javaconv_differential_test.sh` with no
`impl/build/sleela` present (103 of 103).

## Addition record (2026-11-20)

`working-binaries/javaconv` added, built from `master` `3679be471` with `make -C sleelactl/convert` on Linux x86-64
(GCC 11.5). A clean rebuild gives the same bytes. Checked: `javaconv --self-check` (model `java-to-sleela 1.6.0`) and
`tests/javaconv_differential_test.sh` with this copy as the converter (116 of 116). `sleela` and `nordshrift` were
rebuilt from the same commit and came out byte-identical to `SLEELA-BIN-1.0122` and `NORDSHRIFT-BIN-1.0121`, so
their payloads and identities are unchanged.
