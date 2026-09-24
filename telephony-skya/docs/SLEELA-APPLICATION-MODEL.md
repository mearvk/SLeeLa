# Skya SLeeLa Application Model

SLeeLa Wrapper program -> Skya application contract -> C ABI/C++ engine -> existing SLeeLa runtime, HTTP multiplexing, sockets/files/threads, security, NAT/server facilities, and media adapters -> JavaFX/BODI presentation.

A .sleela program may exercise SLeeLa-native network primitives directly. It should not duplicate native room state or invent another platform abstraction. When a production native binding is exposed, the Wrapper should call that binding rather than shelling out to the C++ executable.

This separation lets runnable source act as a language-level smoke test while C/C++ remains the performance and operating-system boundary.
