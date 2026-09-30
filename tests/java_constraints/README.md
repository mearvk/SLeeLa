<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Java Negative and Constraint Corpus

This corpus records Java source-level programs that are expected to be rejected
or diagnosed under a named semantic constraint. It is a qualification corpus,
not a JVM/runtime test suite.

Each fixture uses an `EXPECT:` marker. The deterministic suite validates the
fixture inventory and then exercises the corresponding SLeeLa semantic
qualification implementation where one exists.

Current diagnostic families include:
- `UseBeforeAssignment`
- `FinalReassignment`
- `UnreachableStatement`
- `UnhandledCheckedException`
- `RedundantCatch`
- `IncompatibleOverrideThrows`
- `Ambiguous`
- `InterfaceDefaultConflict`
- `MissingJavaApiCounterpart`
- `ForbiddenConversion`

The corpus intentionally distinguishes source constraints from JVM execution.