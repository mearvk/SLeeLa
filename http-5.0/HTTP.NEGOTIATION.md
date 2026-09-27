# SLeeLa HTTP 1–9 Internet Negotiation

This module is part of the SLeeLa HTTP family. Versions 4.0 through 9.0 are SLeeLa experimental/application protocol generations and are not presented as replacements for IETF HTTP standards.

A SLeeLa peer may begin with ordinary HTTP/1.0 or HTTP/1.1 when the remote peer does not understand the requested SLeeLa generation. Negotiation is explicit: identify the requested generation, exchange necessary capabilities, upgrade only after acceptance, and fall back to HTTP/1.1 or HTTP/1.0 when permitted. Unsupported extensions are never presented as Internet standards.

Field names, status values, units, and configuration use plain English so operators and users in the United States and elsewhere can understand the behavior. The layer grants no special governmental, law-enforcement, political, or other authority.

Deployments may advertise application capabilities with ordinary metadata such as `X-SLeeLa-Version: 9.0` and `X-SLeeLa-Accept: 1.1, 1.0`. These are application headers, not new HTTP standards; normal HTTP/1.x peers may ignore them.

Fallback must not silently weaken authentication, authorization, integrity, or privacy. If stronger security is required, fail closed instead of downgrading solely for connectivity.

HTTP/1.0 and HTTP/1.1 are compatibility targets. SLeeLa 2.0–9.0 are project-specific application/protocol generations. Internet-wide deployment requires independent peer support and standards/interoperability review.
