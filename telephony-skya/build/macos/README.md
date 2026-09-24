# Skya macOS Build

This directory is the macOS-specific Skya build surface.

- `build.sh` builds `skya` and `skya-server`.
- `client.sh` launches the full `SkyaClientApp`, including rooms, private groups, video targets, search, configuration, and file/audio/video UI surfaces.
- `client_monitor.sh` launches `SkyaApp`.
- `remote-client.sh` launches the remote connection GUI.
- `firewall-check.sh` performs a non-destructive macOS Application Firewall preflight at client startup.
- `assets/skya-logo-blue.jpeg` is staged automatically from the repository-owned logo.

The firewall check reports state but does not silently change firewall policy. Outbound client connections normally require no inbound rule; private-group/server hosting may require allowing the Skya executable/application.

The build output and branding are kept together under `telephony-skya/build/macos`.
