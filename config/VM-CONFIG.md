# SLeeLa VM Selection Configuration

config/vm.conf is the standard persistent selection point for choosing the SLeeLa VM generation.

## Version

Set vm.version = 11. Accepted generations are 1 through 11.

The setting selects the requested VM generation; it does not claim that every generation is a separate interpreter. SLeeLa uses /impl as the operational Core substrate while VM generations 1–11 provide progressively stronger execution, security, recovery, filesystem, and module-hosting layers.

## Precedence

1. explicit launcher/CLI selection
2. SLEELA_VM_VERSION environment variable
3. config/vm.conf
4. compiled default

With vm.strict = true, an invalid or unavailable requested generation must fail closed instead of silently falling back.

## Profile

vm.profile records who owns the persistent selection: user, administrator, or developer. It is descriptive policy metadata and does not grant capabilities.

The format is deliberately small key = value text so it works across Linux, Windows, and macOS without a third-party parser.
