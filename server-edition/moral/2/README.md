# SLeeLa Server and Service — Service 2

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
