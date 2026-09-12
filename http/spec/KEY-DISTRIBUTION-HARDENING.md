# HTTP 3.0 Key Distribution Server — Hardening and Operations

## 1. Purpose

This document extends `KEY-DISTRIBUTION-SERVER.md` into the operational security profile for the SLeeLa HTTP 3.0 Key Distribution Server (KDS).

The KDS is a **bootstrap service**, not a permanent decryption authority. Its job is to help a new client establish an exact cryptographic contract. Once the client has a valid route session, normal HTTP/3 and QUIC traffic does not need to return to the KDS for every packet or route transition.

The architecture is therefore:

```text
Authenticated key discovery / trust roots
                |
                v
        TLS 1.3 KDS bootstrap
          X25519 + HKDF
                |
                v
       Exact encrypted contract
                |
                v
     HTTP 3.0 route cryptography
       CIC + route map + rekey
                |
                v
        HTTP/3 + QUIC traffic
```

## 2. Security Boundary

The KDS distributes **public bootstrap information and encrypted contracts**. It does not distribute plaintext private keys.

A server private bootstrap key remains on the KDS host. A client private X25519 key remains in the client process. No protocol endpoint is permitted to request the other party's private key.

There is no universal interception key and no universal master decryption key.

## 3. Cryptographic Profile

The reference implementation uses established OpenSSL primitives:

- X25519 for the initial Diffie-Hellman agreement.
- HKDF-SHA256 for context-bound key derivation.
- AES-256-GCM for authenticated encryption of bootstrap contracts.
- TLS 1.3 for the reference network transport.

The KDS does not define a new cipher.

The bootstrap derivation remains:

```text
DH = X25519(server_private, client_public)
key = HKDF-SHA256(DH, contract_key_id, SLeeLa/HTTP3/KDS/bootstrap/v1)
contract = AES-256-GCM(key, contract_key_id, plaintext_contract)
```

The contract binds at least the client public key, client identifier, jurisdiction, server public key, contract identifier, issuance time, and expiration time.

## 4. Key Rotation

Long-lived KDS bootstrap keys are deployment credentials and must have a defined rotation policy.

A production deployment SHOULD:

1. assign every published KDS public key a stable key version or key identifier;
2. publish the active key and its validity interval through an authenticated discovery mechanism;
3. introduce a replacement key before the current key expires;
4. permit a bounded overlap period in which both old and new keys can be validated;
5. stop issuing new contracts under an expired key;
6. retain old private key material only for the minimum contract-validation or migration period required by policy;
7. securely destroy retired private key material after the retention period;
8. record key-version transitions in an audit stream without recording private keys or plaintext contracts.

Rotation must never silently downgrade a client to an older cryptographic policy.

The current reference server stores its bootstrap X25519 key in the configured local key file. A deployment implementing automated rotation should treat the active-key identifier, validity interval, and trust metadata as a versioned control-plane object rather than modifying a key file in place while the process is serving traffic.

## 5. Authenticated Public-Key Discovery

`GET /v1/public-key` is a discovery mechanism, not by itself a trust anchor.

A client must have an independent reason to believe that the returned public key belongs to the intended KDS. Suitable deployment profiles include:

- preconfigured trust roots;
- PKI certificate validation;
- signed key manifests;
- DNSSEC/DANE where operationally appropriate;
- an authenticated enterprise configuration channel;
- a transparency or append-only key publication service;
- direct authenticated peer exchange.

A deployment MUST NOT treat an unauthenticated key retrieved from an arbitrary network location as sufficient proof of server identity.

The KDS can therefore remain replaceable: the trust system authenticates the KDS key, while the KDS performs the bootstrap contract operation.

## 6. Replay and Contract Freshness

Every issued contract has:

- a fresh 128-bit contract identifier;
- issuance time;
- expiration time;
- client public-key binding;
- server public-key binding;
- jurisdiction binding.

Clients MUST reject expired contracts.

Deployments requiring strict one-time bootstrap semantics SHOULD add a client-generated request nonce or monotonic request sequence and retain a bounded replay cache for the bootstrap lifetime. A replay cache may be partitioned by client identity and key version.

A repeated bootstrap request MUST NOT cause an already-established route session to be silently downgraded or replaced by weaker credentials.

## 7. Identity and Policy Binding

The bootstrap contract is a policy object, not a container for raw sensitive identifiers.

Client identifiers and jurisdiction identifiers are bounded protocol labels. National identification numbers, telephone numbers, raw DNA identifiers, biometric templates, passwords, or other unnecessary sensitive values MUST NOT be inserted into the bootstrap contract merely because a deployment has access to them.

Where a protected identity reference is necessary, it should point to an independently controlled authorization system. The KDS should carry the minimum information needed to bind the cryptographic contract to that reference.

## 8. Rate and Resource Limits

The reference server enforces a hard ceiling of:

**10,000 successfully issued bootstrap contracts per minute per server instance.**

The limit is intentionally an issuance limit, not a claim that a host can sustain 10,000 arbitrary TLS handshakes per minute.

The reference network executable additionally bounds concurrent connections to 256 and bootstrap request bodies to 4096 bytes. HTTP responses use bounded buffers, and TLS writes are completed through a write-all loop so a partial `SSL_write()` does not become a truncated response.

Production deployments SHOULD additionally apply:

- per-client quotas;
- per-source-address quotas;
- per-jurisdiction quotas where appropriate;
- connection and handshake rate controls;
- operating-system file-descriptor and process limits;
- upstream load-balancer limits;
- request timeouts;
- monitoring and alerting.

When multiple KDS replicas are deployed, the 10,000/minute ceiling is currently **per server instance**. A fleet-wide ceiling requires an external coordinated quota service or admission-control layer.

## 9. HTTP Request Validation

The reference executable uses exact request-method and path matching for the KDS endpoints:

- `GET /v1/public-key`
- `POST /v1/bootstrap`

Bootstrap requests require a bounded `Content-Length` and are read until the complete body has arrived. Requests exceeding the body limit receive `413 Payload Too Large`.

The implementation does not treat a prefix such as `/v1/bootstrap-extra` as the bootstrap endpoint.

JSON parsing remains intentionally small and bounded for the reference bootstrap message. Production deployments using a larger protocol profile SHOULD use a strict JSON parser with duplicate-key rejection, Unicode validation, schema validation, and explicit rejection of unknown security-sensitive fields.

## 10. Failure and Recovery

If the KDS is unavailable after a client already has a valid route session:

1. continue the existing session while its cryptographic and policy lifetime remains valid;
2. do not fall back to plaintext;
3. do not silently lower the cryptographic grade;
4. if a route change requires rekeying, attempt direct authenticated X25519 rekeying where the deployment permits it;
5. if the new route cannot satisfy policy, fail closed for the affected operation rather than weakening the session.

The KDS therefore is not required to remain in the packet path.

## 11. Route Integration

After bootstrap, the HTTP 3.0 route-session layer owns route-generation and key-epoch changes.

A route transition can cause:

```text
route comparison
      |
      +-- KEEP -> continue current session
      |
      +-- REKEY -> fresh ephemeral X25519 exchange
      |
      +-- REJECT -> do not transmit under an unacceptable route
```

Every actual route/key-epoch transition receives fresh ephemeral key material. A prior X25519 private key is not reused as the new route generation's ephemeral private key.

The KDS does not decrypt application traffic and does not need to inspect ordinary QUIC packets.

## 12. Jurisdictional and CIC Integration

The Cryptographic Identity Capsule (CIC) remains a separate privacy and jurisdiction layer.

A jurisdiction is a recipient/policy boundary. It is not a direct cryptographic strength score.

If several jurisdictions are authorized to receive protected metadata, the capsule-set layer creates independently encrypted packages. One jurisdiction's private key cannot decrypt another jurisdiction's package.

The KDS can bootstrap the client without creating a universal jurisdictional key.

## 13. Audit Logging

A production KDS SHOULD produce structured security events containing only operationally necessary metadata, such as:

- timestamp;
- server key version;
- contract identifier;
- route/bootstrap profile version;
- bounded client reference;
- jurisdiction label;
- issuance result;
- failure reason;
- rate-limit state;
- software/build version.

Logs MUST NOT contain:

- server private keys;
- client private keys;
- plaintext bootstrap contracts where avoidable;
- decrypted application payloads;
- passwords or authentication secrets;
- raw national IDs;
- telephone numbers unless independently required and authorized;
- DNA or biometric material.

Audit records should themselves be access-controlled and retained according to a documented policy.

## 14. Decentralized Operation

The KDS is intentionally not the sole mechanism for distributing cryptographic trust.

A deployment may combine:

```text
PKI / signed manifest / DNSSEC / trust roots
                    |
          authenticated KDS key
                    |
              bootstrap
                    |
          direct route rekeying
                    |
            HTTP/3 + QUIC
```

For environments where a central KDS is undesirable or unavailable, authorized peers may exchange authenticated public keys directly. The same X25519, HKDF, route binding, and no-downgrade principles continue to apply.

## 15. Post-Quantum Migration

The KDS interface should be treated as a KEM-agreement boundary rather than an assumption that X25519 will be the only algorithm forever.

A future profile can use a hybrid construction such as:

```text
X25519 shared secret
        +
PQC KEM shared secret
        |
        v
HKDF-SHA256 / future approved KDF
        |
        v
contract encryption key
```

Migration must be negotiated and authenticated. A client MUST NOT silently replace an approved hybrid policy with X25519-only operation when policy requires the hybrid profile.

## 16. Operational Health Checks

A production deployment SHOULD expose a separately protected health mechanism that can verify:

- TLS certificate validity;
- active KDS key version;
- key-file availability and permissions;
- clock correctness;
- OpenSSL/provider availability;
- issuance counter health;
- active connection count;
- recent issuance failures;
- audit pipeline health.

Health endpoints should not expose private keys or decrypted contract material.

## 17. Integration Test Sequence

The complete intended lifecycle is:

```text
client generates ephemeral X25519 key
            |
            v
authenticated KDS public-key discovery
            |
            v
TLS 1.3 bootstrap request
            |
            v
X25519 DH + HKDF
            |
            v
AES-256-GCM encrypted exact contract
            |
            v
client validates contract freshness and bindings
            |
            v
route session established
            |
            v
HTTP/3 + QUIC transmission
            |
            v
route changes
            |
            v
route comparison
       /          \
    KEEP          REKEY
      |              |
      |       fresh ephemeral X25519
      |              |
      +-------> continue
                     |
                     v
              next key epoch
```

The repository cryptographic self-test covers the core primitives and route-session rekey path. Deployment integration tests should additionally cover malformed HTTP, incomplete bodies, oversized requests, exact endpoint matching, partial TLS writes, expired contracts, rate exhaustion, key rotation, KDS outage, and trust-root failure.

## 18. Security Invariants

The following invariants are normative for this project profile:

1. No private key crosses the network.
2. No universal interception key exists.
3. No route transition silently downgrades cryptographic protection.
4. Every contract is bound to its intended cryptographic context.
5. Expired contracts are not accepted.
6. KDS bootstrap is not required for every HTTP/3 packet.
7. Jurisdiction labels do not themselves determine cryptographic strength.
8. Raw sensitive identity data is not inserted merely for convenience.
9. Issuance is bounded by the 10,000-per-minute server-instance ceiling.
10. Production fleet-wide limits must be coordinated outside the local counter.
11. Public-key discovery must have an independent trust mechanism.
12. Key rotation must be authenticated and versioned.
13. Route rekeying uses fresh ephemeral key material.
14. Failures never trigger plaintext or weaker-policy fallback.

## 19. Current Project Status

The SLeeLa project is **happy with the HTTP 3.0 Key Distribution Server setup so far**. The current architecture is considered **satisfactory and well-structured at this stage of development**.

The separation of responsibilities between authenticated key discovery, KDS bootstrap, route-aware cryptography, Cryptographic Identity Capsules, jurisdictional protection, and automatic route-session rekeying provides a strong foundation for continued development.

The present design also preserves an important architectural property: the KDS helps a new client establish an exact cryptographic contract without becoming a permanent dependency for ordinary HTTP/3 traffic.

Further improvements may be made as implementation and deployment experience develops, but the current architecture provides a coherent, bounded, and useful foundation for continuing the HTTP 3.0 work.

**Status: SATISFACTORY — CONTINUE DEVELOPMENT.**

## 20. Scope

This document defines the SLeeLa HTTP 3.0 KDS operational profile. It is not an IETF standard and does not alter the HTTP/3 or QUIC wire protocol by itself.

The profile is designed to sit alongside the existing HTTP 3.0 Cryptographic Route Map, Cryptographic Identity Capsule, route-session rekeying, and jurisdictional capsule-set specifications.
