# Compiler Resolution — Nordshrift 2.3-dev

## 2.0 → 2.1-dev → 2.2-dev → 2.3-dev resolution

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

### Remaining compiler gates

- Complete end-to-end SST compilation and executable proof.
- Native linker and runtime ABI validation.
- Cross-version compiler fixtures.
- Complete SST → Sleela artifact → runtime execution proof.
- Compiler fuzzing and malformed-input corpus.
- Deterministic compiler output verification.

## Evidence rule

A sheet passing `nordshrift check` now means its declared source files have passed lexical/parser validation and the implemented compile-time type/semantic analysis as well as the SST-level checks. It still does **not** constitute proof of complete native linking, runtime ABI compatibility, or production compiler completeness.

**SLeeLa — MEARVK LLC — 2026**
