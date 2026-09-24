# Skya Platform Builds

The `telephony-skya/build` tree provides platform entry points for the native Skya engine and both JavaFX client GUIs.

## Linux
`./telephony-skya/build/linux/build.sh` — native `skya` and `skya-server`.
`./telephony-skya/build/linux/client.sh` — local Client Monitor GUI.
`./telephony-skya/build/linux/remote-client.sh` — Remote Server Connection GUI.

## Windows 10+
`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\build.ps1` — native executables.
`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\client.ps1` — local Client Monitor GUI.
`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\remote-client.ps1` — Remote Server Connection GUI.

## macOS
`./telephony-skya/build/macos/build.sh` — native `skya` and `skya-server`.
`./telephony-skya/build/macos/client.sh` — local Client Monitor GUI.
`./telephony-skya/build/macos/remote-client.sh` — Remote Server Connection GUI.

Both JavaFX applications use Guia™ as the GUI-to-client/listener contract. `SkyaApp` monitors the local SLeeLa circuit; `SkyaConnectApp` connects to a remote server and communicates through the client/listener surface.

The remote GUI currently uses a TCP connection foundation. Selecting HTTP/2 or HTTP/3 records the intended protocol; it does not claim a plain Java TCP socket implements HTTP/2 or HTTP/3. Native HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication and media transport remain native implementation layers.

JavaFX target: Java 17 / JavaFX 21. Maven is required for the GUI launchers.
