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

The last example uses a documentation-only address. A remote bind is not safe by itself: the inspected server path does not provide built-in TLS or authenticated peer identity. Do not expose it directly to untrusted networks. Use a protected deployment boundary and service-level authorization; these controls remain release requirements.

## Processing boundary

The current implementation parses and inspects packets and returns an admission status. Do not assume that an accepted packet has been executed by an application service handler unless that handler integration is present and tested.

Payloads are bytes. The server must not execute programs, shell commands, scripts, XML procedures, or metadata merely because they arrive in a packet.

POLICE, SECURITY, SAFETY, CLASSIFICATION, DARK-BAND, frequency, grade, and related fields are application metadata. They do not establish identity, clearance, authority, network control, or access rights.

## Security and standards

Read SECURITY-STANDARDS.md for the hardening baseline and release gates, and 1-2-3-4.md for the prioritized implementation plan.

Max Rupplin — MEARVK LLC — 2026
