<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# server-edition3: Explanation

| | |
|---|---|
| Nature | daemon (`bin/sleelactl help server-edition3`: verbs `start stop restart reload status enable disable`) |
| Registry summary | Server edition, moral tier 3 (serverd) |
| Lives in | `server-edition/moral/3/` (Service 3, "Discord-3"), plus the shared `server-edition/` helpers |
| Writeup type | Explanation, because the registered daemon path (`sleelactl` -> `sleela-server3.service` -> `moral/3/serverd`) cannot be operated here: no service manager, and a full `serverd` run aborts on an unbound variable. |
| Verified | 2026-11-13 against `master` e048707fd: `moral/3/serverd --preflight` PASS with a scratch config; a full run exited 1 with `PORTCTL: unbound variable`; the shipped example config is rejected (`unknown configuration key: mature_services`, rc 2). |

## What it is

Service 3 is the "remedial" generation of the Server Edition launcher. `server-edition/moral/README.md` says generations are additive: Service 3 does not replace Service 2. It adds a preflight check, a recovery log (`state/recovery.log` with `STARTED`, `COMPLETED`, `FAILED:<rc>`, `PREFLIGHT_FAILED`, `FIREWALL_*_FAILED` records) and a `--design-activity` pass-through to `sleela design-activity`. Like Service 2 it is a shell script (`moral/3/serverd`) that runs `impl/build/sleela run moral/3/src/Server.sleela` once and exits.

The registry (`bin/sleelactl.d/registry.conf:31`) maps it to unit `sleela-server3.service` and launchd label `com.mearvk.sleela-server3`. There is no `dir=` key.

## How it works

1. `serverd` reads `$SLEELA_SERVER_CONFIG`, or `moral/3/config/server.conf`, as `key=value` lines. Unknown keys exit 2.
2. With `preflight=true` (default) it checks the engine, the `Server.sleela` source, the SHA-256 manifest and `port-awareness/portctl.sh`, and creates the state and inbox directories. `serverd --preflight` stops after that and prints `preflight: PASS`.
3. It validates `http_version` (`1.0|2.0|2.1|3.0`, default `3.0`), ports and booleans, runs `http/httpctl`, starts the traffic logger, mesh listener, "sternary" contact and mesh sender (all with `|| true`), takes a lock directory `state/.serverd3.lock`, opens firewall ports (`basic-probe` 2222, `participation` 22221, `basic-scan` 22220, `Discord-3` 19866) through `portctl.sh`, runs the SLeeLa source, and on exit closes the ports and stops the helpers (`trap cleanup`).
4. The unit `server-edition/moral/3/service/sleela-server3.service` is `Type=oneshot`, `User=sleela`, `ExecStart=/opt/sleela/server-edition/moral/3/serverd`. The plist `com.mearvk.sleela-server3.plist` has the same command with `RunAtLoad`. Both real files exist and match the registry names.
5. `moral/3/install/linux-install.sh` copies the tree to `$PREFIX` (variable spelled `SLEEELA_PREFIX`, with three E's, in the script), builds `config/server.conf` from the example with `sed`, and writes `/etc/systemd/system/sleela-server3.service`. I did not run it (it writes under `/etc`).

## Where it lives

| File | Role |
|---|---|
| `bin/sleelactl.d/registry.conf:31` | registry entry |
| `server-edition/moral/3/serverd` | launcher script |
| `server-edition/moral/3/service/sleela-server3.service`, `com.mearvk.sleela-server3.plist` | systemd unit and launchd job |
| `server-edition/moral/3/config/server.conf.example` | config template |
| `server-edition/moral/3/src/Server.sleela` | SLeeLa program run by the launcher |
| `server-edition/moral/3/install/*` | install/uninstall for Linux, macOS, Windows |
| `server-edition/moral/3/README.md`, `INSTALL.md`, `IMPLEMENTATION.md` | docs |
| `server-edition/port-awareness/portctl.sh`, `traffic-log.sh`, `known-connections.sh` | firewall and traffic helpers |
| `server-edition/participation/*`, `international-strernary/*` | mesh and contact helper scripts |

## Status and limits

Evidence (all from the repo root; scratch config and state in a `mktemp -d` directory, removed afterwards):

- `bin/sleelactl status server-edition3` exits 1 (`not loaded/active`; `systemctl` reports it was not booted with systemd). `bin/sleelactl --dry-run start server-edition3` prints `[dry-run] would run: systemctl --system start sleela-server3.service`. With `SLEELACTL_SERVICE_MANAGER=launchd` the dry-run for `server-edition` printed `launchctl kickstart system/com.mearvk.sleela-server`; I did not repeat it for the `3` label.
- `SLEELA_SERVER_CONFIG=moral/3/config/server.conf.example sh moral/3/serverd` prints `serverd: unknown configuration key: mature_services` and exits 2. The same line `mature_services=auto` is in the Service 2 example. The key is not in the `case` list of either script.
- With no config, `serverd` prints `configuration not found: .../moral/3/config/server.conf` and exits 1.
- With a scratch config (engine `impl/build/sleela`, manifest `security/important-sha256-manifest.json`, `http_api=/bin/true`), `serverd --preflight` printed `preflight: PASS` (rc 0).
- A full run with that scratch config exited 1 after `./serverd: line 106: PORTCTL: unbound variable` and a second error at line 94. `moral/3/serverd` uses `$PORTCTL` in the cleanup function and when opening ports, but only `moral/2/serverd` assigns it (`moral/2/serverd:63`). Because `set -u` is active, Service 3 cannot reach the point where it runs `Server.sleela`. The failing cleanup also leaves `state/.serverd3.lock` behind, so a retry would answer `Service 3 is already active` (I did not test the retry).
- That full run was not as inert as I intended. Before the failure it had already started the traffic logger, the mesh listener, the contact script and the mesh sender (each `|| true`), and wrote under the scratch state directory. Cleanup stopped them. I checked `ps` afterwards and found no leftover processes. The firewall steps were not reached. No network egress is available here, so I cannot say whether the contact script reached its default target `lauradei.us:20000`.
- `httpctl` cannot be built (`make -C server-edition/http` fails: `api/http-multiplexing/` is missing, see [server-edition](server-edition.md)), so a real run needs a stand-in or a fixed path.

Service 3 is therefore not operable end to end in this checkout. The documentation (`moral/3/README.md`, `INSTALL.md`) describes preflight, recovery logging and a non-public listener; only the preflight claim was reproduced.

## Related

- [server-edition](server-edition.md): Service 2 and the `sleelas` transport server.
- [antivirus](antivirus.md).
- [`server-edition/moral/3/README.md`](../../../server-edition/moral/3/README.md), [`INSTALL.md`](../../../server-edition/moral/3/INSTALL.md), [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/SLEELACTL.md`, `server-edition/moral/README.md`, `moral/3/serverd`, `moral/3/service/*`, `moral/3/config/server.conf.example`, `moral/3/install/linux-install.sh`, `moral/3/INSTALL.md`, `moral/3/src/Server.sleela` (head), `moral/2/serverd` (comparison), `port-awareness/portctl.sh`.

Commands run: `bin/sleelactl help|status|--dry-run start server-edition3`, `sh moral/3/serverd` (no config, example config), `serverd --preflight` and a bounded full run (`timeout 60`) with a scratch config, `ps` check for leftovers.
