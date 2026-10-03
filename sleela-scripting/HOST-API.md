# Sleela Script Host API

The host API is the security and interoperability boundary between Sleela
Script and the native SLeeLa runtime.

## Core functions

| Function | Purpose | Default |
|---|---|---|
| `print(value)` | write script output | allowed |
| `config.get(name)` | read resolved configuration | allowed |
| `env.get(name)` | read an environment variable | restricted |
| `fs.read(path)` | read an approved file | restricted |
| `fs.write(path,value)` | write an approved file | denied |
| `process.run(command)` | launch a process | denied |
| `vm.select(version)` | select VM 1–11 | restricted |
| `sleela.object(name)` | resolve a SLeeLa object | restricted |

The default policy is deliberately conservative.

## Capability registration

Hosts register a function under a stable name and assign a capability class.
Scripts can invoke only functions registered in the current context.

The capability table should be inherited from the SLeeLa security/runtime
policy rather than duplicated in this subsystem.

## Configuration

Host functions resolve configuration through the unified configuration root
defined by `config/CONFIGURATION.md`.

A script must never assume that its current working directory is the project
configuration directory.

## Process execution

`process.run` is an explicit capability, not a shell escape. A conforming
host should apply executable allow-lists, argument boundaries, working-directory
rules, environment rules, and resource limits before executing it.
