# Discord-2™ — SLeeLa Server and Service — Service 2 — v2.0.1

Service 2 is the expanded Server and Service package. It separates the
authoritative SLeeLa server program from configuration and lifecycle control.

## Layout

- serverd: configuration-driven service launcher.
- src/Server.sleela: authoritative SLeeLa service program.
- config/: explicit declarative configuration.
- service/: platform service definitions.
- install/: installation and removal files.

Runtime state remains in server-edition/inbox and server-edition/state.

## Configuration

The configuration is plain KEY=VALUE data. Unknown keys and invalid values
are rejected. Configuration is never evaluated as shell code.

Supported keys are engine, inbox, state, server_source, log, mode, tick,
foreground, sha256_manifest, and allow_network.

The default network policy is false. Service 2 does not itself create a
public listening socket.

## Server and Service

Server means the SLeeLa program that processes owned inputs. Service means
the lifecycle/configuration boundary that starts, constrains, records, and
stops that program. This follows the useful systems distinction that a
service is a software component providing functionality or participating in
a capability. NIST also recommends careful installation and configuration
before external exposure of server software. citeturn0search2turn0search12

## Integrity

The package prefers security/important-sha256-manifest.json and passes that
location to the SLeeLa engine. Configuration changes are therefore part of
the deployable baseline. NIST configuration-management guidance emphasizes
identifying, controlling, recording, and reporting controlled components and
configuration changes. citeturn0search9turn0search13

## Design Activity

Service 2 uses the shared SLeeLa Design Activity model. Correctness,
reproducibility, observability, safety, resource discipline, and
interoperability are engineering dimensions. Variance is a comparison against
a declared engineering reference profile; it is not scientific measurement
uncertainty.

## Installation

See INSTALL.md and the platform files under install/ and service/.

Example:

server-edition/moral/2/serverd --help
server-edition/moral/2/serverd --design-activity physics 98 95 100 95 85 85


## Programmatic theme

**Discord-2™** is the programmatic name and implementation theme assigned to
Service 2 in the SLeeLa Server Edition series. It is an implementation name
only and does not indicate sponsorship, endorsement, ownership, or technical
affiliation with Discord Inc.

Discord and related Discord marks are third-party trademarks. No Discord
logo, trade dress, or proprietary Discord asset is included by this naming
convention. Discord's published brand guidance states that permission is
required for uses of its marks beyond permitted cases. citeturn0search3turn0search2

## Module version

**Service 2 module: 2.0.1**

The port-awareness addition is a backward-compatible patch-level update to this implementation generation. Shared firewall lifecycle support is provided by port-awareness module **1.0.0**.

## Port awareness and firewall lifecycle

Service 2 declares `port`, `port_protocol`, `firewall`, and
`firewall_required`. The default is TCP port 19866 with automatic
host-firewall selection.

At startup Service 2 removes a stale Discord-2™ rule, opens the configured
port, and only then starts Server.sleela. On normal completion or failure,
the lifecycle cleanup closes that rule. The shared controller detects the
host OS/version and uses UFW or firewalld on Linux, PF on macOS, or the
Windows Defender Firewall PowerShell controller on Windows.

The firewall rule is owned by the Server Edition lifecycle and is not a
replacement for socket binding, NAT, TLS, or application authentication.
 
## HTTP multiplexing and large-file DOWNLOAD

Service 2 is wired to the shared `api/http-multiplexing/` source through
`server-edition/http/httpctl`. Its configuration now supports
`http_version`, `logical_port`, `download_size_bytes`, and `http_api`.
The logical PORT namespace is exactly 10^48 values and is independent of the
native firewall port. HTTP 2.1 uses HTTP/2 stream multiplexing. Files over
50,000,000 bytes select resumable DOWNLOAD mode with SESSION-ID, DATETIME,
FILE-ID, FILE-NAME, INDEX, OFFSET, and TOTAL-SIZE resume state.
