# Improvements from SLVM/8 to SLVM/9

SLVM/9 (**Authority**) extends SLVM/8 (**Executive**) with a portable,
capability-controlled filesystem substrate and explicit operating-system
adapters. It keeps /8's admission, supervision, lease, transaction, and audit
contracts and reaches storage only through the SLeeLa capability/broker
boundary.

| Area | /8 | /9 |
|---|---|---|
| Storage | OS boundary only | Filesystem Abstraction Layer (FAL) |
| Capabilities | Execution capabilities | Filesystem capability negotiation + discovery |
| OS | Capability/OS boundary | Explicit Linux / Windows / macOS adapters |
| Native | — | Host-native probe (statfs/statvfs/stat; Win volume APIs) |
| Identity | Execution identity | Mount/namespace identity + generation tracking |
| Transactions | Execution transactions | Transactional filesystem operations |
| Durability | — | Integrity + durability capability reporting (claimed vs real) |
| Recovery | Supervisor recovery | Recovery-safe handle invalidation + re-acquisition |
| Extensibility | — | Novel filesystems (e.g. TAC3) as a profile/adapter, not core |

Core rule: the VM speaks a stable filesystem capability contract; the adapter
speaks the native filesystem. No filesystem may silently claim durability,
atomicity, identity, or recovery it cannot actually provide. The native probe
reports evidence and identity but never grants SLeeLa authority.

SLVM/9 remains an execution evolution, not a second language implementation.
