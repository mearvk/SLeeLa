# Discord-1™ — SLeeLa Server Launcher — sleelas

**Discord-1™** is the programmatic name for the native SLeeLa Server Launcher.
It is an SLeeLa implementation name only and is not an official Discord
product name or indication of affiliation with Discord Inc.

`SLeeLa` now has a native compiled server launcher named **`sleelas`**.

The launcher starts the authoritative Server Edition reference:

    server-edition/src/Server.sleela

It does not replace `Server.sleela`. The executable is the native process
boundary that locates the SLeeLa engine, establishes the Server Edition state
directories, applies the single-run lock, and starts the reference program.

## Platform targets

| Platform | Binary |
|---|---|
| Linux x86_64 | `sleelas` |
| macOS | `sleelas` |
| Windows 10+ x86_64 | `sleelas.exe` |

The source is C++17 and uses native process creation on each platform:
`fork/exec/waitpid` on POSIX and `CreateProcess/WaitForSingleObject` on
Windows.

## NAT-aware operation

The server can be deployed behind NAT without changing the authoritative
Server.sleela program. Supported deployment paths are direct addressing,
explicit port forwarding/PCP, native IPv6, persistent outbound TLS
rendezvous/reverse connectivity, and authenticated relay operation.

See NAT.md for the complete deployment model, CGN/double-NAT guidance,
keepalive behavior, security requirements, firewall checklist, failure modes,
and the roadmap for concrete transport adapters.

The compiled launcher includes the nat_aware C++ configuration module.
--nat-plan validates and prints the selected plan without making network
connections or changing router state.

For difficult NAT/CGN environments, the intended long-term path is an
authenticated outbound TLS channel to a public rendezvous service. This keeps
the local server behind the NAT while allowing the public service to associate
remote requests with the established outbound connection.

## NAT configuration

    SLEELA_NAT_MODE
    SLEELA_NAT_BIND_HOST
    SLEELA_NAT_BIND_PORT
    SLEELA_NAT_PUBLIC_HOST
    SLEELA_NAT_PUBLIC_PORT
    SLEELA_NAT_RENDEZVOUS
    SLEELA_NAT_KEEPALIVE
    SLEELA_NAT_TLS_REQUIRED

Example:

    export SLEELA_NAT_MODE=outbound
    export SLEELA_NAT_RENDEZVOUS=relay.example:443
    export SLEELA_NAT_KEEPALIVE=25
    export SLEELA_NAT_TLS_REQUIRED=true
    sleelas --nat-plan

--nat-plan is deliberately a plan/validation command. It does not perform NAT
traversal by itself.

## Engine resolution

`SLEELA_BIN` may explicitly identify the engine. Otherwise the launcher looks
for:

    bin/sleela
    impl/build/sleela

`SLEELA_ROOT` can explicitly identify the SLeeLa installation/repository root.

## Commands

    sleelas
    sleelas --tick
    sleelas --foreground
    sleelas --help

The launcher preserves the Server Edition's quiet, one-pass behavior. The
server itself remains local and does not become a public network listener.

## Compilation

Linux/macOS:

    cd api/server
    make

Windows (MinGW):

    cd api/server
    make

The GitHub Actions server-build workflow produces native compiled artifacts
for all three target platforms. Source commits contain the reproducible build
inputs; generated binaries are workflow artifacts rather than source files.

## Relationship to `serverd`

`serverd` remains the POSIX shell launcher. `sleelas` is the compiled,
cross-platform launcher for the same `Server.sleela` reference.

Both launch the same engine and server source; they do not implement separate
server semantics.

## Single-instance behavior

Before starting the engine, `sleelas` checks the Server Edition lock and its recorded owner process. If the owner is still alive, the launcher reports that the server is already running and exits successfully without starting another instance. A lock whose recorded process is no longer alive is treated as stale and may be replaced atomically.

## Single-instance behavior

Before starting the engine, `sleelas` checks the Server Edition lock and its recorded owner process. If the owner is still alive, the launcher reports that the server is already running and exits successfully without starting another instance. A lock whose recorded process is no longer alive is treated as stale and may be replaced atomically.


## Server Edition names

- **Discord-1™** — native compiled launcher/process boundary.
- **Discord-2™** — configured Service 2 server.
- **Discord-3™** — remedial Service 3 server.

Discord's marks and Brand Assets remain the property of Discord Inc.; this
repository does not include Discord logos, trade dress, or proprietary brand
assets. citeturn0search3turn0search2

## HTTP Server Edition capability

Discord-1™ uses the shared `server-edition/http/httpctl` capability validator
before launching Server.sleela. Set `SLEELA_HTTP_LOGICAL_PORT` and
`SLEELA_HTTP_DOWNLOAD_SIZE` to validate the logical application PORT and
transfer mode. HTTP 2.1 is used for Server Edition level 2 and HTTP 3.0 for
level 3. The native firewall port remains separate from the logical PORT.
