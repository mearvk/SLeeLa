# Bodi Witness Layer

Bodi is the witness layer to object change in SLeeLa. It gives a programmer a
stable semantic address for a known or knowable system and records the change
that is attempted against that address.

The preferred programming form is:

```java
Bodi.system("xxx").propagative("001").install("yyy");
```

The important distinction is that Bodi is the semantic layer while Java RMI
is a compatibility transport and registry mechanism underneath it.

## 1. Witness Model

A Bodi operation is understood as a sequence:

```text
system -> reference -> change -> method -> sequence -> starter -> man
```

These terms have operational meaning in the backend:

- **system** — the named system/object architecture being addressed.
- **reference** — the registry address used to resolve that system.
- **change** — the intended object operation.
- **method** — the operation to invoke.
- **sequence** — the propagation/order identifier, such as `001`.
- **starter** — the actor or subsystem that initiated the operation.
- **man** — the declared human/operator identity supplied by the caller.

A `BodiWitness` records this tuple together with a deterministic path and
creation timestamp. The witness does not replace the target object; it records
the relationship between an address and an object change.

## 2. RMI Equivalence

The older mental model can be expressed as:

```text
RMI.lookup("remote_name").put("datum")
```

Bodi makes the same architectural idea explicit:

```text
Bodi.system("remote_name").install("datum")
```

With propagation information:

```text
Bodi.system("remote_name")
    .propagative("001")
    .install("datum");
```

The RMI registry remains available through `pull()` and `push()` so existing
Java remote objects can participate without making RMI the programmer's
primary semantic vocabulary.

## 3. XML as the Agreed System Description

Bodi also accepts an agreed XML representation for network communication:

```xml
<bodi system="xxx"
      method="install"
      sequence="001"
      starter="network"
      man="operator">
    <datum>yyy</datum>
</bodi>
```

The schema is `src/implementations/_001_/bodi/bodi.xsd`.

The parser is intentionally small and deterministic. It reads the root
attributes, then takes `<datum>` when present. If `<datum>` is absent, child
element text is collected in document order. This provides the left-to-right /
top-to-bottom interpretation requested for the system description while
remaining compatible with ordinary XML tree structure.

External DTDs and external entities are disabled by the parser.

## 4. Network Duality

Bodi therefore has two complementary network views:

```text
                 Bodi system address
                         |
             +-----------+-----------+
             |                       |
       Java RMI registry        XML TCP endpoint
             |                       |
       Remote object            BodiChange
             |                       |
             +-----------+-----------+
                         |
                    BodiExtender
                         |
                    BodiWitness
```

RMI is useful for Java-native remote objects. XML is the agreed language-level
boundary for SLeeLa-aware systems that need a small, inspectable system
representation.

## 5. TCP Endpoint

`BodiNetworkServer` listens for one XML request per TCP connection and returns:

```xml
<bodi-response status="ok"><message>...</message></bodi-response>
```

Errors are returned with `status="error"`.

The listener uses a worker pool so the acceptor remains responsive while
individual requests perform blocking object operations. This is the same
blocking-I/O versus non-blocking-execution distinction used by SLeeLa's wider
network architecture: the socket itself is blocking, while requests execute
independently.

## 6. Configuration

Use `config/bodi.properties.example` as the starting configuration:

```properties
bind.address=127.0.0.1
rmi.port=8888
bodi.port=8890
bodi.enabled=false
```

The network listener is an explicit opt-in. A configuration with
`bodi.enabled=false` does not open the Bodi XML TCP port.

For an exposed service, select the intended interface explicitly and apply a
host firewall policy. Do not treat the XML endpoint as authenticated merely
because it is on a private network.

## 7. Installation Lip

`tools/bodi/install-bodi.sh` prepares a per-user Bodi installation directory,
creates the configuration file if it does not exist, and installs a launcher
configuration under `~/.sleela` by default.

The installer deliberately leaves the listener disabled until the operator
sets:

```properties
bodi.enabled=true
```

## 8. Backend Map

```text
Bodi.java
  |
  +-- system(name)
  |
  +-- startNetwork(config)
  |
  +-- pull/push (RMI compatibility)
  |
  v
BodiExtender.java
  |
  +-- registry lookup/bind
  +-- reflection metadata
  +-- invoke(BodiChange)
  +-- witness collection
  |
  +--> BodiWitness
  +--> BodiObjectReference

BodiXmlDocument.java <--> BodiNetworkServer.java
        ^                       ^
        |                       |
    bodi.xsd             BodiNetworkClient.java

BodiNetworkConfig.java <--> BodiNetworkBootstrap.java
        ^
        |
 config/bodi.properties.example
        ^
 tools/bodi/install-bodi.sh
```

## 9. Design Boundary

This implementation establishes the backend and protocol shape; it does not
claim that an arbitrary Java object is automatically a safe or meaningful
SLeeLa system. A system must be registered, its callable operation must be
known, and its XML request must conform to the agreed Bodi shape.

The next natural extension is a descriptor/discovery operation that exposes
an XML description of registered systems, methods, sequence identifiers and
capabilities before an `install` or other change is attempted.
