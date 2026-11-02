# Modium Storage Region — design proposal (for review)

> Status: **PROPOSAL / design-only.** No behavioral code ships with this note.
> It describes a new Modium capability and, critically, the **safety and
> ethical rules** for using removable media. Nothing here writes to a device.

## 1. What exists today vs. what this adds

**Today** "Modium" is a per-VM *file-resolution / integrity scanner*
([`lib/vm/src/sleela/SLVmModium.sleela`](src/sleela/SLVmModium.sleela), native
`sleela modium` in `impl/frontend/driver.cpp`, service opcode
`OP_MODIUM_SCAN=149`). It verifies a VM version's files against `MODIUM.json`
(exist / placed / SHA-256 intact) and recovers from `rebuild.sql`. It holds
**no data region and has no memory footprint to speak of** — the "~2 MB" figure
is not a real Modium limit (the only MB-scale limit in the tree is the separate
**Memory Manager**, whose hard ceiling is 2 **GB**).

**This proposal** adds a **Modium Storage Region (MSR)**: an addressable,
sized data region Modium can own — **256 MB minimum, 64 GB maximum** — backed
by either a host file or an **inserted USB stick** claimed as Modium storage.

## 2. The storage region (sizing)

| Property | Value |
|---|---|
| Minimum region | **256 MB** (`SL_MODIUM_MIN_BYTES`) |
| Maximum region | **64 GB** (`SL_MODIUM_MAX_BYTES`) |
| Growth | base-2 steps on demand (256M → 512M → 1G → … → 64G), like the layout table |
| Default | 256 MB when unspecified |
| Backing | a region file `modium.region` inside a Modium **container** (see §4) |

The region is **file-backed, not resident RAM**: a 64 GB region does not consume
64 GB of process memory. The Memory Manager (`slmm`) and its 2 GB RAM ceiling are
unchanged and orthogonal — MSR is persistent storage, not heap.

## 3. "SLeeLa checks the OS runtime to find how to handle it"

Device/removable-media handling differs per OS, so MSR **asks the running host**
rather than assuming. It uses the primitives already in the tree:

- `slplatform_detect()` → OS identity + version + architecture (`sleela_platform.h`).
- `slos_*` → `exists / is_dir / file_size / mkdir / rename / run` (`sleela_os.h`).

The native MSR backend resolves **exactly one** handling strategy for the current
host and uses **only** that one (no fallback guessing):

| Host (runtime-detected) | How a stick is enumerated / its mount path resolved |
|---|---|
| Linux | the OS's own mount view (`/proc/self/mountinfo`), filtered to removable block devices |
| macOS | the OS volume service (`/Volumes` + the system disk-arbitration view) |
| Windows | the OS volume API (removable `DRIVE_REMOVABLE` volumes) |

`slos_platform_name()` already returns the authoritative answer; MSR selects the
matching backend and **appropriates the correct answer only** — an unknown/
unsupported host returns a clean "unsupported on this OS" status, never a guess.

## 4. The `.modium` container and claim protocol (USB)

A USB stick is used as Modium storage **only** when it carries a valid
**`.modium`** descriptor at its mount root. The descriptor is a small JSON file:

```json
{
  "modium": "storage-region",
  "format": "1.0",
  "regionBytes": 268435456,
  "created": "2026-11-01T00:00:00Z",
  "owner": "<user-or-org label>",
  "deviceSeal": "<sha256 of (volume-uuid | regionBytes | owner)>",
  "author": "MEARVK LLC / the Author"
}
```

**Claim / resolution protocol (fail-closed, consent-gated):**

1. **Insert & detect.** MSR asks the OS (per §3) for removable volumes and their
   mount roots. It never scans fixed/system disks.
2. **Inspect, read-only.** For a candidate volume, MSR reads `<root>/.modium`
   **without writing anything**.
3. **Three honest outcomes — the "empty or otherwise" cases:**
   - **Already Modium** — valid `.modium` + matching `deviceSeal` → adopt it as
     this VM's storage region. No formatting.
   - **Empty volume, no `.modium`** — MSR will **initialize** it as Modium
     storage **only on explicit opt-in** (`--claim`), and only if the OS reports
     the volume as empty/removable. Initialization writes `.modium` +
     `modium.region` and nothing else.
   - **Non-empty, not Modium** (has unrelated files) — **REFUSE.** MSR never
     reformats or claims a stick that holds someone's data. Status: "volume in
     use; refusing to claim (pass --claim-force only for a volume you own and
     have backed up)."
4. **Seal.** On claim, `deviceSeal = SHA-256(volume-uuid | regionBytes | owner)`
   binds the region to that physical volume, so a `.modium` copied to a
   different stick is detected as foreign and rejected.

## 5. Ethical & safety guardrails (hard rules)

- **No silent writes, ever.** Detection and inspection are strictly read-only.
  The *only* write paths are an explicit `--claim` (init an empty volume) or an
  explicit `--claim-force` (init a volume the operator asserts they own); both
  print the target device, its size, and its current contents summary and
  require confirmation.
- **Never touch fixed/system disks.** MSR acts only on volumes the OS reports as
  **removable**. The host OS is the authority on what is removable.
- **Refuse non-empty, non-Modium media by default** — protects a user's
  existing files; forcing requires an explicit, logged flag.
- **Bounded-handle discipline** (matching sockets/files/processes): Sleela source
  never sees a raw device path or handle — only a VM-local integer region handle.
- **Size is clamped** to [256 MB, 64 GB]; out-of-range requests fail closed.
- **Fail-closed on unknown OS / no permission** — report and stop, never guess.
- **Provenance**: every `.modium` carries `author` + `deviceSeal`, consistent
  with Modium's existing seal discipline.

## 6. Proposed surface (shape, not final)

Native (new, behind a service opcode in the Modium family, e.g. `OP_MODIUM_STORE`):
```
int  slmodium_region_open(const char* container, int64_t bytes);   /* file-backed region; clamps size */
int  slmodium_usb_list(SLModiumVolume* out, int cap);              /* OS-resolved removable volumes (read-only) */
int  slmodium_usb_inspect(const char* mountRoot, SLModiumDesc* d); /* read .modium only; never writes */
int  slmodium_usb_claim(const char* mountRoot, int64_t bytes, int force); /* explicit init; consent required */
int  slmodium_region_read (int handle, int64_t off, void* buf, int64_t n);
int  slmodium_region_write(int handle, int64_t off, const void* buf, int64_t n);
void slmodium_region_close(int handle);
```

Sleela-side (new class `SLVmModiumStore`, sibling of `SLVmModium`, same metadata
metapattern + VM-exit seam):
```
SLVmModiumStore s = new SLVmModiumStore();
s.openRegion("~/.sleela/modium", 268435456);   // 256 MB file-backed
int vols = s.listUsb();                         // OS-resolved removable volumes
s.inspect(0);                                   // read-only .modium check
// s.claim(0, 268435456, false);                // explicit, consent-gated
```

CLI (extends the existing `sleela modium`):
```
sleela modium store open   [--bytes 256M..64G] [--container DIR]
sleela modium store usb list
sleela modium store usb inspect <mount>
sleela modium store usb claim  <mount> [--bytes ...] [--claim | --claim-force]
```

## 7. Open questions for sign-off

1. **Scope now vs. staged.** Proposed order: (a) file-backed region with the
   256M–64G clamp first (pure, testable, no device risk); (b) read-only USB
   detect + `.modium` inspect; (c) consent-gated claim/init last. OK to stage?
2. **Container location** for the file-backed default — `~/.sleela/modium`?
3. **Consent UX** — is an interactive confirmation acceptable, or must it be a
   non-interactive explicit flag only (for headless use)?
4. **`.modium` format version** and whether the MySQL `rebuild.sql` recovery
   model should extend to the region, or the region is opaque bytes.

---
*Mirrors existing Modium provenance/seal discipline; uses the in-tree
`sleela_platform`/`sleela_os` OS-abstraction and the service-opcode VM-exit
pattern. No device is written by anything in this document.*
