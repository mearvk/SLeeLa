# SLVM/1 Source-to-VM Contract

SLVM/1 participates in the common pipeline defined by `/VM.SOURCE.PIPELINE.md`.

1. Accept a validated SLeeLa VM artifact.
2. Verify source identity, artifact ABI and dependencies.
3. Verify the canonical source-defined ISA registry.
4. Apply SLVM/1 control and security requirements.
5. Delegate actual bytecode execution to the authoritative execution substrate where applicable.
6. Preserve source, artifact, capability and diagnostic identity.
7. Reject missing prerequisites instead of changing language semantics.

Every `.sleela` under `/lib` is part of the recursive source inventory. New library source therefore cannot silently become an untracked VM feature.

SLVM/1 is an ordered architectural layer, not a separate SLeeLa language.
