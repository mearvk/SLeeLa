# SLeeLa VM Selection Configuration

config/vm.conf is the source template for persistent VM selection. A launcher loads it from the resolved configuration root or uses the repository template during development.

## Supported versions

vm.version accepts integer generations 1 through 11; default is 11. This selects a requested generation, not a guarantee that its runtime artifacts are installed. /impl remains the operational Core substrate while VM generations provide their defined execution and hosting layers.

## Selection precedence

1. Explicit launcher/CLI VM selection.
2. SLEELA_VM_VERSION environment override, when applied by the launcher.
3. vm.version in the selected vm.conf.
4. Compiled default (11).

With vm.strict = true, invalid versions must fail validation rather than be silently changed. A launcher must separately verify that the requested VM is available; the current C parser validates the version range, not installed-artifact presence.

vm.profile is descriptive ownership metadata: user, administrator, or developer. It does not grant capabilities. The key/value format is intended to be portable across Linux, Windows, and macOS. Validate with make config from the repository root.

`config/slvm-runtime.conf` is a separate SLVM/6 runtime profile (`vm.instance = 6`), not a competing global default. It applies only when a deployment explicitly selects that SLVM/6 profile; `config/vm.conf` remains the general default selection (generation 11).
