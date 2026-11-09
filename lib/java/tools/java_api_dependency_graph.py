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

JAVA_ROOTS = ("java", "javax", "jdk", "sun", "com.sun", "org.w3c", "org.xml", "org.ietf", "org.omg")
IDENT = r"[A-Za-z_$][\w$]*"
QNAME = rf"{IDENT}(?:\.{IDENT})+"
IMPORT_RE = re.compile(rf"(?m)^\s*import\s+(?P<static>static\s+)?(?P<name>{QNAME})(?P<wild>\.\*)?\s*;")
PACKAGE_RE = re.compile(rf"(?m)^\s*package\s+({QNAME})\s*;")
MODULE_RE = re.compile(rf"\b(?:open\s+)?module\s+({QNAME})\s*\{{")
REQUIRES_RE = re.compile(rf"\brequires\s+(?:transitive\s+|static\s+)*({QNAME})\s*;")
EXPORTS_RE = re.compile(rf"\bexports\s+({QNAME})(?:\s+to\s+[^;]+)?\s*;")
OPENS_RE = re.compile(rf"\bopens\s+({QNAME})(?:\s+to\s+[^;]+)?\s*;")
USES_RE = re.compile(rf"\buses\s+({QNAME})\s*;")
PROVIDES_RE = re.compile(rf"\bprovides\s+({QNAME})\s+with\s+([^;]+)\s*;")
JAVA_ROOT_PATTERN = "|".join(re.escape(x) for x in JAVA_ROOTS)
FQ_RE = re.compile(rf"\b(?:{JAVA_ROOT_PATTERN})\.(?:{IDENT}\.)*{IDENT}(?:\.{IDENT})*\b")
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

def java_namespace(qname: str) -> bool:
    return any(qname == root or qname.startswith(root + ".") for root in JAVA_ROOTS)

def qname_from_path(path: Path, root: Path) -> str | None:
    parts = list(path.relative_to(root).with_suffix("").parts)
    if parts and parts[0] == "early_access":
        # early_access/jdk28/docs/api/<module>/<package>/<Type>
        if len(parts) < 7 or parts[1:4] != ["jdk28", "docs", "api"]:
            return None
        parts = parts[5:]
    if not parts or parts[-1] in {"module-info", "package-info"}:
        return None
    qname = ".".join(parts)
    return qname if java_namespace(qname) else None

def line_of(source: str, offset: int) -> int:
    return source.count("\n", 0, offset) + 1

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[3])
    ap.add_argument("--json", type=Path, help="write deterministic graph JSON")
    ap.add_argument("--check", action="store_true", help="fail on unresolved explicit/FQ references")
    ap.add_argument("--strict", action="store_true", help="also fail on duplicate counterpart paths")
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
        enclosing_type = qname.rpartition(".")[0]
        if enclosing_type in index and enclosing_type != qname:
            local_edges[(enclosing_type, "nested-type-owner")] = 1
        for match in IMPORT_RE.finditer(source):
            name = match.group("name")
            is_static = bool(match.group("static"))
            wildcard = bool(match.group("wild"))
            # Static member imports depend on the declaring owner type.
            owner = name if is_static else name
            if wildcard:
                line = line_of(source, match.start())
                wildcard_edges.append({"from": qname, "package": owner, "kind": "static-wildcard" if is_static else "package-wildcard", "line": line})
                imports.append({"name": owner + ".*", "kind": "static-wildcard" if is_static else "package-wildcard", "line": line})
                if is_static:
                    if owner in index and owner != qname:
                        local_edges[(owner, "static-wildcard-import")] = line
                    elif owner not in index:
                        unresolved.append({"from": qname, "reference": owner, "kind": "static-wildcard-import", "line": line, "source": rel})
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
            # A wildcard import makes an unqualified name a dependency only
            # when that package contains exactly one matching counterpart.
            wildcard_packages = [x["package"] for x in imports if x["kind"] == "package-wildcard"]
            wildcard_candidates = sorted({pkg + "." + simple for pkg in wildcard_packages
                                           if pkg + "." + simple in index})
            if len(wildcard_candidates) == 1 and wildcard_candidates[0] != qname:
                local_edges.setdefault((wildcard_candidates[0], "wildcard-import-type-use"), 1)
            elif len(wildcard_candidates) > 1:
                unresolved.append({"from": qname, "reference": simple,
                                   "kind": "ambiguous-wildcard-type", "line": 1,
                                   "source": rel, "candidates": wildcard_candidates})

        # A nested Java type is owned by its enclosing type when that
        # counterpart exists as a separate indexed source unit.
        for (target, kind), line in list(local_edges.items()):
            parent = target.rpartition(".")[0]
            if parent in index and parent != qname:
                local_edges.setdefault((parent, "nested-type-owner"), line)
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

    # Compute transitive closure from the direct type graph. This is a graph
    # reachability view, not a claim that every dependency is behaviorally
    # implemented.
    adjacency: dict[str, set[str]] = defaultdict(set)
    for edge in edges:
        adjacency[edge["from"]].add(edge["to"])
    transitive = []
    for source in sorted(index):
        seen: set[str] = set()
        pending = list(sorted(adjacency[source]))
        while pending:
            target = pending.pop()
            if target == source or target in seen:
                continue
            seen.add(target)
            pending.extend(sorted(adjacency[target] - seen))
        transitive.append({"name": source, "dependencies": sorted(seen),
                           "dependency_count": len(seen)})

    # Module/package descriptors are first-class graph metadata. They
    # remain distinct from type edges because module names are not types.
    modules = []
    for descriptor in sorted(api_root.rglob("module-info.sleela")):
        source = strip_comments(descriptor.read_text(encoding="utf-8", errors="replace"))
        module_match = MODULE_RE.search(source)
        if module_match:
            modules.append({
                "name": module_match.group(1),
                "path": descriptor.relative_to(repo).as_posix(),
                "requires": sorted(set(REQUIRES_RE.findall(source))),
                "exports": sorted(set(EXPORTS_RE.findall(source))),
                "opens": sorted(set(OPENS_RE.findall(source))),
                "uses": sorted(set(USES_RE.findall(source))),
                "provides": [{"service": service, "implementations": sorted(x.strip() for x in impls.split(","))} for service, impls in PROVIDES_RE.findall(source)],
            })
    package_metadata = []
    for descriptor in sorted(api_root.rglob("package-info.sleela")):
        source = strip_comments(descriptor.read_text(encoding="utf-8", errors="replace"))
        package_match = PACKAGE_RE.search(source)
        if package_match:
            package_metadata.append({
                "name": package_match.group(1),
                "path": descriptor.relative_to(repo).as_posix(),
                "annotations": sorted(set(FQ_RE.findall(source))),
                "imports": [{"name": m.group("name"), "static": bool(m.group("static")), "wildcard": bool(m.group("wild"))} for m in IMPORT_RE.finditer(source)],
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
            "transitive_dependency_pairs": sum(x["dependency_count"] for x in transitive),
            "wildcard_package_edges": len(wildcard_edges),
            "unresolved_references": len(missing),
            "duplicate_type_paths": len(duplicates),
            "packages": len(package_index),
        },
        "nodes": nodes,
        "edges": sorted(edges, key=lambda x: (x["from"], x["to"], x["kind"], x["line"])),
        "transitive_dependencies": transitive,
        "wildcard_dependencies": sorted(wildcard_edges, key=lambda x: (x["from"], x["package"], x["kind"], x["line"])),
        "packages": [{"name": name, "types": sorted(set(types))} for name, types in sorted(package_index.items())],
        "package_metadata": package_metadata,
        "modules": modules,
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
