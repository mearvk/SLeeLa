#!/usr/bin/env python3
"""Regression tests for the Java API dependency graph auditor."""
from __future__ import annotations
import json
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
TOOL = REPO / "lib/java/tools/java_api_dependency_graph.py"

def put(root: Path, path: str, body: str) -> None:
    target = root / path
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(body, encoding="utf-8")

with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    put(root, "lib/java/java/lang/String.sleela", '#sleela 2.5\nclass String {}\n')
    put(root, "lib/java/java/util/Map.sleela", '#sleela 2.5\nclass Map {}\n')
    put(root, "lib/java/java/util/Map.Entry.sleela", '#sleela 2.5\ninterface Entry {}\n')
    put(root, "lib/java/java/util/List.sleela", '#sleela 2.5\ninterface List {}\n')
    put(root, "lib/java/java/lang/package-info.sleela", '@java.lang.Deprecated package java.lang;\n')
    put(root, "lib/java/early_access/jdk28/docs/api/java.base/module-info.sleela",
        'module java.base { requires java.logging; exports java.lang; }\n')
    put(root, "lib/java/early_access/jdk28/docs/api/java.base/java/lang/String.sleela",
        '#sleela 2.5\nclass String {}\n')
    put(root, "Example.sleela", '''#sleela 2.5
import java.lang.String;
import java.util.*;
import static java.util.Map.Entry.comparingByKey;
class Example {
  String text;
  String typeName = "java.lang.String";
  java.util.Map.Entry entry;
  // java.lang.DoesNotExist must not become a dependency
}
''')
    put(root, "lib/java/java/lang/Broken.sleela", '''#sleela 2.5
import java.lang.MissingType;
class Broken {}
''')
    out = root / "graph.json"
    p = subprocess.run([sys.executable, str(TOOL), "--root", str(root),
                        "--json", str(out)], capture_output=True, text=True)
    if p.returncode:
        raise SystemExit(p.stdout + p.stderr)
    graph = json.loads(out.read_text(encoding="utf-8"))
    assert graph["summary"]["indexed_types"] == 5, graph["summary"]
    assert graph["summary"]["duplicate_type_paths"] == 1, graph["summary"]
    assert graph["summary"]["wildcard_package_edges"] == 1, graph["summary"]
    assert any(x["reference"] == "java.lang.MissingType" for x in graph["unresolved"])
    assert not any(x["reference"] == "java.lang.DoesNotExist" for x in graph["unresolved"])
    assert any(x["to"] == "java.util.Map.Entry" for x in graph["edges"])
    assert any(x["kind"] == "static-import" for x in graph["edges"])
    nested = next(x for x in graph["transitive_dependencies"] if x["name"] == "java.util.Map.Entry")
    assert "java.util.Map" in nested["dependencies"], nested
    assert graph["modules"][0]["requires"] == ["java.logging"], graph["modules"]
    assert graph["modules"][0]["exports"] == ["java.lang"], graph["modules"]
    assert graph["package_metadata"][0]["name"] == "java.lang", graph["package_metadata"]
    check = subprocess.run([sys.executable, str(TOOL), "--root", str(root),
                            "--check"], capture_output=True, text=True)
    assert check.returncode == 1, (check.stdout, check.stderr)

print("java-api-dependency-graph-suite: PASS")
