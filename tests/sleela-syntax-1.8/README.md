<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa syntax 1.8 regression fixtures

These fixtures exercise the two syntax 1.8 features: local type inference with
`let`, and constructor arguments on `new Type(args)`.

Inferred locals (`let`):

- `let-inference-pass.sleela` must parse, pass semantic analysis, compile, and
  print the inferred integer, string, and boolean values.
- `let-inference-invalid-null.sleela` must fail semantic analysis with
  `cannot infer a usable type`; a null initializer must not create an untyped
  local.

Constructor arguments:

- `constructor-args-pass.sleela` must build and run, invoking a declared
  constructor through `new Point(3, 4)` (and through a `let`-inferred local) and
  printing the field values the constructor assigned.
- `constructor-arity-invalid.sleela` must fail semantic analysis: the single
  declared constructor takes one argument but is called with two.
- `constructor-overload-unsupported.sleela` must fail: a class declaring more
  than one constructor is rejected because syntax 1.8 does not support
  constructor overload resolution.

Run them from the repository root after building the native compiler:

```sh
make -C impl
impl/build/sleela run tests/sleela-syntax-1.8/let-inference-pass.sleela
impl/build/sleela run tests/sleela-syntax-1.8/let-inference-invalid-null.sleela
impl/build/sleela run tests/sleela-syntax-1.8/constructor-args-pass.sleela
impl/build/sleela run tests/sleela-syntax-1.8/constructor-arity-invalid.sleela
impl/build/sleela run tests/sleela-syntax-1.8/constructor-overload-unsupported.sleela
```

The `-invalid`/`-unsupported` commands are expected to fail with an explicit
semantic error. The 1.8 constructor feature is limited to a single declared
constructor whose arity and argument types match; overload resolution and
`this(...)`/base delegation are intentionally rejected rather than silently
narrowed.