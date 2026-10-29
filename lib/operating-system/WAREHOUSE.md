# OS-grade C/C++ warehouse — `OS.Sensitivity.III`

The SLeeLa OS Creator sources the real **OS-grade C and C++** for each
operating-system shape from a dedicated warehouse repository,
[`mearvk/OS.Sensitivity.III`](https://github.com/mearvk/OS.Sensitivity.III),
rather than vendoring it in this repo. SLeeLa drives the contract; the warehouse
holds the bytes.

## The contract

- **`SLOSSensitivitySource`** (`SLOSSensitivitySource.sleela`) — the zero-payload
  reference: owner/repo, clone URL, **pinned commit**, default branch, and the
  map from an OS shape + component to the warehouse directory that provides it.
- **`SLOSSensitivityFetch`** (`SLOSSensitivityFetch.sleela`) — the opt-in, per-user
  downloader: composes a `git clone` from the reference and runs it through the
  `osRun` bridge (shallow + single-branch by default), then resolves the on-disk
  path of the chosen shape and its components. Nothing downloads automatically.
- **`os-sensitivity-fetch.sleela`** — the runnable OS Creator front door: prints
  the reference sheet and the exact clone command as a dry run, probes
  reachability, and (only with `OS_FETCH=1`) downloads the chosen shape's C/C++
  into a user-named output root for a **preliminary ISO source build**.

## Warehouse layout (the directory map `SLOSSensitivitySource` resolves)

The warehouse isolates the C/C++ per shape into a top-level folder:

```
OS.Sensitivity.III/
├── linux/     ─┐
├── windows/    ├─ per-shape C/C++, each with the components below
└── macos/     ─┘
```

Per shape, the components are resolved in firmware → boot → kernel → userspace →
desktop order:

| Component | Directory |
|---|---|
| UEFI firmware | `uefi/` |
| BIOS / CSM | `bios/` |
| Boot loader | `bootloader/` |
| Kernel loader | `kernel-loader/` |
| Driver loader | `driver-loader/` |
| Base drivers | `base-drivers/` |
| OS loader (PID 1) | `os-loader/` |
| Desktop loader | `desktop-loader/` |
| OS core | `os-core/` |

A path is `"<shape>/<component>"`, e.g. `linux/os-core` — resolved by
`SLOSSensitivitySource.dirFor(shape, component)`.

## Flow (per user, at home or office)

1. The user runs an OS Creator runnable (`os-sensitivity-fetch.sleela`, or an
   application that embeds the two classes).
2. SLeeLa references the warehouse at the **pinned commit** and fetches only the
   chosen shape's C/C++ into a user-named output root.
3. SLeeLa manipulates the fetched C/C++ into the exact OS output for that user —
   the **preliminary ISO source build** (reusing `SLISOBuilder` and the
   `os-generate.sleela` emitters).

## Notes

- **No Git LFS.** The warehouse uses no LFS filters; a normal clone retrieves
  everything. Keeping generated/sourced C/C++ out of this repo (and out of LFS)
  keeps SLeeLa small while provenance stays explicit via the pinned reference.
- **Reproducible.** `SLOSSensitivitySource.pin(commit)` repins to an exact
  revision; a full fetch hard-checks-out the pinned commit.
- This mirrors the `SLDistroSource` / `SLDistroFetch` pattern used for the
  upstream Ubuntu-lineage parts, applied to the per-shape C/C++ warehouse.
