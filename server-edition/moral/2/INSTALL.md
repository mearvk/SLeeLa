# Discord-2™ — Service 2 Installation

Service 2 provides installation artifacts for Linux/systemd, macOS/launchd,
and Windows 10+.

Linux:
1. Copy the package to /opt/sleela or another controlled prefix.
2. Copy config/server.conf.example to config/server.conf.
3. Set engine, source, state, log, and manifest paths.
4. Install service/sleela-server.service under systemd.
5. Validate the configuration before enabling the service.

macOS:
1. Install the package under a controlled prefix.
2. Create the launchd plist from service/com.mearvk.sleela-server.plist.
3. Load it only after configuration validation.

Windows:
1. Run install/windows-install.ps1 as Administrator.
2. The package installs configuration and service metadata.
3. Register serverd with an approved Windows service wrapper or host.

Installers do not download arbitrary software and do not silently enable
network listeners. Required dependencies remain explicit.

NIST server-security guidance recommends installing and configuring server
software carefully before network exposure, while configuration-management
guidance treats installation and configuration as controlled system changes.
citeturn0search12turn0search13


## Naming notice

Discord-2™ is the programmatic theme/name used by this SLeeLa implementation.
It is not presented as an official Discord product or service. Discord marks
remain the property of their respective owner. citeturn0search3

## Port awareness and firewall lifecycle

Service 2 declares `port`, `port_protocol`, `firewall`, and
`firewall_required`. The default is TCP port 19866 with automatic
host-firewall selection.

At startup Service 2 removes a stale Discord-2™ rule, opens the configured
port, and only then starts Server.sleela. On normal completion or failure,
the lifecycle cleanup closes that rule. The shared controller detects the
host OS/version and uses UFW or firewalld on Linux, PF on macOS, or the
Windows Defender Firewall PowerShell controller on Windows.

The firewall rule is owned by the Server Edition lifecycle and is not a
replacement for socket binding, NAT, TLS, or application authentication.

The installer also deploys `server-edition/port-awareness/`. See the repository-level `server-edition/PORT-AWARENESS.md` for the lifecycle contract.
