# Sleela ⇄ SecureJDK 28 — Secure Transition (STP-0001)

This directory is the **Sleela client side** of the *Secure Transition Protocol*
(**STP-0001**, spec in [`STP.model`](STP.model)). It lets a Sleela program
*transition* its execution under the supervision of **SecureJDK 28**, over a
cryptographically secured **local pipe** (or an optional **remote** channel).

The supervisor side lives in the Ubuntu.Determinant repo under
`userland/openjdk/transition-supervisor/`.

## What it does

When you run `sleela run --secure <file>`, before executing the program Sleela:

1. **Connects** to the SecureJDK 28 Transition Supervisor — a local
   UNIX-domain socket by default (`/run/sleela/stp.sock`, fallback
   `/tmp/sleela-stp.sock`), or `--remote=host:port`.
2. **Handshakes** with state-of-the-art crypto — Ed25519 identities (with key
   pinning), ephemeral X25519 (forward secrecy), HKDF-SHA-256, and
   ChaCha20-Poly1305 AEAD for every frame. Even on the local pipe the transition
   is end-to-end encrypted and authenticated.
3. **Submits its memory model** (globals, functions, code length, thread/lock/
   mailbox usage, estimated heap) + a parse digest, asking SecureJDK for
   monitorability, admin, and secure supervision.
4. On **ADMIT**, receives a *regioned, memorable, secured* acknowledgement — a
   named supervised region (e.g. `reg-emerald-0007`) plus an HMAC ack tag the
   client verifies against the derived region key — and runs **under
   supervision** (heartbeating the observer circuit).
5. On **failure** (DENY / crypto failure / timeout / unreachable), Sleela does
   not abort: it **continues locally as a secure "safe trim"** (thread cap
   lowered to 16) and **records the failed transition** for later Admin review
   in the private secured MySQL (or a local JSONL fallback when MySQL is down).

## Files

| File | Role |
|------|------|
| `STP.model`                | The normative protocol spec (STP-0001). |
| `stp_wire.{h,c}`           | Frame header + portable flat-map TLV codec (byte-for-byte compatible with the Java `Wire`). |
| `stp_crypto.{h,c}`         | OpenSSL 3 (`libcrypto`) crypto: Ed25519 / X25519 / HKDF-SHA256 / ChaCha20-Poly1305 / SHA-256. Compiled only with `-DSTP_HAVE_OPENSSL`. |
| `stp_crypto_optional.h`, `stp_sha256_portable.c` | Dependency-free SHA-256 for the parse digest (always built). |
| `stp_client.{h,c}`         | Connect, handshake, submit, ACK/DENY handling, and the always-available failure recorder. |
| `mem_model.{h,cpp}`        | Derives the memory-model summary from the parsed AST. |

## Build

The Makefile links the client into `sleela` by default (OpenSSL present):

```sh
cd impl && make                 # STP_SSL=1 (default): full crypto client, links -lcrypto
cd impl && make STP_SSL=0       # no-crypto build: --secure always safe-trims
```

## Run

```sh
# Local pipe (needs the supervisor running; see the supervisor README):
sleela run --secure --pin=<supervisor_ed25519_hex> examples/hello.sleela

# Remote:
sleela run --secure --remote=host:8443 --pin=<hex> examples/hello.sleela

# With no supervisor reachable, it safe-trims and records the failure:
sleela run --secure examples/threads.sleela
#   [sleela] SECURE TRANSITION: FALLBACK (UNREACHABLE: no supervisor on the pipe)
#   [sleela]   continuing LOCALLY as a secure safe-trim (thread cap 16)
```

Environment: `SLEELA_STP_PIPE` (pipe path), `SLEELA_STP_FALLBACK` (JSONL failure
file), `SLEELA_SHEET` (catalog, for the Constitution values in the request).
