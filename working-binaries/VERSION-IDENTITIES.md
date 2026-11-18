# Binary version identities

**Registry edition:** 1.0121  
**Effective date:** 2026-11-18  
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

The product identities above are retained; the artifact identity is an
additional monotonically versioned binary-build identity. No binary payload
is relabelled by this document. The committed prebuilt executables must be
replaced and their SHA-256 values regenerated as part of the reproducible
build/release step.

## Additional tracked binary artifacts

The repository tree inventory also finds these non-toolchain binary payloads:

| Artifact | Identity | Classification / action |
|---|---:|---|
| `ide/sleela-intellij/.../sleela-intellij-0.3.0-dev.jar` | 1.012 | IntelliJ plugin; Gradle source version advanced to 1.012, but the checked-in JAR is still the old 0.3.0-dev payload and must be rebuilt/replaced |
| `modules/AE6E66/source/EmailDistributor.class` | 1.0121 | Previously unversioned Java class artifact |
| `telephony-skya/javafx/target/classes/com/mearvk/sleela/skya/*.class` (9 class files) | 1.0121 | Previously unversioned Java build artifacts |
| `api/sleela/2.8/generic.dna.bin` | 1.0121 | Binary data artifact; identity assigned without changing its data format |
| `utf-4088/tools/utf4088-million.bin` | 1.0121 | Generated binary data artifact; identity assigned without changing its data format |

The 1.0121 assignments are registry identities. They do not alter byte payloads.
The class and data artifacts should be regenerated from their owning sources when
those projects next build. The old IntelliJ JAR is explicitly not represented as
rebuilt: its source build identity is 1.012, while the committed JAR still needs
replacement. The two SLeeLa toolchain binaries remain product-versioned and
must be rebuilt to carry the current Syntax 3.2 source gate.

## New identities

- `SLEELA-BIN-1.012` — SLeeLa compiler/runtime executable artifact identity.
- `NORDSHRIFT-BIN-1.012` — Nordshrift executable artifact identity.
- `SLEELA-UNVERSIONED-BIN-1.0121` — reserved initial identity for any
  previously unversioned SLeeLa binary discovered during inventory.
- `NORDSHRIFT-UNVERSIONED-BIN-1.0121` — reserved initial identity for any
  previously unversioned Nordshrift binary discovered during inventory.

The reserved 1.0121 identities are not assigned to the two known prebuilt
executables because those already have product versions. Any additional
unversioned binary must receive a concrete name, path, platform, architecture,
SHA-256, and 1.0121 identity before publication.
