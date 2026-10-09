# SLeeLa syntax 1.8 regression fixtures

These fixtures exercise the first syntax 1.8 feature: local type inference with
`let`.

- `let-inference-pass.sleela` must parse, pass semantic analysis, compile, and
  print the inferred integer, string, and boolean values.
- `let-inference-invalid-null.sleela` must fail semantic analysis with
  `cannot infer a usable type`; a null initializer must not create an untyped
  local.

Run them from the repository root after building the native compiler:

```sh
make -C impl
impl/build/sleela tests/sleela-syntax-1.8/let-inference-pass.sleela
impl/build/sleela tests/sleela-syntax-1.8/let-inference-invalid-null.sleela
```

The second command is expected to fail. These fixtures do not certify constructor
overload resolution: the native compiler currently rejects constructor arguments
and concise `Type(args)` construction is not yet implemented end-to-end.
