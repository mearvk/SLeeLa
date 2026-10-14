# SLVM/9 Filesystem Adapter Contract

## Linux

Provide a native adapter around Linux filesystem APIs and mount metadata. It should support ordinary POSIX-like filesystems while allowing custom kernel filesystems to advertise additional feature bits.

The adapter must not reduce a custom filesystem to ext-like assumptions when its identity, recovery, or durability semantics differ.

## Windows

Provide a native adapter around Windows file handles, volume identity, filesystem information, and flush semantics. Native HANDLE lifetime remains below the SLeeLa capability boundary.

## macOS

Provide a native adapter around Darwin/POSIX filesystem facilities and native volume metadata. APFS-specific behavior may be reported as capabilities rather than hard-coded into the VM.

## TAC3 / custom filesystem profile

A custom filesystem such as TAC3 should register a profile describing its filesystem type, version, contextual identity support, recovery support, durability status, and any filesystem-specific administrative namespace.

The profile is descriptive. It does not grant authority.

## Future adapters

The adapter ABI should permit additional operating systems and filesystems without changing SLeeLa language semantics. New capabilities should be negotiated by feature bit/version rather than inferred from a filename extension or path.
