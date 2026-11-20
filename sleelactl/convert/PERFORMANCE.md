# javaconv performance

Speed and memory of the converter binary (`javaconv`) and of the programs it converts, run on the SLeeLa VM.
Coverage numbers (how much converts) are in `README.md`; this file is about time.

## Measurement of 2026-11-20

Environment: Amazon Linux 2023, x86-64, AMD EPYC 9R14 (8 cores), 30 GB RAM, GCC 11.5.0, Corretto 25.0.5.1.
Binaries: `working-binaries/javaconv` (`JAVACONV-BIN-1.0121`, model `java-to-sleela 1.6.0`) and `impl/build/sleela`
(byte-identical to `working-binaries/sleela`, `SLEELA-BIN-1.0122`), from `master` `ffc820696`. Times were taken with
a Python wrapper (`time.perf_counter` for wall time, `getrusage` of the child for CPU time and peak resident memory),
because `/usr/bin/time` is not installed in the sandbox.

### The converter binary

| Run | Wall time | CPU time | Peak memory |
|---|---|---|---|
| `java.base` subset (`lang`, `util`, `math`, `text`): 884 files, 488,871 lines | 60.0 s | 60.1 s | 54 MB |
| one small file (`tests/java/Operands.java`, `--file`), best of 3 | 0.80 s | 0.88 s | 11 MB |
| `--self-check` (load and check the model) | 0.006 s | | |

- About 8,100 Java lines or 14.7 files per second on the subset. The converter is single-threaded (CPU time equals wall time).
- A single file costs about 0.8 s, mostly fixed setup: indexing the 4,700+ `lib/java` units and loading the model.
- All of `java.base` (3,344 files) took about 5.5 minutes in the same session. That run converts to a fixpoint, so
  it includes repeated rounds after units fail closed.

### Converted programs on the SLeeLa VM

Small CPU-bound programs were converted with `javaconv --file` and run with `sleela run` (best of 3) against
`java -cp` (best of 3):

| Program | Workload | JDK | SLeeLa VM | Output identical |
|---|---|---|---|---|
| `Hello` | one `println` | 0.046 s | 3.488 s | yes |
| `IntLoop` | 300,000 iterations of int multiply-add with `%` | 0.047 s | 3.615 s | yes |
| `Sieve` | primes up to 200,000 | 0.047 s | 3.628 s | yes |
| `Fib` | recursive `fib(22)` | 0.045 s | 3.563 s | yes |
| `Strings` | 20,000 `StringBuilder.append` calls | 0.047 s | 3.588 s | yes |

Every converted program printed exactly what the JDK printed. The times are dominated by start-up:

| Start-up part | Time |
|---|---|
| a hand-written SLeeLa program that prints one line (`sleela run`) | 3.56 s |
| of which the SHA-256 execution gate (`tools/verify-before-execution.py`, 18,673 files hashed before every run) | 1.89 s |
| JVM start-up (`Hello` on the JDK) | 0.046 s |

So the fixed cost of about 3.5 s is the SLeeLa runtime itself, the same for converted and hand-written code. The
work above start-up in these programs was 0.07 to 0.14 s on the VM. That is too small to give a reliable compute ratio
against the JDK, so no ratio is claimed here.

### Not measured yet

Steady-state compute speed of converted code. A second run with larger workloads (10,000,000 loop iterations, a sieve
to 5,000,000, `fib(30)`, 1,000,000 appends) was converted and compiled, but the timing run was interrupted before it
produced results. It should be repeated with one run per program and a time limit on each.

### Benchmark sources

```java
public class Hello { public static void main(String[] a) { System.out.println("hi"); } }

public class IntLoop {
  public static void main(String[] a) {
    int n = Integer.parseInt(a.length > 0 ? a[0] : "300000");
    int acc = 0;
    for (int i = 0; i < n; i++) { acc = acc * 31 + (i % 7); }
    System.out.println(acc);
  }
}

public class Sieve {
  public static void main(String[] a) {
    int n = 200000;
    boolean[] comp = new boolean[n + 1];
    int count = 0;
    for (int i = 2; i <= n; i++) {
      if (!comp[i]) { count++; for (long j = (long) i * i; j <= n; j += i) comp[(int) j] = true; }
    }
    System.out.println(count);
  }
}

public class Fib {
  static int fib(int k) { return k < 2 ? k : fib(k - 1) + fib(k - 2); }
  public static void main(String[] a) { System.out.println(fib(22)); }
}

public class Strings {
  public static void main(String[] a) {
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < 20000; i++) { sb.append(i % 10); }
    System.out.println(sb.length());
  }
}
```

To reproduce: `working-binaries/javaconv --file X.java --out DIR`, then `sleela run X.sleela` in `DIR` with
`SLEELA_HOME`, `SLEELA_LIB`, `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` set to the checkout, against
`javac -d cls X.java && java -cp cls X`.
