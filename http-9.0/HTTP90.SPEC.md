# SLeeLa HTTP 9.0 Specification

**Status:** Experimental project specification. This is not an IETF HTTP/9 standard and does not replace HTTP/1.1 or HTTP/2/3.

## 1. Scope

HTTP 9.0 defines an application metadata model extending the SLeeLa HTTP 8.0 generation. It carries prior metadata and adds structured international security, safety, police-category, national signal/frequency, and Dark Band records. These are descriptive fields, not commands or grants of authority.

## 2. Metadata model

The C API (`http90.h`) is the authoritative field layout for this implementation:

- protocol grade and sequence;
- prior packet metadata;
- police and international identifiers;
- optional monitoring/frequency metadata;
- national emblem, signal, frequency unit/range, jurisdiction, source, and timestamp;
- international security, safety, police-category, jurisdiction, organization, identifier, classification, source, and timestamp;
- Dark Band name and original-content hexadecimal representation.

Applications should preserve source, jurisdiction, and timestamp when records are forwarded. Classification labels must be interpreted by the consuming application's authorization policy; a label does not itself grant access.

## 3. Defaults and validation

Initialization sets identifiers to explicit unassigned placeholders and monitoring values to `DISABLED`. Monitoring is disabled by default. If a monitoring field is enabled while its value is `DISABLED`, validation fails with `HTTP90_NOT_CONFIGURED`.

The Dark Band default representation is `0x4441524B42414E44`. It is a public identifier/content representation, not a credential, cryptographic key, authorization token, or hidden channel.

## 4. Compatibility and negotiation

A peer must explicitly accept the requested SLeeLa generation. Fallback to HTTP/1.1 or HTTP/1.0 is permitted only when configured by the application and must not weaken required authentication, authorization, integrity, or privacy. SLeeLa version headers are application metadata, not standard HTTP protocol features.

## 5. Frequency and public-safety data

This specification intentionally embeds no real-world police frequencies and provides no instructions for interception, interference, jamming, or bypassing communications controls. Frequency fields are configurable metadata for authorized deployments. Logical ports are SLeeLa application identifiers and are not native TCP/UDP ports.

## 6. Error handling and limits

Callers must check every API return value. Fixed-size C fields are bounded storage; consumers must validate inputs before treating them as canonical identifiers. A successful metadata validation does not establish sender authenticity, legal authority, or transport security.

## 7. Verification

See `TESTING.md` for compile and runtime conformance targets. Syntax-only compilation is a build check, not proof of complete protocol conformance.
