"""The regression corpus: hostile and malformed inputs with a documented outcome.

Every case is a fixture plus an expectation.  File cases live under
test-suites/regression/corpus/<family>/ as NAME.<sleela|hex> with a sidecar
NAME.expect; generated cases (inputs too large to commit) are listed in
GENERATED below.  The rules, the .expect format and the case table are in
corpus/README.md, and a test here fails if a case is missing from that table.

A case passes when
  * the tool neither crashed (signal), nor hung, nor was killed by the harness
    timeout, whatever the expectation says, and
  * the outcome matches: a rejected input exits non-zero with every expected
    message on stderr; an accepted input exits 0 with the expected output.
A case marked `status: known-issue` states the contract the code is supposed to
honour.  While the code does not honour it the case is reported as OPEN and does
not fail the run; as soon as it does, the case says FIXED and should be promoted.
"""
import re
import subprocess
import unittest
from pathlib import Path
from typing import Callable, Dict, List, Optional

import regression_support as rs

CORPUS = rs.REG / "corpus"
README = CORPUS / "README.md"
CLI_TIMEOUT = 90


# --------------------------------------------------------------------- cases

class Case:
    def __init__(self, fields: Dict[str, List[str]], source: Optional[Path] = None,
                 build: Optional[Callable[[], str]] = None, kind: str = "sleela"):
        self.f = fields
        self.source = source
        self.build = build
        self.kind = kind

    def one(self, key: str, default: str = "") -> str:
        v = self.f.get(key)
        return v[0] if v else default

    def many(self, key: str) -> List[str]:
        return list(self.f.get(key, []))

    @property
    def id(self) -> str:
        return self.one("id")

    @property
    def known_issue(self) -> bool:
        return self.one("status") == "known-issue"

    def text(self) -> bytes:
        if self.build is not None:
            return self.build().encode("utf-8")
        assert self.source is not None
        return self.source.read_bytes()


def parse_expect(path: Path) -> Dict[str, List[str]]:
    fields: Dict[str, List[str]] = {}
    for n, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        key, sep, value = line.partition(":")
        if not sep:
            raise ValueError(f"{path}:{n}: expected 'key: value'")
        fields.setdefault(key.strip(), []).append(value.strip())
    return fields


def file_cases() -> List[Case]:
    out = []
    for expect in sorted(CORPUS.glob("*/*.expect")):
        for ext in ("sleela", "hex"):
            src = expect.with_suffix("." + ext)
            if src.exists():
                out.append(Case(parse_expect(expect), source=src, kind=ext))
                break
    return out


HEADER = "#sleela 3.1\n"


def program(body: str) -> str:
    return HEADER + "public class A { void main() { " + body + " } }\n"


def gen(cid, family, expect, build, note, message=None, command="check", output=None, ref="impl/frontend/parser.cpp",
        defect=None):
    f = {"id": [cid], "family": [family], "expect": [expect], "command": [command], "reference": [ref], "note": [note]}
    if message:
        f["message"] = [message]
    if output:
        f["output"] = [output]
    if defect:
        f["defect"] = [defect]
    return Case(f, build=build)


NEST = "nesting is too deep"
GENERATED = [
    gen("LIMIT-NEST-PARENS", "resource-limits", "reject", lambda: program("int x = " + "(" * 20000 + "1" + ")" * 20000 + ";"),
        "20,000 nested parentheses. The parser recursed once per level and crashed with a segmentation fault.",
        NEST, defect="PARSE-STACK-OVERFLOW"),
    gen("LIMIT-NEST-BLOCKS", "resource-limits", "reject", lambda: program("{" * 5000 + "}" * 5000),
        "5,000 nested blocks.", NEST, defect="PARSE-STACK-OVERFLOW"),
    gen("LIMIT-OPERATOR-CHAIN", "resource-limits", "reject", lambda: program("int x = 1" + "+1" * 100000 + ";"),
        "One statement of 100,000 chained operators builds a left-deep tree that later passes walk recursively.",
        "expression is too long", defect="TREE-WALK-STACK-OVERFLOW"),
    gen("LIMIT-PREFIX-RUN", "resource-limits", "reject", lambda: program("int x = " + "-" * 5000 + "1;"),
        "5,000 prefix operators in a row.", "too many prefix operators", defect="PARSE-STACK-OVERFLOW"),
    gen("LIMIT-CAST-CHAIN", "resource-limits", "reject", lambda: program("int x = " + "(int)" * 30000 + "1;"),
        "30,000 back-to-back casts nest without any open bracket.", "too many chained casts", defect="PARSE-STACK-OVERFLOW"),
    gen("LIMIT-NEST-AT-CAP", "resource-limits", "accept", lambda: program("int x = " + "(" * 900 + "1" + ")" * 900 + "; print(x);"),
        "900 nested parentheses is under the cap and still runs.", command="run", output="1"),
    gen("LIMIT-CHAIN-AT-CAP", "resource-limits", "accept", lambda: program("int x = 1" + "+1" * 4000 + "; print(x);"),
        "A 4,000-operator statement is under the cap and still runs.", command="run", output="4001"),
    gen("LIMIT-LONG-IDENTIFIER", "resource-limits", "accept", lambda: program("int " + "a" * 2000000 + " = 1;"),
        "A 2,000,000-character identifier is legal. Dependency discovery used std::regex, which recurses per character and crashed.",
        ref="impl/frontend/driver.cpp (loadReferencedLibraryUnits)", defect="REGEX-STACK-OVERFLOW"),
    gen("LIMIT-LONG-IDENTIFIER-RUN", "resource-limits", "accept",
        lambda: program("int " + "a" * 50000 + " = 1; print(" + "a" * 50000 + ");"),
        "A 50,000-character identifier runs end to end.", command="run", output="1",
        ref="impl/frontend/driver.cpp (loadReferencedLibraryUnits)", defect="REGEX-STACK-OVERFLOW"),
    gen("PARSE-OK-CRLF", "parser-acceptance", "accept",
        lambda: '#sleela 3.1\r\npublic class A {\r\n  void main() {\r\n    print("ok");\r\n  }\r\n}\r\n',
        "A file with Windows line endings is the same program. Generated because the repository normalizes text files to LF.",
        command="run", output="ok", ref="impl/frontend/lexer.cpp"),
    gen("LIMIT-MANY-CLASSES", "resource-limits", "accept",
        lambda: HEADER + "".join(f"class C{i} {{ }}\n" for i in range(20000)) + "public class A { void main(){} }\n",
        "20,000 declarations."),
    gen("LIMIT-STRING-1MB", "resource-limits", "accept", lambda: program('String s = "' + "x" * 1000000 + '"; print(s.length());'),
        "A one-megabyte string literal.", command="run", output="1000000"),
    gen("RUN-RECURSION", "resource-limits", "reject",
        lambda: HEADER + "public class A { int f(int n){ return f(n+1); } void main(){ print(f(0)); } }\n",
        "Unbounded recursion ends with a runtime error, not a crash.", "call depth exceeded", command="run",
        ref="impl/core/sleela_core.c"),
    gen("RUN-ARRAY-TOO-LARGE", "resource-limits", "reject", lambda: program("int[] a = new int[2000000000]; print(a.length);"),
        "An array far beyond the heap limit.", "array allocation failed", command="run", ref="impl/core/sleela_core.c"),
    gen("RUN-ARRAY-NEGATIVE", "resource-limits", "reject", lambda: program("int[] a = new int[-1]; print(a.length);"),
        "A negative array size.", "array allocation failed", command="run", ref="impl/core/sleela_core.c"),
    gen("RUN-DIVIDE-BY-ZERO", "resource-limits", "reject", lambda: program("int a = 0; print(1 / a);"),
        "Integer division by zero.", "integer divide by zero", command="run", ref="impl/core/sleela_core.c"),
    gen("RUN-MANY-LOCALS", "resource-limits", "reject",
        lambda: program("".join(f"int v{i} = {i}; " for i in range(30000)) + "print(v29999);"),
        "30,000 locals in one method exhaust the VM stack with a runtime error.", "stack overflow", command="run",
        ref="impl/core/sleela_core.c"),
]


def all_cases() -> List[Case]:
    return file_cases() + GENERATED


# ------------------------------------------------------------------ execution

def evaluate(case: Case, returncode: int, out: str, err: str) -> Optional[str]:
    """None when the outcome matches the expectation, else what differs."""
    expect = case.one("expect")
    if expect == "reject":
        if returncode == 0:
            return "expected a rejection, but the tool exited 0"
        for m in case.many("message"):
            if m not in err and m not in out:
                return f"diagnostic does not contain {m!r}; got: {err.strip()[:300]!r}"
        return None
    if expect == "accept":
        if returncode != 0:
            return f"expected acceptance, but the tool exited {returncode}: {err.strip()[:300]!r}"
        for m in case.many("warning"):
            if m not in err:
                return f"expected a warning containing {m!r}; got: {err.strip()[:300]!r}"
        for m in case.many("output"):
            if m not in out:
                return f"output does not contain {m!r}; got: {out.strip()[:300]!r}"
        return None
    return f"unknown expect value {expect!r}"


def crashed(returncode: int) -> bool:
    return returncode < 0 or returncode >= 126


def finish(test: unittest.TestCase, case: Case, problem: Optional[str]) -> None:
    if case.known_issue:
        rs.record_known_issue(case.id, f"{case.id}: {case.one('note')}", still_open=problem is not None,
                              source="corpus")
        return
    if problem:
        test.fail(f"{case.id}: {problem}\n  fixture: {case.source or 'generated'}\n  note: {case.one('note')}")


def run_cli_case(test: unittest.TestCase, case: Case) -> None:
    d = rs.fresh_dir("corpus")
    name = case.source.name if case.source else "case.sleela"
    (d / name).write_bytes(case.text())
    command = case.one("command", "check")
    try:
        cp = rs.run_sleela([command, name], cwd=d, timeout=int(case.one("timeout", str(CLI_TIMEOUT))))
    except subprocess.TimeoutExpired:
        test.fail(f"{case.id}: no result within {CLI_TIMEOUT}s (a hang is never an acceptable outcome)")
    if crashed(cp.returncode):
        test.fail(f"{case.id}: the tool crashed (exit status {cp.returncode}); hostile input must be rejected, not crash it\n"
                  f"  stderr: {rs.diagnostics(cp)[:300]!r}")
    finish(test, case, evaluate(case, cp.returncode, "\n".join(rs.program_output(cp)), rs.diagnostics(cp)))


_driver: Dict[str, Path] = {}


def http_driver() -> Path:
    if "bin" not in _driver:
        out = rs.BUILD / "bin" / "http6_decode"
        out.parent.mkdir(parents=True, exist_ok=True)
        # the code under test is built as the project builds it; only the driver is held to -Werror
        objs = [rs.library_object("http6"), rs.library_object("router")]
        cmd = [rs.CC, *rs.STRICT_C, *rs._extra_flags(), rs.REG / "corpus" / "tools" / "http6_decode.c", *objs, "-o", out]
        r = subprocess.run([str(c) for c in cmd], capture_output=True, text=True)
        if r.returncode != 0:
            raise rs.BuildError(f"building the corpus HTTP driver failed:\n{r.stderr}")
        _driver["bin"] = out
    return _driver["bin"]


def run_http_case(test: unittest.TestCase, case: Case) -> None:
    assert case.source is not None
    try:
        cp = subprocess.run([str(http_driver()), str(case.source)], capture_output=True, text=True, timeout=30)
    except subprocess.TimeoutExpired:
        test.fail(f"{case.id}: the decoder did not return")
    if cp.returncode != 0:
        test.fail(f"{case.id}: decoder driver failed (exit {cp.returncode}): {cp.stderr.strip()[:300]}")
    m = re.match(r"rc=(-?\d+) consumed=(\d+) type=(\d+) length=(\d+)", cp.stdout)
    if not m:
        test.fail(f"{case.id}: unexpected driver output {cp.stdout!r}")
    rc = int(m.group(1))
    problem = None
    if rc != int(case.one("rc")):
        problem = f"decode returned {rc}, expected {case.one('rc')}"
    elif (rc == 0) != (case.one("expect") == "accept"):
        problem = f"decode result {rc} does not agree with expect: {case.one('expect')}"
    finish(test, case, problem)


# ---------------------------------------------------------------------- tests

@unittest.skipIf(rs.sleela_binary() is None, "no sleela binary")
class Corpus(unittest.TestCase):
    maxDiff = None


def _make(case: Case):
    def test(self):
        if case.kind == "hex":
            if rs.have_toolchain():
                self.skipTest(rs.have_toolchain())
            run_http_case(self, case)
        else:
            run_cli_case(self, case)
    test.__name__ = "test_" + re.sub(r"[^A-Za-z0-9]+", "_", case.id)
    test.__doc__ = f"{case.id}: {case.one('note')[:90]}"
    return test


for _case in all_cases():
    setattr(Corpus, "test_" + re.sub(r"[^A-Za-z0-9]+", "_", _case.id), _make(_case))


class CorpusBookkeeping(unittest.TestCase):
    """The corpus is only trustworthy while its files and its table agree."""

    def test_every_fixture_has_an_expectation_and_back(self):
        fixtures = {p.with_suffix("") for ext in ("sleela", "hex") for p in CORPUS.glob(f"*/*.{ext}")}
        expects = {p.with_suffix("") for p in CORPUS.glob("*/*.expect")}
        self.assertEqual(sorted(map(str, fixtures - expects)), [], "fixtures without a .expect file")
        self.assertEqual(sorted(map(str, expects - fixtures)), [], ".expect files without a fixture")

    def test_ids_are_unique_and_fields_complete(self):
        seen = set()
        for c in all_cases():
            self.assertTrue(c.id, f"case without an id: {c.source}")
            self.assertNotIn(c.id, seen, f"duplicate case id {c.id}")
            seen.add(c.id)
            for key in ("family", "expect", "reference", "note"):
                self.assertTrue(c.one(key), f"{c.id}: missing '{key}'")
            self.assertIn(c.one("expect"), ("accept", "reject"), c.id)
            if c.kind == "hex":
                self.assertTrue(re.fullmatch(r"-?\d+", c.one("rc")), f"{c.id}: hex cases need 'rc'")

    def test_every_reference_points_at_a_file(self):
        for c in all_cases():
            path = c.one("reference").split(" ")[0]
            self.assertTrue((rs.ROOT / path).exists(), f"{c.id}: reference {path!r} does not exist")

    def test_every_case_is_listed_in_the_readme(self):
        text = README.read_text(encoding="utf-8")
        missing = [c.id for c in all_cases() if c.id not in text]
        self.assertEqual(missing, [], "cases missing from corpus/README.md")

    def test_the_corpus_covers_every_declared_family(self):
        text = README.read_text(encoding="utf-8")
        families = {c.one("family") for c in all_cases()}
        for fam in ("traversal", "percent-encoding", "annotations", "http-framing", "resource-limits",
                    "parser-rejection", "parser-acceptance"):
            self.assertIn(fam, families, f"no case in family {fam}")
            self.assertIn(fam, text, f"family {fam} is not described in corpus/README.md")


if __name__ == "__main__":
    unittest.main()
