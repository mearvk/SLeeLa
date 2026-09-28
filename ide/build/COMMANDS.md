# IDE Command Contract

Version: 0.2.0-dev

The IDE exposes these project operations through one command adapter:

`check`
`build`
`run`
`test`
`clean`
`diagnostics`

SLeeLa commands must invoke the repository's actual compiler/runtime entrypoints.

C and C++ operations use the configured native build system and compile database where available.

Java operations use the configured JDK/build system.

A mixed-language build reports each language-domain result separately and returns a combined project status. No successful native or Java sub-build may mask a failed SLeeLa build, and vice versa.