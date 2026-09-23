# SLeeLa Server Edition Port Awareness

All Server Editions use the same lifecycle rule for network exposure:

- Server starts: the configured server port is opened in the host firewall.
- Server remains running: the rule remains present.
- Server stops normally: the server port is closed again.
- Startup removes any stale rule for the same SLeeLa Server Edition before opening a fresh rule.
- Firewall control is host- and OS-aware: Linux prefers UFW, then firewalld; macOS uses PF; Windows uses Windows Defender Firewall.
- A missing required firewall controller is a startup failure when firewall_required=true.

## Configuration

Each edition declares:

    port=19866
    port_protocol=tcp
    firewall=auto
    firewall_required=true

The port is configurable. The existing SLeeLa server/network vocabulary can therefore select another port without changing the lifecycle implementation.

## OS and version awareness

The port controller identifies Linux distribution and VERSION_ID from /etc/os-release, macOS release from sw_vers, and Windows release/build through PowerShell/CIM.

On Linux, Ubuntu/Debian-family systems normally use UFW when installed. Other Linux systems may use firewalld when available. On macOS the controller uses PF. On Windows it uses Windows Defender Firewall through PowerShell.

The detected platform/version is recorded in the lifecycle log.

## Rule ownership

Rules are uniquely named by:

    SLeeLa-<edition>-<protocol>-<port>

The controller only adds/removes its own rule. It does not flush or replace the host firewall policy.

## Lifecycle contract

    preflight
      |
      v
    remove stale SLeeLa rule
      |
      v
    open configured port
      |
      v
    start Server.sleela
      |
      +---- normal exit ----+
      |                     |
      +---- failure --------+
                            v
                      close port
                            |
                           done

An abrupt power loss or SIGKILL can prevent in-process cleanup. Every later startup therefore removes the previous SLeeLa rule for the same edition/port before opening a fresh rule.

## Security boundary

Port awareness controls the host firewall only. It does not grant arbitrary network access to SLeeLa source code, shell commands, or XML procedures. allow_network=false remains the default policy until an operator explicitly enables network use.

A firewall rule is not proof that an application is listening. Server socket binding, NAT, TLS, authentication, and application-level authorization remain separate controls.
