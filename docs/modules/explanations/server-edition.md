<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# server-edition: Explanation

| | |
|---|---|
| Nature | daemon (`bin/sleelactl help server-edition`: verbs `start stop restart reload status enable disable`) |
| Registry summary | Bounded transport + packet-admission server (sleelas/serverd) |
| Lives in | `server-edition/` (transport server), `server-edition/moral/2/` (Service 2, `serverd`, unit, plist), `server-edition/http/`, `server-edition/port-awareness/`, `server-edition/participation/` |
| Writeup type | Explanation, because the registered daemon path (`sleelactl` -> systemd unit `sleela-server.service` -> `moral/2/serverd`) cannot be operated in this checkout: there is no service manager here and `serverd` fails to parse. |
| Verified | 2026-11-13 against `master` e048707fd: `make -C server-edition` builds `sleelas`, which ran in the foreground for 5 s (killed by `timeout`, rc 124); `server_protocol_test` PASS; `make test` stops at the TLS test (no `openssl` binary); `moral/2/serverd` has a shell syntax error. |

## What it is

"Server Edition" is two different things under one registry name, and the registry entry (`bin/sleelactl.d/registry.conf:30`) has no `dir=` key, so `sleelactl` does not say where either lives.

1. **`sleelas`**, a C++17 transport server built from `server-edition/server.cpp`, `sleela_server.cpp`, `packet_logger_bridge.cpp` and `../logger/sleela_logger.cpp` (`server-edition/Makefile`). It listens on loopback port 19866 by default, admits SLeeLa wire packets, and only allows non-loopback binds with mutual TLS 1.3 plus explicit authorization rules (`server-edition/README.md`).
2. **`serverd` (Service 2, "Discord-2")**, a POSIX shell script at `server-edition/moral/2/serverd`. It is the thing the systemd unit and launchd plist actually name. It reads a `key=value` config, opens firewall ports through `port-awareness/portctl.sh`, starts helper scripts (traffic logger, mesh listener, mesh sender, a "sternary" contact script), runs the HTTP capability check (`http/httpctl`), then runs `impl/build/sleela run moral/2/src/Server.sleela` once and exits.

`sleelactl` only talks to the service manager. The registry maps `server-edition` to unit `sleela-server.service` and launchd label `com.mearvk.sleela-server`.

## How it works

- **Unit and plist exist as real files.** `server-edition/moral/2/service/sleela-server.service` and `server-edition/moral/2/service/com.mearvk.sleela-server.plist` (Label `com.mearvk.sleela-server`) match the registry names. Both point at `/opt/sleela/server-edition/moral/2/serverd`, so they only make sense after `moral/2/install/linux-install.sh` or `macos-install.sh` has copied the tree to a prefix (default `/opt/sleela`). Neither is installed here.
- **The unit is `Type=oneshot`** with no `RemainAfterExit`. That matches `serverd` finishing after one `sleela run`, but it is not a long-lived daemon despite the registry nature. The unit also sets `User=sleela` while `serverd` calls `portctl.sh`, which uses `sudo` when it is not root (`portctl.sh`, `as_root`). I did not test that combination.
- **`sleelactl` verbs** map to `systemctl --system <verb> sleela-server.service` (`bin/SLEELACTL.md`, adapter table). Options such as `--dry-run` must come before the verb.
- **Service level.** `server-edition/moral/README.md` says the native `sleelas` launcher picks Service 2 by default and Service 3 with `SLEELA_SERVER_LEVEL=3`. That logic is in `api/sleela/2.1/server/sleelas.cpp:301-309`, a different file from the one `server-edition/Makefile` builds. The built `server-edition/sleelas` never reads `SLEELA_SERVER_LEVEL` (no match in `server.cpp` or `sleela_server.cpp`). I did not try to build `api/sleela/2.1/server/sleelas.cpp`.

## Where it lives

| File | Role |
|---|---|
| `bin/sleelactl.d/registry.conf:30` | registry entry (`unit=`, `launchd=`, no `dir=`) |
| `server-edition/Makefile` | builds `sleelas`, `server_protocol_test`; `make test` |
| `server-edition/server.cpp`, `sleela_server.cpp(.hpp)` | the transport server (`--bind --port --allow-remote --tls-* --allow-peer --authorize`) |
| `server-edition/tests/server_protocol_test.cpp`, `test_nat_awareness.py`, `test_tls_integration.py` | tests |
| `server-edition/moral/2/serverd` | Service 2 launcher script |
| `server-edition/moral/2/service/` | `sleela-server.service`, `com.mearvk.sleela-server.plist` |
| `server-edition/moral/2/install/` | Linux/macOS/Windows install and uninstall scripts |
| `server-edition/moral/2/config/server.conf.example` | config template |
| `server-edition/moral/2/src/Server.sleela` | SLeeLa source the launcher runs |
| `server-edition/http/` | `httpctl` capability checker |
| `server-edition/port-awareness/portctl.sh` | firewall open/close (ufw, firewalld, pf) |
| `server-edition/README.md`, `SECURITY-STANDARDS.md`, `WIRE-PROTOCOL.md`, `moral/README.md` | documentation |

## Status and limits

What works (commands run from the repo root with the `SLEELA_*` environment of the other writeups):

- `timeout 120 bin/sleelactl help server-edition` prints the daemon verbs. `bin/sleelactl check-registry` reports `44 modules, 0 error(s), 0 warning(s)`.
- `bin/sleelactl --dry-run start server-edition` prints `[dry-run] would run: systemctl --system start sleela-server.service`. With `SLEELACTL_SERVICE_MANAGER=launchd` it prints `launchctl kickstart system/com.mearvk.sleela-server`.
- `make -C server-edition` builds `server-edition/sleelas` (many `-Wmisleading-indentation` warnings, no errors).
- `./sleelas --help` prints the options. `./sleelas --version` is not supported (`Unknown option: --version`, then usage).
- `timeout 5 ./sleelas --port 19866` prints `{"time":"2026-11-13T20:27:53Z","level":"NOTICE","event":"server_started"}` and stays up until killed (rc 124). It also creates `server-edition/logs/packets-000000.jsonl`, which I deleted.
- `make -C server-edition test`: `server_protocol_test: PASS`; the 4 NAT parser tests pass.

What does not work here:

- `bin/sleelactl status server-edition` exits 1 with `not loaded/active`. Real `systemctl` fails with `System has not been booted with systemd as init system (PID 1)`. Running `bin/sleelactl start server-edition --dry-run` (flag after the module) skips dry-run and runs the real `systemctl`, which failed harmlessly with that error.
- `make -C server-edition test` ends with `FileNotFoundError: ... 'openssl'` in `tests/test_tls_integration.py` (no `openssl` command in this sandbox), so the TLS integration test did not run.
- `make -C server-edition/http` fails: `No rule to make target '../../api/http-multiplexing/sleela_http_api.c'`. `api/http-multiplexing/` does not exist; the sources are at `api/sleela/2.1/http-multiplexing/sleela_http_api.{c,h}`. So `httpctl` cannot be built, and `serverd` refuses to run without it (`HTTP API controller not executable`).
- `sh -n server-edition/moral/2/serverd` fails: `line 34: syntax error near unexpected token 'newline'` at `case "$line" in ""|#*) continue;; esac`. The `#` is unquoted, so the shell reads it as a comment. `moral/3/serverd` writes `\#*` and parses. `/bin/sh` here is bash; I had no `dash` to test. Running `moral/2/serverd` with the example config also stops with rc 2 at that line.
- The example config (`moral/2/config/server.conf.example`) contains `mature_services=auto`, which the key list in `moral/2/serverd` does not accept (`unknown configuration key`). `linux-install.sh` copies this example into `config/server.conf`, so an installed Service 2 would hit the same rejection once the syntax error is fixed. I confirmed the rejection for Service 3 (see `server-edition3.md`); for Service 2 I only read the code.
- `server-edition/moral/2/config/server.conf` does not exist in the checkout, so `serverd` with no config says `configuration not found`.

I did not run `moral/2/serverd` past parsing. It would open firewall ports and start helper scripts, including one that targets `lauradei.us:20000` by default.

## Related

- [server-edition3](server-edition3.md): Service 3 (`moral/3`), same layout.
- [antivirus](antivirus.md): the third daemon in the registry.
- [`server-edition/README.md`](../../../server-edition/README.md), [`SECURITY-STANDARDS.md`](../../../server-edition/SECURITY-STANDARDS.md), [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/SLEELACTL.md`, `bin/sleelactl` (option parsing, lines 985-1000), `server-edition/README.md`, `Makefile`, `VERSION`, `moral/README.md`, `moral/2/serverd`, `moral/2/service/*`, `moral/2/config/server.conf.example`, `moral/2/INSTALL.md`, `moral/3/install/linux-install.sh`, `port-awareness/portctl.sh`, `http/Makefile`, `api/sleela/2.1/server/sleelas.cpp`.

Commands run: `bin/sleelactl help|status|--dry-run start|--dry-run enable|check-registry`, `make -C server-edition`, `make -C server-edition/http`, `make -C server-edition test`, `./sleelas --help`, `timeout 5 ./sleelas --port 19866`, `sh -n moral/2/serverd`, `bash -n`, `./moral/2/serverd` with the example config.
