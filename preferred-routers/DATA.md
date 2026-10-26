# Preferred Router Data

The dataset is intentionally separated from executable code. The runtime consumes a small policy configuration file and returns a routing-role preference. The CSV/JSON files are data catalogs for later resolver/refresh tooling.

Source references used for the catalog design:
- PeeringDB (`https://www.peeringdb.com/`): interconnection networks, IXPs, facilities and carriers.
- RIPE NCC country ASN data.
- RIPE NCC country resource data.

A score of 5 means “highest local preference tier” in SLeeLa policy. It does not mean that a network is guaranteed to be faster, safer, more reliable, or objectively superior.

## Versioning

The catalog is versioned by `schema_version` in `preferred-routers.json`. Prior editions are kept
under [`backup/`](./backup) with SHA-256 digests in [`backup/MANIFEST.md`](./backup/MANIFEST.md).

- **v1.0** (generated 2026-10-01): initial catalog; cited PeeringDB as `www.peeringdb.org`.
- **v1.1** (2026-10-26): corrected the PeeringDB source URL to its canonical domain
  `www.peeringdb.com` across all entries and references (matching `refresh_peeringdb.py`). Country,
  role, rank, candidate, and preference-tier data are otherwise unchanged.
