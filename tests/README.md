<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">




# SLeeLa Native and Integration Tests

Tests are organized around completion gates: memory safety, reflection metadata, Nordshrift binding resolution, lowering/artifact compatibility, networking, HTTP, server lifecycle, VoIP, drivers, package verification and cross-platform behavior.

## JDK 28 Java Conformance

`jdk28_functional_equivalence.py` audits every `lib/java/**/*.sleela` declaration against the official JDK 28 API index.

It has two gates:

1. **API identity / envelope contract** — verifies that each declaration identifies a documented JDK 28 type and exposes the required SLeeLa Java bridge operations.
2. **Functional equivalence** — requires the SLeeLa declaration to represent the Java cousin's actual API surface. A generated source envelope alone does not satisfy this gate.

This distinction is intentional: a type-name envelope is not evidence that the Java class is functionally implemented. CI therefore reports the current gap instead of treating `invoke()` as a substitute for the Java API.

Run locally:

```bash
python3 tests/jdk28_functional_equivalence.py
python3 tests/jdk28_functional_equivalence.py --functional
```