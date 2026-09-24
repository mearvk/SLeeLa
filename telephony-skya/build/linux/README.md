# Skya Linux Build

This directory is the Linux-specific Skya build surface.

- `build.sh` builds `skya` and `skya-server`.
- `client.sh` launches `SkyaClientApp`.
- `client_monitor.sh` launches `SkyaApp`.
- `remote-client.sh` launches the remote connection GUI.
- `assets/skya-logo-blue.jpeg` is staged automatically from the repository-owned logo.

The build is self-contained under `telephony-skya/build/linux`; source remains under `telephony-skya/native` and JavaFX remains under `telephony-skya/javafx`.
