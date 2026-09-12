# PCC Science Router Map

## Purpose

The Science Router Map adds a research-oriented ancillary-data plane to the PCC architecture. A Science Router or Science Hub may receive separately protected metadata or ancillary measurements associated with an HTTP 3.0 flow while the primary application payload continues over its normal encrypted route.

This creates a useful separation between **primary communication** and **scientific ancillary exchange**.

## Map

```text
                         PCC SCIENCE ROUTER MAP

             +---------------- Developmental Region ----------------+
             |                                                       |
             |   Science Router A ---- Science Router B              |
             |          |                    |                       |
             |          +---------+----------+                       |
             |                    |                                  |
             |               Science Hub                            |
             |                    |                                  |
             +--------------------+----------------------------------+
                                  |
                         protected ancillary data
                                  |
                           Research / Archive
```

## Science Router

A Science Router is an authorized PCC registration or ancillary-data endpoint associated with a scientific, academic, research, observatory, measurement, or computational network.

It may:

- issue PCC registration receipts;
- receive a separately encrypted ancillary-data package;
- relay ancillary data to a Science Hub;
- associate measurements with a PCC circuit commitment;
- provide authenticated acknowledgement.

It must not receive application plaintext merely because it is a Science Router.

## Science Hub

A Science Hub is an aggregation point for authorized ancillary data. It may provide:

- research aggregation;
- measurement collection;
- scientific archives;
- computational processing;
- cross-router correlation under an explicit policy;
- authenticated acknowledgements.

Ancillary data should be encrypted for the intended scientific recipient or hub. The primary HTTP/3 payload remains independently protected.

## Ancillary Data Package

A conceptual package is:

```text
SCIENCE-ANCILLARY
  PCC-ID
  Circuit-ID
  Ancillary-Type
  Observation-Epoch
  Data-Commitment
  Protected-Data
  Recipient-Key-ID
  Expiration
  Sender/Source Authorization
```

The package should contain the minimum information needed for the scientific purpose.

## Interesting Architectural Possibilities

The Science Router Map permits future research-oriented services such as:

- network telemetry without application-content inspection;
- distributed scientific measurements;
- observatory and sensor aggregation;
- independently encrypted research annotations;
- reproducible packet/measurement provenance;
- regional research exchanges;
- authenticated data-drop points;
- time-bounded scientific experiments;
- independently verifiable registration and measurement receipts.

These are ancillary services, not a mechanism for universal surveillance.

## Security Boundary

Science Routers and Science Hubs do not inherit decryption authority from their registry role. They receive only the protected ancillary packages for which they are authorized recipients.

The existing SLeeLa jurisdictional capsule-set principle applies: separate recipients receive independently protected packages rather than a universal jurisdictional key.
