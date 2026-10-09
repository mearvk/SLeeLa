# SLeeLa syntax 1.8 regression fixtures

- `let-inference-pass.sleela` should parse, pass semantic analysis, compile, and print the inferred integer, string, and boolean values.
- `let-inference-invalid-null.sleela` should fail semantic analysis with `cannot infer a usable type`.

Run from the repository root after building the native compiler:

```sh
make -C impl
impl/build/sleela tests/sleela-syntax-1.8/let-inference-pass.sleela
impl/build/sleela tests/sleela-syntax-1.8/let-inference-invalid-null.sleela
```

The second command is expected to fail. These fixtures do not certify constructor overload resolution; constructor arguments and concise `Type(args)` are not implemented end-to-end.
