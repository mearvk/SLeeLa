"""End-to-end language semantics, checked against independent Python oracles.

Each test writes a SLeeLa program into its own scratch directory, runs it with
the compiler under test (`sleela run`) and compares the program's output line
by line with what Python computes for the same program.

Pinned language facts (observed with the committed compiler and documented in
SLEELA.syntax / impl/DESIGN.md):
  * `int` and `long` are 64-bit two's complement and wrap on overflow;
  * `/` and `%` truncate toward zero (the remainder takes the dividend's sign);
  * `>>` is arithmetic, `>>>` is a logical shift of the 64-bit value;
  * String is UTF-16: length(), charAt() and hashCode() follow java.lang.String;
  * doubles print with C `%g` formatting (6 significant digits).
"""
import random
import subprocess
import unittest

import regression_support as rs

MASK = (1 << 64) - 1


def wrap(v: int) -> int:
    v &= MASK
    return v - (1 << 64) if v >= 1 << 63 else v


def trunc_div(a: int, b: int) -> int:
    q = abs(a) // abs(b)
    return q if (a < 0) == (b < 0) else -q


def trunc_mod(a: int, b: int) -> int:
    return a - b * trunc_div(a, b)


PREC = {"*": 12, "/": 12, "%": 12, "+": 11, "-": 11, "<<": 10, ">>": 10, ">>>": 10, "&": 7, "^": 6, "|": 5}


class Expr:
    """Random integer expression: renders as Java-precedence source and
    evaluates with 64-bit wrapping semantics."""

    def __init__(self, kind, a=None, b=None, op=None):
        self.kind, self.a, self.b, self.op = kind, a, b, op

    def value(self) -> int:
        k = self.kind
        if k == "lit":
            return self.a
        if k == "neg":
            return wrap(-self.a.value())
        if k == "not":
            return ~self.a.value()
        x, y, op = self.a.value(), self.b.value(), self.op
        if op == "+": return wrap(x + y)
        if op == "-": return wrap(x - y)
        if op == "*": return wrap(x * y)
        if op == "/": return wrap(trunc_div(x, y))
        if op == "%": return wrap(trunc_mod(x, y))
        if op == "&": return x & y
        if op == "|": return x | y
        if op == "^": return x ^ y
        if op == "<<": return wrap(x << y)
        if op == ">>": return x >> y
        if op == ">>>": return wrap((x & MASK) >> y)
        raise AssertionError(op)

    def render(self) -> str:
        k = self.kind
        if k == "lit":
            if self.a < 0:
                return f"({self.a}L)" if self.a < -(1 << 31) or self.a > (1 << 31) else f"({self.a})"
            return f"{self.a}L" if self.a > (1 << 31) else str(self.a)
        if k == "neg":
            return "-(" + self.a.render() + ")"
        if k == "not":
            return "~(" + self.a.render() + ")"
        p = PREC[self.op]
        left, right = self.a.render(), self.b.render()
        if self.a.kind == "bin" and PREC[self.a.op] < p:
            left = f"({left})"
        if self.b.kind == "bin" and PREC[self.b.op] <= p:     # operators are left-associative
            right = f"({right})"
        return f"{left} {self.op} {right}"


def random_expr(rng: random.Random, depth: int) -> Expr:
    if depth == 0 or rng.random() < 0.15:
        return Expr("lit", rng.choice((0, 1, 2, 3, 7, 10, 255, 1000, 65535, 1 << 20, 2147483647, 4294967296,
                                       rng.randint(0, 100000), -rng.randint(1, 100000))))
    if rng.random() < 0.12:
        return Expr("neg" if rng.random() < 0.5 else "not", random_expr(rng, depth - 1))
    op = rng.choice(list(PREC))
    left = random_expr(rng, depth - 1)
    if op in ("<<", ">>", ">>>"):
        right = Expr("lit", rng.randint(0, 40))
    else:
        right = random_expr(rng, depth - 1)
        if op in ("/", "%"):
            d = right.value()
            if d == 0 or (d == -1 and left.value() == -(1 << 63)):
                op = "+"                                     # avoid /0 and MIN / -1
    return Expr("bin", left, right, op)


class ProgramTestCase(unittest.TestCase):
    """Runs one program and returns its stdout lines."""

    @classmethod
    def setUpClass(cls):
        if rs.sleela_binary() is None:
            raise unittest.SkipTest("no sleela binary (build impl/build/sleela or use working-binaries/)")

    def run_program(self, source: str, name: str = "Prog", expect_rc: int = 0, syntax: str = "3.1"):
        d = rs.fresh_dir(name)
        path = rs.write_program(d, f"{name}.sleela", f"#sleela {syntax}\n{source}")
        cp = rs.run_sleela(["run", path.name], cwd=d)
        self.assertEqual(cp.returncode, expect_rc, f"rc={cp.returncode}\n{rs.diagnostics(cp)}")
        return rs.program_output(cp), cp


class IntegerArithmetic(ProgramTestCase):
    def test_random_expressions_match_64bit_oracle(self):
        rng = random.Random(0x5EEDAB1E)
        for batch in range(3):
            exprs = [random_expr(rng, rng.randint(1, 5)) for _ in range(120)]
            body = "\n".join(f"        print({e.render()});" for e in exprs)
            out, _ = self.run_program(f"class Arith {{ void main() {{\n{body}\n    }} }}", "Arith")
            want = [str(e.value()) for e in exprs]
            self.assertEqual(len(out), len(want))
            for e, got, w in zip(exprs, out, want):
                self.assertEqual(got, w, f"batch {batch}: {e.render()}")

    def test_wraparound_division_and_shift_facts(self):
        out, _ = self.run_program("""
class Facts { void main() {
    long max = 9223372036854775807L;
    print(max + 1); print(0 - max - 2); print(max * 2);
    print(7 / 2); print(-7 / 2); print(7 / -2); print(-7 / -2);
    print(7 % 3); print(-7 % 3); print(7 % -3); print(-7 % -3);
    print(1 << 62); print(-1 >> 10); print(-1 >>> 60); print(-16 >>> 28); print(5 >>> 1);
    print(2147483647 + 1); print(2147483647 * 2147483647);
    print(~0); print(~5); print(-(-5));
    print(0x7FFFFFFF); print(0xFF_FF); print(0b1010); print(0o17); print(1_000_000);
} }""", "Facts")
        want = [max_ + 1 for max_ in ()]  # placeholder to keep the list explicit below
        want = [
            str(wrap((1 << 63) - 1 + 1)), str(wrap(0 - ((1 << 63) - 1) - 2)), str(wrap(((1 << 63) - 1) * 2)),
            "3", "-3", "-3", "3",
            "1", "-1", "1", "-1",
            str(1 << 62), "-1", "15", str(((1 << 64) - 16) >> 28), "2",
            "2147483648", str(2147483647 * 2147483647),
            "-1", "-6", "5",
            "2147483647", "65535", "10", "15", "1000000",
        ]
        self.assertEqual(out, want)

    def test_comparisons_and_booleans(self):
        rng = random.Random(404)
        pairs = [(rng.randint(-50, 50), rng.randint(-50, 50)) for _ in range(40)] + [(3, 3), (0, 0), (-1, -1)]
        lines, want = [], []
        for a, b in pairs:
            for op in ("<", "<=", ">", ">=", "==", "!="):
                lines.append(f"print(({a}) {op} ({b}));")
                want.append(str({"<": a < b, "<=": a <= b, ">": a > b, ">=": a >= b, "==": a == b, "!=": a != b}[op]).lower())
        body = "\n        ".join(lines)
        out, _ = self.run_program(f"class Cmp {{ void main() {{\n        {body}\n    }} }}", "Cmp")
        self.assertEqual(out, want)

    def test_short_circuit_evaluation_order(self):
        out, _ = self.run_program("""
class Sc {
    static int calls = 0;
    static boolean t(String tag) { print("t" + tag); calls = calls + 1; return true; }
    static boolean f(String tag) { print("f" + tag); calls = calls + 1; return false; }
    void main() {
        print(f("1") && t("2"));
        print(t("3") || f("4"));
        print(t("5") && f("6") || t("7"));
        print(f("8") || f("9") && t("10"));
        print(calls);
        boolean x = true; boolean y = false;
        print(x & y); print(x | y); print(x ^ y); print(!x);
    }
}""", "Sc")
        self.assertEqual(out, ["f1", "false", "t3", "true", "t5", "f6", "t7", "true", "f8", "f9", "false", "7",
                               "false", "true", "true", "false"])

    def test_compound_assignment_and_increment_forms(self):
        out, _ = self.run_program("""
class Inc { void main() {
    int x = 10;
    x += 5; print(x); x -= 3; print(x); x *= 4; print(x); x /= 5; print(x); x %= 7; print(x);
    x <<= 3; print(x); x >>= 1; print(x); x |= 1; print(x); x &= 6; print(x); x ^= 15; print(x);
    int i = 5; print(i++); print(i); print(++i); print(i); print(i--); print(--i);
    int j = 3; int k = j++ + ++j; print(k); print(j);
} }""", "Inc")
        x = 10
        want = []
        for op, rhs in (("+", 5), ("-", 3), ("*", 4)):
            x = {"+": x + rhs, "-": x - rhs, "*": x * rhs}[op]; want.append(x)
        x = trunc_div(x, 5); want.append(x)
        x = trunc_mod(x, 7); want.append(x)
        x <<= 3; want.append(x); x >>= 1; want.append(x); x |= 1; want.append(x); x &= 6; want.append(x); x ^= 15; want.append(x)
        want += [5, 6, 7, 7, 7, 5]
        j = 3; first = j; j += 1; j += 1; want += [first + j, j]
        self.assertEqual(out, [str(v) for v in want])


class AlgorithmsAgainstPython(ProgramTestCase):
    def test_numeric_algorithms(self):
        out, _ = self.run_program("""
class Algo {
    static int fibRec(int n) { if (n < 2) { return n; } return fibRec(n - 1) + fibRec(n - 2); }
    static int fibIter(int n) { int a = 0; int b = 1; for (int i = 0; i < n; i++) { int t = a + b; a = b; b = t; } return a; }
    static int gcd(int a, int b) { while (b != 0) { int t = a % b; a = b; b = t; } return a; }
    static int collatz(int n) { int s = 0; while (n != 1) { if (n % 2 == 0) { n = n / 2; } else { n = 3 * n + 1; } s++; } return s; }
    static int powmod(int b, int e, int m) { int r = 1; b = b % m; while (e > 0) { if ((e & 1) == 1) { r = r * b % m; } b = b * b % m; e = e >> 1; } return r; }
    static long fact(int n) { long r = 1; for (int i = 2; i <= n; i++) { r *= i; } return r; }
    void main() {
        for (int n = 0; n <= 20; n++) { print(fibRec(n) + " " + fibIter(n)); }
        print(gcd(48, 18)); print(gcd(17, 5)); print(gcd(0, 9)); print(gcd(1071, 462));
        for (int n = 1; n <= 12; n++) { print(collatz(n)); }
        print(powmod(2, 10, 1000)); print(powmod(3, 200, 1000007)); print(powmod(7, 0, 13));
        print(fact(0)); print(fact(10)); print(fact(20)); print(fact(21));
        boolean[] comp = new boolean[200];
        int count = 0; String primes = "";
        for (int i = 2; i < 200; i++) { if (!comp[i]) { count++; primes = primes + i + ","; for (int j = i * i; j < 200; j += i) { comp[j] = true; } } }
        print(count); print(primes);
    }
}""", "Algo")
        import math
        fib = [0, 1]
        for _ in range(25):
            fib.append(fib[-1] + fib[-2])
        want = [f"{fib[n]} {fib[n]}" for n in range(21)]
        want += [str(math.gcd(48, 18)), str(math.gcd(17, 5)), str(math.gcd(0, 9)), str(math.gcd(1071, 462))]

        def collatz(n):
            s = 0
            while n != 1:
                n = n // 2 if n % 2 == 0 else 3 * n + 1
                s += 1
            return s
        want += [str(collatz(n)) for n in range(1, 13)]
        want += [str(pow(2, 10, 1000)), str(pow(3, 200, 1000007)), str(pow(7, 0, 13))]
        want += ["1", str(math.factorial(10)), str(math.factorial(20)), str(wrap(math.factorial(21)))]
        primes = [p for p in range(2, 200) if all(p % d for d in range(2, int(p ** 0.5) + 1))]
        want += [str(len(primes)), "".join(f"{p}," for p in primes)]
        self.assertEqual(out, want)

    def test_sorting_and_searching_over_lcg_data(self):
        out, _ = self.run_program("""
class Sorts {
    void main() {
        int n = 64; int[] a = new int[n]; int x = 12345;
        for (int i = 0; i < n; i++) { x = (x * 1103515245 + 12345) % 2147483648; a[i] = x % 1000; }
        for (int i = 0; i < n; i++) { for (int j = 0; j < n - 1 - i; j++) { if (a[j] > a[j + 1]) { int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; } } }
        String s = ""; for (int i = 0; i < n; i++) { s = s + a[i] + " "; } print(s);
        int lo = 0; int hi = n - 1; int target = a[37]; int found = -1;
        while (lo <= hi) { int mid = (lo + hi) / 2; if (a[mid] == target) { found = mid; hi = mid - 1; } else if (a[mid] < target) { lo = mid + 1; } else { hi = mid - 1; } }
        print(found); print(target);
        int[][] m = new int[3][3]; int[][] id = new int[3][3]; int[][] p = new int[3][3];
        for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { m[i][j] = i * 3 + j + 1; id[i][j] = i == j ? 1 : 0; } }
        for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { int acc = 0; for (int k = 0; k < 3; k++) { acc += m[i][k] * m[k][j]; } p[i][j] = acc; } }
        for (int i = 0; i < 3; i++) { print(p[i][0] + "," + p[i][1] + "," + p[i][2]); }
    }
}""", "Sorts")
        x, data = 12345, []
        for _ in range(64):
            x = (x * 1103515245 + 12345) % 2147483648
            data.append(x % 1000)
        data.sort()
        target = data[37]
        first = data.index(target)
        m = [[i * 3 + j + 1 for j in range(3)] for i in range(3)]
        prod = [[sum(m[i][k] * m[k][j] for k in range(3)) for j in range(3)] for i in range(3)]
        want = ["".join(f"{v} " for v in data), str(first), str(target)] + [",".join(map(str, row)) for row in prod]
        self.assertEqual(out, want)

    def test_control_flow_forms(self):
        out, _ = self.run_program("""
class Flow { void main() {
    int s = 0; for (int i = 1; i <= 10; i++) { if (i % 2 == 0) { continue; } if (i > 7) { break; } s += i; } print(s);
    int k = 0; do { k += 3; } while (k < 10); print(k);
    int w = 100; while (w > 1) { w = w / 3; } print(w);
    for (int d = 0; d < 6; d++) { switch (d) { case 0: print("zero"); break; case 1: case 2: print("one-two"); break; case 4: print("four"); default: print("fall-or-default"); } }
    outer: for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { if (j == 2) { continue outer; } if (i == 2) { break outer; } print(i * 10 + j); } }
    int t = 0; for (int a = 0; a < 5; a++) { for (int b = 0; b < 5; b++) { if (b > a) { break; } t++; } } print(t);
    print(5 > 3 ? "yes" : "no"); print(2 > 3 ? 1 : 2 > 1 ? 20 : 30);
} }""", "Flow")
        want = ["16", "12", "1", "zero", "one-two", "one-two", "fall-or-default", "four", "fall-or-default",
                "fall-or-default", "0", "1", "10", "11", "15", "yes", "20"]
        self.assertEqual(out, want)


class StringSemantics(ProgramTestCase):
    @staticmethod
    def literal(text: str) -> str:
        """Source form of a string literal. Non-ASCII text is written as raw UTF-8
        (the lexer rejects \\uD83D-style surrogate escapes), controls are escaped."""
        out = []
        for ch in text:
            cp = ord(ch)
            if ch == "\\": out.append("\\\\")
            elif ch == '"': out.append('\\"')
            elif cp == 0: out.append("\\0")
            elif cp < 0x20 or cp == 0x7F: out.append("\\u%04x" % cp)
            else: out.append(ch)
        return "".join(out)

    def test_string_methods_against_python(self):
        rng = random.Random(0x57B1)
        words = ["", "a", "hello", "Hello, World", "  padded  ", "aaa", "abcabc", "MiXeD cAsE", "x" * 40, "line one", "tab\there"]
        cases = words + ["".join(rng.choice("abcXYZ 019_-") for _ in range(rng.randint(1, 14))) for _ in range(25)]
        lines, want = [], []
        for i, s in enumerate(cases):
            lit = self.literal(s)
            sub = rng.choice(["a", "ab", "l", "c", " ", "zz", "Hello", ""])
            lines.append(f'String s{i} = "{lit}";')
            lines.append(f'print(s{i}.length()); print(s{i}.toUpperCase()); print(s{i}.toLowerCase());')
            lines.append(f'print(s{i}.indexOf("{sub}")); print(s{i}.contains("{sub}")); print(s{i}.startsWith("{sub}")); print(s{i}.endsWith("{sub}"));')
            lines.append(f'print(s{i}.trim().length()); print(s{i}.replace("{sub or "q"}", "#")); print(s{i}.hashCode());')
            want += [str(len(s)), s.upper(), s.lower(), str(s.find(sub)), str(sub in s).lower(),
                     str(s.startswith(sub)).lower(), str(s.endswith(sub)).lower(), str(len(s.strip(" \t\n\r\x0b\x0c"))),
                     s.replace(sub or "q", "#"), str(java_hash_str(s))]
        body = "\n        ".join(lines)
        out, _ = self.run_program(f"class Str {{ void main() {{\n        {body}\n    }} }}", "Str")
        self.assertEqual(out, want)

    def test_utf16_semantics_follow_java(self):
        samples = ["\U0001F600", "h\u00e9llo", "\u20ac", "a\U0001F600b", "\U00010000\U0010FFFF", "polygenelubricants", "\u0000"[:0], "Aa", "BB"]
        lines, want = [], []
        for i, s in enumerate(samples):
            lines.append(f'String u{i} = "{self.literal(s)}"; print(u{i}.length()); print(u{i}.hashCode());')
            u = to_units(s)
            want += [str(len(u)), str(java_hash(u))]
            if u:
                lines.append(f"print(u{i}.charAt(0) + 0); print(u{i}.charAt({len(u) - 1}) + 0);")
                want += [str(u[0]), str(u[-1])]
        body = "\n        ".join(lines)
        out, _ = self.run_program(f"class Utf {{ void main() {{\n        {body}\n    }} }}", "Utf")
        self.assertEqual(out, want)

    def test_concatenation_equality_and_ordering(self):
        out, _ = self.run_program("""
class Cat { void main() {
    print(1 + 2 + "3" + 4 + 5); print("a" + 1 + 2); print("" + 1.5); print("v=" + true); print("n" + null);
    print("abc".equals("abc")); print("abc".equals("abd")); print("abc".equals(""));
    print("b".compareTo("a")); print("a".compareTo("b")); print("abc".compareTo("abcd")); print("same".compareTo("same"));
    print("abc".substring(1)); print("abc".substring(1, 2)); print("abc".substring(3).length());
    String s = ""; for (int i = 0; i < 5; i++) { s = s + i; } print(s); s += "!"; print(s);
    StringBuilder sb = new StringBuilder(); sb.append("x"); sb.append(1); sb.append(true); print(sb.toString());
} }""", "Cat")
        self.assertEqual(out, ["3345", "a12", "1.5", "v=true", "nnull", "true", "false", "false", "1", "-1", "-1", "0",
                               "bc", "b", "0", "01234", "01234!", "x1true"])


def to_units(text: str):
    import struct
    raw = text.encode("utf-16-le", "surrogatepass")
    return list(struct.unpack("<%dH" % (len(raw) // 2), raw))


def java_hash(units) -> int:
    h = 0
    for u in units:
        h = (h * 31 + u) & 0xFFFFFFFF
    return h - (1 << 32) if h >= 1 << 31 else h


def java_hash_str(s: str) -> int:
    return java_hash(to_units(s))


class ObjectModel(ProgramTestCase):
    def test_inheritance_dispatch_interfaces_and_statics(self):
        out, _ = self.run_program("""
interface Shape { int area(); }
abstract class Base implements Shape {
    String tag;
    Base(String t) { tag = t; }
    abstract int area();
    String describe() { return tag + ":" + area(); }
}
class Rect extends Base {
    int w; int h;
    Rect(int w, int h) { super("rect"); this.w = w; this.h = h; }
    int area() { return w * h; }
}
class Sq extends Rect {
    static int made = 0;
    Sq(int s) { super(s, s); tag = "sq"; made++; }
    String describe() { return "[" + super.describe() + "]"; }
}
class Obj { void main() {
    Shape[] shapes = new Shape[3];
    shapes[0] = new Rect(2, 3); shapes[1] = new Sq(4); shapes[2] = new Sq(5);
    int total = 0;
    for (int i = 0; i < shapes.length; i++) { Shape s = shapes[i]; total += s.area(); print(s.area()); }
    print(total);
    Rect r = new Sq(6); print(r.describe()); print(r instanceof Rect); print(r instanceof Sq); print(r instanceof Shape);
    Rect plain = new Rect(1, 1); print(plain instanceof Sq); print(plain.describe());
    print(Sq.made);
} }""", "Obj")
        self.assertEqual(out, ["6", "16", "25", "47", "[sq:36]", "true", "true", "true", "false", "rect:1", "3"])

    def test_exceptions_finally_and_unwinding(self):
        out, _ = self.run_program("""
class Ex {
    static void thrower(int n) { if (n == 0) { throw 42; } thrower(n - 1); }
    static int viaFinally() { try { return 1; } finally { print("f-finally"); } }
    static int loopFinally() { int n = 0; for (int i = 0; i < 3; i++) { try { if (i == 1) { continue; } n += 10; } finally { n += 1; } } return n; }
    void main() {
        try { thrower(5); print("unreachable"); } catch (Any e) { print("caught " + e); }
        print(viaFinally());
        try { try { throw 1; } finally { print("inner-finally"); } } catch (Any e) { print("outer " + e); }
        try { try { throw 1; } catch (Any e) { print("c1"); throw 2; } } catch (Any e) { print("c2 " + e); }
        print(loopFinally());
        int kept = 7; try { throw "text"; } catch (Any e) { print(e); } print(kept);
        int[] a = new int[3];
        try { a[3] = 1; print("no error"); } catch (Any e) { print("bounds"); }
        try { print("body"); } catch (Any e) { print("never"); } finally { print("always"); }
    }
}""", "Ex")
        self.assertEqual(out, ["caught 42", "f-finally", "1", "inner-finally", "outer 1", "c1", "c2 2", "23",
                               "text", "7", "bounds", "body", "always"])

    def test_uncaught_exception_and_runtime_errors_exit_nonzero(self):
        out, cp = self.run_program("class Boom { void main() { print(\"before\"); throw 99; } }", "Boom", expect_rc=1)
        self.assertEqual(out, ["before"])
        self.assertIn("uncaught language exception", rs.diagnostics(cp))
        out, cp = self.run_program("class Div { void main() { int z = 0; print(\"a\"); print(5 / z); print(\"b\"); } }", "Div", expect_rc=1)
        self.assertEqual(out, ["a"])
        self.assertIn("divide by zero", rs.diagnostics(cp))
        out, cp = self.run_program("class Md { void main() { int z = 0; print(5 % z); } }", "Md", expect_rc=1)
        self.assertIn("zero", rs.diagnostics(cp))

    def test_class_with_more_than_64_instance_fields_is_rejected_consistently(self):
        """Contract (SL_MAX_STRUCT_FIELDS = 64): `check` and `run` must agree and
        name the cap, instead of check passing and run failing with a misleading
        layout-count error."""
        def source(n):
            fields = "\n".join(f"    int f{i};" for i in range(n))
            return f"class Wide {{\n{fields}\n    void main() {{ print(\"ok\"); }}\n}}"
        d = rs.fresh_dir("wide")
        ok = rs.write_program(d, "Wide64.sleela", "#sleela 3.1\n" + source(64).replace("Wide", "Wide64"))
        bad = rs.write_program(d, "Wide65.sleela", "#sleela 3.1\n" + source(65).replace("Wide", "Wide65"))
        for mode in ("check", "run"):
            self.assertEqual(rs.run_sleela([mode, ok.name], cwd=d).returncode, 0, f"64 fields must be accepted by {mode}")
        results = {m: rs.run_sleela([m, bad.name], cwd=d) for m in ("check", "run")}
        for mode, cp in results.items():
            msg = rs.diagnostics(cp)
            if cp.returncode == 0:
                rs.record_known_issue("VM-WIDE-CLASS", f"`{mode}` accepts a 65-field class", True, "test_cli_language")
            self.assertTrue(cp.returncode != 0 or mode == "check" and results["run"].returncode == 0,
                            f"{mode} accepted 65 fields while the other rejected it:\n{msg}")
        self.assertEqual(results["check"].returncode != 0, results["run"].returncode != 0,
                         "check and run must agree on a 65-field class")
        if results["run"].returncode != 0:
            self.assertIn("64", rs.diagnostics(results["run"]))


class StringCompoundAssignment(ProgramTestCase):
    def test_string_plus_equals_appends_the_text_form_of_any_value(self):
        """Regression (was LANG-STR-PLUSEQ): Java allows `s += <int|double|boolean|String>`."""
        out, _ = self.run_program("""
class Pe {
    String f = "f";
    void main() {
        String s = "a"; s += 1; print(s); s += 2.5; print(s); s += true; print(s); s += "x"; print(s);
        s += 7 * 6; print(s);
        String[] arr = new String[2]; arr[0] = "p"; arr[0] += 9; print(arr[0]);
        f += 3; print(f);
        int n = 5; n += 1; print(n);
    }
}""", "Pe")
        self.assertEqual(out, ["a1", "a12.5", "a12.5true", "a12.5truex", "a12.5truex42", "p9", "f3", "6"])

    def test_string_compound_assignment_other_than_plus_is_still_rejected(self):
        d = rs.fresh_dir("strminus")
        p = rs.write_program(d, "Sm.sleela", '#sleela 3.1\nclass Sm { void main() { String s = "a"; s -= 1; print(s); } }')
        self.assertNotEqual(rs.run_sleela(["run", p.name], cwd=d).returncode, 0)
        p = rs.write_program(d, "Si.sleela", '#sleela 3.1\nclass Si { void main() { int n = 1; n += "x"; print(n); } }')
        self.assertNotEqual(rs.run_sleela(["run", p.name], cwd=d).returncode, 0)


class NumericFormatting(ProgramTestCase):
    def test_double_arithmetic_and_printing(self):
        """CHARACTERIZATION: doubles print with C %g (6 significant digits)."""
        out, _ = self.run_program("""
class Dbl { void main() {
    print(7.0 / 2); print(1 / 3.0); print(0.1 + 0.2); print(10 / 4.0); print((double) 7 / 2); print(7 / 2 * 2.0);
    print((int) 3.99); print((int) -3.99); print(1e10); print(1.0e-5); print(100.0); print(123456789.0); print(2.5 * 4);
    double d = 0.5; d += 0.25; print(d); print(d > 0.7); print(d == 0.75); print(1 == 1.0);
} }""", "Dbl")
        self.assertEqual(out, ["3.5", "0.333333", "0.3", "2.5", "3.5", "6", "3", "-3", "1e+10", "1e-05", "100", "1.23457e+08",
                               "10", "0.75", "true", "true", "true"])


class ArrayText(ProgramTestCase):
    """Regression (was VM-ARRAY-TEXT-DEADLOCK): converting an array of arrays to
    text took the VM's array lock, then took it again for each inner array, and
    hung forever. print(m), "m=" + m and new T[a][b] printing all hit it."""

    def run_bounded(self, source: str, name: str):
        d = rs.fresh_dir(name)
        rs.write_program(d, f"{name}.sleela", "#sleela 3.1\n" + source)
        try:
            cp = rs.run_sleela(["run", f"{name}.sleela"], cwd=d, timeout=60)
        except subprocess.TimeoutExpired:
            self.fail(f"{name}: the VM hung converting an array to text")
        self.assertEqual(cp.returncode, 0, rs.diagnostics(cp))
        return rs.program_output(cp)

    def test_nested_arrays_print_and_concatenate(self):
        out = self.run_bounded("""
class Nest { void main() {
    int[][] m = {{1, 2}, {3}}; print(m); print("m=" + m);
    int[][] w = new int[2][2]; w[1][0] = 5; print(w);
    int[][] half = new int[2][]; print(half); int[] row = {7}; half[1] = row; print(half);
    String[][] s = {{"a", "b"}, {}}; print(s);
    int[][][] cube = {{{1}, {2, 3}}, {}}; print(cube);
    int[] flat = {4, 5}; print(flat); int[] none = new int[0]; print(none);
} }""", "Nest")
        self.assertEqual(out, ["[[1, 2], [3]]", "m=[[1, 2], [3]]", "[[0, 0], [5, 0]]", "[null, null]",
                               "[null, [7]]", "[[a, b], []]", "[[[1], [2, 3]], []]", "[4, 5]", "[]"])

    def test_deep_nesting_is_cut_off_not_recursed_forever(self):
        decls, t = ["    int[] a0 = {7};"], "int[]"
        for k in range(1, 21):
            t += "[]"
            decls.append(f"    {t} a{k} = {{a{k - 1}}};")
        body = "\n".join(decls)
        out = self.run_bounded(f"class Deep {{ void main() {{\n{body}\n    print(a20);\n}} }}", "Deep")
        self.assertEqual(out, ["[" * 16 + "[...]" + "]" * 16])


if __name__ == "__main__":
    unittest.main()
