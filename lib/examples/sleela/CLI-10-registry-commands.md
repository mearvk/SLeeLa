# Registry commands

Start with read-only inspection:

```sh
sleela registry
sleela registry review
sleela registry get KEY
sleela registry feature NAME
```

The command family also documents `set`, `save`, and `export`:

```sh
sleela registry set KEY VALUE
sleela registry save
sleela registry export
```

Replace placeholders with keys/features that exist in the registry. Treat
`set` and `save` as mutations and inspect the registry guide first.


## Registry 3.0 native record tool

The first versioned record CLI is a separate native command so existing sleela registry scripts keep their behavior:

    registry/build/sleela-registry3 init registry-v2.sleelareg 0123456789abcdef0123456789abcdef
    registry/build/sleela-registry3 add registry-v2.sleelareg 11111111111111111111111111111111 sleela.runtime service "Runtime Core"
    registry/build/sleela-registry3 identify registry-v2.sleelareg sleela.runtime "Runtime Core"
    registry/build/sleela-registry3 describe registry-v2.sleelareg 11111111111111111111111111111111
    registry/build/sleela-registry3 verify registry-v2.sleelareg
    registry/build/sleela-registry3 review registry-v2.sleelareg
    registry/build/sleela-registry3 migrate-v1 old.sleelareg new-v2.sleelareg 0123456789abcdef0123456789abcdef

Migration writes a new destination; it does not overwrite the source file. Verify checks the SLREG 2 structure and references; it is not a digital-signature or authorization check. Registry resolution and verification do not execute a program or grant permission.

See [Registry 3.0 architecture](../../registry/REGISTRY-3.0-ARCHITECTURE.md), [native binding contract](../../registry/REGISTRY-3.0-BINDINGS.md), and [Modium catalog contract](../../registry/REGISTRY-3.0-MODIUM.md).
