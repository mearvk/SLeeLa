# SLeeLa Unified Configuration

SLeeLa shares a configuration-location policy across runtime, compiler, decompiler, VM, garbage collector, and server components.

- Location policy: config/config-location.conf
- Runtime settings template: config/sleela.conf
- VM selection template: config/vm.conf

The repository config/ directory holds source policy and templates. The default runtime root is <PROJECT_ROOT>/.sleela/config/, not the repository's config/ directory.

## Canonical settings

| Setting | Meaning |
|---|---|
| config.project.name | Logical project identity |
| config.location.relative | Safe relative path under project root |
| config.location.install_relative | Safe relative path under install root |
| config.location.absolute | Absolute system fallback |
| SLEELA_PROJECT_ROOT | Absolute project root |
| SLEELA_INSTALL_ROOT | Absolute install root |
| SLEELA_CONFIG_ROOT | Absolute override, below an explicit caller argument |
| vm.version | Requested VM generation, 1 through 11 |
| vm.strict | Reject invalid VM version rather than silently substituting |

The C resolver parses location keys from config/config-location.conf. config/sleela.conf mirrors the paths and documents runtime settings; its INI settings are not parsed by the location resolver.

## Resolution order

1. Explicit absolute root supplied by caller.
2. Absolute SLEELA_CONFIG_ROOT, if set.
3. Project root plus config.location.relative.
4. Installation root plus config.location.install_relative.
5. System-wide absolute fallback.

Unsafe relative policy values and buffer truncation are errors. The resolver does not create directories, choose permissions, or grant capabilities.

VM generation defaults to 11; valid generations are 1 through 11. SLEELA_VM_VERSION can override file selection when applied by the launcher. vm.strict validates the version range but does not prove the implementation is installed; the launcher must check availability before execution.

New runtime code must not create competing configuration roots. Legacy configuration may only be read during explicit migration.

## Build and validation

From the repository root run: make config, make -C config test, and make -C config clean. Validation checks policy/template consistency and properties examples, then compiles and runs the C resolver smoke test without changing system files.
