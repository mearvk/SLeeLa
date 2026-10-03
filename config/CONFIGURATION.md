# SLeeLa Unified Configuration

## Purpose

SLeeLa now uses one configuration-location policy for the runtime, compiler,
decompiler, VM family, garbage collector, server editions, and other native
subsystems.

The authoritative repository configuration is:

`config/sleela.conf`

At runtime, configuration is resolved to **one absolute canonical directory**.
Components must read their configuration through that directory instead of
creating unrelated `config/`, `.config/`, or per-subsystem roots.

## Canonical location

The default project-relative location is:

`<PROJECT_ROOT>/.sleela/config/`

For example:

`/home/user/projects/SLeeLa/.sleela/config/`

The location itself is configurable, so the same project can use another
absolute location when required by an administrator or installation.

### Required concepts

| Setting | Meaning |
|---|---|
| `project.name` | Logical project name |
| `project.root` | Absolute root of the project |
| `config.mode` | `project` or `absolute` |
| `config.relative_path` | Path appended to the project root |
| `config.absolute_path` | Explicit canonical configuration directory |
| `install.root` | Optional absolute installation root |
| `vm.version` | Selected VM family, 1 through 11 |
| `vm.config_filename_pattern` | Location of the selected VM configuration |
| `SLEELA_CONFIG_ROOT` | Optional administrator environment override |

## Resolution rules

1. If `config.mode = absolute` and `config.absolute_path` is set, use it.
2. Otherwise, if `project.root` is absolute, resolve
   `project.root + config.relative_path`.
3. Otherwise, use the discovered installation/executable project root.
4. If `SLEELA_CONFIG_ROOT` is enabled and supplied, it may override the
   discovered location.
5. The final path **must be absolute** before any component opens a file.
6. All VM/GC/compiler/runtime configuration is located below that root.

The launcher should create the directory when it is safe and appropriate to do
so. It must not silently fall back to several competing configuration
directories.

## Project-name addressing

A caller may specify a project name and a project root:

`sleela config --project SLeeLa --project-root /opt/projects/SLeeLa show`

The project name identifies the configuration namespace; the absolute project
root identifies its filesystem location. The two are deliberately separate so
that two projects with the same executable can have independent configuration.

## VM configuration

The selected VM is controlled by:

`vm.version = "1"` through `"11"`

The per-VM configuration path is:

`<CANONICAL_CONFIG_ROOT>/vm/vm-<VERSION>.conf`

Examples:

- `.sleela/config/vm/vm-1.conf`
- `.sleela/config/vm/vm-7.conf`
- `.sleela/config/vm/vm-11.conf`

This permits the standard GC to be shared by VMs 1–11 while allowing a VM
profile to override thresholds and implementation details.

## Legacy configuration

Legacy component-specific configuration may be read only during an explicit
migration operation. New runtime code must not create or prefer legacy
locations.

## Implementation contract

Native C/C++ components should receive the resolved configuration root from
the common configuration layer. They should not independently interpret the
current working directory.

The configuration resolver should expose:

- canonical absolute root;
- project name;
- project root;
- installation root;
- selected VM version;
- selected VM configuration path;
- GC configuration path.

This makes configuration deterministic for terminals, services, IDE launches,
CI, installers, and embedded SLeeLa applications.
