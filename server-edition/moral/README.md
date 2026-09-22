# SLeeLa Server Edition / Moral Source

The Server Edition is organized by implementation generation:

- `moral/2/` — configured Server and Service.
- `moral/3/` — remedial Server and Service with preflight, recovery, and
  evidence boundaries.

Service generations are additive. Service 3 does not silently replace Service
2.

Runtime state remains outside these source packages in `server-edition/inbox/`
and `server-edition/state/`.

The native `sleelas` launcher selects Service 2 by default and supports
explicit Service 3 selection with `SLEELA_SERVER_LEVEL=3`.


## Discord™ server naming theme

The three Server Edition programmatic names are:

- **Discord-1™** — native `sleelas` server-launcher/process boundary.
- **Discord-2™** — Service 2 configured Server and Service implementation.
- **Discord-3™** — Service 3 remedial Server and Service implementation.

These are SLeeLa implementation names only. They do not imply affiliation,
sponsorship, endorsement, ownership, or authorization by Discord Inc. No
Discord logo or proprietary Discord Brand Asset is included. Discord's brand
guidance states that use of Discord Marks in product or organization names
requires permission except where otherwise permitted. citeturn0search3
