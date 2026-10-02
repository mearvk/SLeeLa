# SLeeLa VM Source Classes

Source-level objects for constructing SLVM and SLJVM pieces. The authoritative SLeeLa compiler analyzes architecture, physical limits, capabilities, and module requirements before emitting C/C++ construction units.

Targets: SLVM native C ABI with optional C++ orchestration; SLJVM through the JVM/object-broker boundary.

Construction: SLeeLa Source -> VM Source Objects -> compiler analysis -> resource/architecture plan -> C/C++ pieces -> link/package -> verified VM.
