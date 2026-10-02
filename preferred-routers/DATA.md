# Preferred Router Data

The dataset is intentionally separated from executable code. The runtime consumes a small policy configuration file and returns a routing-role preference. The CSV/JSON files are data catalogs for later resolver/refresh tooling.

Source references used for the catalog design:
- PeeringDB: interconnection networks, IXPs, facilities and carriers.
- RIPE NCC country ASN data.
- RIPE NCC country resource data.

A score of 5 means “highest local preference tier” in SLeeLa policy. It does not mean that a network is guaranteed to be faster, safer, more reliable, or objectively superior.
