# Discord-3™ — SLeeLa Server and Service — Service 3 — v3.0.1

Service 3 is the next Server Edition implementation layer after Service 2. It
introduces a remedial server model: controlled preflight, execution, recovery
state, and completion evidence.

`moral/2/` remains an independent implementation. Service 3 is additive.

## Technology and implementation style

1. Preflight validates the engine, source, configuration, state, and integrity
   inputs.
2. Server.sleela owns request-processing semantics.
3. Recovery records lifecycle state without rewriting the implementation.
4. Evidence records what was attempted without treating logs as proof of
   cryptographic integrity.

NIST guidance treats configuration validation, verification, change control,
and recording configuration state as explicit engineering activities.
citeturn0search12turn0search13

## Configuration

Service 3 supports engine, inbox, state, server_source, log, recovery_log,
mode, tick, foreground, preflight, sha256_manifest, and allow_network.

The default network policy remains false.

## Remedial behavior

A remedial run does not silently substitute another executable or server.
If preflight fails, Service 3 records the failure and stops. If execution
fails, it records the exit status and leaves source and configuration intact.

## Design Activity

Service 3 uses the shared six-dimension Design Activity model. Its additional
preflight and recovery controls provide implementation evidence; they do not
redefine the scientific domains or claim scientific measurement uncertainty.

## Selecting Service 3

The native launcher supports an explicit implementation-level selection:

`SLEELA_SERVER_LEVEL=3 sleelas`

The supported level is explicit rather than discovered from arbitrary paths.


## Programmatic theme

**Discord-3™** is the programmatic name and implementation theme assigned to
Service 3. It identifies the remedial implementation generation and does not
indicate sponsorship, endorsement, ownership, or technical affiliation with
Discord Inc.

No Discord logo, trade dress, or proprietary Discord asset is included by the
naming convention. Discord's published brand guidance governs use of its marks. citeturn0search3turn0search2

## Module version

**Service 3 module: 3.0.1**

The port-awareness addition is a backward-compatible patch-level update to this implementation generation. Shared firewall lifecycle support is provided by port-awareness module **1.0.0**.

## Port awareness and firewall lifecycle

Service 3 declares `port`, `port_protocol`, `firewall`, and
`firewall_required`. The default is TCP port 19866 with automatic
host-firewall selection.

Preflight requires the shared port controller. Service 3 removes a stale
Discord-3™ rule, opens the configured port, records firewall failures in the
recovery log, and starts Server.sleela only after the port is open. Cleanup
closes the rule on normal or failed termination.

The controller is OS/version-aware: Linux prefers UFW then firewalld, macOS
uses PF, and Windows uses the Windows Defender Firewall PowerShell interface.

## HTTP multiplexing and large-file DOWNLOAD

Service 3 is wired to the shared `api/http-multiplexing/` source through
`server-edition/http/httpctl`. Its configuration now supports
`http_version`, `logical_port`, `download_size_bytes`, and `http_api`.
HTTP 3.0 uses HTTP/3/QUIC stream multiplexing. The logical PORT namespace is
exactly 10^48 values and is independent of the native firewall port. Files
over 50,000,000 bytes select resumable DOWNLOAD mode with SESSION-ID,
DATETIME, FILE-ID, FILE-NAME, INDEX, OFFSET, and TOTAL-SIZE resume state.


## Port lifecycle and service negotiation

Each Server Edition has a named basic scanning/service-discovery port (`scan_port`, default `22220`). The port is opened by the firewall controller while the server is running and closed when the server stops. Multiplex-capable services may negotiate their logical service through this port; a Mature Service may then open its own native port only for the duration of that service. Logical SLeeLa HTTP ports remain distinct from native firewall ports. UFW, firewalld, PF, or Windows Defender Firewall is used according to the host OS. A shutdown cleanup path must remove both the scan-port rule and any service-specific rules.


## Traffic and packet logging

The Server Edition maintains a traffic log for the named basic probe port (default TCP 2222), the basic scan/service-discovery port (default TCP 22220), the configured server port, and the International Strernary reference port 20000. When packet capture is available, the logger records timestamps, numeric source/destination IP addresses and ports, protocol information, and packet payload bytes in a diagnostic packet representation. HTTP traffic is therefore retained to the extent it is visible at the transport layer; encrypted HTTPS/HTTP/3 payloads remain encrypted rather than being decrypted by the logger. Packet capture is best-effort and requires the host's packet-capture privileges. Logging stops during server shutdown.

The port-20000 reference comes from mearvk/Java.Web.Server.Telnet.Front.Java.21, whose repository identifies Strernary on port 20000 and includes a Strernary liveness check. See the cited repository evidence in the project history.


### Privacy-preserving network records

The Server Edition consumes its raw traffic log at least daily and when it reaches 50 MiB. The retained `message.digest` is redacted for IP addresses, national-data fields, contact data, MAC addresses, and packet payloads. Records are stored under a UTC dated hierarchy:

`state/message-digest/DATE/TIME/message.digest`

A separate `known.connections` record is created under:

`state/known-connections/DATE/TIME/known.connections`

Active port verification is **disabled by default**. If an operator explicitly enables it with `SLEELA_ACTIVE_PROBING=true` and supplies `SLEELA_PROBE_ALLOWLIST`, the shared verifier checks only the configured known ports (default 2222, 22220, 19866, and the external Strernary reference 20000). Retained records do not contain the probed IP/hostname. This prevents the Server Edition from becoming an unrestricted Internet scanner.
