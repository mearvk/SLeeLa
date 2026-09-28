# psychiatry

## `Secret.key`

[`Secret.key`](Secret.key) is a **38,246-hex-digit CSPRNG value** (152,984 bits),
generated with a cryptographically secure random source and stored as lowercase
hexadecimal on a single line.

It is a **project artifact / token**, in the same spirit as
[`../CLASS.md`](../CLASS.md) and [`../MIL.SPEC.md`](../MIL.SPEC.md) — a fixed,
committed random value used as a repository identifier/marker, **not** a live
cryptographic credential.

> **Note.** Because it lives in version control, do not treat this file as an
> actual secret to protect. If a genuine, protectable secret is ever needed, it
> must be generated out-of-band and kept out of the repository (e.g. supplied at
> runtime or via an ignored path), never committed.

- **Length:** 38,246 hexadecimal digits (exact)
- **Encoding:** lowercase hex, single trailing newline
- **Source:** cryptographically secure RNG (`secrets`/`os.urandom`)

## `keysearch` — verify the key against GitHub

From the SLeeLa terminal:

```text
$> SLeeLa keysearch
SLeeLa keysearch: MATCH
  sha256=<64-hex> (38246 hex digits)
```

`keysearch` ([`keysearch.py`](keysearch.py), dispatched by
[`../bin/SLeeLa`](../bin/SLeeLa)) makes a **read-only HTTPS GET** of the repo's
published `Secret.key` and compares it to the local copy. Results: `MATCH`,
`MISMATCH`, `NOT FOUND` (remote missing / offline / HTTP error), or `ERROR`
(local key unreadable). Exit codes: `0` / `1` / `2` / `3` respectively.

**It never uploads the key.** Comparison is by **SHA-256 digest** computed on
both sides, so the raw key material never leaves the machine and is never logged,
and any network problem **fails closed** to `NOT FOUND` (never a false `MATCH`).

Configuration: `SLEELA_KEY_URL` overrides the remote URL (default: the repo's
`main`-branch raw URL); `SLEELA_KEY_PATH` overrides the local key path.
