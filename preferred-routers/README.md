# Preferred Routers

Configuration-driven preferred/well-known routing anchors for SLeeLa HTTP implementations.

## Purpose

The selector gives HTTP 1.0+ implementations a common policy layer for preferring national backbone/ISP and IXP anchors before regional and international interconnection roles. It does **not** install routes, alter the operating-system routing table, bypass ISP policy, or claim that a provider is objectively “perfect.”

The checked-in rating is a **routing preference tier** from 1–5. Tier 5 is the highest preference; it is not a latency, uptime, security, or quality measurement.

## Data

- `preferred-routers.csv`: 996 entries, four routing-role anchors for each of 249 ISO 3166 country/territory records.
- `preferred-routers.json`: same data plus schema and source references.
- Current public interconnection references include PeeringDB and RIR-derived country resources. PeeringDB describes its database as a source for networks, IXPs and facilities; RIPE NCC exposes country ASN and resource data. citeturn0search0turn0search3turn0search14
- The role-anchor rows intentionally avoid fabricating a named router where the checked-in source data has not been independently resolved. A deployment refresh can bind each role to current ASN/IXP/facility identities.

## Configuration

`preferred-routers.conf` is read by the native selector. Set `country`, `preferred_role`, `min_score`, and `international`.

## Build

```sh
make
```

The HTTP build also compiles this directory before checking HTTP 7/8/9 syntax. HTTP servers 1, 2 and 3 link the C selector into their native startup path.

## Safety / scope

This component is a route **preference policy**. Actual route installation and forwarding remain the responsibility of the host networking stack and authorized network operators.
