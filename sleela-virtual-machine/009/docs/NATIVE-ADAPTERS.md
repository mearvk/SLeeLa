# SLVM/9 Native Filesystem Adapters

SLVM/9 now contains a real native-probe layer beneath the Filesystem Abstraction Layer (FAL).

## Significance

The VM can establish a first, evidence-based relationship with storage without hard-coding ext4, XFS, APFS, NTFS, TAC3, or another future filesystem into the language runtime. The adapter observes the host, converts observations into the SLVM/9 contract, and leaves authority with SLVM/8 admission and policy.

## Native implementations

- Linux: statfs, statvfs, and stat provide filesystem identity, capacity, read-only state, file identity, and access state.
- macOS: Darwin statfs, statvfs, and stat provide the corresponding native observations; APFS is reported rather than assumed.
- Windows: GetVolumeInformationW, GetDiskFreeSpaceExW, and GetFileInformationByHandle provide volume and file identity; UTF-8 paths are converted at the adapter boundary.

## Generation safety

A native probe produces a deterministic generation token from observed filesystem identity, block geometry, filesystem type, and mount path. A fresh probe can therefore be compared with an admitted filesystem contract and rejected as stale when the identity no longer matches. This is an identity guard, not a claim that every operating system exposes an internal filesystem generation counter.

## TAC3 and future filesystems

TAC3 remains an advanced adapter/profile above this native layer. A TAC3 adapter can verify its own superblock, FILE, HEALTH, ADMIN, RECOVERY, contextual identity, and durability semantics, then enrich the common capability profile. The native adapter deliberately does not invent those semantics.

## Security boundary

Native probing never grants SLeeLa a capability. The intended path remains:

SLeeLa Artifact -> SLVM/8 Admission -> Policy -> Capability Lease -> SLVM/9 Supervisor -> Native Adapter -> Operating System

## Testing

make test builds the ordinary SLVM/9 contract test and the native adapter test. Unsupported operating systems fail closed instead of fabricating filesystem behavior.
