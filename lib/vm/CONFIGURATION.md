# VM 1–11 Configuration

The VM family uses the SLeeLa unified configuration root. Do not place a
second permanent VM configuration tree under `lib/vm/` in an installed
runtime.

The repository template is:

`lib/vm/vm-configuration.conf`

At installation/project runtime it is materialized as:

`<CANONICAL_CONFIG_ROOT>/vm/vm-<version>.conf`

where the canonical root is defined by `config/sleela.conf`.

The same configuration system selects:

- VM 1
- VM 2
- VM 3
- VM 4
- VM 5
- VM 6
- VM 7
- VM 8
- VM 9
- VM 10
- VM 11

The GC is common across the family, with per-VM configuration permitted for
documented compatibility settings.

## Important rule

Source-controlled defaults are templates. Runtime configuration is always
resolved through the canonical absolute configuration root. This prevents
`lib/vm`, `impl`, server editions, compiler tools, and launchers from
drifting into separate configuration locations.
