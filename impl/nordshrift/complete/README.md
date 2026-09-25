# Nordshrift Complete

Nordshrift Complete is the application/service authoring edition of Nordshrift. It consumes the canonical Sleela-Complete class library rather than duplicating it.

Canonical designer classes remain under api/sleela-complete/. Nordshrift supplies discovery, class-surface cataloging, import/reference resolution, capability selection, platform/backend selection, build/test/package policy, diagnostics and completeness gates.

Native implementation remains under api/native/ and project implementation directories.

Module families include core/runtime, API/types, platform, network/security, data/database, HTTP/web, server/service, I/O, synchronization, UI/application, XML, email, media, telephony/VoIP, memory, reflection, AI/inference, analytics, regex/text, subjects/science, terminal, decompiler/tooling and native implementation.

Build relationship: .sst -> semantic validation -> Sleela source/core artifact -> target/native build -> tests -> package.
