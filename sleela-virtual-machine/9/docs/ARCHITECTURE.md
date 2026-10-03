# SLVM/9 Architecture

SLVM/9 takes SLVM/8's supervised execution model and makes filesystem behavior a first-class, negotiated capability.

## Layering

SLeeLa Source -> Compiler -> Artifact -> SLVM Supervisor -> Admission -> Filesystem Abstraction Layer -> OS Adapter -> Native Filesystem

The VM never assumes that pathname, inode, mount, flush, rename, transaction, or recovery semantics are universal. An adapter must explicitly report what the target filesystem and operating system can guarantee.

## Filesystem Abstraction Layer

The FAL describes filesystem identity, generation, capacity, block size, mount identity, feature bits, integrity, and whether durability is actually known.

The feature contract includes read/write access, atomic rename, transactions, durable flush, contextual identity, recovery, and native handles.

A filesystem may implement a subset. Unsupported operations are denied rather than emulated with an unsafe claim.

## TAC3 example

TAC3 is a particularly useful advanced adapter target because its documented architecture includes a versioned on-disk format, explicit superblock/extents, a read-only persistent mount phase, a FILE identity layer, HEALTH/ADMIN/RECOVERY regions, contextual identity, system-pointer relationships, and a separate policy for dynamic memory/swap behavior.

SLVM/9 should therefore discover TAC3 capabilities rather than assuming conventional inode semantics. For example, contextual identity can be carried through slvm9_context_identity_t, while recovery and durability are advertised independently.

## Operating-system adapters

Linux, Windows, and macOS receive separate adapter entry points. The common VM contract remains portable; native APIs stay below the adapter boundary.

The adapter is responsible for translating stable SLeeLa filesystem requests into native OS calls and reporting native errors without converting them into false success.

## Stale handles and generations

Filesystem generations and mount identity are checked before operations that depend on a previously observed filesystem state. Recovery, remount, replacement, or migration can invalidate old handles.

A stale generation returns SLVM9_STALE and requires rediscovery/re-acquisition.

## Durability rule

A successful write does not automatically mean durable storage. Durability is a separate capability and must be explicitly reported by the adapter.

## Security rule

Filesystem adapters never grant themselves SLeeLa capabilities. Admission and the existing SLVM/8 capability layer remain authoritative.
