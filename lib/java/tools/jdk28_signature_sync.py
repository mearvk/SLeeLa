#!/usr/bin/env python3
"""Synchronize JDK 28 declared member signatures into SLeeLa source.

The official JDK 28 Javadoc is the authority. Existing SLeeLa envelopes are
enriched in-place with Java-like constructors and method declarations. The
generated declarations preserve return types and parameter types; behavior is
not invented and uses deterministic placeholder returns.

Use --check for a non-mutating audit.
"""

from __future__ import annotations
import argparse, html, re, sys, urllib.request
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urljoin

INDEX_URL = "https://download.java.net/java/early_access/jdk28/docs/api/allclasses-index.html"
ROOT = Path(__file__).resolve().parents[1]
BEGIN = "// BEGIN JDK28 API SIGNATURES (generated; do not edit)"
END = "// END JDK28 API SIGNATURES"

class SignatureParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.active = False
        self.depth = 0
        self.parts = []
        self.signatures = []
    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "div" and "member-signature" in attrs.get("class", "").split():
            self.active = True
            self.depth = 1
            self.parts = []
        elif self.active:
            self.depth += 1
    def handle_endtag(self, tag):
        if self.active:
            self.depth -= 1
            if self.depth <= 0:
                value = re.sub(r"\s+", " ", html.unescape("".join(self.parts))).strip()
                if value:
                    self.signatures.append(value)
                self.active = False
                self.parts = []
    def handle_data(self, data):
        if self.active:
            self.parts.append(data)

def fetch(url: str) -> str:
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-JDK28-Signature-Synchronizer/2.0"})
    with urllib.request.urlopen(req, timeout=60) as response:
        return response.read().decode("utf-8", errors="replace")

def index_types(document: str) -> list[tuple[str,str]]:
    found = set()
    for raw in re.findall(r'href=["\']([^"\']+\.html(?:#[^"\']*)?)["\']', document):
        href = html.unescape(raw.split("#",1)[0])
        parts = href.split("/")
        if len(parts) < 3 or not parts[-1].endswith(".html"):
            continue
        name = parts[-1][:-5]
        if name in {"module-info","package-info","package-summary","module-summary","class-use","serialized-form"}:
            continue
        if any(x in parts for x in ("doc-files","index-files")):
            continue
        package = ".".join(parts[1:-1])
        if package:
            found.add((package + "." + name, href))
    return sorted(found)

def normalize(raw: str) -> str | None:
    s = re.sub(r"\s+", " ", html.unescape(raw)).strip()
    s = re.sub(r"\b(?:public|protected|private|abstract|static|final|default|synchronized|native|strictfp|sealed|non-sealed)\b\s*", "", s)
    s = re.sub(r"@[\w$.]+(?:\([^)]*\))?\s*", "", s)
    s = re.split(r"\s+throws\s+", s, maxsplit=1)[0].strip()
    s = s.replace("...", "[]")
    return s if "(" in s and ")" in s else None

def extract(page: str) -> list[str]:
    p = SignatureParser()
    p.feed(page)
    out=[]; seen=set()
    for raw in p.signatures:
        sig=normalize(raw)
        if sig and sig not in seen:
            seen.add(sig); out.append(sig)
    return out

def split_signature(sig: str, simple: str):
    sig = re.sub(r"\s+", " ", sig).strip()
    if sig.startswith(simple + "("):
        return ("constructor", simple, "", sig[len(simple):].strip())
    # Find the method identifier immediately preceding the opening parenthesis.
    m = re.match(r"(?P<prefix>.+?)\s+(?P<name>[A-Za-z_$][\w$]*)\s*(?P<params>\(.*\))$", sig)
    if not m:
        return None
    prefix=m.group("prefix").strip()
    name=m.group("name")
    params=m.group("params")
    return ("method", name, prefix, params)

def default_body(return_type: str) -> str:
    t=return_type.strip()
    if t == "void":
        return ""
    if t == "boolean":
        return " return false;"
    if t in {"byte","short","int","long","float","double","char"}:
        return " return 0;"
    return " return null;"

def render(signatures: list[str], qualified: str) -> str:
    simple=qualified.rsplit(".",1)[-1]
    lines=[BEGIN, "// Exact declared JDK 28 member signatures; generated from official Javadoc."]
    for sig in signatures:
        parsed=split_signature(sig,simple)
        if not parsed:
            lines.append("  // " + sig)
            continue
        kind,name,ret,params=parsed
        if kind == "constructor":
            lines.append(f"  {simple}{params} {{ }}")
        else:
            body=default_body(ret)
            lines.append(f"  {ret} {name}{params} {{{body} }}")
    lines.append(END)
    return "\n".join(lines)

def replace_section(source: str, section: str) -> str:
    pattern=re.escape(BEGIN)+r".*?"+re.escape(END)
    if re.search(pattern,source,re.S):
        return re.sub(pattern,section,source,count=1,flags=re.S)
    pos=source.rfind("}")
    if pos < 0:
        return source.rstrip()+"\n"+section+"\n"
    return source[:pos].rstrip()+"\n"+section+"\n"+source[pos:]

def main() -> int:
    ap=argparse.ArgumentParser()
    ap.add_argument("--check",action="store_true")
    ap.add_argument("--index",type=Path)
    args=ap.parse_args()
    index=args.index.read_text(encoding="utf-8") if args.index else fetch(INDEX_URL)
    types=index_types(index)
    changed=missing=checked=0
    for qualified,href in types:
        target=ROOT/(qualified.replace(".","/")+".sleela")
        if not target.exists():
            continue
        checked += 1
        try:
            signatures=extract(fetch(urljoin(INDEX_URL,href)))
        except Exception as exc:
            print(f"ERROR {qualified}: {exc}",file=sys.stderr)
            return 1
        if not signatures:
            missing += 1
            print(f"NO_DECLARED_SIGNATURES {qualified}")
            continue
        source=target.read_text(encoding="utf-8",errors="replace")
        updated=replace_section(source,render(signatures,qualified))
        if updated != source:
            changed += 1
            if not args.check:
                target.write_text(updated,encoding="utf-8")
    print(f"JDK 28 documented types: {len(types)}")
    print(f"Existing SLeeLa types checked: {checked}")
    print(f"Types without extracted declarations: {missing}")
    print(f"Files {'requiring update' if args.check else 'updated'}: {changed}")
    return 1 if args.check and changed else 0

if __name__ == "__main__":
    raise SystemExit(main())
