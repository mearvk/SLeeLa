<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# route: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help route`) |
| Registry summary | Shared route-data contract (ROUTE.DATA.json) |
| Lives in | `route/` (`ROUTE.DATA.json`, `ROUTE.DATA.md`, `route_data.h`, `route_data.c`) |
| Writeup type | Explanation, because the module is a data contract plus a 25-line lookup table with no build target, no CLI and no README. |
| Verified | 2026-11-13 against `master` e048707fd: `make route` (top-level) exit 0; a scratch program compiled against `route_data.c` and run; JSON and C tables compared. |

## What it is

`route/` defines one schema, `sleela.route.data` version 1.0, that the Server Edition and the HTTP 1.0 to 9.0 servers are meant to share ([`route/ROUTE.DATA.md`](../../../route/ROUTE.DATA.md)). It names 12 "VM generations" (0 `/impl` Core up to 11 `/11` Sovereign), maps each HTTP generation to the VM generation that handles it, and lists the 13 fields a route must carry. `route_data.c` is a small C lookup of the same VM table.

## How it works

- **The schema** is `ROUTE.DATA.json`, with keys `schema`, `schema_version`, `authority`, `config_root` (`config/sleela.conf`), `vm_generations`, `route_fields`, `http_generations` and `standalone_servers`. `route_fields` is: protocol, server_surface, http_generation, vm_generation, vm_name, config_root, route_id, target, resolution_mode, capability, transport, port, status. These match the "Required route fields" list in `ROUTE.DATA.md`.
- **HTTP mapping.** HTTP/N.0 maps to VM generation N for N = 1 to 9 (Foundation, Operator, Specialist, Supervisor, Manager, Director, Administrator, Executive, Authority). The `standalone_servers` list maps `server-edition` to 0, `http-servers/1` to 1, `http-servers/2` to 2, and `http-servers/3`, `api/server` and `api/webserver` to 3.
- **The C API** (`route_data.h`): `sleela_route_data_for_protocol`, `sleela_route_data_validate`, `sleela_route_data_required_vm`, `sleela_route_data_vm_name`, `sleela_route_data_vm_path`. A protocol string starting with `HTTP/` gets generation `p[5]-'0'` when that is 1 to 9; every other string gets generation 0.
- **Rules** in `ROUTE.DATA.md`: VM generation and name never imply capability or authority; dynamic targets must go through the [resolver](../tutorials/resolver.md) before an address is accepted; the canonical config root is resolved through `config/sleela.conf` (that file exists).

## What I ran

```sh
python3 -m json.tool route/ROUTE.DATA.json >/dev/null   # what 'make route' does; exit 0
make route
```

```
SLeeLa unified route data: route/ROUTE.DATA.json
```

The top-level `Makefile` target `route:` only checks that the JSON parses. It does not compile `route_data.c`.

A scratch program (built in `/tmp` against `route_data.c`, not added to the repo) called the C API:

```
HTTP/1.0   req=1 rc=0 gen=1 name=Foundation path=/1 surface=HTTP/1.0 valid=0
HTTP/9.0   req=9 rc=0 gen=9 name=Authority path=/9 surface=HTTP/9.0 valid=0
HTTP/10.0  req=1 rc=0 gen=1 name=Foundation path=/1 surface=HTTP/10.0 valid=0
HTTP/0.9   req=0 rc=0 gen=0 name=Core path=/impl surface=server-edition valid=0
FTP        req=0 rc=0 gen=0 name=Core path=/impl surface=server-edition valid=0
(empty)    req=0 rc=0 gen=0 name=Core path=/impl surface=server-edition valid=0
(null)     req=-1 rc=-1
HTTP/x     req=0 rc=0 gen=0 name=Core path=/impl surface=server-edition valid=0
vm_name(12)=(nil) vm_name(-1)=(nil) vm_name(11)=Sovereign
```

(`valid=0` means `sleela_route_data_validate` returned 0, which is success.) A short Python check confirmed the 12-entry table in `route_data.c` is identical to `vm_generations` in `ROUTE.DATA.json` (generation, name, path).

## Where it lives

| File | Role |
|---|---|
| `route/ROUTE.DATA.json` | the schema and tables (40 lines) |
| `route/ROUTE.DATA.md` | the prose contract: required fields, VM identities, 7 routing rules, list of consumers |
| `route/route_data.h` | C API and `SLEELA_ROUTE_DATA_VERSION "1.0"` |
| `route/route_data.c` | 12-entry VM table and the five functions |

## Status and limits

- **Not operable as a tool.** No `route/Makefile`, no README, no CLI. `bin/sleelactl info route` lists only `doc: ROUTE.DATA.md` and prints no `see:` line. `validate` prints `route has no validate/syntax/test target; nothing to run` (exit 3). `negotiate` and `grade` print the "protocol-metadata operation" text and `no standalone ... CLI is built` (exit 3). They point to no README section, because `route/` has none. `simulate` prints `no simulator registered (sim= unset)` (exit 3).
- **`route_data.c` has no consumers.** A search of all `*.c`, `*.cpp`, `*.h`, `Makefile` and `*.sleela` files outside `route/` for `route_data.h` or `sleela_route_data_` found nothing. The HTTP servers (for example `http-1.0/http1_server.c:496`) only print the literal `"route_data_schema":"1.0"` in a status line. `preferred-routers/preferred_router.c` has its own copy of the VM name and path tables rather than calling this module. The doc names consumers (`server-edition`, `http-1.0` to `http-9.0`, `api/server`, `impl/fundamental/Router`, `lib/http`, and others). I did not trace all of them; none uses the header.
- **Protocol parsing is loose.** `HTTP/10.0` maps to generation 1, and any non-`HTTP/` string, an empty string, `HTTP/0.9` or `HTTP/x` maps to generation 0 with surface `server-edition`, and `for_protocol` still returns success. For HTTP strings, `server_surface` is the protocol string (`HTTP/1.0`), not the surface name used in the JSON (`http-1.0`).
- **Unused fields.** `route_id`, `target`, `resolution_mode`, `capability`, `transport`, `port` and `status` appear in `route_fields`, but `sleela_route_data` (the C struct) holds only `vm_generation`, `vm_name`, `vm_path`, `protocol` and `server_surface`.

## Related

- [resolver](../tutorials/resolver.md): rule 4 of the contract.
- [preferred-routers](../tutorials/preferred-routers.md): has its own VM tables, keyed the same way.
- [connector](connector.md) and [telephony-skya](../tutorials/telephony-skya.md): other protocol modules.

## Sources

- Files read: `route/ROUTE.DATA.md`, `route/ROUTE.DATA.json`, `route/route_data.h`, `route/route_data.c`, top-level `Makefile` (route target), `bin/sleelactl.d/registry.conf` (line 64).
- Commands run: `bin/sleelactl --dry-run info|validate|negotiate|grade|simulate route`, `make route`, a scratch C program and a scratch Python comparison (both in `/tmp`, nothing added to the repo), and a recursive search for `route_data.h` / `sleela_route_data_`.
