# SLeeLa HTTP Integration Guide

**Scope:** HTTP 1.0 through HTTP 9.0 module integration on Linux, Windows, and macOS.

> **Protocol status:** HTTP/1.0 and HTTP/1.1 are compatibility targets. SLeeLa HTTP 2.0 through 9.0 are project-specific SLeeLa protocol/application generations unless separately identified as an Internet standard. Installing a SLeeLa module does not change what the public Internet recognizes as HTTP.

## 1. Integration model

The intended lifecycle is:

```text
Detect OS/CPU
    -> select SLeeLa HTTP generation
    -> verify source/package
    -> build and self-test
    -> install beside existing generations
    -> configure
    -> optionally register service
    -> explicitly configure firewall/network access
    -> run interoperability test
    -> operate / upgrade / uninstall
```

Installation and service activation are separate operations. A package must not silently open network ports or start a listener.

All generations share the HTTP negotiation layer. When a peer does not advertise the requested SLeeLa generation and fallback is allowed, the compatibility layer can select HTTP 1.1 and then HTTP 1.0. Stronger security requirements must be allowed to fail closed rather than being silently weakened.

## 2. Module map

| Module | Integration role |
|---|---|
| HTTP 1.0 | Legacy/compatibility endpoint |
| HTTP 2.0 | SLeeLa application generation |
| HTTP 3.0 | SLeeLa generation with its own crypto, routing and station components |
| HTTP 4.0 | SLeeLa packet/application generation |
| HTTP 5.0 | SLeeLa application generation |
| HTTP 6.0 | SLeeLa application generation |
| HTTP 7.0 | SLeeLa semantic/assertion application generation |
| HTTP 8.0 | SLeeLa cryptographic/session generation |
| HTTP 9.0 | SLeeLa international identity/metadata generation |

The module-specific trees are `http-1.0/` through `http-9.0/`. The canonical mature implementations also appear under `http/`.

## 3. Linux

### Development/source installation

From the SLeeLa repository:

```sh
./http/install/sleela-http-install.sh --version 9.0 --source-root .
```

Or install all module trees:

```sh
./http/install/sleela-http-install.sh --all --source-root .
```

The default source/bootstrap prefix is:

```text
/usr/local/share/sleela/http/
```

For a non-privileged development installation:

```sh
./http/install/sleela-http-install.sh \
  --version 9.0 \
  --source-root . \
  --prefix "$HOME/.local/share/sleela/http"
```

### Native Linux packaging

Production Linux distributions should use native package conventions such as .deb or .rpm. systemd units should be installed through the package build and distribution mechanism, not improvised by the source installer. Debian's systemd guidance recommends installing units into the systemd unit directory and keeping activation explicit; unit files should include an appropriate `[Install]` section. citeturn0search0turn0search11

Example operational sequence for an administrator:

```sh
systemctl daemon-reload
systemctl status sleela-http-9
systemctl enable sleela-http-9
systemctl start sleela-http-9
```

The actual service name and unit must be supplied by the package being deployed. Do not create a service unit merely because the source tree exists.

### Linux firewall

Firewall configuration remains an administrator/deployment decision. Installing HTTP 9.0 must not automatically expose a listening socket to the network.

## 4. Windows 10 and Windows 11

### Source/bootstrap installation

From PowerShell:

```powershell
.\http\install\sleela-http-install.ps1 -Version 9.0 -SourceRoot .
```

All modules:

```powershell
.\http\install\sleela-http-install.ps1 -All -SourceRoot .
```

The installer records the Windows product name, current build and processor architecture.

### Windows 12 and future Windows releases

The integration design deliberately does **not** hard-code a nonexistent or unverified Windows 12 build number.

The Windows adapter should determine support from:

- required Win32/API availability;
- CPU architecture;
- compiler/runtime requirements;
- Windows Installer/service capabilities;
- filesystem and security-policy behavior;
- tested build ranges.

Microsoft's current Windows documentation applies its servicing model to Windows 10 and Windows 11. citeturn0search12

If Microsoft releases a product marketed as Windows 12, SLeeLa should add tested build information without changing the protocol source architecture.

### Native Windows packaging

Production deployments should use signed MSI/EXE or an enterprise deployment format. Microsoft documents Windows Installer as the standard Windows installation/configuration service for component management and corporate deployment. citeturn0search13

A Windows service package should use the Windows Service Control Manager rather than pretending a normal console process is a service. Microsoft documents the service entry point, ServiceMain and service control handler requirements for native service programs. citeturn0search14

### Windows firewall

Installation and firewall authorization remain separate. A deployment administrator should explicitly decide which executable, interface, address and port are permitted.

## 5. macOS

### Source/bootstrap installation

```sh
./http/install/sleela-http-install-macos.sh --version 9.0 --source-root .
```

For development:

```sh
./http/install/sleela-http-install.sh \
  --version 9.0 \
  --prefix "$HOME/.local/share/sleela/http"
```

### Native macOS packaging

Production distribution should use a signed `.pkg` and the normal Apple code-signing/notarization process appropriate to the distribution channel.

Daemonized deployments should use launchd. Source installation alone does not register a daemon.

### Network access

Do not treat package installation as authorization to expose a network service. Configure application and host security separately.

## 6. Architecture targets

The installer and release pipeline should distinguish:

- x86_64 / amd64;
- ARM64 / aarch64;
- other architectures only after explicit build/test support.

The protocol source should remain architecture-neutral wherever possible. Architecture-specific code belongs under the OS/build abstraction.

## 7. Configuration locations

Recommended platform locations:

| Platform | System configuration | Installed module data |
|---|---|---|
| Linux | /etc/sleela/http/ | /usr/local/share/sleela/http/ or native package path |
| Windows | %ProgramData%\SLeeLa\http\ | %ProgramFiles%\SLeeLa\http\ |
| macOS | /Library/Application Support/SLeeLa/http/ | /usr/local/share/sleela/http/ or package-defined location |

User-level development configurations may live under the user's platform-specific application-data directory.

Secrets, private keys and credentials must never be committed into the module source tree.

## 8. Service model

A SLeeLa HTTP module may run:

1. as a foreground development process;
2. as a user-level service;
3. as a system service;
4. embedded inside another SLeeLa application.

The protocol implementation should not assume that it owns PID 1, the Windows SCM, launchd, or a particular init system.

For Linux, systemd service units can express process supervision, dependencies and hardening controls. citeturn0search1turn0search5

## 9. Networking and ports

The integration layer must distinguish:

- compiled protocol capability;
- configured listener;
- operating-system firewall authorization;
- externally reachable network path.

These are four different conditions.

A successful installation does not imply that any of them except the first has occurred.

## 10. Security installation checklist

Before production use:

- verify release/source SHA-256;
- verify signatures where available;
- verify compiler and dependency versions;
- run module syntax/self-tests;
- review configuration;
- establish least-privilege service identity;
- protect private keys;
- explicitly configure firewall policy;
- test local connectivity;
- test peer negotiation;
- test HTTP 1.0/1.1 fallback if required;
- verify logs;
- verify clean shutdown;
- record the source commit and installed module version.

HTTP 8.0 cryptographic deployments additionally require the configured cryptographic provider and the HTTP 8 security policy to be satisfied.

HTTP 9.0 identity/frequency metadata is configuration data; installing HTTP 9.0 does not activate a radio receiver, police monitoring system, or other external communications equipment.

## 11. Upgrade model

Install the newer generation beside the existing one.

```text
Existing 1.0/1.1 compatibility
        |
        +-- install 7.0
        +-- install 8.0
        +-- install 9.0
        |
        +-- test negotiation
        +-- migrate application
        +-- retire old module when verified
```

Do not remove HTTP 1.0/1.1 compatibility merely because a newer SLeeLa generation has been installed.

## 12. Uninstallation

Native packages should be removed with the native package manager.

Source/bootstrap installations should use the recorded `INSTALL.MANIFEST` and remove only files owned by that installation.

Service registrations must be removed through the native service manager. Firewall rules must be reviewed separately.

## 13. Interoperability test plan

For each deployment target:

1. start with local loopback;
2. test the requested SLeeLa generation;
3. test explicit peer acceptance;
4. test fallback to 1.1;
5. test fallback to 1.0;
6. test rejection when fallback is prohibited;
7. test malformed/unsupported version input;
8. test restart;
9. test upgrade;
10. test uninstall;
11. test firewall-denied operation;
12. test non-privileged operation where supported.

The resulting report should record OS version/build, CPU architecture, compiler, SLeeLa commit, module generation, negotiated generation and test result.

## 14. Operator quick reference

### Linux

```sh
./http/install/sleela-http-install.sh --version 9.0 --source-root .
make -C http-9.0/build syntax
```

### Windows

```powershell
.\http\install\sleela-http-install.ps1 -Version 9.0 -SourceRoot .
```

### macOS

```sh
./http/install/sleela-http-install-macos.sh --version 9.0 --source-root .
```

### All module generations

Linux/macOS:

```sh
./http/install/sleela-http-install.sh --all --source-root .
```

Windows:

```powershell
.\http\install\sleela-http-install.ps1 -All -SourceRoot .
```

## 15. Integration principle

SLeeLa HTTP is intended to be **easy to install but conservative to activate**.

Installation should be simple.

Configuration should be explicit.

Security should be verifiable.

Network exposure should be deliberate.

Protocol negotiation should be observable.

OS integration should use the operating system's normal facilities rather than replacing them.

That separation is what allows HTTP 1.0 through 9.0 to coexist on modern computers without requiring every operating system to understand every SLeeLa generation natively.
