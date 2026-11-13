<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# compiler: Tutorial

| | |
|---|---|
| Nature | tool (`bin/sleelactl help compiler`) |
| Registry summary | SLeeLa compiler toolchain component |
| Lives in | `compiler/` (binary `sleelac`, built to `compiler/build/sleelac`) |
| Writeup type | Tutorial, because `sleelac` is a command-line tool and every step below was run successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl build compiler`, `--check`, `--emit-ir`, `--run`, `--emit-bytecode`, `--emit-c`, `--emit-cpp` (generated C and C++ compiled with `-Werror` and run), the bundled test suite (1 of 1 passed), and three error cases. |

## What it is

`sleelac` is a separately buildable C++20 prototype compiler for a very small SLeeLa subset
(`compiler/README.md`). It parses `let`, `print` and `return` statements with integer arithmetic,
lowers them to stack bytecode, and can check, print, run, or write that bytecode. It can also emit a
C or C++ file that embeds the bytecode and a small interpreter. It is **not** the production
frontend: `compiler/README.md` says it does not replace `impl/` and supports no classes, strings or
imports. Step 7 shows that the production `sleela` rejects this subset's files.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
mkdir -p /tmp/sleelac-demo
```

You need CMake and a C++20 compiler. Any scratch directory works; `/tmp/sleelac-demo` is used below.

## Step 1 - Build

```sh
bin/sleelactl build compiler
```

```
... Building CXX object CMakeFiles/sleelac.dir/src/main.cpp.o
[ 66%] Linking CXX executable sleelac
...
  ✓ compiler built
```

This runs `make -C compiler`, which delegates to CMake. The products are `compiler/build/sleelac`,
`compiler/build/sleela_compiler_tests` and `libsleela_compiler_core.a`. `compiler/build/` is
gitignored. Note that `compiler/README.md` shows `build/compiler/` as the build directory; the
Makefile and `sleelactl` use `compiler/build/`.

## Step 2 - Check, show IR, run the bundled example

```sh
S=compiler/build/sleelac; X=compiler/examples/arithmetic.sleela
$S --check $X
$S --emit-ir $X
$S --run $X
```

```
syntax and semantic checks passed
0: PUSH_INT 6
1: STORE_LOCAL 0
...
6: MUL
...
9: PRINT
10: PUSH_INT 0
11: RETURN
12: HALT
42
```

`arithmetic.sleela` declares `width = 6`, `height = 7`, prints their product, and returns 0.

## Step 3 - Write your own program

```sh
cat > /tmp/sleelac-demo/mine.sleela <<'EOF'
let a = 10;
let b = a - 4 * (2 + 1);
print b;
print -a / 3;
return b;
EOF
bin/sleelactl run compiler --run /tmp/sleelac-demo/mine.sleela; echo "exit=$?"
```

```
  $SLEELA_HOME/compiler/build/sleelac --run /tmp/sleelac-demo/mine.sleela
-2
-3
exit=254
```

`sleelactl run compiler` forwards the arguments to `sleelac`. Do not put `--` before them: the run
`sleelactl run compiler -- --run ...` failed with `unknown option: --` (exit 2). `-10 / 3` prints `-3`
(truncating division), and `return b` (-2) became the process exit status 254.

## Step 4 - Emit bytecode

```sh
$S --emit-bytecode /tmp/sleelac-demo/a.slbc $X
```

The file is 131 bytes and starts with the bytes `SLBC` and a version byte. `compiler/ARCHITECTURE.md`
calls this a development artifact with host-endian fields and no stable ABI.

## Step 5 - Emit C and C++ and run them natively

```sh
$S --emit-c   /tmp/sleelac-demo/a.c   $X && cc  -std=c11   -Wall -Wextra -Werror /tmp/sleelac-demo/a.c   -o /tmp/sleelac-demo/a-c   && /tmp/sleelac-demo/a-c
$S --emit-cpp /tmp/sleelac-demo/a.cpp $X && c++ -std=c++20 -Wall -Wextra -Werror /tmp/sleelac-demo/a.cpp -o /tmp/sleelac-demo/a-cpp && /tmp/sleelac-demo/a-cpp
```

```
42
42
```

Both generated files compiled without warnings and print the same result as `--run`.

## Step 6 - See the errors

| Input | Command | Result |
|---|---|---|
| `let a = 1;` then `print c;` | `--check` | `bad.sleela:2:7: error: unknown variable 'c'`, exit 1 |
| `let a = 1 / 0; print a;` | `--run` | `runtime error: division by zero`, exit 1 |
| `let s = "hi";` | `--check` | `str.sleela:1:9: error: unexpected character '"'` (twice), exit 1 |

## Step 7 - Run the test suite and compare with the production frontend

```sh
ctest --test-dir compiler/build
```

```
1/1 Test #1: sleela_compiler_tests ............   Passed    0.06 sec
100% tests passed, 0 tests failed out of 1
```

The same source file given to the production CLI is refused:

```sh
impl/build/sleela check /tmp/sleelac-demo/mine.sleela
```

```
sleelvac: ...mine.sleela: Syntax error (line 1, col 1): expected annotation, import, struct, or Java type declaration at top level
```

So `sleelac` sources are a separate dialect from files that `sleela` accepts.

## What you did

You built `sleelac`, checked, lowered, ran, serialized and natively compiled a small integer program,
and saw the diagnostics. You also saw that the subset is not interchangeable with the production
frontend.

## Next steps

- [`compiler/README.md`](../../../compiler/README.md), [`compiler/ARCHITECTURE.md`](../../../compiler/ARCHITECTURE.md), [`compiler/API.md`](../../../compiler/API.md)
- [`compiler/PRODUCTION-INTEGRATION-GATE.md`](../../../compiler/PRODUCTION-INTEGRATION-GATE.md)
- [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md)

## Sources

Read: `bin/sleelactl.d/registry.conf` (line 69), `compiler/README.md`, `compiler/ARCHITECTURE.md`,
`compiler/examples/arithmetic.sleela`. Ran: `bin/sleelactl help|info|inspect|build|run compiler`,
`compiler/build/sleelac` with each option above, `make -C compiler run-example` (it only runs `--check`),
`ctest --test-dir compiler/build`, `impl/build/sleela check`.
