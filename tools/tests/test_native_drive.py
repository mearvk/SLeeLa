#!/usr/bin/env python3
"""Smoke and integration tests for the SLeeLa Native Drive."""
import ctypes
import pathlib
import shutil
import subprocess
import sys
import tempfile
import unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "sleela-native-drive.py"
SERVICE = SCRIPT.parents[0] / "sleela-native-service.py"

def executable_path(path):
    return path.with_suffix(path.suffix + ".exe") if sys.platform == "win32" and not path.suffix else path

def c_compiler():
    return shutil.which("cc") or shutil.which("gcc") or shutil.which("clang") or (shutil.which("cl") if sys.platform == "win32" else None)

def cpp_compiler():
    return shutil.which("c++") or shutil.which("g++") or shutil.which("clang++") or (shutil.which("cl") if sys.platform == "win32" else None)

class NativeDriveTests(unittest.TestCase):
    def test_c_and_cpp_language_detection(self):
        for name in ("unit.c", "unit.cpp", "unit.cc", "unit.cxx"):
            result = subprocess.run([sys.executable, str(SCRIPT), "build", name, "-o", "out"],
                                    text=True, capture_output=True)
            self.assertIn("source file does not exist", result.stderr)

    def test_unknown_extension_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = pathlib.Path(tmp) / "unit.txt"
            src.write_text("not a supported source")
            result = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(pathlib.Path(tmp) / "out")],
                                    text=True, capture_output=True)
            self.assertIn("cannot infer source language", result.stderr)

    def test_compile_and_run_c_source(self):
        compiler = c_compiler()
        if not compiler: self.skipTest("no C compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            src, out = root / "hello.c", executable_path(root / "hello")
            src.write_text('#include <stdio.h>\nint main(void) { puts("native-c-ok"); return 0; }\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                                    "--compiler", compiler], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            ran = subprocess.run([str(out)], text=True, capture_output=True)
            self.assertEqual(ran.returncode, 0)
            self.assertIn("native-c-ok", ran.stdout)

    def test_compile_and_run_cpp_source(self):
        compiler = cpp_compiler()
        if not compiler: self.skipTest("no C++ compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            src, out = root / "hello.cpp", executable_path(root / "hello")
            src.write_text('#include <iostream>\nint main() { std::cout << "native-cpp-ok\\n"; }\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                                    "--compiler", compiler], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            ran = subprocess.run([str(out)], text=True, capture_output=True)
            self.assertEqual(ran.returncode, 0)
            self.assertIn("native-cpp-ok", ran.stdout)

    def test_build_native_shared_library_and_validate_abi(self):
        if sys.platform == "win32": self.skipTest("POSIX shared-library fixture")
        compiler = c_compiler()
        if not compiler: self.skipTest("no C compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            src = root / "plugin.c"
            suffix = ".dylib" if sys.platform == "darwin" else ".so"
            out = root / ("plugin" + suffix)
            src.write_text('#include <stdint.h>\n'
                '#if defined(_WIN32)\n#define EXPORT __declspec(dllexport)\n'
                '#else\n#define EXPORT __attribute__((visibility("default")))\n#endif\n'
                'EXPORT uint32_t sl_native_abi_version(void) { return 1u; }\n'
                'EXPORT int sl_native_run(int argc, const char *const argv[]) { (void)argv; return argc; }\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                "--kind", "shared", "--require-sleela-abi", "--compiler", compiler],
                text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            module = ctypes.CDLL(str(out))
            module.sl_native_abi_version.restype = ctypes.c_uint32
            self.assertEqual(module.sl_native_abi_version(), 1)

    def test_multi_unit_c_build(self):
        compiler = c_compiler()
        if not compiler: self.skipTest("no C compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "main.c").write_text('#include <stdio.h>\nint answer(void);\nint main(void) { printf("%d\\n", answer()); return 0; }\n')
            (root / "answer.c").write_text('int answer(void) { return 42; }\n')
            out = executable_path(root / "multi")
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(root / "main.c"),
                str(root / "answer.c"), "-o", str(out), "--compiler", compiler],
                text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            ran = subprocess.run([str(out)], text=True, capture_output=True)
            self.assertEqual(ran.returncode, 0)
            self.assertEqual(ran.stdout.strip(), "42")

    def test_missing_native_abi_export_is_rejected_before_publish(self):
        if sys.platform == "win32": self.skipTest("POSIX shared-library fixture")
        compiler = c_compiler()
        if not compiler: self.skipTest("no C compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            src = root / "wrong_abi.c"
            suffix = ".dylib" if sys.platform == "darwin" else ".so"
            out = root / ("wrong_abi" + suffix)
            src.write_text('#include <stdint.h>\n'
                '__attribute__((visibility("default"))) uint32_t sl_native_abi_version(void) { return 1u; }\n')
            result = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                "--kind", "shared", "--require-sleela-abi", "--compiler", compiler],
                text=True, capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("missing required SLeeLa ABI exports", result.stderr)
            self.assertFalse(out.exists())

    def test_systemd_unit_generation_is_unprivileged_and_hardened(self):
        if sys.platform == "win32": self.skipTest("systemd unit generation requires POSIX executable semantics")
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            exe = root / "worker"
            exe.write_text("#!/bin/sh\nexit 0\n")
            exe.chmod(0o755)
            result = subprocess.run([sys.executable, str(SERVICE), "--name", "sleela-worker",
                "--executable", str(exe), "--working-directory", str(root)],
                text=True, capture_output=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn("NoNewPrivileges=yes", result.stdout)
            self.assertIn("ProtectSystem=strict", result.stdout)
            self.assertIn("ExecStart=", result.stdout)
            self.assertNotIn("systemctl", result.stdout)

    def test_systemd_unit_rejects_unsafe_unit_names(self):
        if sys.platform == "win32": self.skipTest("systemd unit generation requires POSIX executable semantics")
        with tempfile.TemporaryDirectory() as tmp:
            exe = pathlib.Path(tmp) / "worker"
            exe.write_text("#!/bin/sh\nexit 0\n")
            exe.chmod(0o755)
            result = subprocess.run([sys.executable, str(SERVICE), "--name", "../bad",
                "--executable", str(exe), "--working-directory", tmp],
                text=True, capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("service name", result.stderr)

    def test_system_unit_rejects_root_account(self):
        if sys.platform == "win32": self.skipTest("systemd is Linux-specific")
        with tempfile.TemporaryDirectory() as tmp:
            exe = pathlib.Path(tmp) / "worker"
            exe.write_text("#!/bin/sh\nexit 0\n")
            exe.chmod(0o755)
            result = subprocess.run([sys.executable, str(SERVICE), "--name", "root-worker",
                "--executable", str(exe), "--working-directory", tmp,
                "--system", "--install", "--user", "root"],
                text=True, capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("must run as an unprivileged account", result.stderr)

    def test_run_capture_enforces_combined_output_limit(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "run", "--capture",
            "--max-output-bytes", "1024", "--", sys.executable, "-c",
            "import sys; sys.stdout.write('x'*1000000); sys.stderr.write('y'*1000000)"],
            text=False, capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 6, result.stderr.decode(errors="replace"))
        self.assertLessEqual(len(result.stdout) + len(result.stderr), 1024 + 256)
        self.assertIn(b"exceeded --max-output-bytes", result.stderr)

    def test_pipeline_enforces_output_limit_and_terminates_children(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "pipe", "--timeout", "5",
            "--max-output-bytes", "2048", "--", sys.executable, "-c",
            "import sys; sys.stdout.write('z'*1000000); sys.stdout.flush()", "::",
            sys.executable, "-c", "import sys; sys.stdout.write(sys.stdin.read())"],
            text=False, capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 6, result.stderr.decode(errors="replace"))
        self.assertLessEqual(len(result.stdout) + len(result.stderr), 2048 + 256)
        self.assertIn(b"exceeded --max-output-bytes", result.stderr)

    def test_process_pipeline(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "pipe", "--timeout", "5", "--",
            sys.executable, "-c", "print('sleela-pipe')", "::",
            sys.executable, "-c", "import sys; print(sys.stdin.read().upper(), end='')"],
            text=True, capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("SLEELA-PIPE", result.stdout)

if __name__ == "__main__":
    unittest.main()
