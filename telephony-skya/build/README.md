# Skya Native Platform Builds

Each platform build produces the general Skya executable and a dedicated Skya Server executable.
The dedicated server entry point is `telephony-skya/native/skya_server_main.cpp`.

## Linux
`./telephony-skya/build/linux/build.sh`

Outputs: `skya` and `skya-server`.

## Windows 10+
`powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\build.ps1`

Outputs: `skya.exe` and `skya-server.exe`.

## macOS
`./telephony-skya/build/macos/build.sh`

Outputs: `skya` and `skya-server`.

## Server options
`--room <name>` `--port <port>` `--max-peers <n>` `--http2` `--http3`.

The dedicated executable starts the Skya engine with `SKYA_SERVER`. It is the native server entry point; the underlying production HTTP/2, HTTP/3/QUIC, media, NAT traversal, relay, and firewall lifecycle implementations remain separate work items.
