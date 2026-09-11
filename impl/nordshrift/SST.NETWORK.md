# `.sst` Network Compile-Type Extension — NS-SST-NET-0001

This document extends the Nordshrift `.sst` model for network-oriented builds.

## Compile types

A network build may target one of these representations:

| `target-language` | Output | Execution / use |
|---|---|---|
| `sleela` | `.sleela` | Parsed and executable on the Sleela C core |
| `c` | C | Native C compilation against the Sleela runtime model |
| `java` | Java | Java compiler target |
| `xclass` | SecureJDK 28 `.xclass` | Structural XML class representation; ingestible by the existing XCI-0001 loader |

## Network components

`Endpoint`, `NIC`, `Link`, `Packet`, `Queue`, `Switch`, `Router`, `Fabric`,
`Listener`, `Connector`, `Gateway`, `LoadBalancer`, `Service`, `TLS`, and `DNS`.

## Required network ABI

The Sleela core exposes the network operations:

`LISTEN`, `ACCEPT`, `CONNECT`, `SOCKREAD`, `SOCKWRITE`, `SOCKCLOSE`.

These use VM-local socket handles and are independent of the host operating
system descriptor values.

## Compatibility rule

The network component list is descriptive metadata. Executable behavior must
lower to the existing Sleela language built-ins and VM network opcodes. This
keeps `.sleela`, C, Java, and `.xclass` representations structurally aligned.

## Example

```sst
#nordshrift 1.0
#sleela 1.0

sheet network-build:
  version 1.0.0
  author "Sleela Design Council"
  description "Network compilation surface"

source:
  root "src"
  glob "**/*.sleela"

target:
  root "out"
  layout mirror-source
  java-version 21
  target-language sleela
```

For `.xclass`, the selected representation is structural and is intended to be
round-tripped through the existing XCI-0001 loader back into Sleela.
