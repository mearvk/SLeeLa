# advantage / transport — secure acquisition layer

A secure, fast, and **idempotent** transport layer for searching, reviewing,
obtaining, downloading, and storing remote data over **HTTP(S)**, **SFTP**,
and **SSH**, with **Diffie-Hellman / ECDH key exchange** and an
**encrypted-at-rest key store**.

It is built on **OpenSSL** (the audited library the repository's own
`crypto/README.md` recommends for protecting real data) and on the system
**OpenSSH** client for SSH/SFTP. It deliberately does not hand-roll TLS or the
SSH protocol.

## Components

| File | Lang | Responsibility |
|------|------|----------------|
| `keystore.{h,c}` | C | AES-256-GCM encrypted secret store; PBKDF2-HMAC-SHA-256 key derivation; atomic writes |
| `kex.{h,c}` | C | DH (FFDHE2048/3072) and ECDH (X25519, P-256) key agreement → HKDF-SHA-256 session key |
| `digest.{h,cpp}` | C++ | SHA-256 content addressing / integrity |
| `uri.{h,cpp}` | C++ | URI parsing for https/http/sftp/ssh/file |
| `http_client.{h,cpp}` | C++ | TLS-verified HTTP(S) GET/HEAD with redirects, timeouts, size caps |
| `sftp_client.{h,cpp}` | C++ | SSH/SFTP download via OpenSSH `scp` (argv-only, no shell) |
| `acquire.{h,cpp}` | C++ | Idempotent, content-addressed acquisition facade (search/review/obtain/download/store) |
| `transport_cli.cpp` | C++ | Command-line front end |

## Build

```sh
make            # produces ./transport
```

Requires OpenSSL development headers (`libssl`/`libcrypto`). For `sftp://`
and `ssh://` fetches, the OpenSSH `scp` binary must be on `PATH` at runtime.

## Usage

```sh
# Review remote metadata without downloading the body (HTTP HEAD)
transport review https://example.com/file.bin

# Obtain + download + store (idempotent, content-addressed)
transport fetch  https://example.com/file.bin --store ./store
transport fetch  sftp://user@host/path/file   --store ./store
transport fetch  file:///etc/hostname          --store ./store

# Pin expected content; a mismatch fails and discards the bytes
transport fetch  https://example.com/f --sha256 <hex> --store ./store

# Encrypted key store (passphrase via $ADVANTAGE_KS_PASS, never argv)
export ADVANTAGE_KS_PASS='…'
transport keystore secrets.ks put api_token 's3cr3t'
transport keystore secrets.ks get api_token
transport keystore secrets.ks list
transport keystore secrets.ks rm  api_token

# Demonstrate a key exchange + derived session key
transport kex x25519        # also: p256, ffdhe2048, ffdhe3072
```

## Security model

- **TLS verification on by default.** HTTPS verifies the full certificate
  chain (`SSL_VERIFY_PEER`, TLS 1.2 minimum) and matches the hostname to the
  certificate (`X509_VERIFY_PARAM_set1_host`). SNI is sent. `--insecure`
  disables this and is intended only for local testing.
- **SSH host-key checking enforced.** `scp` runs with
  `StrictHostKeyChecking=yes` (or `accept-new` for trust-on-first-use) and
  `BatchMode=yes`; it is never set to `no`. Arguments are passed as an argv
  vector via `posix_spawn`, so remote paths/hosts cannot inject shell
  commands.
- **Encrypted key store.** Secrets are sealed with AES-256-GCM under a key
  derived from a passphrase with PBKDF2-HMAC-SHA-256 (600k iterations). A
  wrong passphrase or any tampering fails the GCM authentication check. The
  passphrase is read from an environment variable, never from argv. Secret
  buffers are zeroed before being freed.
- **Key exchange.** Ephemeral DH/ECDH keypairs; the raw shared secret is run
  through HKDF-SHA-256 with an application `info` label and then wiped. The
  private key never leaves the process.

## Idempotency & integrity

- Downloads are stored in a **content-addressed store** (CAS):
  `objects/<aa>/<sha256>`. Identical content always maps to the same object,
  so re-fetching is a no-op (`fromCache = true`).
- Writes are **atomic**: data is written to a temp file and `rename(2)`'d into
  place, so an interrupted transfer never corrupts the store.
- An optional `--sha256` pin verifies downloaded content against an expected
  digest and discards it on mismatch.

## Verified behavior

The build and the following were exercised locally:

- All four key-exchange groups complete and both parties derive **identical**
  session keys.
- Keystore: put/get/list/remove round-trip; idempotent replace; **wrong
  passphrase rejected**; secret absent from the on-disk ciphertext.
- Acquisition: first fetch stores, re-fetch reports **cached**; correct hash
  pin passes, **wrong pin rejected**.
- HTTPS against a local server: **default verification rejects a self-signed
  cert**; with trust established the body downloads, is stored
  content-addressed, and re-fetch is cached.

## Notes / limits

- Live public-internet fetches could not be exercised in the build sandbox
  (outbound egress is filtered); the TLS client path was validated against a
  local HTTPS server instead.
- The HTTP client is scoped to GET/HEAD acquisition; it is not a full
  HTTP/1.1 engine (no keep-alive pooling, no chunked upload).
- Post-quantum key exchange (ML-KEM) is out of scope here; see the
  repository's `crypto/PQ-INTEGRATION.md` for that track.
