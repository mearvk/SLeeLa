# Skya Platform Builds

The `telephony-skya/build` tree separates the normal Skya Client GUI from the administrative Client Monitor.

## Client vs Client Monitor

The **Client** is the normal, non-administrative Skya user experience. It launches the JavaFX `SkyaClientApp` and provides the user-facing communications surface:

- Chat
- Video
- Audio
- File Transfer
- connection and room controls

The normal Client does **not** expose administrative Start, Pause, or Stop controls.

The **Client Monitor** is the separate administrative JavaFX GUI. It uses `SkyaApp` and provides local SLeeLa circuit/process monitoring and administrative lifecycle controls.

This distinction is intentional:

```text
client.sh
    ↓
SkyaClientApp
    ↓
Chat / Video / Audio / File Transfer
    ↓
SLeeLa / Skya client runtime

client_monitor.sh
    ↓
SkyaApp
    ↓
SLeeLa SkyaClient.sleela circuit / administrative monitoring
```

The normal Client is therefore the default user interface; the Monitor is an administrative interface.

## Linux

`./telephony-skya/build/linux/build.sh` — builds native `skya` and `skya-server`.

`./telephony-skya/build/linux/client.sh` — builds the native components and launches the normal JavaFX Skya Client.

`./telephony-skya/build/linux/client_monitor.sh` — builds the native components and launches the administrative JavaFX Client Monitor.

`./telephony-skya/build/linux/remote-client.sh` — starts the Remote Server Connection GUI.

## Windows 10+

`powershell -ExecutionPolicy Bypass -File .\\telephony-skya\\build\\windows\\build.ps1` — builds native executables.

`powershell -ExecutionPolicy Bypass -File .\\telephony-skya\\build\\windows\\client.ps1` — launches the normal Skya Client GUI.

`powershell -ExecutionPolicy Bypass -File .\\telephony-skya\\build\\windows\\client_monitor.ps1` — launches the administrative Client Monitor.

`powershell -ExecutionPolicy Bypass -File .\\telephony-skya\\build\\windows\\remote-client.ps1` — starts the Remote Server Connection GUI.

## macOS

`./telephony-skya/build/macos/build.sh` — builds native `skya` and `skya-server`.

`./telephony-skya/build/macos/client.sh` — launches the normal Skya Client GUI.

`./telephony-skya/build/macos/client_monitor.sh` — launches the administrative Client Monitor.

`./telephony-skya/build/macos/remote-client.sh` — starts the Remote Server Connection GUI.

## GUI and runtime relationship

`SkyaClientApp` is the normal user-facing JavaFX application. `SkyaApp` remains the administrative monitoring application.

Both use Guia™ as the GUI-to-client/listener contract. JavaFX is the presentation adapter.

The Chat, Audio, Video, and File Transfer controls are the user-facing application surface. Their current JavaFX implementations establish the UI and request/status boundary; production transport, media capture/playback, codec integration, authentication, NAT traversal, and file-transfer transport remain native implementation layers.
