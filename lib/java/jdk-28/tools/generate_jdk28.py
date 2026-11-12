#!/usr/bin/env python3
"""Generate /lib/java/jdk-28: one SLeeLa file per public Java SE / JDK 28 type (1:1).

Rule: every Java type documented in the JDK 28 API has exactly one SLeeLa
file, at the same path the online API documentation uses:

    online : https://download.java.net/java/early_access/jdk28/docs/api/<module>/<pkg/path>/<Type>.html
    SLeeLa : lib/java/jdk-28/docs/api/<module>/<pkg/path>/<Type>.sleela

The docs/api segment is the online path, and it also tells the /lib loader that
a real library unit of the same name (lib/java/java/...) wins over these files.

Each file names its Java Source (the online API page) and carries the initial
SLeeLa reference for that type, taken from the API only (no Java source text
is copied or expanded):

  * the Java type identity (module, package, kind, superclass, interfaces);
  * constants with their real values, public/protected fields;
  * complete enums and records;
  * every public/protected constructor and method with its exact Java
    signature. A body that is not yet SLeeLa logic throws
    AbstractMethodError (abstract) or UnsupportedOperationException naming
    the Java signature.

Inputs (all in-repo):
  * lib/java/early_access/jdk28/docs/api/   the JDK 28 type inventory, mirrored
                                            from the online JDK 28 API index
  * lib/java/api/jdk-api.jsonl              reflected API (members, constants)
  * lib/java/api/jdk-signatures.jsonl       exact generic signatures
A JDK 28 type that the reflected data does not know (new in JDK 28) gets its
identity and Source link only; its members are marked pending.

Outputs: lib/java/jdk-28/docs/api/<module>/..., lib/java/jdk-28/INDEX.tsv (Java type -> SLeeLa file -> Source).
Idempotent: rewrites every generated file. usage: python3 lib/java/jdk-28/tools/generate_jdk28.py
"""
from __future__ import annotations

import json
import os
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
OUT = REPO / "lib" / "java" / "jdk-28"
API_DIR = OUT / "docs" / "api"
LIBJAVA = REPO / "lib" / "java"
DOCS = LIBJAVA / "early_access" / "jdk28" / "docs" / "api"
ONLINE = "https://download.java.net/java/early_access/jdk28/docs/api/"
DATE = "2026-11-12"

sys.path.insert(0, str(LIBJAVA / "tools"))
import jdk_api_declare as dec  # noqa: E402
import jdk_api_signatures as sg  # noqa: E402

FIXED_FIELDS = 11 + 5  # metadata + javaType/javaModule/javaSource/javaKind/javaSuper
MAX_FIELDS = 64
HELPERS = {"javaModule", "javaSource", "javaKind", "javaSuper", "javaInterfaces", "sourceUrl"}


def inventory():
    """(module, fq name, relative path) for every JDK 28 type in the docs mirror."""
    out = []
    for root, _dirs, files in os.walk(DOCS):
        for f in files:
            if not f.endswith(".sleela"):
                continue
            rel = Path(root, f).relative_to(DOCS)
            parts = rel.with_suffix("").parts
            if len(parts) < 3:  # docs pages such as help-doc, search
                continue
            module, pkg, simple = parts[0], ".".join(parts[1:-1]), parts[-1]
            out.append((module, pkg + "." + simple, pkg, simple, rel))
    return sorted(out, key=lambda x: (x[0], x[1]))


def meta_id(path: Path) -> int:
    m = re.search(r"int metaId = (\d+);", path.read_text(encoding="utf-8", errors="replace"))
    return int(m.group(1)) if m else 0


def metadata(mid: int, fq: str) -> str:
    return f'''  < ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- >
  << Eleven catalogued attributes with getter/setter pairs. Values are faithful
     to the real origin of the Java type: the Java SE / JDK 28 API by Oracle
     and the OpenJDK community. >>
  String metaVersion = "1.0.0";
  int metaId = {mid};
  String metaAuthor = "Oracle / OpenJDK";
  String metaDate = "{DATE}";
  String metaCopyright = "GPLv2 with Classpath Exception (OpenJDK)";
  String metaTrademark = "Java";
  String metaCorporation = "Oracle Corporation";
  < metaOrigin: National | <nation> | International | Galactic >
  String metaOrigin = "International";
  String metaBirthDate = "{DATE}";
  < metaScienceRating: maturity/robustness on a 1..10 scale >
  int metaScienceRating = 9;
  < metaNationalSchedule: a real, citable instrument or "none" >
  String metaNationalSchedule = "Java SE 28 API Specification (JDK 28 early access)";

  String getMetaVersion() {{ return metaVersion; }}
  void setMetaVersion(String v) {{ metaVersion = v; }}
  int getMetaId() {{ return metaId; }}
  void setMetaId(int v) {{ metaId = v; }}
  String getAuthor() {{ return metaAuthor; }}
  void setAuthor(String v) {{ metaAuthor = v; }}
  String getDate() {{ return metaDate; }}
  void setDate(String v) {{ metaDate = v; }}
  String getCopyright() {{ return metaCopyright; }}
  void setCopyright(String v) {{ metaCopyright = v; }}
  String getTrademark() {{ return metaTrademark; }}
  void setTrademark(String v) {{ metaTrademark = v; }}
  String getCorporation() {{ return metaCorporation; }}
  void setCorporation(String v) {{ metaCorporation = v; }}
  String getOrigin() {{ return metaOrigin; }}
  void setOrigin(String v) {{ metaOrigin = v; }}
  String getBirthDate() {{ return metaBirthDate; }}
  void setBirthDate(String v) {{ metaBirthDate = v; }}
  int getScienceRating() {{ return metaScienceRating; }}
  void setScienceRating(int v) {{ metaScienceRating = v; }}
  String getNationalSchedule() {{ return metaNationalSchedule; }}
  void setNationalSchedule(String v) {{ metaNationalSchedule = v; }}
'''


def member_blocks(t, sig, cls: str):
    """Declarations, constructors and methods for a type known to the reflected API."""
    lines = []
    taken = set(dec.ENVELOPE) | HELPERS
    budget_used = FIXED_FIELDS + (2 if t["kind"] == "enum" else 0) + len(t.get("components", []))
    decl = dec.declarations(t, cls, taken, set(), budget_used, False)
    if decl:
        lines.append("  < ---- BEGIN java declarations (from the JDK 28 API) ---- >")
        lines += decl
        lines.append("  < ---- END java declarations ---- >")
    dtext = "\n".join(decl)
    declared = set(dec.FIELD_DECL.findall(dtext)) | set(dec.METHOD_DECL.findall(dtext)) | set(sg.METHOD.findall(dtext))
    if sig is None:
        return lines, 0, 0
    ctv = sg.tvar_names(sig["tparams"])
    has_ctor_arity = set()
    if t["kind"] == "enum":
        has_ctor_arity.add(2)
    if t["kind"] == "record":
        has_ctor_arity.add(len(t.get("components", [])))
    ctors, cseen = [], set()
    for m in sig["members"]:
        if m["name"] != "<init>" or m["tparams"] or not sg.renderable(m) or len(m["params"]) in has_ctor_arity:
            continue
        key = tuple(sg.erased(p[0], ctv) for p in m["params"])
        if key in cseen:
            continue
        cseen.add(key)
        ctors.append(sg.render_ctor(t["name"], cls, m))
    meths, mseen = [], set()
    for m in sig["members"]:
        if m["name"] == "<init>" or m["name"] in declared or m["name"] in taken or not dec.usable(m["name"], cls) or not sg.renderable(m):
            continue
        tv = ctv | sg.tvar_names(m["tparams"])
        key = (m["name"], tuple(sg.erased(p[0], tv) for p in m["params"]))
        if key in mseen:
            continue
        mseen.add(key)
        meths.append(sg.render(t["name"], m))
    if ctors:
        lines.append("")
        lines.append("  < ---- BEGIN java constructors (exact JDK 28 API signatures) ---- >")
        lines += ctors
        lines.append("  < ---- END java constructors ---- >")
    if meths:
        lines.append("")
        lines.append("  < ---- BEGIN java signatures (exact JDK 28 API signatures) ---- >")
        lines.append("  << A body that is not yet SLeeLa logic throws AbstractMethodError for an")
        lines.append("     abstract method, else UnsupportedOperationException naming the Java")
        lines.append("     signature. Produce the SLeeLa logic from the Java Source above. >>")
        lines += meths
        lines.append("  < ---- END java signatures ---- >")
    return lines, len(ctors), len(meths)


def main() -> int:
    api = {}
    for l in (LIBJAVA / "api" / "jdk-api.jsonl").read_text(encoding="utf-8").splitlines():
        if l.strip():
            t = json.loads(l)
            api[(t["module"], t["name"])] = t
    sigs = {}
    for l in (LIBJAVA / "api" / "jdk-signatures.jsonl").read_text(encoding="utf-8").splitlines():
        if l.strip():
            s = json.loads(l)
            sigs[s["name"]] = s

    index = ["# java_type\tmodule\tkind\tsleela_file\tjava_source\tmembers"]
    stats = {"files": 0, "with_members": 0, "pending": 0, "ctors": 0, "methods": 0}
    for module, fq, pkg, simple, rel in inventory():
        cls = simple.split(".")[-1]
        t = api.get((module, fq))
        sig = sigs.get(fq) if t else None
        src_url = ONLINE + rel.with_suffix(".html").as_posix()
        out_rel = Path(module, *pkg.split("."), simple + ".sleela")
        kind = t["kind"] if t else "type"
        sup = (t.get("super") or "") if t else ""
        ifaces = ", ".join(t.get("interfaces", [])) if t else ""
        tps = sig["tparams"] if sig and sig["tparams"] and sg.renderable_text(sig["tparams"]) else ""
        if t:
            body, nc, nm = member_blocks(t, sig, cls)
            status = "members from the JDK 28 API (exact signatures)"
            stats["with_members"] += 1
            stats["ctors"] += nc
            stats["methods"] += nm
        else:
            body = ["  < ---- members pending: new in JDK 28, read them from the Java Source page ---- >"]
            status = "pending (new in JDK 28; members to be read from the Source page)"
            stats["pending"] += 1
        nested = ""
        if "." in simple:
            nested = f"\n   Nested Java type: the file keeps the documented name {simple}."
        text = f'''<< lib/java/jdk-28/docs/api/{out_rel.as_posix()}
   SLeeLa file for the Java type {fq} (module {module}). One Java type, one SLeeLa file.
   Java Source (JDK 28 API): {src_url}
   Java kind: {kind}{("; extends " + sup) if sup else ""}{("; implements " + ifaces) if ifaces else ""}
   Status: {status}.{nested}
   The SLeeLa functions here are produced from that Java Source. No Java source text is copied. >>
#sleela 3.0
material;

class {cls}{tps} {{
{metadata(meta_id(DOCS / rel), fq)}
  String javaType = "{fq}";
  String javaModule = "{module}";
  String javaSource = "{src_url}";
  String javaKind = "{kind}";
  String javaSuper = "{sup}";

''' + "\n".join(body) + "\n}\n"
        dest = API_DIR / out_rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        if dest.is_file() and "BEGIN java implementation" in dest.read_text(encoding="utf-8", errors="replace"):
            stats["kept"] = stats.get("kept", 0) + 1  # hand-written SLeeLa logic: never overwritten
        else:
            dest.write_text(text, encoding="utf-8")
        stats["files"] += 1
        index.append("\t".join([fq, module, kind, "lib/java/jdk-28/docs/api/" + out_rel.as_posix(), src_url, "api" if t else "pending"]))
    (OUT / "INDEX.tsv").write_text("\n".join(index) + "\n", encoding="utf-8")
    print(f"jdk-28: {stats['files']} files (1:1 with JDK 28 types); {stats['with_members']} with API members, "
          f"{stats['pending']} pending; {stats['ctors']} constructors, {stats['methods']} methods")
    return 0


if __name__ == "__main__":
    sys.exit(main())
