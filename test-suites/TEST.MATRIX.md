# SLeeLa Test Matrix

## Contract levels

| Level | Test | Meaning |
|---|---|---|
| C-ABI | C compiler + assertions | C functions and public structs obey documented contracts |
| C++-type | Header compilation + type traits | Public C++ classes are complete and destructible |
| C++-behavior | Runtime unit test | A documented operation produces the required result |
| TU-audit | Syntax-only compilation | A discovered C/C++ implementation unit is syntactically compilable in repository context |
| Integration-smoke | Multiple modules | Adjacent SLeeLa layers exchange defined data |
| Negative/security | Rejection tests | Invalid, unsafe, or malformed input is rejected |

## Foundational class coverage

The C++ class-contract suite covers the actual classes currently defined under impl/fundamental and impl/conversation, plus focused tests under impl/annotation.

## Runtime coverage

The focused runtime tests cover annotation creation/storage, @next forwarding metadata, forwarding destination validation, malformed destinations, HTTP generation range validation, and the Holding Document / Forwarding Annotation / Nexter Colony bridge.

## Translation-unit coverage

run-all.sh --audit discovers repository .c, .cpp, .h, and .hpp files outside test output and attempts a conservative syntax-only check.

A failed audit is evidence for investigation. It is never converted to a pass merely because the source belongs to an optional platform, third-party subtree, or unfinished component.

## Adding a test

1. Add a deterministic C or C++ test.
2. Test a successful case.
3. Test an invalid or boundary case where the contract defines one.
4. Keep external services out of unit tests.
5. Keep network tests local and bounded.
6. Clean up temporary resources.
7. Return non-zero on failure.

## Important distinction

The testbed is a verification system, not a claim that every one of SLeeLa's many source functions has already been semantically verified. The discovery audit makes missing coverage visible; focused tests turn that discovery into behavioral coverage over time.
