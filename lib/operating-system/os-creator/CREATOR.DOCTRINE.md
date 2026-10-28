# OS Creator Doctrine — the ordered composition of a Creator document

**Schema:** [`os-creator.xsd`](os-creator.xsd) · **Version:** 1.0.0

This doctrine is the normative statement of **how the OS Creator parts go
together — in order, not in an unordered way** — and how the SLeeLa compiler
and tooling keep that integrity. The machine-checkable form is the XSD; this
document is its rationale and the contract authors follow.

## 1. The parts go one way: firmware → desktop

An OS model (`SLOSModel`) composes its layers in the exact order a system boots.
A Creator document MUST present them in this sequence (the `<xs:sequence>` of
`os-creator.xsd`):

```
identity → shape → arch(+) → bootloader → kernel → base-drivers
        → filesystem → [swap] → packages → services → [compat] → [desktop]
```

Rationale — each layer depends only on the ones before it:

1. **identity / shape / arch** — what OS, which family, which CPU(s). The shape
   fixes every downstream default.
2. **bootloader** — firmware + loader; nothing boots without it.
3. **kernel** — the kernel the loader hands control to (family + version).
4. **base-drivers** — the minimum-boot driver series. The bus and storage
   drivers MUST be present here or the kernel cannot reach the root filesystem;
   this is why base-drivers precede filesystem.
5. **filesystem** — the root filesystem the early userspace mounts, reachable
   only because the storage driver above exists.
6. **swap** *(optional)* — paging store; meaningful only once a filesystem is
   chosen.
7. **packages** — the userland seed installed into the mounted rootfs.
8. **services** — what the init manager starts, from the installed packages.
9. **compat** *(optional)* — foreign-OS execution; a userspace capability that
   needs the services/libraries above.
10. **desktop** *(optional)* — the session stack, last and topmost.

Required vs. optional is normative: `swap`, `compat`, and `desktop` are
`minOccurs="0"`; everything else is mandatory. A server edition omits `desktop`.

## 2. What may not go together

The XSD encodes the illegal combinations as closed value sets and cardinalities:

- A **kernel family** must be `Linux` | `NT` | `XNU` — not an arbitrary string.
- A **root filesystem** must be a rootable type (`ext4`/`xfs`/`btrfs`/`zfs`/
  `f2fs`/`ntfs`/`refs`/`apfs`/`hfs+`/`ufs`); an ESP, if present, must be FAT
  (`fat32`/`fat16`/`vfat`). Optical/pseudo/network filesystems may not be a root.
- **base-drivers** requires **at least two** drivers, and by doctrine the `bus`
  and `storage` entries must be `phase="boot"` (the irreducible pair). A series
  without them is rejected by `SLBaseDriverSeries.canBoot()` and by the model's
  `isBuildable()`.
- A **swap kind** is a closed set; a compressed kind (`zram`/`zswap`) must name a
  compression algorithm, and `pagefile`/`dynamic` are only valid on their own
  families (enforced by `SLSwapConfig.validForShape`).
- **compat** allows at most three guest routes (one per other family), each
  guest from the closed `{Linux,Windows,macOS}` set.
- A **driver origin** is `in-tree` | `vendor-signed` | `unknown-source` |
  `foreign-OS`; a foreign driver loads only where `SLDriverSource` has a bridge.

## 3. How the compiler keeps integrity — by version

A SLeeLa source that drives the OS Creator binds itself to this schema with a
pragma on an early line (after `#sleela`):

```java
#sleela 1.11
#schema os-creator/os-creator.xsd 1.0.0
```

The compiler (`resolveSchemaReference` in `impl/frontend/version.{h,cpp}`,
invoked from `checkSyntaxVersion`) performs **code recognition** of this binding
on every `check` / `run` / `compile`:

- it resolves the schema path (relative to the source, then the repo root) and
  **errors if the schema file is absent** — the document cannot claim a contract
  that is not present;
- it reads the declared **version** and **errors if it exceeds the compiler's
  supported Creator schema version** (`maxSupportedSchema()`, currently `1.0.0`),
  so a document written against a newer doctrine is not silently accepted;
- a source with **no `#schema`** is accepted (not every source is a Creator
  document) — the binding is opt-in but, once declared, enforced.

Full structural XSD validation of an **exported** Creator document (the XML a
developer can serialise from an `SLOSModel`) is performed by external tooling /
CI against the same `os-creator.xsd`. The division is deliberate: the compiler
guarantees **reference integrity + version compatibility** at build time (fast,
always-on); the XSD is the single normative structure both the compiler's
doctrine and the external validator agree on.

## 4. Versioning the doctrine

The schema carries `version="1.0.0"` on its root and in its `#schema` pragma.
Changes follow semantic versioning of the composition contract:

- **patch** — clarifications, added optional attributes, widened value sets that
  remain backward-compatible;
- **minor** — new optional layers/elements that older documents still satisfy;
- **major** — a reordering or a new required layer (a breaking change to the
  firmware→desktop sequence).

Bump `maxSupportedSchema()` in `version.h` when the compiler is taught a new
Creator schema version, and record the change here and in the XSD's `version`.

## 5. The objects, in order (quick reference)

| # | Layer | SLeeLa object | Required | Notes |
|---|---|---|---|---|
| 1 | identity | `SLOSModel` | yes | name/version/edition |
| 2 | shape | `SLOSShape` | yes | Linux/Windows/macOS |
| 3 | arch | `SLArchitectureSet` | yes | ≥1 target |
| 4 | bootloader | `SLBootloaderSpec` | yes | firmware + loader |
| 5 | kernel | `SLKernelSpec` (+`SLKernelCatalog`) | yes | family + version |
| 6 | base-drivers | `SLBaseDriverSeries` (+`SLDriverSource`) | yes | boot bus+storage |
| 7 | filesystem | `SLFilesystemLayout` (+`SLFilesystemCatalog`) | yes | rootable fs |
| 8 | swap | `SLSwapConfig` | no | kind + quality |
| 9 | packages | `SLPackageSet` | yes | ≥1 package |
| 10 | services | `SLServiceSet` | yes | init manager |
| 11 | compat | `SLCompatSet` | no | foreign-OS routes |
| 12 | desktop | `SLDesktopSpec` | no | omitted on Server |

The emitters (`SLBaseDriverEmitter`, `SLCompatLoaderEmitter`, `SLCEmitter`,
`SLCppEmitter`, …) consume these **in this order** to write `/os` and the
generated tree; the order here is the order there.
