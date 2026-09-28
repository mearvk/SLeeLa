# HTTP 3.0 Key Distribution Server

## Purpose

The SLeeLa HTTP 3.0 Key Distribution Server (KDS) provides a small bootstrap service for clients that need an authenticated starting point for route-aware cryptographic communication. It distributes **public bootstrap information and encrypted bootstrap contracts**; it does not distribute reusable plaintext private keys.

The design is compatible with the existing Cryptographic Identity Capsule (CIC), Cryptographic Route Map, and automatic route-session rekeying layers.

## Initial Contract

A client creates a fresh X25519 ephemeral key pair and submits its public key, a client identifier, and a jurisdiction identifier over an authenticated TLS 1.3 connection.

The KDS holds a long-lived X25519 bootstrap private key and public key. The server computes:

`DH = X25519(server_private, client_public)`

and derives the contract encryption key with HKDF-SHA256 using the contract's fresh 128-bit key identifier and the context:

`SLeeLa/HTTP3/KDS/bootstrap/v1`

The bootstrap contract is then encrypted with AES-256-GCM. The contract binds the client identifier, jurisdiction, issuance time, expiration time, server public key, client public key, and key identifier.

OpenSSL's EVP provider interfaces provide the X25519 and HKDF operations rather than a custom cryptographic implementation. OpenSSL documents X25519 key generation and derivation through EVP_PKEY, and HKDF through EVP_KDF. citeturn0search0turn0search4

## Key Distribution Is Not Private-Key Distribution

The server never returns its private bootstrap key. The client never uploads its private key. A client private key remains local to the client process.

The service therefore establishes an exact initial cryptographic contract without creating a universal interception key.

## 10,000 Keys per Minute Limit

The server contains a synchronized issuance limiter with a hard ceiling of:

**10,000 bootstrap contracts per minute.**

Only successful contract-issuance slots consume the issuance counter. Public-key discovery does not consume an issuance slot. When the limit is reached, the server returns an HTTP `429` response and does not issue another contract until the next minute window.

This is an issuance ceiling, not a promise that a deployment can sustain 10,000 TLS handshakes per minute. Production deployments should use ordinary network load balancing, connection controls, operating-system limits, and observability appropriate to their capacity.

## Endpoints

### `GET /v1/public-key`

Returns the server's X25519 public bootstrap key. The response is cacheable only according to deployment policy; the implementation marks bootstrap responses as `no-store` for conservative key-management behavior.

### `POST /v1/bootstrap`

Expected JSON fields:

- `client_public` — 32-byte X25519 public key encoded as 64 hexadecimal characters.
- `client_id` — bounded client identifier.
- `jurisdiction` — bounded jurisdiction identifier.

The response contains the exact contract metadata and an AES-256-GCM encrypted contract package.

## Transport Security

The reference executable is a TLS 1.3 server using OpenSSL. The certificate and TLS private key are supplied as deployment files; they are not generated into the repository. OpenSSL provides documented TLS server construction using its SSL/TLS APIs. citeturn0search8

The KDS bootstrap key is generated on first start if its local key file does not exist. The file is created with owner-only permissions (`0600`). Deployments should place that file on protected persistent storage and use normal host-level key backup, rotation, and access-control procedures.

## Decentralized Compatibility

The KDS is deliberately optional to the larger HTTP 3.0 design. A deployment may use it as the initial trust/bootstrap service while later route keys are established directly with X25519. Public-key discovery can also be supplied through an independently authenticated mechanism such as preconfigured trust roots, a PKI, signed configuration, DNSSEC/DANE, or another deployment-specific trust system.

The KDS therefore solves the **initial exact-contract problem** without requiring the KDS to remain in the data path for every HTTP 3.0 packet.

## Route Changes

After bootstrap, the existing route session remains responsible for route-aware key epochs. A route change can trigger a fresh ephemeral X25519 exchange and HKDF derivation. The KDS does not decrypt application traffic and does not need to be consulted for every route change.

This preserves the intended architecture:

`KDS bootstrap -> HTTP/3 -> QUIC -> route session -> CIC/jurisdiction packages -> application`

A route change must not silently downgrade an established cryptographic grade.

## Nation-Centric Packages

Jurisdictional encryption remains separate from KDS bootstrap. The existing capsule-set implementation creates independently encrypted packages for authorized jurisdictions. A jurisdiction identifier is a policy and recipient boundary; it is not itself a cryptographic strength score.

The KDS can therefore provide the initial trust material needed to bring a jurisdiction-aware client online without making one country's key capable of decrypting another country's package.

## Security Properties

1. X25519 provides the initial Diffie-Hellman agreement.
2. HKDF-SHA256 separates the DH result into a contract-specific symmetric key.
3. AES-256-GCM authenticates and encrypts the bootstrap contract.
4. Every contract has a fresh 128-bit key identifier.
5. Client private keys remain client-side.
6. The server private bootstrap key is never sent over the wire.
7. The issuance rate is bounded at 10,000 contracts per minute.
8. No universal interception or master decryption key is created.
9. Route-session rekeying can proceed without a centralized key distributor.
10. The design uses established OpenSSL primitives rather than a new cipher.

## Operational Invocation

The reference executable accepts:

```text
key_distribution_server [server.crt] [server.key] [kds-bootstrap.key] [port]
```

Defaults are:

- certificate: `server.crt`
- TLS private key: `server.key`
- KDS X25519 bootstrap key: `kds-bootstrap.key`
- TLS port: `9443`

The repository Makefile builds the executable as `key_distribution_server` and also runs the existing cryptographic self-test and C++ syntax checks.

## Scope

This is a SLeeLa HTTP 3.0 project component, not an IETF-standard HTTP/3 key-distribution protocol. Interoperability with external systems requires an explicit deployment profile covering trust roots, certificate policy, endpoint discovery, key rotation, audit logging, and contract lifetime.
