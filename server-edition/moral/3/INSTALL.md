# Discord-3™ — Service 3 Installation

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

## Windows lifecycle

On Windows 10+ the installed `run/windows-server.ps1` wrapper owns the same firewall lifecycle: it removes a stale edition rule, opens the configured port, runs the SLeeLa engine, and closes the rule in `finally` cleanup.

## Removal

Use the matching uninstall script. Runtime state is retained unless an
operator explicitly removes it.


## Naming notice

Discord-3™ is the programmatic theme/name used by this SLeeLa implementation.
It is not presented as an official Discord product or service. citeturn0search3

## Port awareness and firewall lifecycle

Service 3 declares `port`, `port_protocol`, `firewall`, and
`firewall_required`. The default is TCP port 19866 with automatic
host-firewall selection.

Preflight requires the shared port controller. Service 3 removes a stale
Discord-3™ rule, opens the configured port, records firewall failures in the
recovery log, and starts Server.sleela only after the port is open. Cleanup
closes the rule on normal or failed termination.

The controller is OS/version-aware: Linux prefers UFW then firewalld, macOS
uses PF, and Windows uses the Windows Defender Firewall PowerShell interface.

The installer also deploys `server-edition/port-awareness/`. See the repository-level `server-edition/PORT-AWARENESS.md` for the lifecycle contract.
