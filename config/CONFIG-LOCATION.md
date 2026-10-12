# SLeeLa Configuration Location

The authoritative location policy is config/config-location.conf, parsed by runtime/config_location.c.

## Canonical roots

- Project: <project-root>/.sleela/config/
- Installed package: <install-root>/Config/
- Unix-like system fallback: /etc/sleela/
- Windows system fallback: C:\\ProgramData\\SLeeLa\\Config\\

The repository config/ directory contains policy, templates, and examples; it is not the runtime default.

## Resolution precedence

1. Explicit absolute configuration-root argument.
2. Absolute SLEELA_CONFIG_ROOT environment override.
3. Absolute project root (caller argument or SLEELA_PROJECT_ROOT) plus config.location.relative.
4. Absolute install root (caller argument or SLEELA_INSTALL_ROOT) plus config.location.install_relative.
5. System-wide absolute fallback from the policy.

Invalid relative overrides, invalid project/install roots, unsafe relative policy paths, and paths that exceed the result buffer fail rather than silently truncating. An explicit argument outranks the environment override.

## Environment

SLEELA_PROJECT_NAME sets logical identity; SLEELA_PROJECT_ROOT and SLEELA_INSTALL_ROOT must be absolute; SLEELA_CONFIG_ROOT must be an absolute root override.

Call sleela_config_location_defaults(), optionally load config/config-location.conf with sleela_config_location_load_policy(), then call sleela_config_location_from_environment() and sleela_config_location_resolve(). The resolver selects a path; it does not create directories, grant capabilities, or verify permissions.

Component-private settings may remain beside their component. Shared settings use the canonical root. Legacy component configuration may only be read during explicit migration.
