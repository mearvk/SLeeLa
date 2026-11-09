#!/usr/bin/env python3
"""Build and validate the Java API counterpart dependency graph.

The scanner is deliberately static and conservative: it indexes Java API
counterpart files, resolves explicit/static imports and fully-qualified
references, records wildcard package dependencies separately, and emits
stable JSON. It does not claim that a counterpart's members are behaviorally
equivalent or that a wildcard import proves package completeness.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

JAVA_ROOTS = ("java", "javax", "jdk", "sun", "com.sun")
IDENT = r"[A-Za-z_$][\w$]*"
QNAME = rf"{IDENT}(?:\.{IDENT})+"
IMPORT_RE = re.compile(rf"(?m)^\s*import\s+(?P<static>static\s+)?(?P<name>{QNAME})(?P<wild>\.\*)?\s*;")
PACKAGE_RE = re.compile(rf"(?m)^\s*package\s+({QNAME})\s*;")
FQ_RE = re.compile(rf"\b(?:{'|'.join(JAVA_ROOTS)})\.(?:{IDENT}\.)*{IDENT}(?:\.{IDENT})*\b")
DECL_RE = re.compile(rf"\b(?:class|interface|enum|record)\s+({IDENT})")
COMMENT_RE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/', re.S)

def strip_comments(text: str) -> str:
    # Keep quoted metadata strings: Java API type names are often recorded there.
    def replace(match: re.Match[str]) -> str:
        token = match.group(0)
        if token.startswith(('"', "'")):
            return token
        return re.sub(r"[^\n]", " ", token)
    return COMMENT_RE.sub(replace, text)

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[3])
    ap.add_argument("--json", type=Path, help="write deterministic graph JSON")
    ap.add_argument("--check", action="store_true", help="fail on unresolved explicit/FQ references")
    ap.add_argument("--strict", action="store_true", help="also fail on unresolved simple imported type references")
    args = ap.parse_args()
    repo = args.root.resolve()
    api_root = repo / "lib" / "java"
    if not api_root.is_dir():
        print(f"ERROR: Java API root not found: {api_root}", file=sys.stderr)
        return 2

    files = sorted(api_root.rglob("*.sleela"))
    index: dict[str, str] = {}
    package_index: dict[str, list[str]] = defaultdict(list)
    declarations: dict[str, list[str]] = defaultdict(list)
    for path in files:
        qname = qname_from_path(path, api_root)
        if not qname:
            continue
        rel = path.relative_to(repo).as_posix()
        # Multiple release/source roots may represent the same Java type.
        index.setdefault(qname, rel)
        package_index[qname.rpartition(".")[0]].append(qname)
        declarations[qname].append(rel)

    nodes = []
    edges = []
    unresolved = []
    wildcard_edges = []
    for qname, rel in sorted(index.items()):
        path = repo / rel
        raw = path.read_text(encoding="utf-8", errors="replace")
        source = strip_comments(raw)
        package_match = PACKAGE_RE.search(source)
        declared_package = package_match.group(1) if package_match else qname.rpartition(".")[0]
        imports = []
        local_edges = {}
        for match in IMPORT_RE.finditer(source):
            name = match.group("name")
            is_static = bool(match.group("static"))
            wildcard = bool(match.group("wild"))
            # Static member imports depend on the declaring owner type.
            owner = name if is_static else name
            if wildcard:
                wildcard_edges.append({"from": qname, "package": owner, "kind": "static-wildcard" if is_static else "package-wildcard", "line": line_of(source, match.start())})
                imports.append({"name": owner + ".*", "kind": "static-wildcard" if is_static else "package-wildcard", "line": line_of(source, match.start())})
                continue
            target = owner
            if is_static:
                # A static import may name a member or nested type. Resolve the
                # longest indexed type prefix, preserving the owner dependency.
                candidates = [x for x in index if target == x or target.startswith(x + ".")]
                target = max(candidates, key=len) if candidates else target
            imports.append({"name": name, "kind": "static" if is_static else "explicit", "line": line_of(source, match.start())})
            if target in index and target != qname:
                local_edges[(target, "static-import" if is_static else "import")] = line_of(source, match.start())
            elif target not in index and java_namespace(target):
                unresolved.append({"from": qname, "reference": name, "kind": "static-import" if is_static else "import", "line": line_of(source, match.start()), "source": rel})

        # Resolve fully-qualified references, including nested type names, to
        # the longest indexed owner. Keep only type-level dependencies.
        for match in FQ_RE.finditer(source):
            ref = match.group(0)
            candidates = [x for x in index if ref == x or ref.startswith(x + ".")]
            if candidates:
                target = max(candidates, key=len)
                if target != qname:
                    local_edges.setdefault((target, "qualified-reference"), line_of(source, match.start()))
            elif java_namespace(ref):
                unresolved.append({"from": qname, "reference": ref, "kind": "qualified-reference", "line": line_of(source, match.start()), "source": rel})

        # Resolve simple names only when they are explicitly imported or are
        # in the same package. This avoids treating arbitrary identifiers as
        # JDK types based on spelling alone.
        explicit = {Path(x["name"].replace(".", "/")).name: x for x in imports if x["kind"] in ("explicit", "static")}
        simple_names = set(re.findall(rf"\b{IDENT}\b", source))
        for simple in sorted(simple_names & set(explicit)):
            imp = explicit[simple]
            target_name = imp["name"]
            candidates = [x for x in index if target_name == x or target_name.startswith(x + ".")]
            if candidates:
                target = max(candidates, key=len)
                if target != qname:
                    local_edges.setdefault((target, "imported-type-use"), imp["line"])
        for simple in sorted(simple_names):
            candidate = declared_package + "." + simple
            if candidate in index and candidate != qname:
                local_edges.setdefault((candidate, "same-package-type-use"), 1)

        for (target, kind), line in sorted(local_edges.items()):
            edges.append({"from": qname, "to": target, "kind": kind, "line": line, "source": rel})
        nodes.append({
            "name": qname,
            "path": rel,
            "package": declared_package,
            "declarations": sorted(set(re.findall(DECL_RE, source))),
            "duplicate_paths": sorted(declarations[qname]),
            "imports": imports,
            "dependency_count": len(local_edges),
        })

    # Duplicate counterpart paths are surfaced separately: multiple release
    # trees may be intentional, but ambiguity must never be hidden.
    duplicates = [{"name": name, "paths": paths} for name, paths in sorted(declarations.items()) if len(set(paths)) > 1]
    missing = sorted({(x["from"], x["reference"], x["kind"], x["line"], x["source"]): x for x in unresolved}.values(),
                     key=lambda x: (x["from"], x["line"], x["reference"], x["kind"]))
    graph = {
        "schema": "sleela-java-api-dependency-graph-v1",
        "scope": "static Java API counterpart type dependencies; not behavioral equivalence",
        "root": "lib/java",
        "summary": {
            "source_files": len(files),
            "indexed_types": len(index),
            "dependency_edges": len(edges),
            "wildcard_package_edges": len(wildcard_edges),
            "unresolved_references": len(missing),
            "duplicate_type_paths": len(duplicates),
            "packages": len(package_index),
        },
        "nodes": nodes,
        "edges": sorted(edges, key=lambda x: (x["from"], x["to"], x["kind"], x["line"])),
        "wildcard_dependencies": sorted(wildcard_edges, key=lambda x: (x["from"], x["package"], x["kind"], x["line"])),
        "unresolved": missing,
        "duplicates": duplicates,
    }
    encoded = json.dumps(graph, indent=2, sort_keys=True) + "\n"
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(encoded, encoding="utf-8")
    print("Java API dependency graph")
    for key, value in graph["summary"].items():
        print(f"{key}: {value}")
    print(f"graph: {args.json if args.json else '(not written)'}")
    failures = bool(missing) or (args.strict and bool(duplicates))
    if args.check and failures:
        print("dependency-closure: FAIL")
        return 1
    print("dependency-closure: " + ("REVIEW REQUIRED" if missing or duplicates else "PASS"))
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
