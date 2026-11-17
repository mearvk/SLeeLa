"""The documents that state the current version must agree with the compiler.

The source of truth is the compiler: the release string in impl/frontend/driver.cpp and
the supported syntax range printed by `sleela version`.  The documents below are the
ones that describe the CURRENT state (not history); each must carry the current release
and range, and none may still state the previous ceiling as the current one.  Historical
notes (markdown/VERSION.md increments, api/sleela/3.0, impl/backup) are deliberately not listed.

When the range moves, `python3 test-suites/regression/python/run_regression.py --only integrity -k doc_versions`
names every document that still needs updating.
"""
import re
import subprocess
import unittest

import regression_support as rs

R = rs.ROOT

# Documents that state the current compiler release and/or the supported syntax range.
RANGE_DOCS = [
    "README.md", "working-binaries/README.md", "markdown/COMPILER.md", "markdown/LANGUAGE.md",
    "markdown/LANGUAGE.SPEC.md", "markdown/RUNTIME.md", "impl/README.md",
    "impl/nordshrift/README.md", "impl/nordshrift/NORDSHRIFT.md", "lib/compiler/README.md",
    "lib/nordshrift/README.md", "lib/vm/README.md", "decompiler/README.md", "decompiler/API.md",
    "docs/README.md", "api/sleela/2.1/language/README.md", "api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md",
    "munction/MANUAL.md", "pi-generator/README.md", "ballum/example/README.md",
    "artificial-intelligence/large-language-models/sleela/README.md",
    "install/sleela-clean-install.sh", "install/sleela-clean-install-macos.sh",
    ".kiro/steering/sleela-toolchain.md",
]
RELEASE_DOCS = ["working-binaries/README.md", "markdown/COMPILER.md", "markdown/VERSION.md",
                "man/man1/sleela.1", "install/sleela-clean-install.sh", "install/sleela-clean-install-macos.sh",
                ".kiro/steering/sleela-toolchain.md"]

SEP = r"\s*(?:–|-|\.\.|through|to)\s*"


def read(rel: str) -> str:
    return (R / rel).read_text(encoding="utf-8")


def source_release() -> str:
    m = re.search(r'kReleaseVersion\s*=\s*"([^"]+)"', read("impl/frontend/driver.cpp"))
    assert m, "kReleaseVersion not found in impl/frontend/driver.cpp"
    return m.group(1)


def source_range() -> tuple:
    m = re.search(r"range the toolchain drives:\s*(\d+\.\d+)\s*\.\.\s*(\d+\.\d+)", read("impl/frontend/version.h"))
    assert m, "the supported range comment is missing from impl/frontend/version.h"
    return m.group(1), m.group(2)


class DocVersions(unittest.TestCase):
    def test_binary_reports_the_release_and_range_in_the_source(self):
        binary = rs.sleela_binary()
        if binary is None:
            self.skipTest("no sleela binary")
        out = subprocess.run([str(binary), "version"], capture_output=True, text=True, env=rs.cli_env()).stdout
        low, high = source_range()
        self.assertIn(source_release(), out, "the binary under test reports a different release than the source")
        self.assertRegex(out, rf"{re.escape(low)}{SEP}{re.escape(high)}",
                         "the binary under test reports a different syntax range than version.h")

    def test_documents_state_the_current_range(self):
        low, high = source_range()
        want = re.compile(re.escape(low) + SEP + re.escape(high))
        stale = []
        for rel in RANGE_DOCS:
            text = read(rel)
            if not want.search(text):
                stale.append(f"{rel}: does not state the range {low} .. {high}")
        self.assertEqual(stale, [], "documents out of date")

    def test_no_document_still_states_the_previous_ceiling_as_current(self):
        low, high = source_range()
        major, minor = high.split(".")
        previous = f"{major}.{int(minor) - 1}"
        if int(minor) == 0:
            self.skipTest("no previous minor")
        old = re.compile(re.escape(low) + SEP + re.escape(previous) + r"(?!\d)")
        offenders = [rel for rel in RANGE_DOCS if old.search(read(rel))]
        self.assertEqual(offenders, [], f"documents that still give {low} .. {previous} as the range")

    def test_documents_state_the_current_release(self):
        release = source_release()
        missing = [rel for rel in RELEASE_DOCS if release not in read(rel)]
        self.assertEqual(missing, [], f"documents that do not mention release {release}")

    def test_installers_use_the_current_values(self):
        release = source_release()
        low, high = source_range()
        for rel in ("install/sleela-clean-install.sh", "install/sleela-clean-install-macos.sh"):
            text = read(rel)
            self.assertIn(f'RELEASE="{release}"', text, rel)
            self.assertIn(f'SYNTAX_RANGE="{low} - {high}"', text, rel)


if __name__ == "__main__":
    unittest.main()
