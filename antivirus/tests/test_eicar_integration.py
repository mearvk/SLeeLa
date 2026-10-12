"""Optional EICAR plumbing test; skips when ClamAV/signatures are unavailable."""
import shutil
import subprocess
import tempfile
from pathlib import Path
import unittest


EICAR = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$" + "EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"


class EicarIntegrationTests(unittest.TestCase):
    def test_clamav_detects_harmless_eicar_test_pattern(self):
        clamscan = shutil.which("clamscan")
        if not clamscan:
            self.skipTest("clamscan is not installed")
        with tempfile.TemporaryDirectory() as temp:
            fixture = Path(temp) / "eicar-test.com"
            fixture.write_text(EICAR, encoding="ascii")
            result = subprocess.run(
                [clamscan, "--infected", "--no-summary", "--", str(fixture)],
                capture_output=True, text=True, timeout=30, check=False,
            )
            if result.returncode == 2:
                self.skipTest("ClamAV signature database is unavailable or could not be loaded")
            self.assertEqual(result.returncode, 1, "ClamAV should detect the EICAR test pattern")
            self.assertIn("FOUND", result.stdout)


if __name__ == "__main__":
    unittest.main()
