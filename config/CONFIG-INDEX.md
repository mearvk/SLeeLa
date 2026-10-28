# SLeeLa Configuration Index

Shared configuration policy and templates live in repository config/. Runtime configuration is resolved separately to one canonical absolute root.

## Global policy and templates

- config/config-location.conf — authoritative location policy parsed by the C resolver
- config/sleela.conf — unified runtime settings and mirrored path policy
- config/vm.conf — persistent VM selection defaults
- config/slvm-runtime.conf — SLVM/6 runtime profile
- config/*.properties.example — platform, BODI, and common-runtime examples
- config/CONFIGURATION.md — unified configuration contract
- config/CONFIG-LOCATION.md — path resolution and precedence
- config/VM-CONFIG.md — VM selection contract
- config/Makefile — configuration validation, resolver smoke test, and VM-selection smoke test
- scripts/validate-config.py — cross-template and properties-example validation
- .github/workflows/config-ci.yml — Linux/macOS configuration CI

## Canonical runtime locations

- Project: <project-root>/.sleela/config/
- Installed package: <install-root>/Config/
- Unix-like system fallback: /etc/sleela/
- Windows system fallback: C:\\ProgramData\\SLeeLa\\Config\\

The repository source config/ directory is not the default runtime root.

## Component-local configuration

Component-private settings may stay with their component for packaging or isolation. Examples include api/audio-mixer/, http-3.0/, http-8.0/, http-9.0/, http/8.0/, http/9.0/, http/PRO/, preferred-routers/, server-edition/*/config/, telephony-skya/config/, and sleela-virtual-machine/*/config/. These are component configurations, not competing global roots.

## Generated OS configuration (OS Creator™)

OS Creator™ (lib/operating-system/os-creator/) emits configuration into the generated OS development tree rooted at /os-development, outside this repository: /os-development/config/os.conf; firmware/uefi/uefi.conf and firmware/bios/bios.cfg; boot/loader/loader.conf and boot/kernel-loader/kernel-loader.conf; kernel/driver-loader/drivers.conf; userspace/os-loader/services.conf; desktop/gui-loader/session.conf (desktop editions). These outputs are documented in lib/operating-system/os-creator/ARCHITECTURE.md and do not participate in repository configuration resolution.

## Rules

1. Shared/runtime-wide policy goes in root config/.
2. Component-private settings stay with their component.
3. Document every new global policy/template here.
4. Installed packages expose <install-root>/Config/.
5. System fallback is /etc/sleela/ on Unix-like systems and C:\\ProgramData\\SLeeLa\\Config\\ on Windows.
6. Generated OS configuration is build output, not repository configuration.
7. Run make config before submitting configuration changes.
