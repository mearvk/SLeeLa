#!/usr/bin/env python3
"""Regression suite for member-level Java API dependency extraction."""
import json
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOOL = ROOT / "lib/java/tools/java_api_member_dependencies.py"

def put(root, path, body):
    target = root / path
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(body, encoding="utf-8")

with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    put(root, "lib/java/java/lang/String.sleela", "class String {}\n")
    put(root, "lib/java/java/lang/Number.sleela", "class Number {}\n")
    put(root, "lib/java/java/lang/Comparable.sleela", "interface Comparable {}\n")
    put(root, "lib/java/java/lang/Deprecated.sleela", "class Deprecated {}\n")
    put(root, "lib/java/java/lang/Iterable.sleela", "interface Iterable {}\n")
    put(root, "lib/java/java/util/List.sleela", "interface List {}\n")
    put(root, "lib/java/java/io/IOException.sleela", "class IOException {}\n")
    put(root, "lib/java/java/util/GenericBox.sleela", '''package java.util;
import java.lang.String;
import java.io.IOException;
import java.lang.Deprecated;
class GenericBox<T extends Number & Comparable<T>> implements Iterable<T> {
  List<String> values;
  GenericBox(String name) {}
  @Deprecated
  <R extends Number> R convert(List<String> input) throws IOException { return null; }
}
''')
    out = root / "member-graph.json"
    p = subprocess.run([sys.executable, str(TOOL), "--root", str(root),
                        "--json", str(out)], capture_output=True, text=True)
    if p.returncode:
        raise SystemExit(p.stdout + p.stderr)
    graph = json.loads(out.read_text(encoding="utf-8"))
    assert graph["summary"]["indexed_types"] == 8, graph["summary"]
    edges = graph["edges"]
    assert any(e["member"] == "values" and e["role"] == "field-type" and e["to"] == "java.util.List" for e in edges), edges
    assert any(e["member"] == "convert" and e["role"] == "generic-bound" and e["to"] == "java.lang.Number" for e in edges), edges
    assert any(e["member"] == "convert" and e["role"] == "parameter" and e["to"] == "java.util.List" for e in edges), edges
    assert any(e["member"] == "convert" and e["role"] == "throws" and e["to"] == "java.io.IOException" for e in edges), edges
    assert any(e["member"] == "GenericBox" and e["role"] == "generic-bound" and e["to"] == "java.lang.Comparable" for e in edges), edges
    assert any(e["member"] == "convert" and e["role"] == "annotation" and e["to"] == "java.lang.Deprecated" for e in edges), edges

    put(root, "lib/java/java/util/Broken.sleela", '''package java.util;
import java.io.MissingIOException;
class Broken {
  void fail() throws MissingIOException {}
}
''')
    broken = subprocess.run([sys.executable, str(TOOL), "--root", str(root),
                             "--check"], capture_output=True, text=True)
    assert broken.returncode == 1, (broken.stdout, broken.stderr)
    assert "java.io.MissingIOException" in broken.stdout, broken.stdout

print("java-api-member-dependencies-suite: PASS")
