# Compiler Resolution — Nordshrift 2.6-dev

## 2.0 → 2.1-dev → 2.2-dev → 2.3-dev → 2.4-dev → 2.5-dev → 2.6-dev resolution

The next compiler gate is now implemented at the SST check boundary.

### Resolved

1. **SST lexical/parser gate**
   - Existing Nordshrift lexer and indentation-aware parser remain the canonical SST front end.
2. **Subject semantic diagnostics**
   - Declared subject dependencies are checked.
   - Modeled/inferred/derived quantities without supporting relation, assumption or evidence are diagnosed.
3. **Declared source validation**
   - Every source resolved from an SST `source:` block is checked for readability.
   - Sleela syntax-version declarations are validated.
   - The shared Sleela lexer/parser is run before an SST sheet can pass `nordshrift check`.
4. **Negative compiler evidence**
   - A deliberately malformed Sleela source is included in `impl/tests/nordshrift/`.
   - The test requires diagnostic `NSS-E-SRC-003`.
5. **Build integration**
   - `make test` now includes `test-nordshrift-compiler`.
6. **Full type/semantic analysis**
   - A target-neutral semantic pass validates declarations, names, scalar and struct types, assignments, operators, calls, member access, conditions, returns, and Munction fluent methods.
   - `nordshrift check` now rejects semantically invalid declared Sleela source with `NSS-E-SEM-001`.
   - Sleelvac compilation runs the same semantic pass before bytecode lowering, preventing direct compiler and SST paths from diverging.
7. **Shared target-neutral lowering**
   - Sleela AST is lowered into a deterministic Nordshrift IR before Java/Sleela/C emission.
   - All current AST node families are represented by the common lowering pass.
   - The emitter rejects lowering failures instead of allowing target-specific divergence.
8. **End-to-end SST compilation and runtime execution**
   - A positive SST fixture is compiled through source resolution, parsing, semantic analysis, Sleelvac artifact generation, and persistent `.sleela` artifact creation.
   - The generated artifact is loaded by the native runtime and executed as a test, proving the complete SST → Sleela artifact → runtime path.
   - The test requires the expected runtime output `NORDSHRIFT-E2E-OK` and fails on missing artifacts or non-zero execution.
### Resolved 2.5-dev gate

9. **Runtime artifact ABI validation**
   - `.sleela` artifacts are validated against the runtime VM ABI before execution.
   - Validation rejects unknown opcodes, invalid jump/call targets, invalid constant/global/function/struct indexes, invalid synchronization selectors, malformed function frames, and out-of-range struct metadata.
   - `sleela validate-artifact <file.sleela>` exposes the validation gate directly.
   - The end-to-end SST test now validates the generated artifact before executing it.

### Resolved 2.6-dev gate

10. **Cross-version compiler compatibility fixtures**
   - Added below-floor syntax fixture `#sleela 1.2` and above-ceiling syntax fixture `#sleela 1.4`.
   - The supported compiler range remains `1.3 .. 1.3`.
   - Nordshrift `check` must reject both out-of-range declarations with the source version diagnostic path.
   - `make test-nordshrift-compat` records the compatibility evidence without weakening the supported syntax range.

### Versioned next compiler gates

- **Nordshrift 2.5-dev — Runtime artifact ABI validation**
- **Nordshrift 2.6-dev — Cross-version compiler fixtures and compatibility evidence**
- **Nordshrift 2.7-dev — Compiler fuzzing and malformed-input corpus**
- **Nordshrift 2.8-dev — Deterministic compiler output verification**

### Remaining compiler gates

- Runtime artifact ABI validation (2.5-dev).
- Cross-version compiler fixtures (2.6-dev).
- Compiler fuzzing and malformed-input corpus (2.7-dev).
- Deterministic compiler output verification (2.8-dev).

## Evidence rule

A sheet passing `nordshrift check` now means its declared source files have passed lexical/parser validation and the implemented compile-time type/semantic analysis as well as the SST-level checks. It still does **not** constitute proof of complete native linking, runtime ABI compatibility, or production compiler completeness.

**SLeeLa — MEARVK LLC — 2026**
