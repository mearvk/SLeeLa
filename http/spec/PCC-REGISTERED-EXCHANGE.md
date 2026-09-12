# PCC Registered Exchange and Envoy Integration

## 1. Registered Exchange

The Registered Exchange is the PCC control-plane mechanism by which an HTTP 3.0 endpoint requests, verifies, and receives registration receipts from an authorized Registered Router.

The exchange is associated with a protected packet or flow commitment rather than requiring application plaintext.

## 2. Conceptual Message

```text
PCC-REGISTER
  PCC-ID
  Circuit-ID
  Epoch
  Packet/Flow Commitment
  Requested Registration Class
  Requested Router Role
  Expiration
  Exchange Nonce
```

Response:

```text
PCC-RECEIPT
  PCC-ID
  Circuit-ID
  Router-ID
  Router-Role
  Epoch
  Observation Time
  Packet/Flow Commitment
  Receipt Sequence
  Router Signature
```

The exchange nonce prevents a stale response from being mistaken for the current registration operation.

## 3. Envoy Method

For an Envoy-based deployment, the preferred implementation is a PCC-aware HTTP/3 connection/stream extension or filter that can obtain the relevant route and forwarding metadata without exposing application plaintext solely for registration.

The filter performs:

1. PCC policy lookup.
2. Circuit resolution.
3. Commitment construction or verification.
4. Registered Exchange dispatch.
5. Receipt verification.
6. Bounded receipt caching.
7. Metrics/audit emission.
8. Configured failure handling.

The filter should operate asynchronously where possible so receipt exchange does not unnecessarily serialize packet forwarding.

## 4. Envoy Processing

```text
HTTP/3 listener
      |
      v
PCC-aware filter
      |
      +---- PCC disabled ----> ordinary forwarding
      |
      +---- PCC enabled
              |
              v
       circuit lookup
              |
              v
       commitment binding
              |
              v
       Registered Exchange
              |
              v
       Registered Router
              |
              v
        signed receipt
              |
              v
       verify + cache
              |
              v
        forwarding
```

## 5. Failure Policy

The filter supports `BEST_EFFORT`, `RETRY`, and `FAIL_CLOSED`. Failure handling must never produce plaintext fallback or a weaker cryptographic policy.

## 6. Security Boundary

Envoy must not be configured to terminate HTTP/3 application encryption solely because PCC registration is enabled. PCC registration and HTTP/3 content decryption are separate authorities.

## 7. Future Envoy Adapter

A production adapter can expose a narrow PCC interface to Envoy containing only:

```text
PCC policy
Circuit identifier
Flow/packet commitment
Registration request
Receipt result
Failure disposition
```

This keeps PCC replaceable and prevents the registration layer from becoming an implicit application-data inspection interface.
