# Skya Linux Build

This directory is the Linux Skya build surface.

- `build.sh` builds `skya` and `skya-server`.
- `client.sh` launches the full `SkyaClientApp`, including rooms, private groups, video targets, search, configuration, and file/audio/video UI surfaces.
- `client_monitor.sh` launches `SkyaApp`.
- `remote-client.sh` launches the remote connection GUI.
- `firewall-check.sh` performs a non-destructive UFW preflight at client startup when UFW is installed.

The firewall check reports state but does not change firewall policy. Outbound client connections normally require no inbound rule; private-group/server hosting may require an inbound rule for the selected port.

The build output and branding are kept together under `telephony-skya/build/linux`.
