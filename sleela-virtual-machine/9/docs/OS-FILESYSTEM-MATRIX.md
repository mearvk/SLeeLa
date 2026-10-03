# SLVM/9 OS / Filesystem Matrix

| Target | Native adapter responsibility | Common SLVM contract |
|---|---|---|
| Linux | mount metadata, file descriptors, filesystem-specific APIs | FAL capabilities |
| Windows | HANDLE/volume APIs, native filesystem metadata, flush semantics | FAL capabilities |
| macOS | Darwin/POSIX APIs, volume metadata, APFS-specific capabilities | FAL capabilities |
| TAC3 on Linux | TAC3-specific identity, generation, recovery and feature discovery | FAL + contextual identity |
| Future custom filesystem | native feature negotiation | FAL version/feature bits |

The matrix intentionally avoids declaring that every filesystem provides the same guarantees. The adapter reports actual capabilities at runtime.
