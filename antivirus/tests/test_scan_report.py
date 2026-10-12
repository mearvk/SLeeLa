import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "sleelavirin_scan_report_test", ROOT / "tools" / "sleelavirin_scan.py"
)
scanner = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(scanner)


class ScanReportTests(unittest.TestCase):
    def test_terminal_control_characters_are_escaped(self):
        rendered = scanner.safe_console("name\x1b[31m\nforged")
        self.assertNotIn(chr(27), rendered)
        self.assertNotIn("\\n", rendered)

    def run_scan(self, result_code):
        temp = tempfile.TemporaryDirectory()
        self.addCleanup(temp.cleanup)
        root = Path(temp.name)
        sample = root / "sample.bin"
        sample.write_bytes(b"benign test fixture")
        executable = root / "native-scanner"
        executable.write_text("test stub")
        database = root / "index.txt"
        database.write_text("")
        report = root / "report.json"
        fake = mock.Mock(returncode=result_code, stdout="", stderr="")
        argv = ["scan", "--engine", "native", "--native-scanner", str(executable),
                "--database", str(database), "--report-json", str(report), str(sample)]
        with mock.patch("sys.argv", argv), mock.patch.object(scanner.subprocess, "run", return_value=fake):
            code = scanner.main()
        return code, json.loads(report.read_text(encoding="utf-8"))

    def test_no_match_report_is_explicit_and_complete(self):
        code, report = self.run_scan(0)
        self.assertEqual(code, 0)
        self.assertEqual(report["status"], "CLEAN_NO_MATCH")
        self.assertTrue(report["complete"])
        self.assertEqual(report["eligible_file_count"], 1)

    def test_engine_error_is_not_reported_clean(self):
        code, report = self.run_scan(2)
        self.assertEqual(code, 2)
        self.assertEqual(report["status"], "INCOMPLETE")
        self.assertFalse(report["complete"])
        self.assertEqual(report["error_count"], 1)


if __name__ == "__main__":
    unittest.main()
