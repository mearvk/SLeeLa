# Defender: explicit security gates

Read-only detection:

```sh
sleela defender detect
```

Fetch/build/install/provision are security-sensitive and require explicit
opt-in via `--allow-defender` (or `--optin`) or
`SLEELA_DEFENDER_OPTIN=1`. Privileged installation additionally requires
`--allow-root` or `SLEELA_DEFENDER_ALLOW_ROOT=1`. A fetched archive must
match an independently obtained SHA-256 via `--sha256 HEX` or
`SLEELA_DEFENDER_SHA256=HEX`.

Do not run a mutating example with a placeholder digest. Obtain the expected
hash from a trusted independent channel and inspect the payload first.
