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
