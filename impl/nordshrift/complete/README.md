# Nordshrift Complete

Nordshrift Complete is the application/service authoring edition of Nordshrift. It consumes the canonical Sleela-Complete class library rather than duplicating it.

## Binding rule

Canonical designer classes remain under api/sleela-complete/.

Nordshrift provides module discovery, class-surface cataloging, import/reference resolution, capability selection, platform/backend selection, build/test/package policy, diagnostics and completeness gates.

Native implementation remains under api/native/ and the relevant project implementation directories.

## Module families

Core/runtime; API/type contracts; platform; network/security; data/database; HTTP/web; server/service; I/O; synchronization; UI/application; XML; email; media; telephony/VoIP; memory; reflection; AI/inference; analytics; regex/text; subjects/science; terminal; decompiler/tooling; native implementation.

## Source-of-truth rule

The registry records the canonical path of each Sleela-Complete class. It does not generate a competing copy of those classes.

## Build relationship

.sst -> semantic validation -> Sleela source/core artifact -> target/native build -> tests -> package
