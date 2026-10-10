# SLeeLa Configuration Index

Shared/core configuration has one standard root: `config/`.

## Core
- `config/config-location.conf` — configuration-location policy
- `config/vm.conf` — VM generation/profile
- `config/slvm-runtime.conf` — runtime defaults
- `config/*.properties.example` — platform/example templates

## Component-local configuration
- `api/audio-mixer/`
- `http-3.0/`
- `http-8.0/`, `http-9.0/`
- `http/8.0/`, `http/9.0/`, `http/PRO/`
- `preferred-routers/`
- `server-edition/*/config/`
- `telephony-skya/config/`
- `sleela-virtual-machine/*/config/`

These are discoverable component configurations, not competing global roots.

## Generated OS configuration (OS Creator™)

OS Creator™ (`lib/os/os-creator/`) writes an **output** configuration set into
the OS development tree it produces, rooted at `/os-development` (not under this
repository). These are generated per-build, not repository configuration:

- `/os-development/config/os.conf` — the OS model's global identity/config
- `/os-development/firmware/uefi/uefi.conf`, `/firmware/bios/bios.cfg`
- `/os-development/boot/loader/loader.conf`, `/boot/kernel-loader/kernel-loader.conf`
- `/os-development/kernel/driver-loader/drivers.conf`
- `/os-development/userspace/os-loader/services.conf`
- `/os-development/desktop/gui-loader/session.conf` (desktop editions only)

They are emitted by the OS Creator™ piece emitters and documented in
`lib/os/os-creator/ARCHITECTURE.md`; they do not participate in the repository
configuration policy below.

## Rule for new configuration

1. Shared/runtime-wide configuration goes under root `config/`.
2. Component-private configuration stays with its component.
3. Every new global configuration file is documented here.
4. Installed packages expose `<install-root>/Config/`.
5. Administrator-managed configuration may use `/etc/sleela/`.
6. Generated OS trees (OS Creator™) carry their own `/os-development/config/`,
   which is build output, not repository configuration.
