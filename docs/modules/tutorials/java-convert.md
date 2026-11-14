<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# java-convert: Tutorial

| | |
|---|---|
| Nature | tool (`bin/sleelactl help java-convert`) |
| Registry summary | Java to SLeeLa converter (C lexer, C++ translator): converts a presumed JDK source tree to functionally equivalent SLeeLa under the conversion subject model; fails closed |
| Lives in | `sleelactl/convert/` (binary `javaconv`); status also in `JAVA.CONVERSION.STATUS.md` |
| Writeup type | Tutorial, because `javaconv` is run by a person and a small Java program converted, ran on `impl/build/sleela`, and matched `java` byte for byte. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl build java-convert`, `--self-check`, converted `Tiny.java` and diffed against `java` (identical), a refused file (exit 1, no output), and `tests/javaconv_differential_test.sh` (pass=32 fail=0). |

## What it is

`javaconv` reads Java source and writes SLeeLa 3.1 source that is meant to behave the same
(`sleelactl/convert/README.md`). A C lexer (`java_lex.c`), a model loader (`conv_model.c`) and a C++
translator (`java_to_sleela.cpp`) do the work; operator rules come from
`impl/subjects/conversion/conversion.model`, and converted arithmetic calls
`lib/java/JavaOperands.sleela`. It fails closed: a file with any construct it cannot convert exactly writes
no output and is reported with `file:line:col [construct] reason`. It converts single files or whole
trees; it does not convert the whole JDK (`JAVA.CONVERSION.STATUS.md`: 420 of 884 files of one `java.base` subset).

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
J=/tmp/jc-demo; mkdir -p $J/src $J/out $J/jdk
```

Needs `cc`, `c++`, a JDK (`javac`, `java`; JDK 25 here) and `impl/build/sleela`. Output below was
captured with a different `$J`; only paths differ.

## Step 1 - Build and self-check

```sh
bin/sleelactl build java-convert
bin/sleelactl run java-convert --self-check
```

```
  ✓ java-convert built
model java-to-sleela 1.3.0 (.../impl/subjects/conversion/conversion.model)
  types=10 binop=56 unop=9 cast=18 compound=30 ret=8964 unsupported=16
```

The build writes `sleelactl/convert/build/` (gitignored); here it had been built before and the build
recompiled three of the objects.

## Step 2 - Write and run a Java program on the JDK

```sh
cat > $J/src/Tiny.java <<'EOF'
public class Tiny {
    static int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }
    public static void main(String[] args) {
        int sum = 0;
        for (int i = 0; i < 10; i++) { sum += fib(i); }
        System.out.println("sum=" + sum);
        long big = Integer.MAX_VALUE; big += 1;
        int wrap = Integer.MAX_VALUE; wrap += 1;
        System.out.println(big + " " + wrap + " " + (7 / 2) + " " + (-7 % 3) + " " + (7 / 2.0));
        String s = "SLeeLa";
        System.out.println(s.length() + ":" + s.toUpperCase());
    }
}
EOF
javac -d $J/jdk $J/src/Tiny.java && java -cp $J/jdk Tiny | tee $J/java.out
```

```
sum=88
2147483648 -2147483648 3 -1 3.5
6:SLEELA
```

## Step 3 - Convert it

```sh
bin/sleelactl run java-convert --file $J/src/Tiny.java --out $J/out
```

```
javaconv: model java-to-sleela 1.3.0, JDK 28
  source files 1, converted 1 (1 SLeeLa files), failed closed 0
  output .../out (CONVERSION-REPORT.md / .json)
```

`$J/out/` now holds `Tiny.sleela`, `CONVERSION-REPORT.md/.json` and `.javaconv-emitted` (the list of files
a re-run may delete). The core of `Tiny.sleela`, after a long block of metadata fields with values such as
`"unknown"`:

```
  static int fib(int n) {
    return ((n < 2) ? n : JavaOperands.addInt(fib(JavaOperands.subInt(n, 1)), fib(JavaOperands.subInt(n, 2))));
  }
  ...
    wrap = JavaOperands.addInt(wrap, 1);
```

Integer arithmetic goes through `JavaOperands` so that overflow wraps like Java.

## Step 4 - Run the result and compare with Java

```sh
impl/build/sleela run $J/out/Tiny.sleela | grep -v '^\[security\]\|^verification:' > $J/sleela.out
diff $J/java.out $J/sleela.out && echo IDENTICAL
```

```
IDENTICAL
```

`sleela` prints two verification lines (`[security] SHA-256 verification required ...` and
`verification: PASS (18667 item(s))`) on stdout before the program output, hence the `grep -v`. Its
exit status was 0.

## Step 5 - See a refusal

```sh
cat > $J/src/Bad.java <<'EOF'
import java.util.*;
public class Bad { static void f(Object o){ var x = o; synchronized(o){ } } public static void main(String[] a){ f("x"); } }
EOF
mkdir -p $J/out2; bin/sleelactl run java-convert --file $J/src/Bad.java --out $J/out2; echo "exit=$?"
ls -A $J/out2
```

```
  source files 1, converted 0 (0 SLeeLa files), failed closed 1
  FAIL Bad.java:2:56 [synchronized] synchronized blocks have no SLeeLa form
exit=1
CONVERSION-REPORT.json
CONVERSION-REPORT.md
.javaconv-emitted
```

No `.sleela` file is written for the refused source.

## Step 6 - Run the project's differential suite

```sh
bash tests/javaconv_differential_test.sh
```

```
PASS  Strings      11 lines identical to the JDK
...
PASS  reject/SyncBlock refused [synchronized], no output written
PASS  stale-output output of an earlier run removed when the file now fails closed
javaconv differential: pass=32 fail=0
```

## What you did

You converted a Java program, ran it on the SLeeLa VM, proved its output equal to the JDK's, and saw
the converter refuse something it cannot convert exactly.

## Doc drift noticed

`sleelactl/convert/README.md` and `JAVA.CONVERSION.STATUS.md` say 21 programs and "31 of 31". The directory
has 22 programs (`tests/java/Bounds.java` is not named in either document) and the suite reports 32 passes.

## Next steps

- [`sleelactl/convert/README.md`](../../../sleelactl/convert/README.md), [`JAVA.CONVERSION.STATUS.md`](../../reference/JAVA.CONVERSION.STATUS.md), [`CONVERSION.PRINCIPLES.md`](../../reference/CONVERSION.PRINCIPLES.md)
- [`lib/java/JAVA.SPEC.md`](../../../lib/java/JAVA.SPEC.md) (what is converted and what is refused)
- `bin/sleelactl run java-convert --jdk-version 28` converts `sleelactl/jdk/28/java` (see `sleelactl/jdk/README.md`); not run here.

## Sources

Read: `bin/sleelactl.d/registry.conf` (line 73), `sleelactl/convert/README.md`, `sleelactl/convert/Makefile`, `JAVA.CONVERSION.STATUS.md` (sections 1-3).
Ran: `bin/sleelactl help|info|inspect|build|run java-convert`, `javac`, `java`, `impl/build/sleela run`, `diff`, `tests/javaconv_differential_test.sh`.
Not run: whole-tree conversion (`--jdk`), `--split`/`--bundle`, `make -C sleelactl/convert test` (same script plus `--self-check`).
