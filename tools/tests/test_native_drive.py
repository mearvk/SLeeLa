#!/usr/bin/env python3
"""Smoke tests for the Native Drive CLI."""
import pathlib, shutil, subprocess, sys, tempfile, unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "sleela-native-drive.py"

class NativeDriveTests(unittest.TestCase):
    def test_c_and_cpp_language_detection(self):
        for name in ("unit.c", "unit.cpp", "unit.cc", "unit.cxx"):
            result = subprocess.run([sys.executable, str(SCRIPT), "build", name, "-o", "out"],
                                    text=True, capture_output=True)
            self.assertIn("source file does not exist", result.stderr)

    def test_unknown_extension_is_rejected(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "build", "unit.txt", "-o", "out"],
                                text=True, capture_output=True)
        self.assertIn("cannot infer source language", result.stderr)

    def test_compile_and_run_c_source(self):
        compiler = shutil.which("cc") or shutil.which("gcc") or shutil.which("clang")
        if not compiler: self.skipTest("no C compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            src, out = pathlib.Path(tmp) / "hello.c", pathlib.Path(tmp) / "hello"
            src.write_text('#include <stdio.h>\nint main(void) { puts("native-c-ok"); return 0; }\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                                    "--compiler", compiler], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            ran = subprocess.run([str(out)], text=True, capture_output=True)
            self.assertEqual(ran.returncode, 0)
            self.assertIn("native-c-ok", ran.stdout)

    def test_compile_and_run_cpp_source(self):
        compiler = shutil.which("c++") or shutil.which("g++") or shutil.which("clang++")
        if not compiler: self.skipTest("no C++ compiler installed")
        with tempfile.TemporaryDirectory() as tmp:
            src, out = pathlib.Path(tmp) / "hello.cpp", pathlib.Path(tmp) / "hello"
            src.write_text('#include <iostream>\nint main() { std::cout << "native-cpp-ok\\n"; }\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                                    "--compiler", compiler], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            ran = subprocess.run([str(out)], text=True, capture_output=True)
            self.assertEqual(ran.returncode, 0)
            self.assertIn("native-cpp-ok", ran.stdout)

    def test_build_native_shared_library(self):
        if sys.platform == "win32": self.skipTest("shared-library fixture requires a POSIX compiler")
        compiler = shutil.which("cc") or shutil.which("gcc") or shutil.which("clang")
        if not compiler: self.skipTest("no C compiler installed")
        import ctypes
        with tempfile.TemporaryDirectory() as tmp:
            src = pathlib.Path(tmp) / "plugin.c"
            suffix = ".dylib" if sys.platform == "darwin" else ".so"
            out = pathlib.Path(tmp) / ("plugin" + suffix)
            src.write_text('#include <stdint.h>\\n'
                '#if defined(_WIN32)\\n#define EXPORT __declspec(dllexport)\\n'
                '#else\\n#define EXPORT __attribute__((visibility("default")))\\n#endif\\n'
                'EXPORT uint32_t sl_native_abi_version(void) { return 1u; }\\n'
                'EXPORT int sl_native_run(int argc, const char *const argv[]) { (void)argv; return argc; }\\n')
            built = subprocess.run([sys.executable, str(SCRIPT), "build", str(src), "-o", str(out),
                "--kind", "shared", "--compiler", compiler], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            module = ctypes.CDLL(str(out))
            module.sl_native_abi_version.restype = ctypes.c_uint32
            self.assertEqual(module.sl_native_abi_version(), 1)

    def test_process_pipeline(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "pipe", "--timeout", "5", "--",
            sys.executable, "-c", "print('sleela-pipe')", "::",
            sys.executable, "-c", "import sys; print(sys.stdin.read().upper(), end='')"],
            text=True, capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("SLEELA-PIPE", result.stdout)

if __name__ == "__main__": unittest.main()
