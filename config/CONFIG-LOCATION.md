# SLeeLa Configuration Location

SLeeLa uses one standard configuration-location policy.

## Canonical locations

- Project: `<project-root>/config/`
- Installed runtime: `<install-root>/Config/`
- Administrator/system-wide absolute root: `/etc/sleela/`

The policy is `config/config-location.conf`.

## Project identity

`config.project.name` identifies the project; the default is `SLeeLa`. The project name is an identity, not a guessed filesystem path. The project root is supplied separately.

## Resolution precedence

1. Explicit absolute configuration root or `SLEELA_CONFIG_ROOT`
2. Project root + `config.location.relative`
3. Installation root + `config.location.install_relative`
4. `config.location.absolute`

Environment variables:
- `SLEELA_PROJECT_NAME`
- `SLEELA_PROJECT_ROOT` (absolute)
- `SLEELA_INSTALL_ROOT` (absolute)
- `SLEELA_CONFIG_ROOT` (absolute)

Component-specific configuration may remain beside its component for packaging/isolation (for example `telephony-skya/config/` and `sleela-virtual-machine/*/config/`). New shared/core configuration belongs under root `config/`.

The resolver in `runtime/config_location.[ch]` is cross-platform and only resolves paths; it does not grant capabilities.
