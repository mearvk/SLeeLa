# Skya Connection Security

Skya connections carry **optional, opportunistic crypto**: every connection —
client↔server and client↔client — may establish an encrypted, optionally
authenticated session, and still works in plaintext if crypto is unavailable.

## Principles

- **Crypto if we can, plaintext if we must.** A connection is never blocked by
  the absence of a certificate or a crypto-incapable peer. If either side has no
  certificate, or doesn't negotiate crypto, the session runs in plaintext.
- **Diffie–Hellman for the initial exchange.** Each connection generates an
  ephemeral 2048-bit DH keypair and derives a shared secret; the AES-256-GCM
  session key is `SHA-256(DH shared secret)`.
- **DSA for authentication (small keys).** A peer may attach a DSA-signed
  certificate and a DSA signature over its DH public value, so the other side
  can authenticate it. Unsigned/absent → encrypted but unauthenticated.
- **~30-day certificates, renewable.** The local certificate is valid for 30
  days by default; the GUI prompts to renew within 5 days of expiry, and a
  renewal issues a fresh certificate. A newer connection presenting a fresh
  certificate supersedes an older one.

## Primitives (all pure-JDK, no external libraries)

| Role | Algorithm |
|---|---|
| Key agreement | Diffie–Hellman, 2048-bit, ephemeral per connection |
| Authentication / signatures | DSA, 2048-bit (`SHA256withDSA`) |
| Session cipher | AES-256-GCM (128-bit tag, random 12-byte IV per message) |
| Certificate | self-managed `SKYACERT/1` record, DSA-signed, 30-day validity |

## Handshake (optional, line-oriented)

```
A -> B : SKYACRYPTO/1 HELLO dh=<b64 DH pub> [cert=<b64 of SKYACERT line>] [sig=<b64 DSA over dh>]
B -> A : SKYACRYPTO/1 HELLO dh=<b64 DH pub> [cert=...] [sig=...]
```

Both sides derive the same AES-256-GCM key and exchange `SKYACRYPTO/1 DATA <b64>`
frames. A peer that cannot do crypto replies `SKYACRYPTO/1 PLAIN` (or simply its
normal protocol line); the initiator rewinds and continues in plaintext. The
SLeeLa `.sleela` server has no DH/DSA and always answers `PLAIN`.

## Certificate (`SKYACERT/1`)

A one-line, DSA-signed record binding a subject to a DSA public key for a
validity window — deliberately **not** an X.509/CA certificate (Skya is
peer-to-peer and opportunistic):

```
SKYACERT/1 subject=<b64> notBefore=<ms> notAfter=<ms> dsaPub=<b64> sig=<b64 SHA256withDSA over the body>
```

The local certificate is stored at `~/.sleela/skya/skya-cert.txt` (override with
`SKYA_CERT_FILE`), loaded on startup, and re-issued automatically if missing or
expired.

## Configuration

| skya.conf key | Meaning | Default |
|---|---|---|
| `security.crypto.enabled` | master switch (false → `SKYA_CRYPTO=off`, plaintext) | true |
| `security.crypto.dh.bits` | DH key size | 2048 |
| `security.crypto.dsa.bits` | DSA key size | 2048 |
| `security.crypto.cipher` | session cipher | AES-256-GCM |
| `security.certificate.validity.days` | certificate validity window | 30 |
| `security.certificate.renew.window.days` | prompt-to-renew lead time | 5 |
| `security.certificate.required` | if true, prefer crypto (still falls back) | false |

| Env var | Meaning |
|---|---|
| `SKYA_CRYPTO=off` | disable crypto entirely (plaintext) |
| `SKYA_CERT_FILE` | path to the local certificate store |

## Scope / limits

- The crypto layer lives in the **Java/Guia** components (`SkyaCrypto`,
  `SkyaCertificate`, `SkyaCertStore`), which cover the GUI↔agent and
  client↔client/server connections.
- The native C engine and the `.sleela` programs do **not** implement DH/DSA
  (no crypto library / no VM primitives); they interoperate by negotiating
  plaintext. Bringing encryption to the native transport (e.g. TLS via OpenSSL)
  is future work.
- This is application-layer crypto over TCP, not kernel/OS TLS. It authenticates
  Skya peers to each other; it is not a public-CA trust chain.
