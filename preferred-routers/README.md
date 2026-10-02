<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Preferred Routers

Configuration-driven preferred/well-known routing anchors for SLeeLa HTTP implementations.

## Purpose

The selector gives HTTP 1.0+ implementations a common policy layer for preferring national backbone/ISP and IXP anchors before regional and international interconnection roles. It does **not** install routes, alter the operating-system routing table, bypass ISP policy, or claim that a provider is objectively “perfect.”

The checked-in rating is a **routing preference tier** from 1–5. Tier 5 is the highest preference; it is not a latency, uptime, security, or quality measurement.

## Data

- \`preferred-routers.csv\`: 996 entries, four routing-role anchors for each of 249 ISO 3166 country/territory records.
- \`preferred-routers.json\`: same data plus schema and source references.
- Public references used for the catalog design include PeeringDB and RIPE NCC country ASN/resource data.
- The role-anchor rows intentionally avoid fabricating a named router where current provider identity has not been independently resolved. Run \`refresh_peeringdb.py\` to bind roles to current PeeringDB network, IXP, and facility identities.

## Configuration

\`preferred-routers.conf\` is read by the native selector. Set \`country\`, \`preferred_role\`, \`min_score\`, and \`international\`. Set \`SLEELA_PREFERRED_ROUTER_CONFIG\` to override the configuration path.

## Build

\`\`\`sh
make
\`\`\`

The HTTP build compiles this directory before checking HTTP 7/8/9 syntax. HTTP servers 1, 2 and 3 link the selector into their native startup path.

## Data refresh

\`\`\`sh
python3 refresh_peeringdb.py
\`\`\`

Refresh is opt-in so normal builds remain deterministic and offline.

## Safety / scope

This component is a route **preference policy**. Actual route installation and forwarding remain the responsibility of the host networking stack and authorized network operators.
