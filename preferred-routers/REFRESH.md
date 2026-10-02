# Refreshing Named Network Records

Run `python3 refresh_peeringdb.py` when an operator wants to replace role-anchor placeholders with current PeeringDB network, exchange, and facility names.

PeeringDB documents the `country` field as an ISO 3166-1 alpha-2 country selector and exposes network, exchange, and facility records. citeturn2search0

The refresh is deliberately separate from compilation: builds remain deterministic and offline, while network metadata can be updated on an operator-selected schedule.

The generated rating remains a SLeeLa routing-preference tier, not a measured quality score.
