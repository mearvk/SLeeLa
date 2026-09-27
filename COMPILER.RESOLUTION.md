# Compiler Resolution — Nordshrift 2.1-dev

## 2.0 → 2.1-dev resolution

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

### Remaining compiler gates

- Full type/semantic analysis of Sleela source.
- Shared lowering/IR rather than direct AST-to-target emission.
- Native linker and runtime ABI validation.
- Cross-version compiler fixtures.
- Complete SST → Sleela artifact → runtime execution proof.
- Compiler fuzzing and malformed-input corpus.
- Deterministic compiler output verification.

## Evidence rule

A sheet passing `nordshrift check` now means its declared source files have passed lexical/parser validation as well as the SST-level checks. It does **not** yet constitute proof of full type correctness or production compiler completeness.

**SLeeLa — MEARVK LLC — 2026**
