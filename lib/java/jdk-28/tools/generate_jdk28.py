#!/usr/bin/env python3
"""Generate /lib/java/jdk-28: one SLeeLa file per public Java SE / JDK 28 type (1:1).

Rule: every Java type documented in the JDK 28 API has exactly one SLeeLa
file, at the same path the online API documentation uses:

    online : https://download.java.net/java/early_access/jdk28/docs/api/<module>/<pkg/path>/<Type>.html
    SLeeLa : lib/java/jdk-28/<module>/<pkg/path>/<Type>.sleela

The /lib loader treats java/jdk-28/ as a reference tier: a real library unit of
the same name (lib/java/java/...) wins over these files (impl/frontend/driver.cpp).

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
  * lib/java/jdk-25/early_access/jdk28/docs/api/   the JDK 28 type inventory, mirrored
                                            from the online JDK 28 API index
  * lib/java/jdk-25/api/jdk-api.jsonl              reflected API (members, constants)
  * lib/java/jdk-25/api/jdk-signatures.jsonl       exact generic signatures
A JDK 28 type that the reflected data does not know (new in JDK 28) gets its
identity and Source link only; its members are marked pending.

Outputs: lib/java/jdk-28/<module>/..., lib/java/jdk-28/INDEX.tsv (Java type -> SLeeLa file -> Source).
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
API_DIR = OUT
LIBJAVA = REPO / "lib" / "java" / "jdk-25"  # reflected API data, tools and the JDK 28 index mirror
DOCS = LIBJAVA / "early_access" / "jdk28" / "docs" / "api"
ONLINE = "https://download.java.net/java/early_access/jdk28/docs/api/"
DATE = "2026-11-12"

sys.path.insert(0, str(LIBJAVA / "tools"))
import jdk_api_declare as dec  # noqa: E402
import jdk_api_signatures as sg  # noqa: E402

FIXED_FIELDS = 11 + 6  # metadata + javaType/javaModule/javaSource/javaKind/javaSuper + genericTypeArguments
MAX_FIELDS = 64
# Documented modules that export no packages (no types). Each is in the JDK 25
# runtime this tree was reflected from and has a module-summary page in the
# Oracle API docs; the newest release where that page was confirmed is noted.
# Not listed: jdk.crypto.ec (deprecated for removal since JDK 22; JDK 28 status
# unconfirmed), and jdk.unsupported, jdk.unsupported.desktop and jdk.jsobject,
# which have no types in the JDK 28 API index.
NO_PACKAGE_MODULES = {
    "java.se": "module page in the JDK 27 API docs",
    "jdk.jcmd": "module page in the JDK 27 API docs",
    "jdk.jdwp.agent": "module page in the JDK 27 API docs",
    "jdk.jlink": "module page in the JDK 27 API docs",
    "jdk.jpackage": "module page in the JDK 27 API docs",
    "jdk.zipfs": "module page in the JDK 27 API docs",
    "jdk.jdeps": "module page in the JDK 25 API docs",
    "jdk.editpad": "module page in the JDK 24 API docs",
    "jdk.charsets": "module page in the JDK 22 API docs; in the JDK 25 runtime",
    "jdk.naming.rmi": "module page in the JDK 21 API docs; in the JDK 25 runtime",
    "jdk.crypto.cryptoki": "module page in the JDK 17 API docs; in the JDK 25 runtime",
    "jdk.jstatd": "module page in the JDK 17 API docs; in the JDK 25 runtime",
    "jdk.hotspot.agent": "module page in the JDK 11 API docs; in the JDK 25 runtime",
    "jdk.localedata": "module page in the JDK 11 API docs; in the JDK 25 runtime",
    "jdk.naming.dns": "module page in the JDK 11 API docs; in the JDK 25 runtime",
    "jdk.management.agent": "in the JDK 25 runtime; module page not yet confirmed online",
}
HELPERS = {"javaModule", "javaSource", "javaKind", "javaSuper", "javaInterfaces", "sourceUrl",
           "JAVA_TYPE_PARAMETERS", "genericTypeArguments", "genericTypeParameters", "genericTypeParameterCount",
           "genericTypeParameterName", "bindGenericTypeArguments", "genericTypeArgument", "genericTypeArgumentCount"}


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


IMPL_RE = re.compile(r"^  < ---- BEGIN java implementation.*?^  < ---- END java implementation ---- >\n", re.S | re.M)
IMPL_CTOR = r"^  (?:@\w+\s+)?(?:public |protected |private )?{cls}\s*\(([^)]*)\)"


def impl_cover(impl: str, cls: str):
    """What an implementation block already provides: method kinds, ctor arities, field names."""
    methods = sg.impl_sigs(impl)
    ctor_arities = {len(sg.split_top(x)) if x.strip() else 0
                    for x in re.findall(IMPL_CTOR.format(cls=re.escape(cls)), impl, re.M)}
    fields = set(dec.FIELD_DECL.findall(impl))
    return methods, ctor_arities, fields


PRIM_INT = {"int", "long", "short", "byte", "char"}


def default_return(ret: str) -> str:
    r = ret.strip()
    if r == "void":
        return ""
    if r in PRIM_INT:
        return " return 0;"
    if r in ("double", "float"):
        return " return 0.0;"
    if r == "boolean":
        return " return false;"
    return " return null;"


def throwable(names) -> str:
    return '  @throwable("' + ", ".join(names) + '")'


PRIMS = {"int", "long", "short", "byte", "char", "float", "double", "boolean", "void"}
ANNOT = re.compile(r"@[\w.]+(\([^()]*\))?\s*")


def erase(t: str) -> str:
    """The SLeeLa generics method (SLEELA.syntax 16.3, 38.1): erasure.

    A type variable or any parameterized / package-qualified reference type
    erases to Any; java.lang.String erases to String; primitives stay; array
    and varargs suffixes are kept on the erased element type."""
    t = ANNOT.sub("", t).strip()
    suffix = ""
    if t.endswith("..."):
        suffix, t = "...", t[:-3].strip()
    dims = ""
    while t.endswith("[]"):
        dims, t = dims + "[]", t[:-2].strip()
    raw = t.split("<", 1)[0].strip()
    if raw in PRIMS:
        base = raw
    elif raw in ("java.lang.String", "String"):
        base = "String"
    else:
        base = "Any"
    return base + dims + suffix


def params_of(m) -> str:
    out = []
    for ptype, pname in m["params"]:
        if pname in sg.RESERVED_PARAMS:
            pname = pname + "_"
        out.append(f"{erase(ptype)} {pname}")
    return ", ".join(out)


def generic_note(tparams: str) -> list:
    """A method that Java declares generic keeps its type parameters as data."""
    if not tparams:
        return []
    return ['  @generic("' + tparams.replace('"', "'") + '")']


def erase_name(t: str) -> str:
    """A thrown type named in @throwable, without its type arguments."""
    return ANNOT.sub("", t).split("<", 1)[0].strip().replace('"', "'")


def render_stub(type_name: str, m: dict) -> str:
    """A method with its exact Java signature, not yet SLeeLa logic.

    @throwable names what Java throws here: AbstractMethodError for an abstract
    method, else UnsupportedOperationException, plus the declared throws."""
    mods = m["mods"].split()
    lines = []
    if m["protected"] and not m["static"]:
        mods = [x for x in mods if x != "protected"]
        lines.append("  @JavaProtected")
    first = "java.lang.AbstractMethodError" if m["abstract"] else "java.lang.UnsupportedOperationException"
    lines += generic_note(m["tparams"])
    lines.append(throwable([first] + [erase_name(x) for x in m["throws"]]))
    mods = [x for x in mods if x != "abstract"]
    ret = erase(m["ret"])
    head = " ".join(mods + [ret, m["name"]])
    lines.append(f"  {head}({params_of(m)}) {{{default_return(ret)} }}")
    return "\n".join(lines)


def render_ctor_stub(type_name: str, cls: str, m: dict) -> str:
    mods = [x for x in m["mods"].split() if x in ("public", "protected")]
    lines = []
    if m["protected"]:
        mods = [x for x in mods if x != "protected"]
        lines.append("  @JavaProtected")
    lines += generic_note(m["tparams"])
    lines.append(throwable(["java.lang.UnsupportedOperationException"] + [erase_name(x) for x in m["throws"]]))
    head = " ".join(mods + [cls])
    lines.append(f"  {head}({params_of(m)}) {{ }}")
    return "\n".join(lines)


def generics_record(sig, cls: str) -> str:
    """Every file records its Java type parameters as data (the SLeeLa generics
    method: erasure to Any, no reified type arguments)."""
    tparams = sig["tparams"] if sig and sig.get("tparams") else ""
    names = []
    if tparams:
        inner, depth, cur = tparams[1:-1], 0, ""
        for ch in inner:
            if ch == "<":
                depth += 1
            elif ch == ">":
                depth -= 1
            if ch == "," and depth == 0:
                names.append(cur.strip().split(" ", 1)[0])
                cur = ""
            else:
                cur += ch
        if cur.strip():
            names.append(cur.strip().split(" ", 1)[0])
    self_type = "Any" if cls in dec.RESERVED else cls
    # A Java member named arrayLength (ObjectInputFilter.FilterInfo, MethodHandles)
    # hides the VM builtin inside the class, so the argument count is kept in a
    # field: a full Java binding has one argument per type parameter.
    shadowed = bool(sig) and any(m["name"] == "arrayLength" for m in sig.get("members", []))
    if shadowed:
        count_field = "  int genericTypeArgumentTotal = 0;\n"
        size = "genericTypeArgumentTotal"
        set_total = " if (arguments == null) { this.genericTypeArgumentTotal = 0; } else { this.genericTypeArgumentTotal = genericTypeParameterCount(); }"
    else:
        count_field = ""
        size = "arrayLength(genericTypeArguments)"
        set_total = ""
    lookup = "".join(f'if (index == {i}) {{ return "{n}"; }} ' for i, n in enumerate(names))
    lit = tparams.replace("\\", "\\\\").replace('"', '\\"')
    return f'''  < ---- generics (SLeeLa erasure, SLEELA.syntax 16.3 and 38.1) ---- >
  << The class is written without Java type parameters: every type variable and
     every parameterized or package-qualified reference type erases to Any, and
     java.lang.String erases to String. A value of a type parameter travels as
     Any; test it with instanceof and cast it where it is used. The Java type
     parameters are kept as data in JAVA_TYPE_PARAMETERS. An instance may record
     the type arguments its caller means with bindGenericTypeArguments (for an
     ArrayList of String, bind the single name java.lang.String). Nothing is
     checked against them at run time. >>
  static final String JAVA_TYPE_PARAMETERS = "{lit}";
  String[] genericTypeArguments = new String[0];
{count_field}  public static String genericTypeParameters() {{ return JAVA_TYPE_PARAMETERS; }}
  public static int genericTypeParameterCount() {{ return {len(names)}; }}
  public static String genericTypeParameterName(int index) {{ {lookup}return null; }}
  public {self_type} bindGenericTypeArguments(String[] arguments) {{ if (arguments == null) {{ this.genericTypeArguments = new String[0]; }} else {{ this.genericTypeArguments = arguments; }}{set_total} return this; }}
  public String genericTypeArgument(int index) {{ if (index < 0 || index >= {size}) {{ return null; }} return genericTypeArguments[index]; }}
  public int genericTypeArgumentCount() {{ return {size}; }}
'''


def member_blocks(t, sig, cls: str, impl: str = ""):
    """Declarations, constructors and methods for a type known to the reflected API.

    Members already provided by the implementation block (same name and
    accepted argument kinds, same constructor arity, same field name) are not
    written again as stubs."""
    lines = []
    taken = set(dec.ENVELOPE) | HELPERS
    impl_methods, impl_ctors, impl_fields = impl_cover(impl, cls)
    budget_used = FIXED_FIELDS + (2 if t["kind"] == "enum" else 0) + len(t.get("components", []))
    budget_used += len([f for f in impl_fields])
    decl = dec.declarations(t, cls, taken | impl_fields, set(impl_methods), budget_used, bool(impl))
    if decl:
        lines.append("  < ---- BEGIN java declarations (from the JDK 28 API) ---- >")
        lines += decl
        lines.append("  < ---- END java declarations ---- >")
    dtext = "\n".join(decl)
    declared = set(dec.FIELD_DECL.findall(dtext)) | set(dec.METHOD_DECL.findall(dtext)) | set(sg.METHOD.findall(dtext))
    if sig is None:
        return lines, 0, 0
    ctv = sg.tvar_names(sig["tparams"])
    has_ctor_arity = set(impl_ctors)
    if t["kind"] == "enum":
        has_ctor_arity.add(2)
    if t["kind"] == "record":
        has_ctor_arity.add(len(t.get("components", [])))
    ctors, cseen = [], set()
    for m in sig["members"]:
        if cls in dec.RESERVED:
            break  # a class named by a SLeeLa keyword (String) cannot declare constructors
        if m["name"] != "<init>" or len(m["params"]) in has_ctor_arity:
            continue
        key = tuple(erase(p[0]) for p in m["params"])
        if key in cseen:
            continue
        cseen.add(key)
        ctors.append(render_ctor_stub(t["name"], cls, m))
    meths, mseen = [], set()
    for m in sig["members"]:
        if m["name"] == "<init>" or m["name"] in declared or m["name"] in taken or not dec.usable(m["name"], cls):
            continue
        tv = ctv | sg.tvar_names(m["tparams"])
        want = tuple(sg.kind(p[0], tv) for p in m["params"])
        if any(sg.accepts(have, want) for have in impl_methods.get(m["name"], [])):
            continue
        key = (m["name"], tuple(erase(p[0]) for p in m["params"]))
        if key in mseen:
            continue
        mseen.add(key)
        meths.append(render_stub(t["name"], m))
    if ctors:
        lines.append("")
        lines.append("  < ---- BEGIN java constructors (exact JDK 28 API signatures) ---- >")
        lines += ctors
        lines.append("  < ---- END java constructors ---- >")
    if meths:
        lines.append("")
        lines.append("  < ---- BEGIN java signatures (exact JDK 28 API signatures) ---- >")
        lines.append("  << Not yet SLeeLa logic: each method returns the default value of its")
        lines.append("     Java return type. @throwable names what Java throws for it (abstract:")
        lines.append("     AbstractMethodError, else UnsupportedOperationException, plus the")
        lines.append("     declared throws). Write the logic from the Java Source above into the")
        lines.append("     java implementation block; the generator then drops the stub. >>")
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
        dest = API_DIR / out_rel
        impl = ""
        if dest.is_file():
            mo = IMPL_RE.search(dest.read_text(encoding="utf-8", errors="replace"))
            if mo:
                impl = mo.group(0)
                stats["implemented"] = stats.get("implemented", 0) + 1
        if t:
            body, nc, nm = member_blocks(t, sig, cls, impl)
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
        text = f'''<< lib/java/jdk-28/{out_rel.as_posix()}
   SLeeLa file for the Java type {fq} (module {module}). One Java type, one SLeeLa file.
   Java Source (JDK 28 API): {src_url}
   Java kind: {kind}{("; extends " + sup) if sup else ""}{("; implements " + ifaces) if ifaces else ""}
   Status: {status}.{nested}
   The SLeeLa functions here are produced from that Java Source. No Java source text is copied. >>
#sleela 3.0
material;

class {cls} {{
{metadata(meta_id(DOCS / rel), fq)}
  String javaType = "{fq}";
  String javaModule = "{module}";
  String javaSource = "{src_url}";
  String javaKind = "{kind}";
  String javaSuper = "{sup}";

{generics_record(sig, cls)}
''' + (impl + "\n" if impl else "") + "\n".join(body) + "\n}\n"
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_text(text, encoding="utf-8")
        stats["files"] += 1
        index.append("\t".join([fq, module, kind, "lib/java/jdk-28/" + out_rel.as_posix(), src_url, "api" if t else "pending"]))
    (OUT / "INDEX.tsv").write_text("\n".join(index) + "\n", encoding="utf-8")

    # Every documented module has a folder, as online (docs/api/<module>/module-summary.html).
    # Modules that export no packages hold no types, so their folder carries a
    # module-summary.md naming the online page instead.
    counts = {}
    for module, *_ in inventory():
        counts[module] = counts.get(module, 0) + 1
    mods = ["# module\ttypes\tfolder\tjava_source\tevidence"]
    for module in sorted(counts):
        mods.append(f"{module}\t{counts[module]}\tlib/java/jdk-28/{module}/\t{ONLINE}{module}/module-summary.html\tJDK 28 API index (types)")
    for module, evidence in sorted(NO_PACKAGE_MODULES.items()):
        folder = OUT / module
        folder.mkdir(parents=True, exist_ok=True)
        url = f"{ONLINE}{module}/module-summary.html"
        (folder / "module-summary.md").write_text(
            f"# Module {module} (Java SE 28 & JDK 28)\n\n"
            f"Java Source: {url}\n\n"
            "This module exports no packages, so it has no Java types and no SLeeLa\n"
            "type files. The folder exists so that /lib/java/jdk-28 has every module\n"
            "folder the online JDK 28 API has.\n\n"
            f"Evidence that the module is documented: {evidence}.\n", encoding="utf-8")
        mods.append(f"{module}\t0\tlib/java/jdk-28/{module}/\t{url}\t{evidence}")
    (OUT / "MODULES.tsv").write_text("\n".join(mods) + "\n", encoding="utf-8")
    stats["modules"] = len(counts) + len(NO_PACKAGE_MODULES)
    print(f"jdk-28: {stats['files']} files (1:1 with JDK 28 types); {stats['with_members']} with API members, "
          f"{stats['pending']} pending; {stats['ctors']} constructors, {stats['methods']} methods; "
          f"{stats['modules']} module folders; {stats.get('implemented', 0)} files with SLeeLa logic")
    return 0


if __name__ == "__main__":
    sys.exit(main())
