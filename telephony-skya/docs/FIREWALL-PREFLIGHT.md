# Skya Firewall Preflight

## Purpose

Skya performs a non-destructive firewall preflight before the Windows 10+, macOS, and Linux client launch scripts start the JavaFX client surfaces.

## Windows 10+

`build/windows/firewall-check.ps1` queries Windows Firewall profiles with PowerShell and reports enabled state plus default inbound/outbound actions.

## macOS

`build/macos/firewall-check.sh` queries the macOS Application Firewall state through `/usr/libexec/ApplicationFirewall/socketfilterfw` when available.

## Linux / UFW

`build/linux/firewall-check.sh` reports `ufw status verbose` when UFW is installed.

## Policy

The preflight does **not** automatically enable, disable, open, or close firewall rules. This avoids silently changing host security policy.

Outbound client connections normally do not require an inbound firewall exception. Features that host a private group or server can require an inbound rule for the configured listening port. The future native transport layer should use the same port configuration when requesting or documenting such a rule.

Set `SKYA_FIREWALL_CHECK=0` to skip the preflight for controlled environments.

The check runs for both the normal client and remote-client launch scripts on all three supported platform families.
