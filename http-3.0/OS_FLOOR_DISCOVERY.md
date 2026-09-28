# OS-floor discovery — finding the tidiest fit at install time

When a SLeeLa executable installs on a host, it looks for the **tidiest fit**:
it examines the operating-system "floor" it landed on (OS + filesystem), confers
with any capability responder present — **TAC3 or another**, via the
[bonding collar](BONDING_COLLAR.md) — records what it found in a growing
**registry of OS floors / filesystems**, keeps rollup **statistics**, and can
**advance** new or updated discoveries to a known GitHub endpoint over HTTP.

Two modules:

| Module | Role |
|---|---|
| [`os_floor_registry.py`](os_floor_registry.py) | Detect the current OS floor, confer for its bonded advantage tier, and keep a deduplicated, persistent JSON registry with per-floor first/last-seen, counts, and rollup stats. |
| [`os_floor_report.py`](os_floor_report.py) | Advance new/updated floors to a configurable GitHub endpoint via HTTP POST — **opt-in**, non-identifying payload, fail-closed. |

## What a "floor" is

One record per distinct OS/filesystem SLeeLa has landed on:

| Field | Meaning |
|---|---|
| `floor_id` | Stable non-identifying fingerprint (dedupe key) |
| `os_name`, `os_release` | e.g. `Linux` / kernel release string |
| `filesystem` | Root-fs hint (from `/proc/mounts`, `SLEELA_FS_FLOOR`, or a safe default) |
| `bonded_tier`, `bonded_tier_name` | The bonding-collar tier the host attested (`NONE` if unknown/untrusted) |
| `responder` | `tac3`, `generic`, or `none` |
| `first_seen`, `last_seen`, `seen_count` | Discovery history |

On re-observation a floor is **deduplicated** by `floor_id`: its `seen_count`
and `last_seen` update, and its bonded tier is **upgraded** if the host now
attests to a higher one (never downgraded silently).

## Discovery is safe by construction

- Detection makes **no privileged calls**, mounts nothing, and loads no host
  code. The filesystem hint is read from world-readable `/proc/mounts` (Linux),
  an explicit `SLEELA_FS_FLOOR`, or a conservative default.
- The bonded tier is exactly what the bonding collar **attested** — never
  guessed. On a new/unknown/untrusted OS the collar returns `NONE`, so the floor
  is recorded as unbonded and **no advantage is assumed**.
- Records hold only **non-identifying descriptors** — no secrets, no user PII,
  no host-proprietary internals (the collar guarantees the last point).

## Stats

`OsFloorRegistry.stats()` rolls up the registry:

```json
{
  "distinct_floors": 2,
  "total_observations": 4,
  "bonded_floors": 1,
  "unbonded_floors": 1,
  "by_tier": {"NONE": 1, "T4_RECOVERY": 1},
  "by_os": {"Linux": 2},
  "by_filesystem": {"overlay": 2},
  "by_responder": {"none": 1, "tac3": 1}
}
```

## Advancing discoveries to GitHub (opt-in)

`os_floor_report.py` posts new/updated floors to a known GitHub endpoint. Because
this is telemetry, it is **explicit and consent-based, never covert**:

- **Opt-in.** Disabled unless **both** an endpoint and a token are configured.
  Unconfigured, `report_floors()` is a no-op returning `status="disabled"`.
- **Non-identifying payload.** Only the whitelisted floor descriptors above are
  sent. It never sends tokens, host names, IPs, usernames, or paths. (The token
  travels only in the `Authorization` header, never in the body.)
- **No hardcoded credentials.** Endpoint and token come from config/env at call
  time.
- **Fail closed.** Any network/config error is caught and returned as a
  non-fatal failed result; discovery and the local registry are never blocked.

Configuration:

```sh
export SLEELA_FLOOR_REPORT_URL="https://api.github.com/repos/OWNER/REPO/dispatches"
export SLEELA_FLOOR_REPORT_TOKEN="<github token, minimum scope>"
export SLEELA_FLOOR_REPORT_EVENT="sleela-os-floor"   # optional
```

The default shape targets GitHub's **`repository_dispatch`** REST endpoint — a
clean, low-privilege way for automated software to post a structured event to a
known repo. The payload is:

```json
{
  "event_type": "sleela-os-floor",
  "client_payload": {
    "kind": "sleela.os_floor.discovery",
    "schema": 1,
    "floors": [ { "floor_id": "...", "os_name": "...", "bonded_tier_name": "...", "...": "..." } ]
  }
}
```

## Usage

```python
from os_floor_registry import OsFloorRegistry, observe_current_floor
from os_floor_report import advance_new_floors, ReporterConfig
from bonding_collar import MAC_KEY_BYTES

key = b"..."  # 16-byte shared secret for the bonding collar
reg = OsFloorRegistry("os_floors.json")     # loads if present

floor = observe_current_floor(key)          # detect + confer for tidiest fit
f, is_new = reg.record(floor)
reg.save()

if is_new:
    # opt-in; a no-op unless SLEELA_FLOOR_REPORT_URL + _TOKEN are set
    advance_new_floors(reg, [f.floor_id], ReporterConfig.from_env())
```

## Self-demos (no network required)

```bash
cd http-3.0
python3 os_floor_registry.py     # observe this host's floor + print the registry
python3 os_floor_report.py       # show disabled-by-default, then a mocked POST
```

## Relationship to the bonding collar

Discovery is the install-time *consumer* of the [bonding collar](BONDING_COLLAR.md):
the collar decides *whether* and *at which tier* a host confers an advantage;
this layer records those decisions across hosts, keeps stats, and optionally
advances them to GitHub. TAC3 is one possible responder — a dependency of
neither module, and none of its internals are embedded here.
