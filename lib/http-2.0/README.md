# SLeeLa HTTP 2.0 / 2.1 library contract

This folder contains SLeeLa-level declarations corresponding to the native
protocol implementation in ../../http-2.0/.

- SLPackage.sleela describes the protocol generation and native transport boundary.
- Http21SecurityPolicy.sleela captures deployment requirements; it does not
  implement TLS, authentication, authorization, or integrity.
- Http21Limits.sleela mirrors the intended parser and envelope limits.

The native server currently uses HTTP/1.x. HTTP/2 stream multiplexing is a
design target, not a capability of this server. See ../../http-2.0/HTTP21.SECURITY.md.
