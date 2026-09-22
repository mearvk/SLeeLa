# SLeeLa Server NAT-Aware Operation

## Purpose

The SLeeLa Server Edition remains an authoritative local server program. A deployment may nevertheless need to make that server reachable when its host is behind a home router, enterprise firewall, double NAT, or carrier-grade NAT (CGN).

NAT awareness belongs at the deployment and transport boundary. Server.sleela should continue to own application semantics while a native C/C++ transport layer supplies reachability.

## Supported deployment paths

### Direct public addressing

Use this when the host already has a globally reachable address and the firewall permits the service.

No NAT traversal is supplied by sleelas in this mode.

### Port forwarding

A conventional NAT router can expose an internal server with:

    public-address:public-port -> private-address:private-port

The mapping may be static or, where the network authorizes it, requested dynamically through PCP. PCP is specifically designed to allow hosts behind NAT or CGN to create external-to-internal mappings for server operation.

UPnP IGD may be added as an optional router adapter. Automatic mappings must be explicit, bounded, logged, and removable. Never assume UPnP is available.

### Native IPv6

If the host has globally reachable IPv6, the service may use IPv6 without requiring IPv4 port forwarding.

IPv6 still requires deliberate binding and firewall policy. Global IPv6 reachability must not be treated as permission to expose unrelated local services.

### Persistent outbound reverse connection

This is the primary long-term design for difficult NAT environments.

    private SLeeLa server
            |
            | outbound authenticated TLS
            v
    public rendezvous / reverse service
            |
            +---- authorized remote clients

The private host initiates the connection. The public service associates authenticated remote requests with the established outbound channel.

The current nat_aware C++ module defines and validates this deployment contract. It does not silently create a tunnel or connect to an arbitrary endpoint.

### Public relay

When direct inbound mapping and outbound reverse connectivity are insufficient, a public relay can carry traffic between the private server and remote clients.

TURN is a standardized example of this model. Direct traversal can be attempted first and relay used when a direct path cannot be established.

A relay increases bandwidth, latency, and operating cost, so it should be an explicit deployment choice.

## CGN and double NAT

A local private address is not the only NAT problem. An ISP may place subscribers behind a carrier-grade NAT, producing another translation layer.

The deployment should assume that ordinary home-router forwarding may not be sufficient when the router's WAN address is itself private or belongs to the shared carrier address space 100.64.0.0/10.

For CGN or double-NAT environments use one or more of:

- provider-supported PCP;
- globally reachable IPv6;
- persistent outbound reverse connectivity;
- a VPN/private overlay;
- a public relay.

## NAT mapping lifetime

NAT mappings can expire when traffic is idle. A persistent outbound connection therefore needs a configurable heartbeat/keepalive.

The SLeeLa native module defaults to 25 seconds and accepts 5 through 3600 seconds. This is only a starting configuration; the actual interval must be selected for the network and transport.

The connection manager should:

1. send authenticated keepalives;
2. detect a broken channel;
3. reconnect with bounded backoff;
4. re-register the server identity;
5. avoid creating unlimited reconnect traffic;
6. record safe health information.

## UDP traversal

For UDP-oriented services, STUN/ICE can be used to discover candidate paths and TURN can provide a relay when direct connectivity fails.

SLeeLa should not invent a proprietary NAT-punching protocol when established standards are appropriate.

## Security boundary

NAT traversal is not authorization.

An Internet-facing deployment MUST still provide:

- TLS for control and data channels;
- server identity verification;
- client authentication or capability authorization;
- replay protection;
- request-size and connection-rate limits;
- explicit route authorization;
- safe audit logging;
- key rotation and revocation;
- host and upstream firewall policy;
- a kill switch for the reverse or relay path.

Outbound and relay modes in the current native configuration reject TLS-disabled operation.

Do not place router administrator passwords, private keys, API tokens, or database passwords in NAT configuration variables unless the deployment's secret-management policy explicitly permits it. Prefer protected service configuration or an operating-system secret facility.

## Configuration

The compiled sleelas launcher accepts:

| Variable | Meaning |
|---|---|
| SLEELA_NAT_MODE | direct, port-forward, ipv6, outbound, or relay |
| SLEELA_NAT_BIND_HOST | local service bind address |
| SLEELA_NAT_BIND_PORT | local service port |
| SLEELA_NAT_PUBLIC_HOST | public host for port-forward mode |
| SLEELA_NAT_PUBLIC_PORT | public port for port-forward mode |
| SLEELA_NAT_RENDEZVOUS | trusted public rendezvous or relay endpoint |
| SLEELA_NAT_KEEPALIVE | keepalive interval, 5–3600 seconds |
| SLEELA_NAT_TLS_REQUIRED | TLS requirement |

### Port-forward example

    export SLEELA_NAT_MODE=port-forward
    export SLEELA_NAT_BIND_HOST=127.0.0.1
    export SLEELA_NAT_BIND_PORT=8080
    export SLEELA_NAT_PUBLIC_HOST=server.example
    export SLEELA_NAT_PUBLIC_PORT=8443
    sleelas --nat-plan

### Outbound example

    export SLEELA_NAT_MODE=outbound
    export SLEELA_NAT_RENDEZVOUS=relay.example:443
    export SLEELA_NAT_KEEPALIVE=25
    export SLEELA_NAT_TLS_REQUIRED=true
    sleelas --nat-plan

The nat-plan command validates configuration and reports the intended path. It does not open sockets, modify router state, punch a hole, or connect to the Internet.

## Router and firewall checklist

For manual port forwarding:

1. Give the server a stable LAN address or DHCP reservation.
2. Bind only the required local interface and port.
3. Create one explicit protocol/port mapping.
4. Permit the same port in the host firewall.
5. Permit the same port in the upstream firewall.
6. Test from an external network.
7. Test NAT loopback/hairpin behavior separately if local clients must use the public hostname.
8. Monitor service health and mapping changes.
9. Remove the mapping when the service is disabled.

For PCP or another automatic mapping adapter:

1. Request only the required protocol and port.
2. Bound the requested lifetime and refresh interval.
3. Verify the returned external address and port.
4. Publish the endpoint through a trusted rendezvous mechanism.
5. Log mapping creation and removal without logging secrets.
6. Remove the mapping during orderly shutdown.

## Reverse service requirements

A production reverse service should provide:

- public DNS;
- TLS;
- authenticated enrollment;
- per-server identity;
- persistent connection management;
- heartbeat and timeout handling;
- bounded reconnect backoff;
- request multiplexing;
- route authorization;
- rate limiting;
- audit logging;
- health/status reporting;
- identity revocation;
- graceful disconnect;
- optional relay-region selection.

A server must not trust an arbitrary command merely because it arrived through a reverse connection.

## Failure behavior

| Condition | Expected result |
|---|---|
| NAT mapping unavailable | Remain local and report unreachable state |
| Rendezvous unavailable | Retry with bounded backoff |
| TLS validation failure | Fail closed |
| Authentication failure | Reject the channel |
| Mapping expires | Refresh or re-establish authorized connectivity |
| CGN blocks inbound mapping | Use reverse, IPv6, VPN, or relay |
| Public address changes | Re-register endpoint |
| Duplicate sleelas | Existing single-instance guard prevents a second launcher |
| Local server stops | Remote channel closes |

## Native C++ implementation boundary

api/server/nat_aware.h and api/server/nat_aware.cpp currently provide:

- a closed NAT deployment vocabulary;
- bounded environment parsing;
- port validation;
- keepalive validation;
- TLS enforcement for outbound/relay modes;
- deterministic deployment-plan generation.

The module deliberately does not yet implement:

- router administration;
- UPnP IGD control;
- PCP packet exchange;
- STUN;
- TURN;
- TLS socket transport;
- the public rendezvous server;
- reverse-channel request multiplexing.

Those should be separate, auditable transport adapters.

## SLeeLa architecture objects

NAT-aware deployment is represented as a deployment/network boundary rather than as a new application-language primitive.

A Nordshrift object can declare the intended transport without causing network side effects:

    object deployment NatAwareServer:
      type nat-aware-server
      source "server-edition"
      target "rendezvous"
      inputs: [bind, endpoint, keepalive, tls]
      outputs: [reachability, health]
      property transport outbound
      property authorization required
      property tls required

The object remains declarative. It does not open a port, modify a router, start a relay, or send traffic.

## Standards

The implementation plan follows established NAT traversal standards rather than inventing a private protocol:

- RFC 6887 — Port Control Protocol (PCP)
- RFC 6888 — Common Requirements for Carrier-Grade NATs
- RFC 6598 — Shared Address Space
- RFC 5766 — TURN
- RFC 6062 — TURN TCP allocations
- RFC 6314 — NAT traversal practices and ICE/STUN/TURN discussion

These references should be rechecked when concrete transport adapters are implemented.

## Deployment principle

The desired end state is:

    Server.sleela
        |
        v
    local authoritative processing
        |
        v
    native SLeeLa transport boundary
        |
        +--> direct / port-forward / IPv6
        |
        +--> outbound authenticated reverse channel
        |
        +--> relay fallback

This allows SLeeLa Server Edition to remain useful on ordinary LANs while also providing a controlled path for servers located behind NAT, double NAT, enterprise firewalls, and CGN.
