#!/usr/bin/env python3
"""Smoke tests for the Native Drive CLI."""
import pathlib, subprocess, sys, unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "sleela-native-drive.py"

class NativeDriveTests(unittest.TestCase):
    def test_c_and_cpp_language_detection(self):
        # Exercise the public CLI's inference path with a missing input file.
        for name in ("unit.c", "unit.cpp", "unit.cc", "unit.cxx"):
            result = subprocess.run([sys.executable, str(SCRIPT), "build", name, "-o", "out"],
                                    text=True, capture_output=True)
            self.assertIn("source file does not exist", result.stderr)

    def test_unknown_extension_is_rejected(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "build", "unit.txt", "-o", "out"],
                                text=True, capture_output=True)
        self.assertIn("cannot infer source language", result.stderr)

    def test_process_pipeline(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "pipe", "--timeout", "5", "--",
            sys.executable, "-c", "print('sleela-pipe')", "::",
            sys.executable, "-c", "import sys; print(sys.stdin.read().upper(), end='')"],
            text=True, capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("SLEELA-PIPE", result.stdout)

if __name__ == "__main__": unittest.main()
