"""`sleela nordshrift`: the SLeeLa -> {SLeeLa, Java, C} emitters.

Three independent implementations of the same program are compared:
  1. the SLeeLa VM (`sleela run`),
  2. the emitted Java, compiled with javac and run on the JVM,
  3. the emitted C, compiled with the system C compiler,
and, for generated arithmetic, a Python oracle as a fourth witness.  Java and C
tests are skipped (visibly) when the tool is not installed.
"""
import random
import shutil
import subprocess
import unittest

import regression_support as rs
from test_cli_language import ProgramTestCase, random_expr

HAVE_JAVA = bool(shutil.which("javac") and shutil.which("java"))
HAVE_CC = bool(shutil.which(rs.CC))


def emit(directory, name, target=None, extra=()):
    args = ["nordshrift", *extra]
    if target:
        args.append(f"--target={target}")
    args.append(name)
    return rs.run_sleela(args, cwd=directory)


def emitted_text(cp) -> str:
    return "\n".join(rs.program_output(cp)) + "\n"


PURE = """class Pure {
    static int fib(int n) { if (n < 2) { return n; } return fib(n - 1) + fib(n - 2); }
    static int gcd(int a, int b) { if (b == 0) { return a; } return gcd(b, a % b); }
    static int ack(int m, int n) { if (m == 0) { return n + 1; } if (n == 0) { return ack(m - 1, 1); } return ack(m - 1, ack(m, n - 1)); }
    static int tri(int n) { if (n == 0) { return 0; } return n + tri(n - 1); }
    static boolean even(int n) { if (n == 0) { return true; } return odd(n - 1); }
    static boolean odd(int n) { if (n == 0) { return false; } return even(n - 1); }
    void main() {
        print(fib(15)); print(gcd(1071, 462)); print(ack(2, 3)); print(tri(100));
        print(even(10)); print(odd(7)); print("tri=" + tri(10));
        print(17 % 5); print(0 - 17 % 5); print(0 - 17 / 5); print(3 * 4 + 5 - 6); print(100 / 7 * 7 + 100 % 7);
        print(1 < 2); print(2 <= 1); print(3 == 3); print(3 != 3); print(true && false); print(true || false); print(!true);
    }
}
"""
PURE_WANT = ["610", "21", "9", "5050", "true", "true", "tri=55", "2", "-2", "-3", "11", "100",
             "true", "false", "true", "false", "false", "true", "false"]


ALGO = """class Algo {
    static int collatz(int n) { int s = 0; while (n != 1) { if (n % 2 == 0) { n = n / 2; } else { n = 3 * n + 1; } s++; } return s; }
    static int sumTo(int n) { int t = 0; for (int i = 1; i <= n; i++) { t += i; } return t; }
    static int pick(int a, int b) { return a > b ? a : b; }
    static int digits(int n) { int c = 0; while (n > 0) { n = n / 10; c++; } return c; }
    void main() {
        int k = 0; k += 5; k *= 3; k -= 1; k <<= 2; k ^= 5; k |= 256; k &= 511; k >>= 1; print(k);
        print(collatz(27)); print(sumTo(100)); print(pick(3, 9)); print(pick(9, 3)); print(digits(9876543));
        int j = 10; j--; --j; ++j; print(j);
        for (int i = 0; i < 3; i++) { print("i=" + i); }
        int m = 7; int r = m > 5 ? m * 2 : m; print(r);
    }
}
"""
def _collatz(n):
    s = 0
    while n != 1:
        n = n // 2 if n % 2 == 0 else 3 * n + 1
        s += 1
    return s
_k = 0; _k += 5; _k *= 3; _k -= 1; _k <<= 2; _k ^= 5; _k |= 256; _k &= 511; _k >>= 1
ALGO_WANT = [str(_k), str(_collatz(27)), str(sum(range(1, 101))), "9", "9", "7", "9", "i=0", "i=1", "i=2", "14"]

class Emitters(ProgramTestCase):
    SOURCES = {
        "Hello": 'class Hello { void main() { print("hello"); print(1 + 2); } }',
        "Loop": "class Loop { static int sum(int n) { if (n == 0) { return 0; } return n + sum(n - 1); } void main() { print(sum(10)); } }",
        "Str": 'class Str { static String g(int n) { if (n == 0) { return "end"; } return "n" + n + "," + g(n - 1); } void main() { print(g(3)); print("a" + 1 + 2); } }',
        "Pure": PURE,
        "Algo": ALGO,
        "Obj": 'class B { String hi() { return "b"; } }\n'
               'class K extends B { String hi() { return "k"; } }\n'
               'class Counter { int v = 1; static int made = 7; int get() { return this.v + 1; }\n'
               '                static int twice(int x) { return x * 2; } protected static int hidden = 99; static int reveal() { return hidden; } }\n'
               'class Obj { void main() { B b = new K(); print(b.hi()); print(new B().hi()); Counter c = new Counter();\n'
               '           print(c.get()); print(Counter.twice(21)); print(Counter.made); print(Counter.reveal()); } }',
    }

    def write(self, name):
        d = rs.fresh_dir("ns-" + name.lower())
        rs.write_program(d, f"{name}.sleela", "#sleela 3.1\n" + self.SOURCES[name])
        return d

    def test_emitted_sleela_has_the_same_behaviour_as_the_source(self):
        for name in self.SOURCES:
            d = self.write(name)
            run0 = rs.run_sleela(["run", f"{name}.sleela"], cwd=d)
            self.assertEqual(run0.returncode, 0, f"{name} (source): {rs.diagnostics(run0)}")
            direct = rs.program_output(run0)
            first = emit(d, f"{name}.sleela", extra=("--emit",))
            self.assertEqual(first.returncode, 0, f"{name}: {rs.diagnostics(first)}")
            text = emitted_text(first)
            self.assertIn("Generated by Nordshrift (Sleela target)", text)
            e = rs.fresh_dir("ns-emitted-" + name.lower())
            rs.write_program(e, f"{name}.sleela", "#sleela 3.1\n" + text)
            again = rs.run_sleela(["run", f"{name}.sleela"], cwd=e)
            self.assertEqual(again.returncode, 0, f"{name}: emitted source does not run: {rs.diagnostics(again)}")
            self.assertEqual(rs.program_output(again), direct, f"{name}: emitting changed the behaviour")

    def test_roundtrip_reports_structure_preserved_and_runs_identically(self):
        for name in self.SOURCES:
            d = self.write(name)
            direct = rs.program_output(rs.run_sleela(["run", f"{name}.sleela"], cwd=d))
            rt = emit(d, f"{name}.sleela", extra=("--roundtrip",))
            self.assertEqual(rt.returncode, 0, f"{name}: {rs.diagnostics(rt)}")
            self.assertIn("structure preserved", rt.stderr)
            self.assertEqual(rs.program_output(rt), direct, f"{name}: round trip changed behaviour")

    def test_emit_is_idempotent(self):
        """Regression (was NS-EMIT-BLOCKS): emit(emit(x)) == emit(x), also for if/while bodies."""
        for name in self.SOURCES:
            d = self.write(name)
            first = emit(d, f"{name}.sleela", extra=("--emit",))
            self.assertEqual(first.returncode, 0, f"{name}: {rs.diagnostics(first)}")
            text = emitted_text(first)
            e = rs.fresh_dir("ns-idem-" + name.lower())
            rs.write_program(e, f"{name}.sleela", "#sleela 3.1\n" + text)
            second = emit(e, f"{name}.sleela", extra=("--emit",))
            self.assertEqual(second.returncode, 0, f"{name}: {rs.diagnostics(second)}")
            self.assertEqual(emitted_text(second), text, name)

    def test_emitter_preserves_modifiers_inheritance_and_member_forms(self):
        """Regression (was NS-EMIT-STATIC / NS-EMIT-QUALIFIED / NS-EMIT-SILENT-DROP)."""
        d = self.write("Obj")
        text = emitted_text(emit(d, "Obj.sleela", extra=("--emit",)))
        for needle in ("class K extends B", "static int twice(int x)", "static int made = 7", "protected static int hidden = 99",
                       "new K()", "new B().hi()", "b.hi()", "Counter.twice(21)", "Counter.made", "this.v + 1"):
            self.assertIn(needle, text)
        self.assertNotIn("print()", text)
        self.assertNotIn("= ;", text)

    def test_loop_program_keeps_static(self):
        d = self.write("Loop")
        self.assertIn("static int sum(int n)", emitted_text(emit(d, "Loop.sleela", extra=("--emit",))))

    def test_emitters_fail_closed_on_unsupported_constructs(self):
        # nothing the emitters cannot print may vanish from the output: each case must fail, with no partial output
        cases = [
            ("java", "class B { String h() { return \"b\"; } }\nclass K extends B { String h() { return \"k\"; } }\nclass M { void main() { print(1); } }", "inheritance"),
            ("c", "class B { String h() { return \"b\"; } }\nclass K extends B { String h() { return \"k\"; } }\nclass M { void main() { print(1); } }", "inheritance"),
            ("c", "class B { int f() { return 1; } }\nclass M { void main() { B b = new B(); print(b.f()); } }", "expression"),
            ("java", "class B { int f() { return 1; } }\nclass M { void main() { B b = new B(); print(b.f()); } }", "expression"),
            ("sleela", "class B { B(int x) { } }\nclass M { void main() { print(1); } }", "constructors"),
            ("sleela", "interface I { int f(); }\nclass M { void main() { print(1); } }", "interfaces"),
        ]
        for target, body, why in cases:
            f = rs.fresh_dir("ns-closed")
            rs.write_program(f, "M.sleela", "#sleela 3.1\n" + body)
            r = emit(f, "M.sleela", target)
            self.assertNotEqual(r.returncode, 0, f"{target}: {why} must be rejected:\n{r.stdout[-300:]}")
            self.assertIn("cannot emit", rs.diagnostics(r))
            self.assertEqual([l for l in rs.program_output(r) if l.strip()], [], f"{target}: partial output for {why}")
        for body, why in (("int[] a = {1, 2}; print(a[0]);", "array access"),
                          ("int x = 1; print(x++);", "postfix value in C")):
            d = rs.fresh_dir("ns-more")
            rs.write_program(d, "T.sleela", f"#sleela 3.1\nclass T {{ void main() {{ {body} }} }}")
            cp = emit(d, "T.sleela", "c")
            self.assertNotEqual(cp.returncode, 0, why)
            self.assertTrue("unsupported" in rs.diagnostics(cp) or "cannot emit" in rs.diagnostics(cp), why + ": " + rs.diagnostics(cp))
            self.assertNotIn("int main", cp.stdout, "no partial C may be emitted")


@unittest.skipUnless(HAVE_JAVA, "javac/java not installed")
class JavaTarget(ProgramTestCase):
    def java_run(self, directory, cls):
        cp = emit(directory, f"{cls}.sleela", "java")
        self.assertEqual(cp.returncode, 0, rs.diagnostics(cp))
        (directory / f"{cls}.java").write_text(emitted_text(cp))
        javac = subprocess.run(["javac", f"{cls}.java"], cwd=directory, capture_output=True, text=True, timeout=180)
        self.assertEqual(javac.returncode, 0, javac.stderr)
        jvm = subprocess.run(["java", "-cp", ".", cls], cwd=directory, capture_output=True, text=True, timeout=120)
        self.assertEqual(jvm.returncode, 0, jvm.stderr)
        return jvm.stdout.splitlines()

    def test_pure_program_agrees_with_vm_and_oracle(self):
        d = rs.fresh_dir("java-pure")
        rs.write_program(d, "Pure.sleela", "#sleela 3.1\n" + PURE)
        vm = rs.program_output(rs.run_sleela(["run", "Pure.sleela"], cwd=d))
        self.assertEqual(vm, PURE_WANT)
        self.assertEqual(self.java_run(d, "Pure"), PURE_WANT)

    def test_algorithm_with_loops_assignments_and_ternary(self):
        d = rs.fresh_dir("java-algo")
        rs.write_program(d, "Algo.sleela", "#sleela 3.1\n" + ALGO)
        self.assertEqual(rs.program_output(rs.run_sleela(["run", "Algo.sleela"], cwd=d)), ALGO_WANT, "VM vs oracle")
        self.assertEqual(self.java_run(d, "Algo"), ALGO_WANT, "JVM vs oracle")

    def test_generated_integer_expressions_agree_across_vm_jvm_and_oracle(self):
        rng = random.Random(0x7A5A)
        exprs = [random_expr(rng, rng.randint(1, 5)) for _ in range(150)]
        body = "\n".join(f"        print({e.render()});" for e in exprs)
        d = rs.fresh_dir("java-expr")
        rs.write_program(d, "Ex.sleela", f"#sleela 3.1\nclass Ex {{ void main() {{\n{body}\n    }} }}")
        want = [str(e.value()) for e in exprs]
        self.assertEqual(rs.program_output(rs.run_sleela(["run", "Ex.sleela"], cwd=d)), want, "VM vs oracle")
        self.assertEqual(self.java_run(d, "Ex"), want, "JVM vs oracle")


@unittest.skipUnless(HAVE_CC, "C compiler not installed")
class CTarget(ProgramTestCase):
    def c_run(self, directory, cls):
        cp = emit(directory, f"{cls}.sleela", "c")
        self.assertEqual(cp.returncode, 0, rs.diagnostics(cp))
        (directory / f"{cls}.c").write_text(emitted_text(cp))
        cc = subprocess.run([rs.CC, "-w", "-O1", f"{cls}.c", "-o", cls], cwd=directory, capture_output=True, text=True, timeout=120)
        self.assertEqual(cc.returncode, 0, cc.stderr)
        run = subprocess.run([f"./{cls}"], cwd=directory, capture_output=True, text=True, timeout=60)
        return run

    def test_pure_program_agrees_with_vm_and_oracle(self):
        d = rs.fresh_dir("c-pure")
        rs.write_program(d, "Pure.sleela", "#sleela 3.1\n" + PURE)
        run = self.c_run(d, "Pure")
        self.assertEqual(run.returncode, 0)
        self.assertEqual(run.stdout.splitlines(), PURE_WANT)

    def test_algorithm_with_loops_assignments_and_ternary(self):
        d = rs.fresh_dir("c-algo")
        rs.write_program(d, "Algo.sleela", "#sleela 3.1\n" + ALGO)
        run = self.c_run(d, "Algo")
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(run.stdout.splitlines(), ALGO_WANT, "emitted C vs oracle")

    def test_bitwise_and_shift_operators_compile_correctly(self):
        """Regression (was NS-C-BITOPS): & | ^ << >> >>> and ~ map to real integer operations."""
        d = rs.fresh_dir("c-bits")
        rs.write_program(d, "Bits.sleela", "#sleela 3.1\nclass Bits { void main() { print(6 & 3); print(6 ^ 3); print(1 << 4); print(6 | 3);"
                                          " print(0 - 16 >> 2); print(0 - 16 >>> 60); print(~5); print(5 & 0 - 2); print(true && false); print(false || true); } }")
        vm = rs.program_output(rs.run_sleela(["run", "Bits.sleela"], cwd=d))
        self.assertEqual(vm, ["2", "5", "16", "7", "-4", "15", "-6", "4", "false", "true"])
        run = self.c_run(d, "Bits")
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(run.stdout.splitlines(), vm)

    def test_generated_integer_expressions_agree_across_vm_c_and_oracle(self):
        rng = random.Random(0xC0FFEE)
        exprs = [random_expr(rng, rng.randint(1, 5)) for _ in range(150)]
        body = "\n".join(f"        print({e.render()});" for e in exprs)
        d = rs.fresh_dir("c-expr")
        rs.write_program(d, "Ex.sleela", f"#sleela 3.1\nclass Ex {{ void main() {{\n{body}\n    }} }}")
        want = [str(e.value()) for e in exprs]
        self.assertEqual(rs.program_output(rs.run_sleela(["run", "Ex.sleela"], cwd=d)), want, "VM vs oracle")
        run = self.c_run(d, "Ex")
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(run.stdout.splitlines(), want, "emitted C vs oracle")


if __name__ == "__main__":
    unittest.main()
