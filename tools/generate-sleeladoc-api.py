#!/usr/bin/env python3
"""Generate the SleelaDoc HTML API documentation site from the /lib tree.

This is the HTML-driven documentation system for the SLeeLa standard library.
It walks every `.sleela` source unit under `lib/`, extracts each class/struct
together with its fields, methods, and the human explanations carried in the
source comments (the `<< ... Definition: ... >>` file header and the `<`/`<<`/
`//` comments that precede members), and emits a clean, navigable static HTML
site under `/sleeladoc-api`.

Design goals:
  * A clear hierarchy: Home -> Package -> Class -> Members.
  * Explanations written to be clear to a "Student 2" (defined in the glossary):
    a capable adult learner returning to college-level study to read the books
    and learn what is true -- so prose is plain, defined, and unhurried.
  * A clean house style in blue / light blue / light grey / white with black
    text, matching the brief.

Usage:
  python3 tools/generate-sleeladoc-api.py            # write /sleeladoc-api
  python3 tools/generate-sleeladoc-api.py --out DIR  # choose output directory
"""
import argparse
import html
import os
import re
import sys
from datetime import datetime, timezone
from pathlib import Path


# --------------------------------------------------------------------------
# Repository geometry
# --------------------------------------------------------------------------

def repo_root() -> Path:
    # tools/ lives directly under the repository root.
    return Path(__file__).resolve().parent.parent


# The 22 standard metadata accessors are identical in every class; listing them
# on every page is noise. We detect them, omit them from the per-member tables,
# and instead note that the class "carries the standard Metadata Class record".
_META_ACCESSOR = re.compile(
    r"^(get|set)(MetaVersion|MetaId|Author|Date|Copyright|Trademark|"
    r"Corporation|Origin|BirthDate|ScienceRating|NationalSchedule)$"
)
_META_FIELD = re.compile(
    r"^(metaVersion|metaId|metaAuthor|metaDate|metaCopyright|metaTrademark|"
    r"metaCorporation|metaOrigin|metaBirthDate|metaScienceRating|"
    r"metaNationalSchedule)$"
)

_SCALAR_TYPES = {
    "void", "int", "long", "int64", "int128", "boolean", "double", "float",
    "char", "String", "byte", "short",
}


# --------------------------------------------------------------------------
# Comment harvesting
# --------------------------------------------------------------------------

def strip_comment_line(line: str):
    """If `line` is a pure comment line, return its text; else None.

    Recognizes the angle-bracket forms (`<` line, `<<`/`>>` block edges) and the
    legacy `//`. The returned text has the delimiters and a leading `@doc`/`@page`
    sentinel removed and is whitespace-trimmed.
    """
    s = line.strip()
    if not s:
        return None
    if s.startswith("<<"):
        s = s[2:]
        if s.endswith(">>"):
            s = s[:-2]
    elif s.startswith("<") and not s.startswith("<="):
        s = s[1:]
        if s.endswith(">"):
            s = s[:-1]
    elif s.startswith("//"):
        s = s[2:]
    elif s.startswith("#"):
        return None  # pragma, not a doc comment
    else:
        return None
    s = s.strip()
    # Drop a SleelaDoc sentinel if present.
    for sent in ("@doc", "@page"):
        if s.startswith(sent):
            s = s[len(sent):].strip()
    # Drop decorative rule lines like "---- foo ----".
    if set(s) <= set("- ") and s:
        return ""
    return s


def gather_block_comment(lines, start_idx):
    """Collect a contiguous run of comment lines ending at (and including)
    `start_idx`, walking upward. Returns the joined, cleaned prose.

    `start_idx` is the index of the comment line immediately above a member.
    We walk up while lines are comments (or inside a `<< ... >>` block), stop at
    the first blank or code line, and join the pieces top-to-bottom.
    """
    pieces = []
    i = start_idx
    # Walk upward collecting comment lines.
    while i >= 0:
        raw = lines[i]
        s = raw.strip()
        if not s:
            break
        txt = strip_comment_line(raw)
        if txt is None:
            # Could be a middle line of a << >> block (no delimiter on it).
            # Only accept it if we are clearly inside a block being gathered and
            # the line is not code (heuristic: no ';' or '{' or '(' at end).
            if pieces and not re.search(r"[;{}]\s*$", s) and "(" not in s:
                pieces.append(s)
                i -= 1
                continue
            break
        pieces.append(txt)
        i -= 1
    pieces.reverse()
    text = " ".join(p for p in pieces if p).strip()
    text = re.sub(r"\s+", " ", text)
    return text


# --------------------------------------------------------------------------
# File parsing
# --------------------------------------------------------------------------

class Member:
    def __init__(self, kind, name, sig, desc):
        self.kind = kind          # "field" | "method"
        self.name = name
        self.sig = sig
        self.desc = desc


class ClassDoc:
    def __init__(self, kind, name, extends):
        self.kind = kind          # "class" | "struct" | "interface"
        self.name = name
        self.extends = extends
        self.summary = ""         # from the next member-less header, if any
        self.fields = []
        self.methods = []
        self.has_metadata = False


class UnitDoc:
    def __init__(self, package, relpath, name):
        self.package = package
        self.relpath = relpath
        self.name = name          # file base name without extension
        self.definition = ""      # the file header "Definition:" line
        self.syntax = ""
        self.classes = []


_DEF_RE = re.compile(r"Definition:\s*(.+?)\s*>>", re.S)
_DEF_RE2 = re.compile(r"Definition:\s*(.+)")
_CLASS_RE = re.compile(
    r"^(class|struct|interface)\s+([A-Za-z_][A-Za-z0-9_]*)"
    r"(?:\s+extends\s+([A-Za-z_][A-Za-z0-9_]*))?"
)
_PRAGMA_RE = re.compile(r"^#sleela\s+([0-9.]+)")

# A field:  [static] [final] <Type>[[]] <name> [= ...] ;
_FIELD_RE = re.compile(
    r"^(?:static\s+)?(?:final\s+)?"
    r"([A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*\])?)\s+"
    r"([A-Za-z_][A-Za-z0-9_]*)\s*(?:=.*)?;\s*$"
)
# A method: <Type>[[]] <name> ( ... )
_METHOD_RE = re.compile(
    r"^(?:static\s+)?(?:final\s+)?"
    r"([A-Za-z_][A-Za-z0-9_]*(?:\s*\[\s*\])?)\s+"
    r"([A-Za-z_][A-Za-z0-9_]*)\s*\("
)


def parse_unit(path: Path, package: str, relpath: str) -> UnitDoc:
    unit = UnitDoc(package, relpath, path.stem)
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return unit
    raw = text.replace("\r\n", "\n")
    # File header Definition line (within the first << >> block).
    head = raw[:1200]
    m = _DEF_RE.search(head) or _DEF_RE2.search(head)
    if m:
        unit.definition = re.sub(r"\s+", " ", m.group(1)).strip()
    pm = _PRAGMA_RE.search(raw)
    if pm:
        unit.syntax = pm.group(1)

    lines = raw.split("\n")
    current = None
    depth = 0  # brace depth; members are at class body depth
    in_block_comment = False

    for idx, line in enumerate(lines):
        stripped = line.strip()

        # Track << >> block comments so we do not parse their text as code.
        if in_block_comment:
            if ">>" in stripped:
                in_block_comment = False
            continue
        if stripped.startswith("<<") and ">>" not in stripped:
            in_block_comment = True
            continue
        if stripped.startswith("<") or stripped.startswith("//"):
            continue

        # Class / struct / interface declaration.
        cm = _CLASS_RE.match(stripped)
        if cm:
            current = ClassDoc(cm.group(1), cm.group(2), cm.group(3))
            unit.classes.append(current)
            depth = stripped.count("{") - stripped.count("}")
            continue

        if current is None:
            continue

        open_b = stripped.count("{")
        close_b = stripped.count("}")

        # Only treat declarations found at the class body level (depth == 1
        # before this line's braces) as members.
        at_body = (depth == 1)

        if at_body:
            fm = _FIELD_RE.match(stripped)
            mm = _METHOD_RE.match(stripped)
            if mm and "(" in stripped:
                mtype, mname = mm.group(1), mm.group(2)
                if _META_ACCESSOR.match(mname):
                    current.has_metadata = True
                else:
                    desc = gather_block_comment(lines, idx - 1)
                    sig = re.sub(r"\s*\{.*$", "", stripped).strip()
                    sig = sig.rstrip("{ ").strip()
                    current.methods.append(Member("method", mname, sig, desc))
            elif fm:
                ftype, fname = fm.group(1), fm.group(2)
                if _META_FIELD.match(fname):
                    current.has_metadata = True
                else:
                    desc = gather_block_comment(lines, idx - 1)
                    sig = stripped.rstrip(";").strip()
                    current.fields.append(Member("field", fname, sig, desc))

        depth += open_b - close_b
        if depth <= 0:
            current = None
            depth = 0

    return unit


# --------------------------------------------------------------------------
# HTML rendering
# --------------------------------------------------------------------------

def e(s: str) -> str:
    return html.escape(s or "")


STYLE = """\
/* SleelaDoc API -- house style: blue, light blue, light grey, white; black text. */
:root{
  --blue:#1f5fb0;        /* primary blue */
  --blue-dark:#174a8a;
  --lightblue:#dbeafe;   /* light blue */
  --lightblue-2:#eff6ff;
  --grey:#eef1f4;        /* light grey */
  --grey-2:#f6f8fa;
  --border:#cfd8e3;
  --text:#111111;        /* black text */
  --muted:#425066;
}
*{box-sizing:border-box}
html{scroll-behavior:smooth}
body{margin:0;font-family:-apple-system,Segoe UI,Roboto,Helvetica,Arial,sans-serif;
  color:var(--text);background:#ffffff;line-height:1.55;font-size:16px}
a{color:var(--blue);text-decoration:none}
a:hover{text-decoration:underline}
header.top{background:linear-gradient(180deg,var(--blue),var(--blue-dark));color:#fff;
  padding:18px 26px;border-bottom:4px solid var(--blue-dark)}
header.top a{color:#fff}
header.top .brand{font-size:22px;font-weight:700;letter-spacing:.2px}
header.top .tag{color:var(--lightblue);font-size:13px;margin-top:2px}
.wrap{display:flex;min-height:calc(100vh - 72px)}
nav.side{width:280px;background:var(--grey);border-right:1px solid var(--border);
  padding:16px 14px;overflow:auto;max-height:calc(100vh - 72px);position:sticky;top:0}
nav.side h2{font-size:12px;text-transform:uppercase;letter-spacing:.08em;color:var(--muted);
  margin:14px 0 6px}
nav.side ul{list-style:none;margin:0 0 8px;padding:0}
nav.side li{margin:1px 0}
nav.side a{display:block;padding:3px 8px;border-radius:5px;color:var(--text);font-size:14px}
nav.side a:hover{background:var(--lightblue);text-decoration:none}
main{flex:1;padding:26px 34px;max-width:980px;overflow:auto}
h1{color:var(--blue-dark);font-size:28px;margin:0 0 6px}
h2.section{color:var(--blue-dark);border-bottom:2px solid var(--lightblue);
  padding-bottom:5px;margin-top:34px;font-size:21px}
h3{color:var(--blue-dark);margin:22px 0 4px;font-size:17px}
.crumbs{color:var(--muted);font-size:13px;margin-bottom:14px}
.crumbs a{color:var(--blue)}
.lead{background:var(--lightblue-2);border:1px solid var(--border);border-left:4px solid var(--blue);
  padding:12px 16px;border-radius:7px;margin:14px 0 8px}
.pill{display:inline-block;background:var(--lightblue);color:var(--blue-dark);border-radius:999px;
  padding:1px 10px;font-size:12px;font-weight:600;margin-left:6px;vertical-align:middle}
.pill.struct{background:#e8f0e8;color:#2c5a2c}
.pill.iface{background:#efe8f4;color:#5a2c6a}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(230px,1fr));gap:12px;margin:14px 0}
.card{background:var(--grey-2);border:1px solid var(--border);border-radius:9px;padding:12px 14px}
.card h3{margin:0 0 4px;font-size:16px}
.card p{margin:0;color:var(--muted);font-size:13px}
.card .count{color:var(--blue);font-weight:600;font-size:12px}
table.members{width:100%;border-collapse:collapse;margin:8px 0 18px;background:#fff}
table.members th{background:var(--blue);color:#fff;text-align:left;padding:7px 10px;font-size:13px}
table.members td{border:1px solid var(--border);padding:8px 10px;vertical-align:top;font-size:14px}
table.members tr:nth-child(even) td{background:var(--grey-2)}
code,.sig{font-family:SFMono-Regular,Consolas,Menlo,monospace;font-size:13px}
.sig{background:var(--grey);border:1px solid var(--border);border-radius:5px;padding:1px 6px;color:var(--blue-dark)}
.desc{color:var(--text)}
.muted{color:var(--muted)}
.meta-note{background:var(--grey-2);border:1px dashed var(--border);border-radius:7px;
  padding:8px 12px;margin:10px 0;color:var(--muted);font-size:13px}
dl.glossary dt{font-weight:700;color:var(--blue-dark);margin-top:12px}
dl.glossary dd{margin:2px 0 0 0}
footer{color:var(--muted);font-size:12px;padding:18px 34px;border-top:1px solid var(--border);
  background:var(--grey-2)}
.search{width:100%;padding:7px 10px;border:1px solid var(--border);border-radius:6px;margin-bottom:10px;font-size:14px}
"""


def page(title, body, rel_to_root, nav_html, extra_head=""):
    css = rel_to_root + "style.css"
    home = rel_to_root + "index.html"
    pkgs = rel_to_root + "packages.html"
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{e(title)} — SleelaDoc API</title>
<link rel="stylesheet" href="{e(css)}">
{extra_head}
</head>
<body>
<header class="top">
  <div class="brand"><a href="{e(home)}">SleelaDoc API</a></div>
  <div class="tag">HTML documentation for the SLeeLa standard library (/lib)</div>
</header>
<div class="wrap">
<nav class="side">
  <a href="{e(home)}"><strong>Home</strong></a>
  <a href="{e(pkgs)}"><strong>All packages</strong></a>
  <a href="{e(rel_to_root)}glossary.html"><strong>Glossary</strong></a>
  {nav_html}
</nav>
<main>
{body}
<footer>
Generated by <code>tools/generate-sleeladoc-api.py</code> from the live
<code>/lib</code> tree on {e(datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M UTC'))}.
SleelaDoc — the SLeeLa documentation brand. SLeeLa — MEARVK LLC.
</footer>
</main>
</div>
</body>
</html>
"""


def side_nav(packages, rel_to_root):
    items = []
    items.append('<h2>Packages</h2><ul>')
    for pkg in packages:
        href = f"{rel_to_root}package/{pkg}.html"
        items.append(f'<li><a href="{e(href)}">{e(pkg)}</a></li>')
    items.append("</ul>")
    return "\n".join(items)


def describe_member(m: Member) -> str:
    if m.desc:
        return e(m.desc)
    return '<span class="muted">No description in source.</span>'


def render_class(cd: ClassDoc, unit: UnitDoc) -> str:
    kindpill = ""
    if cd.kind == "struct":
        kindpill = '<span class="pill struct">struct</span>'
    elif cd.kind == "interface":
        kindpill = '<span class="pill iface">interface</span>'
    else:
        kindpill = '<span class="pill">class</span>'
    ext = ""
    if cd.extends:
        ext = f' <span class="muted">extends <code>{e(cd.extends)}</code></span>'
    out = [f'<h3 id="{e(cd.name)}">{e(cd.name)} {kindpill}{ext}</h3>']
    if unit.definition and cd is unit.classes[0]:
        out.append(f'<p class="desc">{e(unit.definition)}</p>')
    if cd.has_metadata:
        out.append('<div class="meta-note">Carries the standard '
                   '<strong>Metadata Class record</strong> — eleven catalogued '
                   'provenance attributes (version, id, author, date, copyright, '
                   'trademark, corporation, origin, birth date, science rating, '
                   'national schedule) with matching get/set accessors. These are '
                   'omitted from the tables below for brevity.</div>')
    if cd.fields:
        out.append('<p class="muted"><strong>Fields</strong> — the named values '
                   'this unit stores.</p>')
        out.append('<table class="members"><tr><th>Field</th><th>Declaration</th>'
                   '<th>What it holds</th></tr>')
        for f in cd.fields:
            out.append(f'<tr><td><code>{e(f.name)}</code></td>'
                       f'<td><span class="sig">{e(f.sig)}</span></td>'
                       f'<td class="desc">{describe_member(f)}</td></tr>')
        out.append("</table>")
    if cd.methods:
        out.append('<p class="muted"><strong>Methods</strong> — the actions this '
                   'unit can perform.</p>')
        out.append('<table class="members"><tr><th>Method</th><th>Signature</th>'
                   '<th>What it does</th></tr>')
        for mth in cd.methods:
            out.append(f'<tr><td><code>{e(mth.name)}</code></td>'
                       f'<td><span class="sig">{e(mth.sig)}</span></td>'
                       f'<td class="desc">{describe_member(mth)}</td></tr>')
        out.append("</table>")
    if not cd.fields and not cd.methods and not cd.has_metadata:
        out.append('<p class="muted">No public fields or methods detected '
                   '(the unit may be a facade or a pure contract).</p>')
    return "\n".join(out)


def render_package_page(pkg, units, packages, out_dir):
    nav = side_nav(packages, "../")
    nunits = len(units)
    nclasses = sum(len(u.classes) for u in units)
    body = [f'<div class="crumbs"><a href="../index.html">Home</a> &rsaquo; '
            f'<a href="../packages.html">Packages</a> &rsaquo; {e(pkg)}</div>']
    body.append(f'<h1>Package <code>{e(pkg)}</code></h1>')
    body.append(f'<div class="lead">This package contains <strong>{nunits}</strong> '
                f'source unit(s) defining <strong>{nclasses}</strong> class(es) / '
                f'struct(s). Each unit below lists its fields (the values it stores) '
                f'and methods (the actions it performs), with a plain explanation of '
                f'each.</div>')
    body.append('<input class="search" placeholder="Filter on this page (Ctrl+F also works)…" '
                'oninput="flt(this.value)">')
    units_sorted = sorted(units, key=lambda u: u.name.lower())
    for u in units_sorted:
        body.append(f'<h2 class="section" data-u="{e(u.name.lower())}">'
                    f'{e(u.name)} '
                    f'<span class="muted" style="font-size:13px">({e(u.relpath)})</span></h2>')
        if not u.classes:
            if u.definition:
                body.append(f'<p class="desc">{e(u.definition)}</p>')
            else:
                body.append('<p class="muted">No class declaration parsed.</p>')
        for cd in u.classes:
            body.append(f'<div data-u="{e(u.name.lower())} {e(cd.name.lower())}">')
            body.append(render_class(cd, u))
            body.append('</div>')
    script = ("<script>function flt(q){q=q.toLowerCase();"
              "document.querySelectorAll('[data-u]').forEach(function(el){"
              "el.style.display = el.getAttribute('data-u').indexOf(q)>=0||q==''?'':'none';});}"
              "</script>")
    body.append(script)
    (out_dir / "package" / f"{pkg}.html").write_text(
        page(f"Package {pkg}", "\n".join(body), "../", nav), encoding="utf-8")


def render_index(packages, pkg_units, out_dir, totals):
    nav = side_nav(packages, "")
    body = [f'<h1>SLeeLa Standard Library — HTML API</h1>']
    body.append(f'<div class="lead">This is the generated HTML documentation for '
                f'every source unit under <code>/lib</code>: '
                f'<strong>{totals["packages"]}</strong> packages, '
                f'<strong>{totals["units"]}</strong> source units, '
                f'<strong>{totals["classes"]}</strong> classes and structs. '
                f'Start at a package, open a unit, and read its fields and methods '
                f'with plain explanations.</div>')
    body.append('<p>New here? Read the <a href="glossary.html">Glossary</a> first — '
                'it defines the reading level these pages are written for '
                '(<strong>Student 2</strong>) and the core SLeeLa terms.</p>')
    body.append('<h2 class="section">Packages</h2>')
    body.append('<input class="search" placeholder="Filter packages…" oninput="fltp(this.value)">')
    body.append('<div class="grid">')
    for pkg in packages:
        units = pkg_units[pkg]
        nclasses = sum(len(u.classes) for u in units)
        href = f"package/{pkg}.html"
        body.append(f'<a class="card" data-p="{e(pkg.lower())}" href="{e(href)}">'
                    f'<h3>{e(pkg)}</h3>'
                    f'<p class="count">{len(units)} unit(s) · {nclasses} class(es)</p>'
                    f'</a>')
    body.append('</div>')
    body.append("<script>function fltp(q){q=q.toLowerCase();"
                "document.querySelectorAll('[data-p]').forEach(function(el){"
                "el.style.display=el.getAttribute('data-p').indexOf(q)>=0||q==''?'':'none';});}</script>")
    (out_dir / "index.html").write_text(
        page("Home", "\n".join(body), "", nav), encoding="utf-8")


def render_packages_list(packages, pkg_units, out_dir, totals):
    nav = side_nav(packages, "")
    body = ['<div class="crumbs"><a href="index.html">Home</a> &rsaquo; Packages</div>']
    body.append('<h1>All packages</h1>')
    body.append(f'<div class="lead">{totals["packages"]} packages, '
                f'{totals["units"]} source units, {totals["classes"]} classes and structs.</div>')
    body.append('<table class="members"><tr><th>Package</th><th>Units</th>'
                '<th>Classes</th></tr>')
    for pkg in packages:
        units = pkg_units[pkg]
        nclasses = sum(len(u.classes) for u in units)
        body.append(f'<tr><td><a href="package/{e(pkg)}.html">{e(pkg)}</a></td>'
                    f'<td>{len(units)}</td><td>{nclasses}</td></tr>')
    body.append('</table>')
    (out_dir / "packages.html").write_text(
        page("All packages", "\n".join(body), "", nav), encoding="utf-8")


def render_glossary(packages, out_dir):
    nav = side_nav(packages, "")
    body = ['<div class="crumbs"><a href="index.html">Home</a> &rsaquo; Glossary</div>']
    body.append('<h1>Glossary — terms and the reading level</h1>')
    body.append('<div class="lead">These pages are written to be clear to a '
                '<strong>Student 2</strong>. That term, and the core SLeeLa terms '
                'used throughout, are defined here.</div>')
    body.append('<dl class="glossary">')
    terms = [
        ("Student 2",
         "A capable adult learner who is good enough to study at college again — "
         "returning to the books and to learning what is true. Not a beginner who "
         "needs everything spelled out, and not a specialist who needs nothing "
         "explained: a serious reader who wants plain, defined, unhurried prose "
         "that respects both their intelligence and their fresh start. Every "
         "explanation on these pages aims at the Student 2."),
        ("Class",
         "A named unit of SLeeLa that bundles together values (fields) and actions "
         "(methods). It is the main building block of the library — one class per "
         "source file."),
        ("Struct",
         "Like a class, but intended chiefly as a plain record of values. In SLeeLa "
         "a struct groups related fields under one name."),
        ("Field",
         "A named value that a class or struct stores. Reading the fields tells you "
         "what the unit remembers."),
        ("Method",
         "A named action a class can perform. Reading the methods tells you what the "
         "unit can do. A method has a signature (its return type, name, and the "
         "values it takes in) and a body (the steps it runs)."),
        ("Signature",
         "The one-line shape of a method or field: its type, its name, and — for a "
         "method — the parameters it accepts. The signature is the contract; the "
         "body is the fulfilment."),
        ("Facade (SLPackage)",
         "A small class named SLPackage that presents a whole package to the "
         "compiler and loader. It names the package and reports whether it is "
         "available; it carries no domain logic of its own."),
        ("Metadata Class record",
         "A standing SLeeLa convention: every new class carries eleven catalogued "
         "provenance attributes — version, id, author, date, copyright, trademark, "
         "corporation, origin, birth date, science rating, and national schedule — "
         "each with a get/set accessor. It records who made a unit, under what "
         "authority, and with what intent. These accessors are summarised rather "
         "than listed on each page."),
        ("SleelaDoc",
         "SLeeLa's in-source documentation convention — doc-comments that make a "
         "source file self-describing, the way Javadoc does for Java, but "
         "documenting intent rather than only signatures. This HTML site is built "
         "from the explanations SleelaDoc and the ordinary source comments carry."),
        ("Package",
         "A top-level folder under /lib that groups related classes into one family "
         "(for example, xml, codecs, or compression)."),
    ]
    for t, d in terms:
        body.append(f'<dt>{e(t)}</dt><dd>{e(d)}</dd>')
    body.append('</dl>')
    (out_dir / "glossary.html").write_text(
        page("Glossary", "\n".join(body), "", nav), encoding="utf-8")


# --------------------------------------------------------------------------
# Driver
# --------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=None, help="output directory (default: /sleeladoc-api)")
    ap.add_argument("--lib", default=None, help="library root (default: <repo>/lib)")
    args = ap.parse_args()

    root = repo_root()
    lib = Path(args.lib) if args.lib else (root / "lib")
    out_dir = Path(args.out) if args.out else (root / "sleeladoc-api")

    if not lib.is_dir():
        print(f"error: library root not found: {lib}", file=sys.stderr)
        return 1

    pkg_units = {}
    for dirpath, _dirs, files in os.walk(lib):
        for fn in files:
            if not fn.endswith(".sleela"):
                continue
            full = Path(dirpath) / fn
            rel = full.relative_to(lib.parent).as_posix()   # lib/<pkg>/.../x.sleela
            parts = rel.split("/")
            if len(parts) < 3:
                continue
            pkg = parts[1]
            unit = parse_unit(full, pkg, rel)
            pkg_units.setdefault(pkg, []).append(unit)

    packages = sorted(pkg_units.keys())
    totals = {
        "packages": len(packages),
        "units": sum(len(v) for v in pkg_units.values()),
        "classes": sum(len(u.classes) for v in pkg_units.values() for u in v),
    }

    # Write output.
    (out_dir / "package").mkdir(parents=True, exist_ok=True)
    (out_dir / "style.css").write_text(STYLE, encoding="utf-8")
    render_index(packages, pkg_units, out_dir, totals)
    render_packages_list(packages, pkg_units, out_dir, totals)
    render_glossary(packages, out_dir)
    for pkg in packages:
        render_package_page(pkg, pkg_units[pkg], packages, out_dir)

    print(f"sleeladoc-api: wrote {totals['packages']} package pages "
          f"({totals['units']} units, {totals['classes']} classes) to {out_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
