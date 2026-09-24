# Skya Platform Builds

The `telephony-skya/build` tree separates the native Skya Client from the JavaFX Client Monitor.

## Client vs Client Monitor

The **Client** is the native Skya runtime process. It starts the native engine directly and does not require JavaFX.

The **Client Monitor** is the administrative JavaFX GUI. It presents the local client/circuit status and provides the Start, Pause, and Stop controls. It uses `SkyaApp` and the Guia™ GUI-to-client/listener contract.

This distinction is intentional:

```text
client.sh
    ↓
native Skya Client
    ↓
Skya engine / client runtime

client_monitor.sh
    ↓
JavaFX SkyaApp
    ↓
SLeeLa SkyaClient.sleela circuit / monitoring surface
```

The Monitor is therefore an administrative interface, not the native client itself.

## Linux

`./telephony-skya/build/linux/build.sh` — builds native `skya` and `skya-server`.

`./telephony-skya/build/linux/client.sh` — builds and starts the native Skya Client with `--client`.

`./telephony-skya/build/linux/client_monitor.sh` — builds and starts the JavaFX Client Monitor.

`./telephony-skya/build/linux/remote-client.sh` — starts the Remote Server Connection GUI.

## Windows 10+

`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\build.ps1` — builds native executables.

`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\client.ps1` — builds and starts the native Skya Client with `--client`.

`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\client_monitor.ps1` — builds and starts the JavaFX Client Monitor.

`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\remote-client.ps1` — starts the Remote Server Connection GUI.

## macOS

`./telephony-skya/build/macos/build.sh` — builds native `skya` and `skya-server`.

`./telephony-skya/build/macos/client.sh` — builds and starts the native Skya Client with `--client`.

`./telephony-skya/build/macos/client_monitor.sh` — builds and starts the JavaFX Client Monitor.

`./telephony-skya/build/macos/remote-client.sh` — starts the Remote Server Connection GUI.

## GUI and runtime relationship

The Client Monitor uses `SkyaApp`. The native Client is implemented by the native Skya engine and `native/main.cpp`.

The Monitor can launch the SLeeLa `SkyaClient.sleela` circuit for its local monitoring surface. Its Pause control currently pauses monitoring/output handling; it does not claim to suspend the native client process.

Both JavaFX applications use Guia™ as the GUI-to-client/listener contract. The remote GUI currently uses a TCP connection foundation. Selecting HTTP/2 or HTTP/3 records the intended protocol; it does not claim a plain Java TCP socket implements HTTP/2 or HTTP/3. Native HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication and media transport remain native implementation layers.

JavaFX target: Java 17 / JavaFX 21. Maven is required for the GUI launchers.
