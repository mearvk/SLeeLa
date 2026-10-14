# /lib → SLVM Coverage Contract

/lib is the authoritative SLeeLa class/object vocabulary. SLVM must not maintain a second manually curated class list.

Every .sleela source object under /lib is routed through the authoritative SLeeLa Lexer → Parser → Compiler → Core representation → SLVM path. Class names, fields, methods, literals, collections, streams, GUI objects, and future library types are represented by VM-managed objects or validated native capabilities.

A library source file is not considered SLVM-compatible merely because it exists. It must compile through the real frontend and execute through SLVM, with unsupported instructions or native capabilities reported explicitly.

Use SLEELA_BIN=./impl/build/sleela tools/check-lib-slvm.sh for local/CI conformance. The checker enumerates /lib/**/*.sleela and invokes the authoritative CLI for each source file.

Required coverage includes primitive values, strings/bytes, lists/maps/structures, user classes and instances, method references, streams/IPC handles, network/resolver objects, audio/media objects, GUI objects, JVM broker proxies, and observer/certificate metadata objects.

Copyright (c) Max Rupplin - MEARVK LLC - 2026