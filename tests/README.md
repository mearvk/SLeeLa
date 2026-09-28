# SLeeLa Native and Integration Tests

Tests are organized around completion gates: memory safety, reflection metadata, Nordshrift binding resolution, lowering/artifact compatibility, networking, HTTP, server lifecycle, VoIP, drivers, package verification and cross-platform behavior.

Each applicable suite must include success, malformed-input, resource-limit, timeout, cancellation and shutdown cases. Compiling a test is not evidence that it passed.
