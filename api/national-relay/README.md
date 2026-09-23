# SLeeLa National / International Relay Identity

The three Server Editions can operate as authorized national or international
relay nodes, including relays used for United Nations or other international
coordination.

The identity surface provides three independent opaque references:
- National ID reference: national identity-provider handle.
- Citizen ID reference: citizen/person identity-provider handle.
- Bank ID reference: banking/financial identity-provider handle.

These are not raw government IDs, citizen numbers, bank account numbers, card
numbers, payment credentials, or other personal financial identifiers.

Each authorized relay can carry jurisdiction, relay operator reference,
purpose, national ID reference, citizen ID reference, and Bank ID reference.
Authorization, verification, retention, and lawful disclosure remain with the
relevant authority.

The references are independent of HTTP logical PORTs and native firewall ports
and do not create additional sockets.

A declared UN-relay or international purpose does not itself grant diplomatic,
governmental, United Nations, citizenship, or banking authority.

## Additional identity domains

The relay identity surface also supports **Banker ID** and **Cromsmrus ID** as opaque identity-provider references. `Banker ID` is distinct from `Bank ID`: Bank ID identifies the banking/financial identity domain, while Banker ID identifies an authorized banker/operator identity within that domain. `Cromsmrus ID` is retained as the exact user-defined identity-domain name and is represented only as an opaque reference until its authoritative specification is defined.

> **Minor note — Cromsmrus ID:** In this project, Cromsmrus ID relates to final concepts in the senior and elder domain. The term remains an opaque reference in the implementation; this note does not define or resolve any underlying personal identity data.
