# Skya Logging

The Skya JavaFX apps and the Guia footer share one logger,
[`SkyaLog`](../javafx/src/main/java/com/mearvk/sleela/skya/SkyaLog.java). It is
dependency-free (no SLF4J/Logback) and writes timestamped, level-filtered lines.

## Format

```
2026-10-04T12:00:00.123Z LEVEL [component] message
```

- Timestamp is UTC, millisecond precision.
- `LEVEL` ∈ `DEBUG | INFO | WARN | ERROR`.
- `component` is a short tag: `client`, `admin`, `remote`, `guia`, `footer`.

## What is logged

- **App lifecycle** — each GUI app logs start/stop (`client`, `admin`, `remote`).
- **Guia traffic** — every command sent (`guia -> GUIA/1 <CMD> (host:port)`) and
  event received (`guia <- GUIA/1 <EVENT>`); offline/error events log at `WARN`.
- **Remote connection** — `SkyaConnectApp` mirrors its on-screen connection log
  to `SkyaLog` (errors/timeouts at `WARN`).
- **BODI/Guia UI load** — the footer logs the `skya-ui.xml` load status.

Console output goes to **stderr**, so it never corrupts any stdout protocol
stream.

## Configuration (environment)

| Variable | Meaning | Default |
|---|---|---|
| `SKYA_LOG_LEVEL` | `DEBUG`/`INFO`/`WARN`/`ERROR`/`OFF` threshold | `INFO` |
| `SKYA_LOG_FILE` | append log lines to this file (in addition to stderr) | *(console only)* |

```sh
SKYA_LOG_LEVEL=DEBUG SKYA_LOG_FILE=/tmp/skya.log telephony-skya/build/linux/client.sh
```

## SLeeLa side

The `.sleela` programs log with `print(...)` lines (e.g. the Guia agent prints
each received command and reply). The SLeeLa VM exposes no logging framework or
environment access, so those are plain stdout lines captured by the launch
scripts into `build/<os>/skya-client.log` and `skya-server.log`.
