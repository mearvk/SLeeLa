<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# telephony-skya: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help telephony-skya`) |
| Registry summary | Skya telephony stack |
| Lives in | `telephony-skya/` (`native/`, `sleela/`, `drivers/`, `javafx/`, `build/`, `config/`, `docs/`) |
| Writeup type | Tutorial, because the native engine, the `sleela-skya` command line and the driver library all build with documented `make` targets and run here. Several other parts do not work; they are listed under "Status and limits". |
| Verified | 2026-11-13 against `master` e048707fd: `make -C telephony-skya/native` and `make -C telephony-skya/drivers` exit 0; `sleela-skya`, `skya` and `SkyaRoom.sleela` ran with the outputs shown. |

## What it is

Skya is the SLeeLa telephony layer ([`README.md`](../../../telephony-skya/README.md)). The C++ engine in `native/` owns the sockets and session state. A command-line program, `sleela-skya`, loads and unloads five named "emblematic modules" and starts an ordered server and GUI. `sleela/` holds `.sleela` programs, `drivers/` holds one C and one C++ adapter per phone or headset model, and `javafx/` holds the GUI. The README says the engine is a bridge, not a full media server, and `docs/BUILD-AND-RUN.md` says it "currently validates engine lifecycle and room selection". What I ran agrees: the engine opens a TCP listener and exchanges one greeting line. It does not do HTTP/2, HTTP/3, TLS or media.

## Before you start

- Work from the repository root. You need `g++` (C++17), `cc`, `ar`, `make`, `timeout`.
- For `.sleela` programs and `sleelactl`, set the usual environment (the SHA-256 check runs before every `sleela` command):

```sh
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

## Step 1 - Ask sleelactl about the module

```sh
bin/sleelactl info telephony-skya
bin/sleelactl validate telephony-skya
```

```
  ✓ telephony-skya — Skya telephony stack
  location: telephony-skya
  see: telephony-skya/README.md
    doc: README.md
  ! telephony-skya has no validate/syntax/test target; nothing to run
```

`validate` exits 3. The module has no top-level Makefile, so the build below is by hand.

## Step 2 - Build the native engine and the CLI

```sh
timeout 300 make -C telephony-skya/native
```

The Makefile builds three things: `skya` (the demo launcher), `libskya.a` (the engine archive) and `sleela-skya` (the CLI). It prints many `-Wmisleading-indentation` warnings from `skya_engine.cpp` and ends with exit 0:

```
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic main.o skya_engine.o -o skya
ar rcs libskya.a skya_engine.o
...
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic sleela_skya_main.o sleela_skya_cli.o skya_engine.o -o sleela-skya
```

Build outputs, all in `telephony-skya/native/`: `skya`, `sleela-skya`, `libskya.a`, and `main.o`, `skya_engine.o`, `sleela_skya_cli.o`, `sleela_skya_main.o`. `*.o` and `*.a` are git-ignored; `skya` and `sleela-skya` show up as untracked files. `make -C telephony-skya/native clean` removes them.

## Step 3 - Use the command line

```sh
cd telephony-skya/native
./sleela-skya list
./sleela-skya load Socio Network
./sleela-skya unload --all
./sleela-skya stop
./sleela-skya gui --headless
./sleela-skya bogus
```

```
sleela-skya modules:
  [unloaded] Socio
  [unloaded] Network
  [unloaded] Servers
  [unloaded] Communication
  [unloaded] RealAcquaintances
sleela-skya components (start order):
  1. SleelaServer
  2. SkyaGui
sleela-skya: loaded module Socio
sleela-skya: loaded module Network
sleela-skya: unloaded all modules
sleela-skya: stop a running server with Ctrl-C (SIGINT/SIGTERM).
sleela-skya: ordered stop (GUI, then SleelaServer) applies in daemon mode.
sleela-skya: SkyaGui skipped (headless); the JavaFX studio lives under telephony-skya/javafx
sleela-skya: unknown command 'bogus' (try 'help')
```

All exit 0 except `bogus`, which exits 2. Module state lives only inside one process: after `./sleela-skya load Socio`, a new `./sleela-skya list` shows every module `[unloaded]`. The docs call load/unload "readiness bookkeeping only" (`docs/SLEELA-SKYA-CLI.md`), which fits.

## Step 4 - Start the engine with the demo launcher

```sh
./skya --both --http3 --room lobby
./skya --client --room x
```

```
Skya Server: listening on 0.0.0.0:8443 (TCP control/session listener).
Skya running-listening HTTP/3 port=8443 room=lobby
Skya running HTTP/3 port=8443 room=x
```

Both exit 0. The program starts the engine, joins a room, prints the status and exits right away. `--client` binds nothing.

## Step 5 - Talk to the engine's listener

`skya_engine.h` is a plain C ABI. This program (compiled in `/tmp`, not added to the repo) starts a server with `max_peers=1`, connects twice over TCP and reads the greeting each time:

```sh
g++ -std=c++17 -pthread -Itelephony-skya/native /tmp/e.cpp telephony-skya/native/libskya.a -o /tmp/e
```

The core of `e.cpp`: `skya_create` with `port=18555, max_peers=1`; `skya_start(e, SKYA_SERVER)`; `skya_join(e, "lobby")`; two `socket`/`connect`/`read` rounds; then `skya_peer_count` and `skya_peer_at`; it ends with `_exit(0)` (see the stop problem below).

```
Skya Server: listening on 0.0.0.0:18555 (TCP control/session listener).
client0 got 50: SKYA/1 server-ready port=18555 room=lobby http=3
client1 got 20: SKYA/1 server-busy
peers=1
peer0 127.0.0.1:59730 http=3
```

A second run showed `skya_message(e, "lobby", "hi")` returns 0 and `skya_message(e, "nope", "hi")` returns -2. The engine's whole wire protocol, as implemented in `skya_engine.cpp`, is the `SKYA/1 server-ready ...` and `SKYA/1 server-busy` lines.

## Step 6 - Build the driver library and look up a driver

```sh
timeout 900 make -j8 -C telephony-skya/drivers
```

Exit 0, no warnings, produces `telephony-skya/drivers/libskya-drivers.a` (97 archive members) and `telephony-skya/drivers/build/` (both git-ignored). It compiles one `driver.c` and one `driver.cpp` per model plus the registry, runtime and the Linux platform driver. There is no test target. A scratch program (in `/tmp`) registered the built-in drivers and looked two devices up:

```
register_builtin rc=54 count=54
 [0] name=yealink_mp45 vendor=yealink
 [1] name=yealink_mp50 vendor=yealink
 [2] name=poly_blackwire_5220 vendor=poly
 [3] name=jabra_evolve2_40 vendor=jabra
find Yealink MP50 -> yealink_mp50
find Nobody X1 -> (none)
```

`drivers/DRIVER.AUDIT.md` says hardware identities (VID/PID, HID reports) are not verified per model. No hardware is involved here.

## Step 7 - Run a .sleela program

```sh
impl/build/sleela run telephony-skya/sleela/SkyaRoom.sleela
```

```
verification: PASS (18667 item(s))
Skya room: lobby
connected peers = 3
room lifecycle check complete
```

Exit 0 (the SHA-256 verification lines before it are abridged).

## What you did

You built the native engine, the CLI and the driver library, ran the CLI commands, started and queried the engine's listener, looked up drivers, and ran one `.sleela` room model.

## What the other sleelactl verbs do for this module

| Command | Result |
|---|---|
| `bin/sleelactl negotiate telephony-skya` | "protocol-metadata operation defined by the generated spec", points to "the negotiate section in telephony-skya/README.md", then `no standalone negotiate CLI is built`. Exit 3. The README has no such section. |
| `bin/sleelactl grade telephony-skya` | same, for `grade`. Exit 3. |
| `bin/sleelactl simulate telephony-skya` | `no simulator registered (sim= unset)`. Exit 3. |

## Status and limits

Things that worked: Steps 1 to 7. Things that did not, each with the command and result:

- **Stopping a running server hangs.** `skya_stop` closes the listening socket and then joins the accept thread (`native/skya_engine.cpp`, `skya_stop`). On Linux, `close()` does not wake a thread blocked in `accept()`. A 12-line program that calls `skya_start(SKYA_SERVER)`, sleeps 300 ms and calls `skya_stop` never returns (`timeout 5` kills it, exit 124). The same applies to `sleela-skya start`: `timeout -k 2 -s TERM 3 ./sleela-skya start --no-gui --room lobby` printed `SleelaServer READY ... 0.0.0.0:8443`, then `components up; press Ctrl-C to stop`, and had to be SIGKILLed (exit 137). SIGTERM had been caught but the shutdown blocked, so the "stopping components" line never appeared. The demo `skya` exits cleanly only because it stops the engine immediately, before the accept thread reaches `accept()`. For that reason I did not use `sleela-skya start` in a step.
- **`Skya.sleela` fails.** `check` says `ok`, but `impl/build/sleela run telephony-skya/sleela/Skya.sleela` exits 1 with `Semantic error: spawn target 'server' must be static because a thread has no instance receiver`. `docs/BUILD-AND-RUN.md` recommends this file for the single-process test. `SkyaServer.sleela` and `SkyaClient.sleela` pass `check`; I did not run them because the documented smoke test needs two processes with the server in the background.
- **15 of the 19 `.sleela` files fail `sleela check`.** Of the four that pass: `Skya`, `SkyaRoom`, `SkyaServer`, `SkyaClient`. The rest:
  - `Socio`, `Network`, `Servers`, `Communication`, `RealAcquaintances`, `SkyaModules` and the three files in `sleela/hechts/`: `Lexical error (line 3, col 12): unexpected character` at an em-dash (U+2014) inside the opening block comment. These are the "emblematic modules" the README presents. (The `hechts` files also warn about a missing `#sleela` version pragma.)
  - `VoIPApplication` (`method 'hangup' requires a class or struct receiver`, `use of undeclared variable 'session'`), `VoIPAppExample`, `CaptureStream` and `PlaybackStream` (`declares multiple constructors; constructor overload resolution requires syntax 3.0`), `VoIPAccount` (`unknown type 'SipEndpoint'`, `no 'main' method found`), `AudioDeviceManager` (`use of undeclared variable 'defaultInput'`).
- **`telephony-skya/build/linux/build.sh` fails.** `sh telephony-skya/build/linux/build.sh` exits with `Skya logo not found: .../images/skya-logo-blue.jpeg`; no `images/skya*` file exists. The other documented path, the top-level `sh build/skya-linux.sh`, works: it builds `build/skya/linux/skya` (git-ignored) and says `Non-interactive build: Skya was built but not started.`
- **JavaFX GUI not built.** `mvn -o -q -f telephony-skya/javafx/pom.xml compile` fails with `LocalRepositoryNotAccessibleException` (no offline Maven repository). `javafx/target/classes` was already in the checkout. I did not run the GUI.
- **`config/skya.conf` is not read.** A search of the module's `*.cpp`, `*.sh`, `*.java` and `*.sleela` files for `skya.conf` finds no reference. The conf lists `security.tls.required=true`, `nat.relay.enabled=true` and similar; the engine implements none of those.
- **Engine scope.** `README.md` lists TLS verification, DH, NAT and relay, file transfer and codec negotiation. `skya_engine.cpp` (103 lines) implements a TCP listener, a room name, a peer list and the two greeting lines.
- **Docs.** `drivers/README.md` ends with a stray `citeturn1search2turn1search7` marker. `lib/telephony-skya/` holds copies of the module `.sleela` files that differ from `telephony-skya/sleela/` (checked with `diff -q` for `Socio.sleela`); `sleela check` on those fails with `no 'main' method found`.

## Next steps

- [`telephony-skya/README.md`](../../../telephony-skya/README.md), [`docs/SLEELA-SKYA-CLI.md`](../../../telephony-skya/docs/SLEELA-SKYA-CLI.md), [`docs/BUILD-AND-RUN.md`](../../../telephony-skya/docs/BUILD-AND-RUN.md), [`docs/RUNNABLES.md`](../../../telephony-skya/docs/RUNNABLES.md)
- [`drivers/README.md`](../../../telephony-skya/drivers/README.md), [`drivers/DRIVER.AUDIT.md`](../../../telephony-skya/drivers/DRIVER.AUDIT.md)
- [resolver](resolver.md), [preferred-routers](preferred-routers.md), [route](../explanations/route.md), [connector](../explanations/connector.md): the other protocol modules.

## Sources

- Files read: `telephony-skya/README.md`, `docs/BUILD-AND-RUN.md`, `docs/RUNNABLES.md`, `docs/SLEELA-SKYA-CLI.md`, `build/README.md`, `build/linux/build.sh`, `native/Makefile`, `native/README.md`, `native/skya_engine.h`, `native/skya_engine.cpp`, `native/main.cpp`, `native/skya_server_main.cpp`, `native/sleela_skya_main.cpp`, parts of `native/sleela_skya_cli.cpp`, `config/skya.conf`, `drivers/Makefile`, `drivers/README.md`, `drivers/DRIVER.AUDIT.md`, driver headers, `sleela/Skya.sleela`, top-level `build/skya-linux.sh`, `bin/sleelactl.d/registry.conf` (line 62).
- Commands run: `bin/sleelactl [--dry-run] info|validate|negotiate|grade|simulate telephony-skya`; `make -C telephony-skya/native`; `sleela-skya help|list|load|unload|stop|gui --headless|bogus|start --no-gui`; `skya --both|--client`; `make -j8 -C telephony-skya/drivers`; `impl/build/sleela check` on all 19 `.sleela` files and `run` on `Skya` and `SkyaRoom`; `sh telephony-skya/build/linux/build.sh`; `sh build/skya-linux.sh`; `mvn -o ... compile`; scratch C++ programs in `/tmp`.
