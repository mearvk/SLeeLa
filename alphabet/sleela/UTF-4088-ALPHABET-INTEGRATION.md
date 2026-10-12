# SLeeLa Alphabet Integration for UTF-4088

UTF-4088 is an experimental catalogue/identifier namespace, not a Unicode UTF encoding. The standard SLeeLa capacity signature for the catalogue is C1048576, while the currently generated front-end registry contains 16,606 records. Declared capacity and populated records are separate metrics.

## Adapter API

- is_frontend_identifier(id): true only when the identifier is present in the published front-end registry.
- lookup_frontend_identifier(id): returns a record for an exact registered identifier, otherwise no value.
- sleela_catalogue_capacity: declared catalogue capacity, not a claim that every slot is populated.

The adapter compares the UTF-4088 identifier against the registry's explicit codepoint field. Despite the field's historical name, values above U+10FFFF are experimental identifiers, not Unicode scalar values. Do not serialize these values as ordinary UTF-8.

## Build

The UTF-4088 CMake target includes the adapter. Build and run the test suite using the commands in README.md. A successful UTF-4088 build verifies this adapter in its own library; it does not by itself establish that SLeeLa source syntax or SLVM native bindings are wired to it.


## Input map and ordered register

The spatial traversal contract is documented in [UTF-4088-INPUT-MAP-AND-PRESSURE.md](UTF-4088-INPUT-MAP-AND-PRESSURE.md), and its subject index policy in [UTF-4088-NATIONAL-LIEN-REGISTER.md](UTF-4088-NATIONAL-LIEN-REGISTER.md). Keep permanent UTF-4088 identifiers separate from alphabetic ranks and map coordinates. Pressure is an abstract integer iteration level from 1 through 50; it is not a Unicode value or physical signal.


## Canonical source and relay

This is a synchronized mirror in the alphabet/sleela directory. Its canonical source is [utf-4088/SLeeLa-ALPHABET-INTEGRATION.md](../../utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Use [UTF-4088 relay index](UTF-4088-RELAY.md) to navigate the complete mirrored series. Update the canonical source and its mirror together when changing the protocol.
