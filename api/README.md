# SLeeLa API

The API tree contains native C/C++ facades and SLeeLa mappings. The documentation is intended to describe source-backed contracts rather than placeholder interfaces.

## General API families

- **Posting** — `api/posting/`: HTTP-style POST representation, validation, and bounded serialization.
- **Listener** — `api/listener/`: listener bind/port/protocol/route vocabulary and validation.
- **Router** — `api/router/`: deterministic method/path route vocabulary and exact matching.
- **Email** — `api/email/`: SMTP-aware submission using plain SMTP, STARTTLS, and implicit TLS.
- **BODI XML** — `api/bodi/`: declarative XML projects and witnessed execution.
- **HTTP 3.0** — protocol-specific packet and logical-port APIs.

## BODI project model

BODI currently represents:

1. Science projects for math, physics, and chemistry.
2. Email/SMTP projects.
3. HTTP-style post projects.
4. Listener declaration projects.
5. Router declaration projects.

See `api/bodi/API.html` for the XML contract and `../examples/` for runnable source examples and expected evidence.

## Network boundary

The XML layer does not turn an XML document into arbitrary network execution. POST, listener, and router declarations are validated through native C APIs and witnessed by the runner. Actual socket lifecycle or external network transmission must be an explicit higher-level operation.

## Build

    cd api/bodi
    make
