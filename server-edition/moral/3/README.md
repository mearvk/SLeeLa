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


## Named scan port and Mature Services

The Server Edition exposes a named basic scanning/service-discovery port, default **TCP 22220**. It is opened in the host firewall while the server is running and closed during shutdown. Multiplex-capable services can negotiate service capabilities through this port. Mature Services are then opened on their own native ports only while those services are active. The native firewall port, the scan port, and SLeeLa's logical HTTP PORT are separate namespaces. Linux uses UFW/firewalld as available; other supported operating systems use their corresponding firewall controller.
