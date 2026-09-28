# SLeeLa Server Edition / Moral Source

The Server Edition is organized by implementation generation:

- `moral/2/` — configured Server and Service.
- `moral/3/` — remedial Server and Service with preflight, recovery, and
  evidence boundaries.

Service generations are additive. Service 3 does not silently replace Service
2.

Runtime state remains outside these source packages in `server-edition/inbox/`
and `server-edition/state/`.

The native `sleelas` launcher selects Service 2 by default and supports
explicit Service 3 selection with `SLEELA_SERVER_LEVEL=3`.

## Versioned Server Edition modules

The Server Edition now carries explicit module versions:

| Programmatic module | Path | Version |
|---|---|---:|
| **Discord-1™** | `api/server/` | **1.0.1** |
| **Discord-2™** | `server-edition/moral/2/` | **2.0.1** |
| **Discord-3™** | `server-edition/moral/3/` | **3.0.1** |
| Shared port awareness | `server-edition/port-awareness/` | **1.0.0** |
| Server Edition umbrella | `server-edition/` | **1.0.0** |

The patch-level increments on Discord-1™, Discord-2™, and Discord-3™ identify
the firewall-port lifecycle addition without changing their existing
implementation generation. The shared port-awareness controller has its own
major/minor/patch lifecycle.

## Discord™ server naming theme

The three Server Edition programmatic names are:

- **Discord-1™** — native `sleelas` server-launcher/process boundary.
- **Discord-2™** — Service 2 configured Server and Service implementation.
- **Discord-3™** — Service 3 remedial Server and Service implementation.

These are SLeeLa implementation names only. They do not imply affiliation,
sponsorship, endorsement, ownership, or authorization by Discord Inc. No
Discord logo or proprietary Discord Brand Asset is included. Discord's brand
guidance states that use of Discord Marks in product or organization names
requires permission except where otherwise permitted. citeturn0search3

## Port awareness

Discord-1™, Discord-2™, and Discord-3™ now share an explicit firewall-port
lifecycle. Each edition declares a port and protocol and is aware of the host
OS/version firewall controller.

The lifecycle is:

1. Remove a stale SLeeLa rule for that edition.
2. Open the configured port before the Server enters its run phase.
3. Keep the rule while the Server is active.
4. Close the rule during shutdown or failure cleanup.

Linux prefers UFW and then firewalld; macOS uses PF; Windows uses Windows
Defender Firewall. See `server-edition/PORT-AWARENESS.md`.

The firewall rule is deliberately separate from application socket binding,
NAT, TLS, authentication, and the existing `allow_network` policy.
