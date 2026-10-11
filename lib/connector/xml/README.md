# XML document connector

Validates well-formed XML with libxml2 and `XML_PARSE_NONET`, preventing network fetches during parsing. This is validation only, not XPath, schema validation, or a mutable DOM. Before using on untrusted XML, deploy a hardened libxml2 configuration and review DTD/entity policy. Link with `pkg-config --cflags --libs libxml-2.0`.

Build from the repository root with `make -C lib/connector/xml build`, or from this directory with `make build`. The group smoke test is `make -C lib/connector test`.
