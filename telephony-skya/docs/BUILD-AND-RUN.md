# Skya Build and Run

## The GUI telephony flow (primary)

The user-facing flow is **JavaFX GUI → Guia/1 → `SkyaClient.sleela` → SKYA/1 →
`SkyaServer.sleela` → back → GUI** (see [`GUIA-PROTOCOL.md`](GUIA-PROTOCOL.md)).
Launch it per platform; the launch script brings the SLeeLa side up (server +
Guia agent) before the GUI and tears it down on exit:

```sh
# Linux / macOS
telephony-skya/build/linux/client.sh          # user client (SkyaClientApp)
telephony-skya/build/linux/client_monitor.sh  # admin monitor (SkyaApp)
telephony-skya/build/linux/remote-client.sh   # remote-connect GUI (SkyaConnectApp)
```

```powershell
# Windows 10+
powershell -ExecutionPolicy Bypass -File telephony-skya\build\windows\client.ps1
```

These need the SLeeLa runtime (`sleela`) on `PATH` and Maven + a JDK with
JavaFX. Endpoints default from [`../config/skya.conf`](../config/skya.conf),
which the launch script exports as environment variables for the GUI:

| skya.conf key | Env var | Used by |
|---|---|---|
| `control.guia.host` / `control.guia.port` | `SKYA_GUIA_HOST` / `SKYA_GUIA_PORT` | GUI → agent Guia endpoint (default `127.0.0.1:8700`) |
| `listen.host` / `listen.port` | `SKYA_SERVER_HOST` / `SKYA_SERVER_PORT` | SKYA/1 server endpoint shown in the GUI (default `localhost:8443`) |
| `default.room` | `SKYA_DEFAULT_ROOM` | default room (default `lobby`) |

Logging is controlled by `SKYA_LOG_LEVEL` (DEBUG/INFO/WARN/ERROR, default INFO)
and `SKYA_LOG_FILE` (append to a file) — see [`LOGGING.md`](LOGGING.md).

> **Note.** The `.sleela` programs' listen/connect ports are fixed in source at
> `8443` (SKYA/1) and `8700` (Guia): the SLeeLa VM exposes no config or
> environment access to a running `.sleela` program. `skya.conf` configures the
> layers that can read it — the JavaFX GUI and the native engine. Keep the
> config ports aligned with those fixed `.sleela` ports.

To run the SLeeLa side by hand (without a launch script):

```sh
export SLEELA_SHEET=SHEET.sheet SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
sleela run telephony-skya/sleela/SkyaServer.sleela &   # SKYA/1 on :8443
sleela run telephony-skya/sleela/SkyaClient.sleela &   # Guia/1 agent on :8700
```

## Native engine (standalone)

The native C engine is a separate, standalone build — the GUI flow above does
**not** require it. Build and run it directly:

```sh
make -C telephony-skya/native
./telephony-skya/native/skya --both --http3 --room lobby   # engine lifecycle + room
./telephony-skya/native/skya --drivers                     # enumerate hardware drivers
./telephony-skya/native/skya-server --room lobby --port 8443
```

`make -C telephony-skya/native` also builds `../drivers/libskya-drivers.a` and
links it into `skya`, so `skya --drivers` lists every registered phone/headset
driver from the per-vendor tree. The native command validates engine lifecycle,
room selection, and driver registration; it is not yet a complete wire-level
media server.

### Platform-native binary entry points

The top-level `build/` directory also has direct native Skya build scripts:

- Linux: `./build/skya-linux.sh`
- Windows 10+: `powershell -ExecutionPolicy Bypass -File .\build\skya-windows.ps1`
- macOS: `./build/skya-macos.sh`

Outputs are staged under `build/skya/linux/`, `build/skya/windows/`, and
`build/skya/macos/`. These are direct builds of the current native Skya engine,
separate from the integrated SLeeLa executable.

## SLeeLa runnables

- `telephony-skya/sleela/SkyaServer.sleela` — SKYA/1 server (loops on :8443)
- `telephony-skya/sleela/SkyaClient.sleela` — Guia control agent + SKYA/1 bridge (:8700)
- `telephony-skya/sleela/Skya.sleela` — one-process combined smoke test
- `telephony-skya/sleela/SkyaRoom.sleela` — bounded multi-peer room model

## Scope

HTTP/2 and HTTP/3 transport, TLS/certificate inspection, media capture/codecs,
NAT traversal, resumable file transfer, and explicit OS firewall integration
remain adapter work against existing SLeeLa subsystems. `SKYA/1` here is TCP on
loopback; the production wire transport is supplied by the SLeeLa HTTP layer.
