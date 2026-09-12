# CRYPTOGRAPHIC_PACKET_IDENTITY.md — Packet Identity and Emergency Capsule

## 1. Purpose

This document defines a packet-level identity and emergency-contact architecture for the HTTP 3.0 project. The design separates a narrowly scoped identity capsule from the protected application payload.

The identity mechanism is intended to support authorized emergency handling, jurisdictional routing, and authenticated identification without creating a universal decryption key for Internet traffic.

**The packet identity capsule is not a substitute for HTTP/3, QUIC, TLS, or end-to-end application security.** It is an additional policy and metadata layer.

## 2. Core Principle

Every packet participating in this architecture may carry a compact **Cryptographic Identity Capsule (CIC)**.

The CIC identifies a protected reference to the sender and provides an emergency contact path. It does not expose the sender's raw National ID, telephone number, DNA identifier, or other sensitive identity attributes in plaintext.

The substantive application payload remains separately protected.

```text
+--------------------------------------------------+
| HTTP/3 / QUIC packet                             |
+--------------------------------------------------+
| Public capsule identifier                        |
| Version / jurisdiction / expiration              |
| Emergency endpoint reference                     |
+--------------------------------------------------+
| Authenticated encrypted identity capsule         |
|                                                  |
|   Sender reference                               |
|   Authorized contact reference                   |
|   Jurisdictional identity reference              |
|   Optional emergency attributes                  |
|                                                  |
+--------------------------------------------------+
| End-to-end protected application data            |
+--------------------------------------------------+
```

## 3. Replacement for Legacy DES

The project does **not** use 32-bit, 48-bit, 64-bit, or 78-bit DES keys as a security mechanism.

DES and deliberately short cryptographic keys are unsuitable for protecting identity information. In particular, a deliberately crackable key must not be used to expose National ID numbers, telephone numbers, DNA identifiers, or equivalent sensitive attributes.

The design instead separates three concepts:

1. **Capsule identifier** — a non-secret identifier used to locate or reference a capsule.
2. **Capsule encryption** — modern authenticated encryption protecting the capsule contents.
3. **Identity resolution** — an authorized process that resolves a pseudonymous reference to sensitive identity records.

A 128-bit-or-larger capsule identifier may be used as an identifier. It is not itself the encryption key.

## 4. Cryptographic Construction

The preferred construction is a modern authenticated-encryption design with a public-key or hybrid mechanism for protecting the capsule key.

A conceptual construction is:

```text
Country / Emergency-Service Public Key
                 |
                 v
                HPKE
                 |
                 v
       Capsule Encryption Key
                 |
                 v
       AES-256-GCM or
       ChaCha20-Poly1305
                 |
                 v
       Cryptographic Identity Capsule
```

A future implementation may specify an approved post-quantum or hybrid KEM while retaining the capsule abstraction.

The HTTP/3 specification should avoid inventing a new cipher.

## 5. Capsule Contents

A CIC may contain:

```text
CIC {
    version
    capsule_id
    jurisdiction_id
    expiration
    emergency_endpoint_reference
    sender_reference
    authorized_contact_reference
    identity_registry_reference
    optional_emergency_attributes
    authentication_data
}
```

### Sender Reference

A pseudonymous sender reference identifies the relevant identity record without placing the underlying identity document directly in every packet.

### Emergency Endpoint Reference

The capsule may identify an authorized emergency service through a hostname, service identifier, or other protocol-defined endpoint reference.

An implementation may maintain a secondary contact mechanism for exceptional circumstances, but that mechanism must not become a universal decryption path.

### Identity Registry Reference

A registry reference permits an authorized authority to resolve the sender reference under its applicable authorization and legal process.

The packet itself should not carry unnecessary copies of sensitive identity records.

## 6. Sensitive Identity Data

The following information is treated as highly sensitive and should not ordinarily appear in plaintext packet headers:

- National ID numbers;
- telephone numbers;
- government identifiers;
- biometric identifiers;
- DNA identifiers;
- complete identity documents;
- unnecessary personal records.

Where such information is legitimately required for an emergency service, it belongs inside an authenticated encrypted capsule or an authorized external identity system.

A DNA identifier, in particular, should not be a routine packet field. Its use requires an explicit policy, lawful authority, data minimization, and appropriate retention controls.

## 7. Horizontal Jurisdictional Isolation

The architecture supports multiple independently encrypted capsules in the same logical transmission.

```text
                         ONE TRANSMISSION
                                |
             +------------------+------------------+
             |                  |                  |
             v                  v                  v
        Country A           Country B           Country C
         capsule             capsule             capsule
             |                  |                  |
           A-key              B-key              C-key
             |                  |                  |
             +------------------+------------------+
                                |
                                v
                     End-to-end application key
```

The essential property is **cryptographic separation**:

> Possession of the key for one jurisdictional capsule does not provide the key material necessary to decrypt another jurisdictional capsule or the end-to-end application payload.

Thus a Country A authority may process Country A's designated capsule without acquiring authority over Country B's capsule or the destination's application data.

## 8. Route-Aware Capsules

The Cryptographic Route Map may select capsules according to the source, destination, authorized jurisdictions, network policy, and measured route conditions.

For a conceptual route:

```text
Source -> Router 1 -> Country A -> Country B -> Destination
```

an implementation could carry:

```text
Source identity capsule
Country A emergency capsule
Country B emergency capsule
End-to-end application protection
```

The route map may add an authorized capsule as policy requires. Existing application protection must not be silently downgraded because a packet enters a different jurisdiction.

## 9. Cryptographic Recovery Grades

The project may use the following policy grades:

| Grade | Purpose |
|---|---|
| CR-1 | Public or minimally identifying capsule reference |
| CR-2 | Authorized emergency lookup reference |
| CR-3 | Authenticated jurisdictional identity capsule |
| CR-4 | Strong jurisdictional capsule with modern/hybrid cryptographic protection |

These grades describe **cryptographic handling**, not a ranking of people or countries.

## 10. No Universal Interception Key

HTTP 3.0 shall not require a universal government, country, router, service provider, or intermediary key capable of decrypting unrelated communications.

There shall be no master key from which all jurisdictional capsules can be recovered.

There shall be no requirement that an intermediate router decrypt the application payload merely to forward a packet.

## 11. No Silent Downgrade

Cryptographic protection may be added or strengthened as a route develops, but an established security level shall not be silently reduced by route selection.

```text
CR-2 -> CR-3 -> CR-4
```

is a permissible strengthening path.

```text
CR-4 -> CR-1
```

is not a valid automatic downgrade.

## 12. Packet-by-Packet Consistency

If a deployment elects to attach a CIC to every packet, each CIC should contain an authenticated association to the relevant flow, sequence context, or packet-generation context.

This prevents an identity capsule from being copied from one flow and attached to an unrelated flow without detection.

A conceptual association is:

```text
CIC = AEAD(
    capsule_key,
    identity_payload,
    packet_context
)
```

where `packet_context` may bind the capsule to a protocol version, flow identifier, direction, epoch, or other implementation-defined context.

## 13. Emergency Operation

An emergency endpoint may receive a capsule when an authorized emergency condition exists. The endpoint should validate:

1. capsule authenticity;
2. jurisdiction or service authorization;
3. expiration;
4. packet/flow association;
5. emergency-policy authorization;
6. minimum necessary identity disclosure.

Emergency processing should disclose only the information required for the authorized purpose.

## 14. Identity Resolution

The preferred architecture is:

```text
Packet
  |
  v
Pseudonymous Sender Reference
  |
  v
Authorized Identity Service
  |
  +--> identity record
  +--> contact association
  +--> jurisdictional information
  +--> other authorized attributes
```

This avoids placing a complete identity dossier in every Internet packet.

## 15. Relationship to the Cryptographic Route Map

The CIC is one component of the broader **Cryptographic Route Map**.

```text
                 CRYPTOGRAPHIC ROUTE MAP
                           |
        +------------------+------------------+
        |                  |                  |
        v                  v                  v
     Source             Route              Destination
     profile            profile              profile
        |                  |                  |
        +------------------+------------------+
                           |
                           v
                  Cryptographic Policy
                           |
             +-------------+-------------+
             |                           |
             v                           v
      Identity capsules          End-to-end payload
```

The country comparator can contribute to policy analysis, but political or economic classification must not itself determine whether a person is entitled to privacy or security.

## 16. Privacy and Minimization

The packet identity layer should follow data minimization principles.

A packet should carry the smallest identity reference necessary for the protocol's authorized purpose. Sensitive attributes should remain in protected systems and should be resolved only when required.

The architecture therefore distinguishes:

**Identification** — determining which protected reference is associated with a packet.

**Authentication** — establishing that the reference was legitimately generated.

**Authorization** — determining who may resolve or use the reference.

**Decryption** — obtaining protected capsule contents.

These are separate operations and should not be collapsed into a universal interception mechanism.

## 17. Interoperability

The identity capsule is an optional policy layer and must not prevent ordinary HTTP/3 implementations from operating when the capability is absent.

A capable implementation should be able to advertise support through an authenticated protocol mechanism rather than assuming that every Internet host understands CICs.

Unknown or unsupported capsule extensions should be safely ignored or rejected according to the relevant HTTP/3/QUIC extension rules.

## 18. Design Summary

The resulting model preserves the project's original packet-level identity concept while replacing obsolete short-key cryptography with modern cryptographic separation:

```text
EVERY PACKET
    |
    +-- compact capsule identifier
    |
    +-- authenticated identity reference
    |
    +-- optional jurisdictional emergency capsule(s)
    |
    +-- independently protected HTTP/3 application payload
```

The governing principle is:

> **Every packet may carry a verifiable identity reference; no packet requires a universally crackable identity cipher, and no jurisdiction receives cryptographic authority over information belonging to another jurisdiction or over the end-to-end application payload merely by observing the packet.**

## 19. Status

This document is a conceptual HTTP 3.0 project specification. It does not assert that the described identity-capsule mechanism is part of the current IETF HTTP/3 or QUIC standards.

Any implementation would require separate security review, privacy review, interoperability analysis, key-management specifications, and applicable legal/policy review before deployment.
