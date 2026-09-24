# Skya Windows Build

This directory is the Windows 10+ Skya build surface.

- `build.ps1` builds `skya.exe` and `skya-server.exe`.
- `client.ps1` launches the full `SkyaClientApp`, including rooms, private groups, video targets, search, configuration, and file/audio/video UI surfaces.
- `client_monitor.ps1` launches `SkyaApp`.
- `remote-client.ps1` launches the remote connection GUI.
- `firewall-check.ps1` performs a non-destructive Windows Firewall preflight at client startup.
- `assets/skya-logo-blue.jpeg` is staged automatically from the repository-owned logo.

The firewall check reports profile state but does not silently change firewall policy. Outbound client connections normally require no inbound rule; private-group/server hosting may require an inbound rule for the selected port.

The build output and branding are kept together under `telephony-skya/build/windows`.
