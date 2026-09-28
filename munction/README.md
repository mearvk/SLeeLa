# Munction™ — C11 and C++17

This directory is the language-neutral native Munction implementation.

The Java implementation remains a first-class implementation. The C11 layer
defines the stable sentence and channel ABI; the C++17 layer provides a fluent
RAII interface over that ABI.

The native core enforces the normative 4..16 call bound, connect-before-movement,
coherent send accounting, truthful receive accounting, optional interim stages,
one latch, one closer, and boundary containment.

Channels are supplied through callbacks. This prevents the Munction state
machine from duplicating SLeeLa's operating-system transport implementations.
The existing SLeeLa VM can therefore adapt its pipe, file, TCP, SDPS, and crypto
channels to this common contract.

C API entry point: sleela_munction_start().
C++ entry point: sleela::munction::Munction::start().
