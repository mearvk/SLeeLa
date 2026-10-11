# Mid — ExpressionCompiler

**Goal:** build a conventional front end for a small expression language.

Example subset: `let total = 12 + 3 * 4; print total;`. Support integer literals, identifiers, parentheses, unary minus, multiplication/division, addition/subtraction, `let`, and `print`. Do not claim full SLeeLa language coverage.

## Pipeline

1. Lexer emits token kinds, lexemes, and source spans.
2. Parser builds an expression tree using precedence-aware parsing (or Pratt parsing).
3. IR represents constants, local reads/writes, arithmetic, and print operations.
4. Semantic checks reject unknown identifiers and malformed expressions.
5. Emitter serializes normalized IR deterministically; it does not execute the source.

See [ExpressionCompiler.sleela](ExpressionCompiler.sleela), a coordinator scaffold to extend with token and syntax-tree types supported by the current compiler.

## Tests

- `2 + 3 * 4` parses as addition with multiplication on the right.
- `(2 + 3) * 4` parses with addition inside parentheses.
- Unknown variables and missing parentheses produce source-located errors.
- Identical source and configuration produce identical normalized IR.
- Any phase failure blocks emission.
