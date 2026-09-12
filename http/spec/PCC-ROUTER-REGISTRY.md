# PCC Router Registry

## Purpose

The PCC Router Registry defines the authenticated set of routers permitted to issue Postal Curious Circuit registration receipts for a country, developmental region, or other administrative network.

A registry entry is an authorization record, not a cryptographic decryption grant.

## Entry Model

```text
Router-ID
Public-Key / Certificate Reference
Router-Role
Country / Region
PCC Profiles
Validity Start
Validity End
Status
Policy Reference
```

Supported roles include `BORDER`, `COUNTRY`, `REGIONAL`, `SOCIAL-SYSTEM`, `SCIENCE`, and `SCIENCE-HUB`.

## Trust

Registries MUST be distributed or resolved through an authenticated trust mechanism. Suitable mechanisms include PKI, signed manifests, authenticated configuration, transparency publication, or another independently trusted control plane.

Router keys require versioning, rotation, expiration, and revocation.

## Social-System Routers

`SOCIAL-SYSTEM` is an administrative designation for a Registered Router operating under a social or institutional network policy. It is deliberately independent of cryptographic strength and does not grant interception authority.

## Science Routers

Science Routers are authorized registration or ancillary-data exchange points for scientific, academic, research, measurement, observatory, or computational networks.

A registry may designate a Science Hub as a higher-capacity aggregation point. The Science Hub can receive separately protected ancillary data while the primary HTTP/3 application payload continues along its ordinary encrypted route.

## Revocation

A revoked router MUST NOT issue new valid receipts after the effective revocation time. Existing receipts remain historical evidence subject to their normal signature and timestamp validation rules.
