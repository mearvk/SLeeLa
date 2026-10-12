import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "sleelavirin_verify_signature_test", ROOT / "tools" / "sleelavirin_verify_signature.py"
)
verifier = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verifier)


class DetachedSignatureTests(unittest.TestCase):
    def test_missing_inputs_fail_closed(self):
        with tempfile.TemporaryDirectory() as temp:
            missing = Path(temp) / "missing.pem"
            with mock.patch("sys.argv", ["verify", "--public-key", str(missing), "--signature", str(missing), "--file", str(missing)]):
                self.assertEqual(verifier.main(), 2)

    @mock.patch.object(verifier.subprocess, "run")
    @mock.patch.object(verifier.shutil, "which", return_value="/usr/bin/openssl")
    def test_valid_signature_requires_verified_ok(self, which, run):
        run.return_value.returncode = 0
        run.return_value.stdout = "Verified OK\n"
        run.return_value.stderr = ""
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            key, sig, artifact = root / "key.pem", root / "file.sig", root / "index.txt"
            key.write_text("key")
            sig.write_text("sig")
            artifact.write_text("content")
            with mock.patch("sys.argv", ["verify", "--public-key", str(key), "--signature", str(sig), "--file", str(artifact)]):
                self.assertEqual(verifier.main(), 0)

    @mock.patch.object(verifier.subprocess, "run")
    @mock.patch.object(verifier.shutil, "which", return_value="/usr/bin/openssl")
    def test_invalid_signature_fails(self, which, run):
        run.return_value.returncode = 1
        run.return_value.stdout = ""
        run.return_value.stderr = "Verification failure"
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            key, sig, artifact = root / "key.pem", root / "file.sig", root / "index.txt"
            key.write_text("key")
            sig.write_text("sig")
            artifact.write_text("content")
            with mock.patch("sys.argv", ["verify", "--public-key", str(key), "--signature", str(sig), "--file", str(artifact)]):
                self.assertEqual(verifier.main(), 1)


if __name__ == "__main__":
    unittest.main()
