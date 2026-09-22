# SLeeLa Server Edition / Moral Source Layout

The Server Edition source is deliberately separated under `server-edition/moral/`.
Runtime state remains outside that source tree:

- `moral/serverd` — executable launcher.
- `moral/src/Server.sleela` — authoritative SLeeLa server source.
- `../inbox/` — runtime input.
- `../state/` — runtime state and results.

## Design Activity

The launcher exposes the same Design Activity interface as the primary SLeeLa
executables:

`serverd --design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>`

The six scores are explicit caller-supplied engineering observations. The
launcher does not invent or silently elevate scores. It delegates normalization
and variance calculation to the authoritative SLeeLa Design Activity
implementation.

Design variance is a software-engineering comparison against a versioned
reference profile. It is not a physical measurement uncertainty statement.

## Integrity

The launcher prefers `security/important-sha256-manifest.json`, matching the
repository's current important-file integrity manifest, while retaining a
backward-compatible fallback for the older manifest filename.

## Runtime boundary

The Server Edition remains local by default. It processes its owned inbox,
writes state locally, and does not create a public listening socket itself.
