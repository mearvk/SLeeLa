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
