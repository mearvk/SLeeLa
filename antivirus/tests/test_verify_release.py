import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "release_verifier_test", ROOT / "tools" / "sleelavirin_verify_release.py"
)
verifier = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verifier)


class SignedReleaseTests(unittest.TestCase):
    def invoke(self, root, data, signature_ok=True):
        manifest = root / "release.json"
        manifest.write_text(json.dumps(data), encoding="utf-8")
        key, signature = root / "trusted.pem", root / "release.sig"
        key.write_text("test key", encoding="utf-8")
        signature.write_text("test signature", encoding="utf-8")
        result = mock.Mock(
            returncode=0 if signature_ok else 1,
            stdout="Verified OK" if signature_ok else "",
            stderr="",
        )
        with mock.patch("sys.argv", [
            "verify-release", "--public-key", str(key), "--signature", str(signature),
            "--manifest", str(manifest), "--root", str(root),
        ]), mock.patch.object(verifier.shutil, "which", return_value="/usr/bin/openssl"), mock.patch.object(
            verifier.subprocess, "run", return_value=result
        ):
            return verifier.main()

    def test_valid_signed_manifest_checks_artifact_hash_and_size(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            artifact = root / "package.zip"
            artifact.write_bytes(b"synthetic package fixture")
            data = {
                "schema": verifier.SCHEMA,
                "artifacts": [{
                    "path": artifact.name,
                    "size": artifact.stat().st_size,
                    "sha256": hashlib.sha256(artifact.read_bytes()).hexdigest(),
                }],
            }
            self.assertEqual(self.invoke(root, data), 0)
            artifact.write_bytes(b"tampered package")
            self.assertEqual(self.invoke(root, data), 1)

    def test_invalid_signature_fails_before_artifact_acceptance(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data = {"schema": verifier.SCHEMA, "artifacts": [{"path": "missing.zip", "size": 0, "sha256": "0" * 64}]}
            self.assertEqual(self.invoke(root, data, signature_ok=False), 1)

    def test_noncanonical_path_fails_closed(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data = {"schema": verifier.SCHEMA, "artifacts": [{"path": "../outside.zip", "size": 0, "sha256": "0" * 64}]}
            self.assertEqual(self.invoke(root, data), 1)


if __name__ == "__main__":
    unittest.main()
