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

## Rule for new configuration

1. Shared/runtime-wide configuration goes under root `config/`.
2. Component-private configuration stays with its component.
3. Every new global configuration file is documented here.
4. Installed packages expose `<install-root>/Config/`.
5. Administrator-managed configuration may use `/etc/sleela/`.
