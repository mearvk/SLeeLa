# Service 3 Installation

Service 3 provides preflight, execution, recovery logging, and platform
lifecycle definitions.

## Linux

Run `install/linux-install.sh` from the repository. It copies the package
to the selected prefix, creates its configuration, installs the systemd unit,
and does not enable a public listener.

## macOS

Run `install/macos-install.sh`. It installs the package and creates the
launchd definition under the current user's LaunchAgents directory.

## Windows 10+

Run `install/windows-install.ps1` in an elevated PowerShell session. It
installs the package and produces service metadata for an approved service
host or wrapper.

## Preflight

Run:

`server-edition/moral/3/serverd --preflight`

NIST SP 800-70 Rev. 5 describes configuration checklists as mechanisms for
secure configuration, verification, and detection of unauthorized changes.
Service 3 applies the same general pattern to its controlled baseline.
citeturn0search2turn0search3

## Removal

Use the matching uninstall script. Runtime state is retained unless an
operator explicitly removes it.
