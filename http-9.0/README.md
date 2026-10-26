<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 9.0

**Status:** Experimental SLeeLa application-protocol generation; not an IETF HTTP/9 standard.

HTTP 9.0 extends the HTTP 8.0 SLeeLa metadata model with structured international security, safety, police-category, national signal/frequency, and Dark Band records. It preserves the distinction between transport behavior and application metadata: fields do not establish identity, authenticity, jurisdiction, authority, or permission to act.

## Contents

- `http90.c`, `http90.h`: C metadata API and validation.
- `http90.cpp`, `http90.hpp`: C++ wrapper.
- `http_negotiation.c/.h`, `http_negotiation.cpp/.hpp`: negotiation adapters.
- `HTTP90.SPEC.md`: application-level specification.
- `HTTP90.conf`: safe placeholder configuration.
- `HTTP.NEGOTIATION.md`: shared SLeeLa negotiation contract.
- `Makefile`, `TESTING.md`: syntax checks and conformance guidance.

## Metadata

The model carries protocol grade, sequence, prior packet metadata, identifiers, optional monitoring metadata, national signal/frequency provenance, international security/safety/police-category records, and Dark Band metadata. Applications should retain source, jurisdiction, and timestamp where relevant.

The default Dark Band original-content representation is `0x4441524B42414E44`. This is a descriptive identifier, not a secret or control mechanism.

## Configuration and safety

Monitoring fields are disabled by default and use `DISABLED` placeholders. Do not place operational frequencies, credentials, private keys, or secrets in source control. Configure only data that the deployment is authorized to use. Labels and identifiers are descriptive and do not grant law-enforcement or other authority.

## Negotiation and downgrade policy

Peers must explicitly accept a requested generation. HTTP/1.1 and HTTP/1.0 fallback is application-controlled and must not silently weaken required security. SLeeLa negotiation headers are application metadata, not new HTTP standards.

## Build

```sh
make syntax
make test
```

See [HTTP90.SPEC.md](HTTP90.SPEC.md), [HTTP.NEGOTIATION.md](HTTP.NEGOTIATION.md), [STATUS.md](STATUS.md), and [TESTING.md](TESTING.md).

## Shared route-data contract

This implementation follows the SLeeLa route-data contract in `route/ROUTE.DATA.json` and `route/ROUTE.DATA.md`. Dynamic targets must be validated by the shared resolver before acceptance. VM names are architectural metadata only and do not grant capabilities.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
