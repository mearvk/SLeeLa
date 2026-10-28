# VM 1–11 Configuration

The VM family uses the SLeeLa unified configuration root. Do not place a
second permanent VM configuration tree under `lib/vm/` in an installed
runtime.

The repository template is:

`lib/vm/vm-configuration.conf`

At installation/project runtime it is materialized as:

`<CANONICAL_CONFIG_ROOT>/vm/vm-<version>.conf`

where the canonical root is defined by `config/sleela.conf`.

The same configuration system selects VM 1 through VM 11. The GC is common
across the family, with per-VM configuration permitted for documented
compatibility settings.

## Java interoperability is separate from VM generation

The selected SLeeLa VM generation (1–11) is not a Java version and does not
implicitly provide an embedded JVM. The current supported integration model is
an optional external Java process, launched only when the input path and
program signals require Java. Native SLeeLa source continues through the
normal SLeeLa execution path.

The canonical top-level `config/vm.conf` describes Java interoperability
policy. The per-VM template may declare whether Java delegation is permitted,
but must not claim that a generation embeds Java unless a tested JNI-enabled
build explicitly provides that capability.

Capabilities should be reported as separate states:

- Java executable discovered.
- Java process launch supported.
- AWT/Swing classes available (part of the JDK/runtime, but graphical display
  may still be unavailable).
- JavaFX modules and native libraries available.
- Embedded JVM available (false unless an explicit JNI build provides it).

## Important rule

Source-controlled defaults are templates. Runtime configuration is always
resolved through the canonical absolute configuration root. This prevents
`lib/vm`, `impl`, server editions, compiler tools, and launchers from
drifting into separate configuration locations.
