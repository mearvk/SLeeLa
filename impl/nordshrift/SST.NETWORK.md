# `.sst` Network Support — NS-SST-NET-0001

Nordshrift `.sst` sheets have a first-class `network` declaration block. The
network declaration is parsed by the same indentation-aware NS-SST parser as
every other section (a `network:` case in the section dispatch), so network
metadata is available to the transpiler without changing the existing
source/target grammar.

## Network object enum set

The supported `NetworkObject` enum is closed to these objects:

- `Endpoint`
- `NIC`
- `Link`
- `Packet`
- `Queue`
- `Switch`
- `Router`
- `Fabric`
- `Listener`
- `Connector`
- `Gateway`
- `LoadBalancer`
- `Service`
- `TLS`
- `DNS`

Unknown object names are rejected with `NSS-E-NET-001`.

## Transport enum set

The supported network transports are:

- `tcp`
- `udp`
- `tls`
- `quic`

Unknown transports are rejected with `NSS-E-NET-002`.

## Address-family enum set

Supported address families are:

- `ipv4`
- `ipv6`
- `dual`

Unknown address families are rejected with `NSS-E-NET-003`.

## SST syntax

A complete network declaration may be written directly in an `.sst` sheet:

```sst
network:
  objects: [Endpoint, NIC, Link, Packet, Queue, Switch, Router, Fabric, Listener, Connector, Gateway, LoadBalancer, Service, TLS, DNS]
  transports: [tcp, udp, tls, quic]
  address-family: dual
  tls: true
```

The singular forms are also supported for repeated declarations:

```sst
network:
  object: Endpoint
  object: Listener
  transport: tcp
  transport: tls
  address-family: ipv6
  tls: true
```

Duplicate enum values are coalesced.

## Sleela lowering

The network declaration supplies the SST compile-time network surface. Actual
executable network behavior is lowered by the existing Sleela front end to the
Sleela core network operations:

`LISTEN`, `ACCEPT`, `CONNECT`, `SOCKREAD`, `SOCKWRITE`, `SOCKCLOSE`.

The corresponding Sleela built-ins are `listen`, `accept`, `connect`,
`sockread`, `sockwrite`, and `sockclose`. Network built-ins require Sleela
syntax version `1.1` or newer; the current compiler supports `1.0` through
`1.1`.

Thus an SST sheet can declare the network object set while its `.sleela`
source contains the executable network methods that the SST build drives.

## Object structure

Every network object is also carried on the repo-root `SHEET.sheet` catalog
(section `network`, role `network`), so a declared network object has a home in
the shared Nordshrift object-compatibility / relevance structure alongside the
system and finance objects. Each converts to a **model** relevance for every
target — it is realized as a modeled network shape rather than a single target
keyword; executable behavior is lowered to the core socket ABI above.

## Diagnostics summary

| Code | Meaning |
|---|---|
| `NSS-E-NET-001` | unknown network object |
| `NSS-E-NET-002` | unknown network transport |
| `NSS-E-NET-003` | unknown network address-family |

## Current executable target

`target-language sleela` is the verified network-capable executable path. The
Sleela target is compiled to the C core and can execute the resulting runnable.
The existing C-level socket smoke test separately exercises TCP
listen/accept/connect/read/write/close behavior.

`xclass` remains documented as a future target extension; the SST network
object support does not claim that an `.xclass` emitter is complete.
