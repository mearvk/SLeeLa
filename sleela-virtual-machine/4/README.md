<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 4

SLVM/4 advances SLVM/3 from verified isolated execution toward distributed execution, stronger identity, secure object transport, and controlled cross-runtime interoperability.

SLVM/4 uses /3 as its baseline. The authoritative SLeeLa compiler and Output Symbol contract remain unchanged.

## Primary Improvements

- authenticated distributed execution;
- stronger JVM/object broker integration;
- remote object identity and lease management;
- secure serialization boundaries;
- replay/freshness protection hooks;
- remote capability delegation with explicit scope and expiry;
- runtime-to-runtime compatibility negotiation;
- richer certificate and attestation exchange;
- distributed observability correlation;
- network-aware resolver integration without bypassing capabilities.

SLVM/4 does not make remote execution implicit. A remote operation must be explicitly represented, authenticated, authorized, observable, and bounded.

Copyright (c) Max Rupplin - MEARVK LLC - 2026