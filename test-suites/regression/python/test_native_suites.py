"""Builds and runs every C and C++ regression suite.

Each suite is compiled with the project's strict audit flags (-Wall -Wextra
-Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror) and linked against the
instrumented allocator, so every case is also leak-checked and heap-guard
checked.  A suite passes only when it exits 0; KNOWN-ISSUE lines are reported
by the runner but do not fail it.
"""
import unittest

import regression_support as rs


@unittest.skipIf(rs.have_toolchain(), rs.have_toolchain() or "")
class NativeSuites(unittest.TestCase):
    maxDiff = None

    def test_every_suite_source_is_registered(self):
        """An orphaned test file would silently never run."""
        on_disk = {p.stem for sub in ("c", "cpp") for p in (rs.REG / sub).glob("reg_*.c*")
                   if p.stem != "reg_support"}
        self.assertEqual(on_disk, set(rs.SUITES), "regression_support.SUITES out of sync with c/ and cpp/")

    def test_every_library_source_exists(self):
        for name, path in rs.SOURCES.items():
            self.assertTrue(path.is_file(), f"{name}: {path} missing")


def _make(name: str):
    def test(self):
        exe = rs.build_suite(name)
        result = rs.run_binary(exe)
        for line in result.known_issues:
            issue_id = line.split()[1]
            rs.record_known_issue(issue_id, line, still_open=" open" in line, source=name)
        detail = "\n".join(result.failures[:25]) or result.stdout[-1500:]
        self.assertEqual(result.returncode, 0, f"{name} failed:\n{detail}")
        self.assertGreater(result.summary.get("checks", 0), 0, f"{name} executed no checks")
        self.assertEqual(result.summary.get("failed_checks"), 0)
        self.assertEqual(result.summary.get("failed_cases"), 0)
    test.__name__ = f"test_{name}"
    test.__doc__ = f"{rs.SUITES[name][0].upper()} suite {name}"
    return test


for _name in rs.SUITES:
    setattr(NativeSuites, f"test_{_name}", _make(_name))
if __name__ == "__main__":
    unittest.main()
