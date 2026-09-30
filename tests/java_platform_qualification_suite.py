#!/usr/bin/env python3
import json
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
matrix = ROOT / "tests/java_platforms.json"
tool = ROOT / "tests/java_platform_qualification.py"

def main():
    data = json.loads(matrix.read_text(encoding="utf-8"))
    assert data["schemaVersion"] == 1
    assert set(data["platforms"]) == {"linux", "windows-10-plus", "macos"}

    for name, spec in data["platforms"].items():
        assert spec["architectures"]
        assert spec["nativeToolchain"]
        assert spec["requiredTools"]
        assert len(spec["qualificationCommands"]) >= 4

        with tempfile.TemporaryDirectory() as td:
            report = Path(td) / f"{name}.json"
            result = subprocess.run(
                ["python3", str(tool), "--platform", name, "--json", str(report)],
                text=True, capture_output=True
            )
            assert result.returncode == 0, result.stdout + result.stderr
            observed = json.loads(report.read_text(encoding="utf-8"))
            assert observed["targetPlatform"] == name
            assert observed["schemaVersion"] == 1
            assert observed["scope"] == data["scope"]
            assert observed["declaredTarget"] == spec
            assert observed["qualificationState"] in {"READY", "NOT_EXECUTED"}

    print("java-platform-qualification-suite: PASS")

if __name__ == "__main__":
    main()
