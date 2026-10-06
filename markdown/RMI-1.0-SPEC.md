# SLeeLa RMI 1.0 Specification

## 1. Status and Intent

SLeeLa RMI 1.0 defines the **common, conservative interoperability profile** for SLeeLa remote services. The objective is to use mechanisms already familiar to open-source software, Java infrastructure, Internet services, routers, network appliances, and physical communications systems.

Version 1.0 favors proven boundaries over novel protocol behavior.

The guiding rule is:

> **Use established standards at the boundary; keep SLeeLa authority behind the boundary.**

RMI 1.0 is therefore an integration specification, not a replacement for TCP/IP, Ethernet, IP routing, HTTP, TLS, Java RMI, or physical-layer standards.

## 2. Layered Model

SLeeLa RMI 1.0 follows a layered model:

```text
+--------------------------------------------------+
| SLeeLa operation / business semantics            |
+--------------------------------------------------+
| SLeeLa Java Connector                            |
+--------------------------------------------------+
| RMI / HTTP / Local Process transport             |
+--------------------------------------------------+
| TCP/IP or local operating-system process boundary |
+--------------------------------------------------+
| Ethernet / Wi-Fi / Fiber / Coax / other medium   |
+--------------------------------------------------+
| Physical transmission system                     |
+--------------------------------------------------+
```

The connector must not pretend that an application-layer operation is itself a physical or routing protocol.

## 3. Open-Source and Internet Compatibility Principle

The 1.0 profile should preferentially use interfaces and concepts that an ordinary open-source systems engineer can recognize and implement.

Relevant established families include:

- POSIX-style process and file concepts where applicable.
- TCP/IP networking.
- DNS naming where network names are required.
- Standard socket behavior.
- Java RMI/JRMP for the RMI transport profile.
- HTTP/HTTPS for the web transport profile.
- TLS for protected network channels.
- Conventional ports and service endpoints.
- Conventional request/response semantics.
- UTF-8 textual interchange for the baseline connector payload.

This profile intentionally avoids requiring a proprietary physical medium.

## 4. Lean Interoperability

RMI 1.0 is the compatibility baseline, but compatibility does not require unnecessary bytes or unnecessary work.

A compliant implementation should prefer:

- Small request and response envelopes.
- Direct operation-name lookup after service discovery.
- Reuse of established connections where the selected transport permits it.
- UTF-8 without redundant transcoding.
- Bounded buffering rather than repeated whole-message copying.
- Immediate dispatch once the required operation identity is known.
- Stable service names that can be cached locally after successful discovery.

The 1.0 wire model remains textual for broad interoperability. Implementations may internally pack, cache, index, or tokenize that representation for speed, provided the external 1.0 contract is unchanged.

## 5. Fast Naming

Service naming should be simple and deterministic.

The preferred path is:

```text
configured host
    -> configured registry/service endpoint
    -> service name
    -> cached remote reference
    -> operation dispatch
```

A client should not repeatedly perform discovery when a valid service reference is already known. DNS, registry lookup, and other naming mechanisms remain conventional lower-layer or deployment mechanisms.

A service name should be concise enough for efficient comparison and logging, while remaining stable and unambiguous within its service registry.

## 6. Physical Medium Independence

SLeeLa RMI 1.0 is medium-independent.

A compliant deployment may travel over:

- Single-mode or multimode fiber.
- Coaxial cable.
- Twisted-pair Ethernet.
- Wi-Fi.
- Cellular or other IP-capable radio networks.
- Satellite links.
- Virtual network interfaces.
- Loopback interfaces.
- Other media capable of carrying the selected network protocol.

Fiber, coax, copper, and wireless characteristics belong to lower layers. SLeeLa RMI should not make assumptions about attenuation, modulation, wavelength, connector type, shielding, or physical signaling.

## 7. Routing Independence

Routers are permitted to forward SLeeLa traffic exactly as they forward other traffic belonging to the selected transport protocol.

SLeeLa RMI 1.0 does not define a special router, switch, optical transport, Ethernet frame, or IP packet format.

A typical deployment may therefore be:

```text
SLeeLa Client
     |
 Ethernet / Wi-Fi
     |
 Router(s)
     |
 Fiber / Coax / Ethernet / Wireless
     |
 Router / Firewall
     |
 SLeeLa RMI Service
```

The application protocol remains independent of the particular physical path.

## 8. Java RMI Profile

Where Java RMI is selected, SLeeLa RMI 1.0 uses standard Java RMI concepts:

- `Remote` interfaces.
- `RemoteException` as the Java transport boundary.
- Java RMI registry service discovery.
- JRMP as the Java RMI transport where the runtime uses it.
- Exported remote objects.
- Explicit service names.
- Explicit registry ports.

The current SLeeLa implementation uses a narrow `SleelaRemote` contract rather than exposing arbitrary Java objects.

The baseline operations are:

```text
serviceName()
invoke(operation, arguments)
health()
```

## 9. Textual Application Boundary

RMI 1.0 uses textual operation arguments and textual results as its conservative common representation.

Example:

```text
operation = calculate
arguments = 42
result    = 1764
```

This deliberately avoids requiring Java serialization of SLeeLa-specific objects.

Typed values may be added by a later specification without invalidating the 1.0 wire model.

## 10. HTTP/Web Profile

A SLeeLa service may expose an HTTP gateway for Java applications or web-driven artifacts.

A baseline gateway may provide:

```text
GET  /sleela/health
POST /sleela/invoke
```

The operation is explicitly identified and the request body carries the textual arguments.

HTTPS should be used whenever the network boundary is not otherwise trusted.

## 11. Local Process Profile

A local Java program may invoke SLeeLa as a separate operating-system process.

This is considered a first-class 1.0 deployment model because it requires no network protocol and preserves a clean process boundary.

```text
Java Application
      |
Process Connector
      |
SLeeLa Executable
```

## 12. Service Discovery

RMI 1.0 prefers explicit service names and known endpoints.

DNS may be used to resolve a host name. A registry may then map a service name to the remote Java object.

The specification does not require multicast discovery, proprietary discovery packets, or automatic Internet-wide service discovery.

## 13. Ports

A deployment should use explicitly configured ports.

The conventional Java RMI registry port is `1099`, but deployments may use another permitted TCP port.

Application object ports must be controlled by the deployment and firewall policy.

## 14. Security Baseline

RMI 1.0 assumes that security is an architectural concern, not an optional afterthought.

A production deployment should provide:

- Network access controls.
- Firewall restrictions.
- Authentication where required.
- Confidentiality through an appropriate protected channel where required.
- Narrow remote interfaces.
- No arbitrary remote method execution.
- No unnecessary Java object exposure.
- Explicit service ownership and lifecycle.

A publicly reachable RMI registry should not be treated as a secure Internet service merely because it uses Java RMI.

## 15. Failure Semantics

The baseline system distinguishes:

1. Successful operation.
2. Application-level operation failure.
3. Transport failure.
4. Service unavailability.
5. Client-side presentation failure.

Health checks should not be confused with business operations.

## 16. Physical and Network Engineering Boundary

The following concerns remain outside SLeeLa RMI 1.0:

- Fiber wavelength and optical power.
- Coaxial impedance and RF signaling.
- Ethernet PHY selection.
- Wi-Fi radio modulation.
- Cable plant engineering.
- Router forwarding algorithms.
- Switch fabric implementation.
- IP routing protocol internals.
- TCP congestion-control implementation.

SLeeLa RMI consumes the resulting reliable application transport rather than replacing those technologies.

## 17. Performance Rule

RMI 1.0 should be the **smallest practical interoperable SLeeLa RMI**, not merely the oldest one.

Implementations should measure and minimize:

```text
name lookup
message packing
message unpacking
dispatch overhead
memory copies
connection setup
```

No optimization may change the defined 1.0 external semantics. Where a faster implementation technique conflicts with interoperability, the interoperable boundary wins.

## 18. Compliance Target

A SLeeLa RMI 1.0 implementation should be understandable to an engineer familiar with ordinary open-source networking and Java infrastructure.

The implementation should avoid unnecessary protocol novelty and should be portable across ordinary IP-capable network environments.

It should also be deliberately lean: fewer layers, fewer bytes, fewer copies, and fewer naming operations wherever the established standards permit.

## 19. Version Boundary

Version 1.0 is the **compatibility baseline**.

It establishes the simple, recognizable association between SLeeLa and existing open standards. Version 2.0 may add stronger typed semantics, richer capability negotiation, structured transport metadata, compact packing, and additional operational controls, but it must preserve the conceptual layering established here.
