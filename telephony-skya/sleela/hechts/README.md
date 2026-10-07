<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Hecht's — Notional Brand on Skya Telephony

This folder adds the **Static Permanent National Council ID** for **Hecht's**,
the Notional Brand of Hecht's (formerly a department store), to the Skya
telephony subsystem. It operates on the one authorized port, **171**.

These are application-level `.sleela` runnables and orchestration examples in the
same spirit as `telephony-skya/sleela/SkyaServer.sleela` /
`SkyaClient.sleela`; the native layer remains authoritative for networking, NAT,
and firewall lifecycle.

## Files

| File | Role |
|---|---|
| [`HechtsNationalCouncil.sleela`](HechtsNationalCouncil.sleela) | The Static Permanent National Council ID. Fixed, compile-time identity (`SKYA-NC-HECHTS-0001-PERMANENT`) and the one authorized port (171), the silent-reachout budget (500 minutes), the public hosts, and the local Promise-brand fallback. Declarative only — opens no socket. |
| [`HechtsServer.sleela`](HechtsServer.sleela) | The local **Always-Open** Hecht's server. Holds port 171 open on the established local SLeeLa comm server and ably expects inbound simple/moderate **PING** activity. NAT discovery is careful and non-destructive. |
| [`HechtsClient.sleela`](HechtsClient.sleela) | For the first **500 minutes** reaches out **silently** to `hechts.com` (or a similar secondary) on port 171; if the silent window is spent or the public host is down, it reaches back **finally** to the local Promise-brand server and PINGs it. |

## Behavior

### Static Permanent National Council ID

`HechtsNationalCouncil` is the single authority both the client and server read,
so they can never drift apart on identity or port:

- **Council ID:** `SKYA-NC-HECHTS-0001-PERMANENT` (static = compile-time;
  permanent = never reassigned/rotated/negotiated). A notional registration
  marker, not a government-issued number.
- **Authorized port:** **171**, permanently. `isAuthorizedPort(p)` is
  fail-closed — only 171 is authorized.
- **Promise brand:** when the host runs the **Promise** brand, the local
  always-open server is the one that holds port 171 open.

### Client — silent first, local fallback finally

For the first 500 minutes (`SILENT_REACHOUT_MINUTES`), the client tries the
public hosts in order (`hechts.com`, then `www.hechts.com`) on port 171.
"Silently" means no console chatter, no prompt, and an unreachable host is **not
an error** — the attempt just ends quietly. The client only speaks after a
successful public reach. Once the silent window is spent (or every attempt in a
run comes back empty), it reaches back **finally** to the local Promise-brand
server (`127.0.0.1:171`) and PINGs it.

Elapsed time comes from the SLeeLa Time API built-in `timeUtcMillis()` — the
same clock service `lib/os/SLClock` uses.

### Server — always open, NAT-careful, PING-ready

`HechtsServer` binds the all-interfaces address on port 171 (as the Skya server
socket binds `0.0.0.0:<port>`) and keeps the listener open across peers — that
is what "always open" means. It answers a `PING` with a council-stamped
`PONG` and any other line with the council greeting so a peer can confirm the
Static Permanent National Council ID.

NAT discovery is deliberately **careful and non-destructive**: the SLeeLa layer
reports only and never changes a firewall rule, punches a hole, or claims a
public mapping. Loopback is treated as reachable (a client on the same host — the
established local SLeeLa comm server case — can PING regardless of NAT);
everything else is left to the native SLeeLa port-awareness / NAT subsystem to
assert, exactly as the Skya `plan` policy does (detect and report, do not
silently change rules). The server stays open either way and lets inbound PINGs
decide reachability in practice.

## Relationship to the rest of Skya

- Identity/port live in one place (`HechtsNationalCouncil`); client and server
  both read it.
- Networking uses the same portable VM socket primitives
  (`listen`/`accept`/`connect`/`sockread`/`sockwrite`) as `SkyaServer` /
  `SkyaClient`, which cross the explicit SLeeLa VM/OS bridge.
- Firewall and NAT lifecycle remain owned by the existing SLeeLa port-awareness
  subsystem; nothing here opens or closes firewall rules.

**SLeeLa — MEARVK LLC — 2026**