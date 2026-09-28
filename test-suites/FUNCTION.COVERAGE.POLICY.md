# Function Verification Matrix

The function inventory is generated rather than manually maintained.

| Status | Meaning | Required next action |
|---|---|---|
| tested | Direct behavioral test maps to the function | Maintain regression coverage |
| integration-tested | Function is exercised through an integration path | Add direct edge-case tests where valuable |
| compile-only | Syntax/build verification exists without behavioral mapping | Candidate for behavioral test |
| untested | No verified test/compile evidence | Highest verification gap |

## Verification ladder

**Discovery → Compile → Behavioral → Integration → Regression**

The inventory is intentionally conservative. Name matching is evidence for triage, not proof of semantic coverage. Before a function is promoted to `tested`, its test should assert observable behavior, including success and relevant failure/edge cases.

## Required coverage targets

For important runtime and language functions, the desired end state is:

1. normal behavior;
2. boundary/empty input behavior;
3. invalid input behavior;
4. resource/error behavior where applicable;
5. concurrency behavior where applicable;
6. integration behavior where the function crosses subsystem boundaries.

`FUNCTION.COVERAGE.md` is a living verification backlog, not a numerical quality score.
