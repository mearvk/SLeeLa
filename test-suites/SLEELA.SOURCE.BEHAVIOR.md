# SLeeLa Source Behavioral Test Suite

## Purpose

This suite treats every `.sleela` file in the repository as a test subject. It does not assume that every Wrapper™ is an executable program: source files are classified as either:

- **program** — contains a `main()` entry point and is checked and executed;
- **library** — has no `main()` and is checked as a compilable language unit;
- **expected-rejection** — a deliberately invalid regression/version fixture that must be rejected by `sleela check`.

For executable Wrappers™, the suite exercises three behavioral dimensions:

1. **Input** — the Wrapper is supplied a deterministic stdin fixture when its source advertises input/read/recv/scan behavior; otherwise it is executed with EOF.
2. **Output** — execution must terminate successfully and produce no `FAIL ` assertion line.
3. **Procedural operations** — the real compiler + VM path is used, with an execution timeout, so control flow, calls, returns, and supported procedural built-ins are exercised by the program itself.

The test is deliberately different from a syntax-only inventory: it drives the SLeeLa executable against the repository's actual source corpus.

## Coverage target

The repository currently contains approximately 1,000 SLeeLa source files. The suite discovers the corpus dynamically with `find`, so newly added `.sleela` files are automatically included.

The generated TSV manifest records one row per source:

`path, class, check, run, input, output, procedural, result`

This provides a file-level accounting trail rather than only an aggregate test count.

## Running

Build the implementation and run the source suite:

```sh
make -C impl build/sleela
bash test-suites/sleela-source-behavior.sh
```

The test log and manifest are written under:

`test-suites/logs/sleela-source/`

Set `SLEELA_SOURCE_TIMEOUT` to change the per-program execution timeout. Set `SLEELA_BIN` to test a different SLeeLa executable.

## Meaning of a pass

A pass means the source file met the contract that can be established automatically from the repository:

- valid sources are accepted by the current SLeeLa checker;
- executable sources start through the real compiler/VM path and terminate successfully;
- deterministic input execution does not fail;
- output does not contain an explicit `FAIL ` assertion;
- known negative fixtures are rejected.

It does **not** claim that arbitrary human-level semantics are proven for every library object. Those require object-specific assertions. This suite establishes the corpus-wide executable/checkable baseline and produces the manifest needed to drive deeper per-object behavioral tests.

## Extension point

Object-specific expected inputs, outputs, and procedures can be added later without changing the corpus scanner. The intended next layer is a sidecar contract manifest mapping a Wrapper™ to exact input fixtures, expected output, and expected state transitions.
