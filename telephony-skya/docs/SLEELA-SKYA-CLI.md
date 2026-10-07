# sleela-skya — Skya command-line program

`sleela-skya` is the Skya telephony command line. It loads and unloads the
emblematic Skya modules and starts the default, ordered runtime components — the
Sleela/Skya **server** and the **GUI**.

Build it with the native Makefile:

```sh
make -C telephony-skya/native sleela-skya
```

## Commands

```
sleela-skya <command> [options]

  load [MODULE...|--all]     load emblematic modules (default: all)
  unload [MODULE...|--all]   unload emblematic modules (default: all)
  list                       list modules and the ordered components
  gui                        launch the Skya GUI (JavaFX studio)
  start [options]            load modules, then start ordered components
  stop                       how to stop a running server
  help                       show help
```

**Modules** (the `/lib/telephony-skya` emblematic Master Classes):
`Socio`, `Network`, `Servers`, `Communication`, `RealAcquaintances`.

**Options:** `--port <p>` (default 8443), `--room <name>` (default `lobby`),
`--max-peers <n>` (default 256), `--http2` | `--http3` (default http3),
`--no-gui` (server only), `--headless` (report instead of failing when no GUI),
`--all`.

## Ordered startup

`start` performs the default, ordered bring-up:

1. **Modules** — any unloaded emblematic modules are loaded first.
2. **Component 1 — `SleelaServer`** — the comm server binds its listener
   (`0.0.0.0:<port>`, room, HTTP/2 or HTTP/3) via the Skya engine.
3. **Component 2 — `SkyaGui`** — the GUI is started after the server is up
   (unless `--no-gui`). On a headless host it reports rather than failing.
4. The long-lived server is then held open until `Ctrl-C` (SIGINT/SIGTERM),
   which stops the components in descending order (GUI, then server).

```sh
sleela-skya start                      # load all modules, server + GUI, ordered
sleela-skya start --no-gui --port 8443 # server only
sleela-skya load Socio Network         # load specific modules
sleela-skya unload --all               # unload everything
sleela-skya list                       # show module + component state
```

## Design

The program is a small, inspectable C++ model (`native/sleela_skya_cli.{h,cpp}`):

- `ModuleRegistry` — the five emblematic modules; load/unload is readiness
  bookkeeping only and never opens a socket or firewall by itself.
- `Component` + declared **start order** (`SleelaServer`=1, `SkyaGui`=2);
  components start ascending and stop descending.
- `App` — command dispatch; `start` orchestrates the existing `skya_engine`
  API for the server and launches the JavaFX studio for the GUI.

The authoritative networking/NAT/firewall lifecycle stays in the native Skya
engine and the SLeeLa port-awareness subsystem; the CLI orchestrates, it does
not create a second runtime.

**SLeeLa — MEARVK LLC — 2026**
