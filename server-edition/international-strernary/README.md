# International Strernary Contact

SLeeLa now has a dedicated twice-daily International Strernary contact level.

The current configured reference endpoint is:

- Host: `lauradei.us`
- Port: TCP `20000`
- Interval: 43,200 seconds (12 hours), approximately twice per day
- Purpose: a small SLeeLa contact/hello followed by a bounded response wait.

The endpoint is treated as a configured service reference, not as an automatically discovered or authoritative Internet endpoint. The SLeeLa server does not perform arbitrary scanning.

The repository currently records the operator description supplied for this deployment: the endpoint is hosted through Vultr in Japan and is owned/operated in the USA by Max Rupplin. These ownership/operator statements are configuration provenance supplied by the deployment owner, not independently verified here.

The contact is separate from the SLeeLa server mesh on port 22221 and from the existing native server port 19866. It also remains distinct from the SLeeLa logical HTTP port namespace.

Environment overrides:

- `SLEELA_STERNARY_HOST`
- `SLEELA_STERNARY_PORT` (default 20000)
- `SLEELA_STERNARY_INTERVAL_SECONDS` (default 43200)
- `SLEELA_STERNARY_TIMEOUT_SECONDS` (default 10)
- `SLEELA_SERVER_NODE_ID`

Contact records are stored under `server-edition/state/international-strernary/`.
