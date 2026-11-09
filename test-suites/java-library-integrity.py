#!/usr/bin/env python3
"""Focused regression tests for the JDK 28 signature catalog and Java inventory validator."""
from __future__ import annotations

import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
SIGNATURE_TOOL = REPO / "lib/java/tools/jdk28_signature_sync.py"
VALIDATOR = REPO / "lib/java/validate.py"


def load_signature_tool():
    spec = importlib.util.spec_from_file_location("jdk28_signature_sync", SIGNATURE_TOOL)
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load signature synchronizer")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class SignatureCatalogTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tool = load_signature_tool()

    def test_extracts_complete_nested_signatures_and_survives_void_tags(self):
        page = """
        <div class="member-signature"><span>public</span>
        &lt;T&gt; java.util.List&lt;T&gt; map(<span>java.util.function.Function&lt;T, R&gt;</span> f)
        throws java.io.IOException<br> </div>
        <div class="member-signature"><span>public</span> void close()</div>
        <main>unrelated page text must not be captured</main>
        """
        self.assertEqual(
            self.tool.extract_signatures(page),
            [
                "public <T> java.util.List<T> map(java.util.function.Function<T, R> f) throws java.io.IOException",
                "public void close()",
            ],
        )

    def test_index_maps_module_package_and_nested_type_path(self):
        index = '<a href="java.base/java/util/Map.Entry.html">Map.Entry</a>'
        self.assertEqual(
            self.tool.index_types(index),
            [("java.util.Map.Entry", "java.base/java/util/Map.Entry.html")],
        )

    def test_render_is_documentary_and_contains_no_fake_method_body(self):
        rendered = self.tool.render(["public int size()"], "java.base/java/util/List.html")
        self.assertIn("// JDK declaration: public int size()", rendered)
        self.assertNotIn("return 0", rendered)
        self.assertIn("not executable SLeeLa methods", rendered)

    def test_replace_section_is_idempotent(self):
        source = "class Example {\\n}\\n"
        section = self.tool.render(["public void run()"], "java.base/java/lang/Example.html")
        once = self.tool.replace_section(source, section)
        twice = self.tool.replace_section(once, section)
        self.assertEqual(once, twice)


class InventoryValidatorTests(unittest.TestCase):
    def _fixture(self, root: Path, count: int) -> None:
        lib = root / "lib/java"
        lib.mkdir(parents=True)
        controls = ["lib/java/MANIFEST.json", "lib/java/validate.py"]
        (lib / "validate.py").write_text("# fixture\\n", encoding="utf-8")
        (lib / "Api.sleela").write_text("#sleela 2.5\\n", encoding="utf-8")
        manifest = {
            "schema": "sleela-java-library-manifest-v1",
            "root": "/lib/java",
            "inventory": {
                "count": count,
                "count_type": "library-source-baseline",
                "control_files": controls,
            },
            "language_bridge": {
                "source_language": "SLeeLa",
                "parallel_runtime": "Java",
                "native_vm_foundation": ["C", "C++"],
            },
            "mapping": {
                "required_concepts": [
                    "java_owner", "sleela_symbol", "source_role",
                    "execution_boundary", "loader_visibility", "compatibility_surface",
                ],
            },
            "source_roles": ["binding"],
            "execution_boundaries": ["java-jvm"],
            "validation": {"validator": "lib/java/validate.py"},
        }
        (lib / "MANIFEST.json").write_text(json.dumps(manifest), encoding="utf-8")
        subprocess.run(["git", "init", "-q"], cwd=root, check=True)
        subprocess.run(["git", "add", "lib/java"], cwd=root, check=True)

    def test_exact_manifest_count_passes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            self._fixture(root, 1)
            result = subprocess.run(
                [sys.executable, str(VALIDATOR), "--root", str(root)],
                text=True, capture_output=True,
            )
            self.assertEqual(result.returncode, 0, result.stderr + result.stdout)

    def test_inventory_count_mismatch_fails_in_both_directions(self):
        for recorded in (0, 2):
            with self.subTest(recorded=recorded), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                self._fixture(root, recorded)
                result = subprocess.run(
                    [sys.executable, str(VALIDATOR), "--root", str(root)],
                    text=True, capture_output=True,
                )
                self.assertEqual(result.returncode, 1)
                self.assertIn("exact inventory mismatch", result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
