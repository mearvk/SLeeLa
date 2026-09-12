# PCC Circuit Specification

## Circuit Descriptor

A PCC circuit identifies the intended registration path without replacing the underlying HTTP/3 route.

```text
PCC-ID
Circuit-Version
Registration-Class
Origin-Policy
Country-Policy
Region-Policy
Router-Role Requirements
Receipt Policy
Failure Policy
Epoch
Expiration
```

## Circuit Processing

```text
sender
  |
  v
create PCC descriptor
  |
  v
resolve authorized routers
  |
  v
create packet/flow commitment
  |
  v
Registered Exchange
  |
  +--> Router receipt
  |
  +--> optional chained receipt
  |
  v
continue ordinary HTTP/3 forwarding
```

The circuit is a registration overlay. It does not require the packet to be routed through an application-level proxy merely because a receipt is requested.

## Registration Scope

The sender may request country, border, developmental-region, multi-country, or full-circuit registration. Network policy may require a minimum registration class.

## Route Changes

If the underlying HTTP 3.0 route changes, PCC evaluates whether the circuit remains valid. A route change may cause a new registration epoch. It must not silently invalidate or rewrite an already authenticated receipt.

## Privacy

The circuit should identify the minimum information needed to satisfy registration policy. It should not contain raw identity documents, phone numbers, DNA identifiers, biometric templates, or unrelated personal information.
