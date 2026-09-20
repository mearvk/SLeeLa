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
