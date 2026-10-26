# preferred-routers data — backup &amp; version manifest

This folder preserves prior and snapshot editions of the preferred-routers data catalog so no
revision is lost. Backups are made whenever the data is edited and there is any doubt about a
change.

## Editions

| Edition | Files | SHA-256 | Notes |
|---------|-------|---------|-------|
| **v1.0** (pre-edit original) | `preferred-routers.csv.v1.0.pre-2026-10-26.bak` | `a9fc8f90c08580054ebcc25b6e4d9e0efd2dc719eb5bfe6c2208f3db2f6edfcb` | the catalog as checked in before the 2026-10-26 URL correction |
| | `preferred-routers.json.v1.0.pre-2026-10-26.bak` | `473d28f4e0c31b4ff44fa326c1bd5a10e5fb5e43d1915c575823f4adbad05261` | schema_version 1.0, generated 2026-10-01 |
| **v1.1** (corrected snapshot) | `preferred-routers.csv.v1.1-2026-10-26.snapshot` | `1ba95db14d79dedb708c23853811acf18d0b2493ca07366fc8f796268a7e14b3` | identical to the current live `../preferred-routers.csv` |
| | `preferred-routers.json.v1.1-2026-10-26.snapshot` | `e512f6342f5b4429b5d4ab203daa95a040351df3e6f47b3aa0ecceba63e0526e` | identical to the current live `../preferred-routers.json`; schema_version 1.1 |

## What changed in v1.1 (2026-10-26)

**Factual correction — PeeringDB source URL.** The checked-in catalog cited the PeeringDB source
as `https://www.peeringdb.org/`, which is **not** PeeringDB's canonical domain. PeeringDB is at
**`www.peeringdb.com`** — which the refresh tool [`../refresh_peeringdb.py`](../refresh_peeringdb.py)
already used (`API = "https://www.peeringdb.com/api"` and its written-back `source_url`). The data
was internally inconsistent with its own refresh script.

v1.1 corrects the domain across the catalog:

- **CSV:** 996 `source_url` fields `www.peeringdb.org` → `www.peeringdb.com`.
- **JSON:** 996 entry `source_url` fields + the `source_references[0]` entry corrected; added a
  `revision_note`; bumped `schema_version` 1.0 → 1.1 and `generated` 2026-10-01 → 2026-10-26.

**Unchanged:** every country, role, rank, candidate role-anchor string, and routing-preference
tier is byte-for-byte identical to v1.0 apart from the domain. No router names were added,
removed, or fabricated. The RIPE `stat.ripe.net` source references were left as-is (not
independently re-verifiable from this offline environment; no evident error).

## Restoring v1.0

```sh
cp backup/preferred-routers.csv.v1.0.pre-2026-10-26.bak  preferred-routers.csv
cp backup/preferred-routers.json.v1.0.pre-2026-10-26.bak preferred-routers.json
```

Verify any file against this manifest with `sha256sum <file>`.
