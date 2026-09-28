# SLeeLa HTTP 1–9 Internet Negotiation

This module is part of the SLeeLa HTTP family. Versions 4.0 through 9.0 are SLeeLa experimental/application protocol generations and are not presented as replacements for IETF HTTP standards.

## Compatibility goal

A SLeeLa peer may begin with ordinary HTTP/1.0 or HTTP/1.1 when the remote peer does not understand the requested SLeeLa generation.

1. Start with a mutually understood wire format.
2. Identify the requested SLeeLa generation.
3. Exchange only necessary capability information.
4. Upgrade only after both peers explicitly accept the generation.
5. If the requested generation is unavailable, use HTTP/1.1 or HTTP/1.0 when fallback is allowed.
6. Never reinterpret an unsupported extension as an Internet standard feature.

## Plain-language interoperability

Field names, status values, units, and configuration are documented in ordinary English so operators and users in the United States and elsewhere can understand the behavior. The compatibility layer grants no special governmental, law-enforcement, political, or other authority.

## Wire fallback

Deployments may advertise application capabilities with ordinary HTTP metadata such as:

`X-SLeeLa-Version: 9.0`
`X-SLeeLa-Accept: 1.1, 1.0`

These are application headers, not new HTTP standards. A normal HTTP/1.x peer may ignore them and continue ordinary request/response processing.

## Security

Fallback must not silently weaken authentication, authorization, integrity, or privacy. If a deployment requires a stronger security property, it must fail closed rather than downgrade solely to obtain connectivity.

## Status

- HTTP/1.0: compatibility target.
- HTTP/1.1: compatibility target.
- SLeeLa 2.0–9.0: project-specific application/protocol generations.
- Internet-wide deployment: requires independent peer support and standards/interoperability review.
