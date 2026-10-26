<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Server Edition

SLeeLa Server Edition is a bounded transport and packet-admission component for the SLeeLa application packet family.

## Supported traffic

The server accepts SLeeLa grades 1 through 9 through the Server Edition wire envelope. Generation-specific fields remain application metadata and follow PACKET.md. This envelope is not itself a claim of native HTTP compatibility; see HTTP-GENERATIONS.md for the current protocol boundary.

## Build

Linux and macOS:

    make -C server-edition

Windows builds require the full source list and Winsock2 linkage to be verified; see the Makefile and platform notes before treating Windows as release-supported.

## Run safely

The default listener is local-only:

    ./server-edition/sleelas

Equivalent explicit local binding:

    ./server-edition/sleelas --bind 127.0.0.1 --port 19866

To request a non-loopback bind, pass --allow-remote explicitly:

    ./server-edition/sleelas --bind 192.0.2.10 --port 19866 --allow-remote

Remote binding is refused unless TLS certificate, private key, client CA, an allowlisted client certificate Common Name, and at least one explicit authorization rule are configured. The remote path uses TLS 1.3 only and requires client certificates signed by the configured CA.

Example (replace paths and identities with your deployment values):

    ./server-edition/sleelas --bind 0.0.0.0 --allow-remote \
      --tls-cert certs/server-chain.pem --tls-key certs/server-key.pem \
      --tls-client-ca certs/client-ca.pem \
      --allow-peer billing-client \
      --authorize billing-client:billing:submit

The server authorizes exact `identity:service:operation` triples; no permissions are granted implicitly. The identity is taken from the verified client certificate's Common Name (CN), not from packet metadata. Do not reuse a CN for different principals. Use a server certificate with the correct Subject Alternative Name (SAN), protect private-key files, and provision client certificates only from a trusted CA. This is an initial mutual-TLS control, not a claim of complete production security; review certificate rotation, revocation, rate limiting, replay protection, and deployment policy before production.

## Diffie–Hellman and forward secrecy

TLS is restricted to TLS 1.3 and explicitly configures the ephemeral key-exchange groups `X25519`, `P-256`, and `P-384` (subject to support in the installed OpenSSL build). TLS 1.3 uses ephemeral (EC)DHE key exchange with its supported cipher suites; the server does not enable static RSA key exchange. Actual negotiation must be verified with a live TLS handshake on each supported platform.

## Router and NAT awareness

The optional, read-only STUN probe reports the local source address and the externally mapped address observed by a STUN server. It does not modify router configuration, firewall rules, or port forwarding:

    python3 server-edition/tools/nat-awareness.py
    python3 server-edition/tools/nat-awareness.py --server stun.example.net --port 3478 --timeout 3

Use a STUN service you trust and allow outbound UDP to that service. A different mapped address suggests address translation or an upstream mapping; it does **not** reliably identify the exact NAT type or prove that unsolicited inbound connections can reach the server. Carrier-grade NAT, double NAT, VPNs, and firewall policy can affect the result. Never automatically create port-forwarding rules based on this probe. Keep the server loopback-only unless remote access is intentionally configured and secured.

Run the protocol and NAT parser tests with:

    make -C server-edition test

The GitHub Actions workflow `.github/workflows/server-edition-security.yml` builds and tests on Linux, macOS, and Windows. The workflow is a release gate; check its actual run result before treating any platform as verified.

## Processing boundary

After admission checks, the server dispatches only to explicitly registered `(SERVICE-ID, OP-ID)` handlers. Missing handlers return `501 service_not_implemented`; handler exceptions return `500 handler_error`. End-to-end dispatch and lifecycle tests remain required.

Payloads are bytes. The server must not execute programs, shell commands, scripts, XML procedures, or metadata merely because they arrive in a packet.

POLICE, SECURITY, SAFETY, CLASSIFICATION, DARK-BAND, frequency, grade, and related fields are application metadata. They do not establish identity, clearance, authority, network control, or access rights.

## Security and standards

Read SECURITY-STANDARDS.md for the hardening baseline and release gates, and 1-2-3-4.md for the prioritized implementation plan.

Max Rupplin — MEARVK LLC — 2026

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
