# National Relay Mode

All three SLeeLa Server Editions expose a common National / International Relay
identity surface.

A relay may be configured for authorized national, United Nations, or other
international purposes using opaque identity-provider references for National
ID, Citizen ID, and Bank ID.

Raw government identifiers, citizen numbers, bank account numbers, payment
credentials, and similar sensitive identifiers are not placed in relay source,
HTTP logical-port records, or routine logs.

Relay identity remains separate from native TCP/UDP firewall ports, the exact
10^48 logical PORT namespace, HTTP/2 and HTTP/3 stream identifiers, and file
DOWNLOAD resume metadata.

Relay mode is an identity and routing declaration. It is not itself proof of
governmental, United Nations, citizenship, or banking authority.
