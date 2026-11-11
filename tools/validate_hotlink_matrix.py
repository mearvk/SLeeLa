#!/usr/bin/env python3
"""Validate the SLeeLa hot-link matrix (lib/hotlink/hotlink-matrix.xml).

Three layers, all fail-closed with exact locations:
  1. XML Schema validation against hotlink-matrix.xsd (via xmllint if present;
     otherwise the built-in structural checks below stand in and a notice is
     printed).
  2. Cross-field rules XSD 1.0 cannot express: native code must require the
     authorizer and a host-pinned digest; cold types carry no links; compartment
     kind ranges are present, ordered and disjoint; every hot type declares the
     baseline gates; native types declare the authorizer and digest-pin gates;
     the type graph is acyclic apart from a contain self-link.
  3. Drift check: the rows and links must match SLHotLinkMatrix.standard()
     in lib/hotlink/SLHotLinkMatrix.sleela exactly.

Stdlib only. Exit status 0 = valid, 1 = invalid, 2 = usage / missing file.
"""
import argparse
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET

NS = "{urn:sleela:hotlink:1.0}"
BASELINE_GATES = {"audit", "trust-floor", "instance-cap"}


def fail(errors, where, message):
    errors.append("%s: %s" % (where, message))


def load_xml(path, errors):
    try:
        return ET.parse(path).getroot()
    except (ET.ParseError, OSError) as exc:
        fail(errors, path, "cannot parse XML: %s" % exc)
        return None


def run_xmllint(xml, xsd, errors):
    exe = shutil.which("xmllint")
    if not exe:
        print("note: xmllint not found; XSD validation skipped (built-in checks only)")
        return
    proc = subprocess.run([exe, "--noout", "--schema", xsd, xml],
                          capture_output=True, text=True)
    if proc.returncode != 0:
        for line in (proc.stderr or proc.stdout).splitlines():
            if line.strip() and not line.endswith("validates"):
                fail(errors, "xsd", line.strip())


def rows_of(root):
    out = []
    for i, ct in enumerate(root.findall("%sclassTypes/%sclassType" % (NS, NS))):
        a = ct.attrib
        out.append({
            "id": a.get("id"),
            "codeArrival": a.get("codeArrival"),
            "instanceArrival": a.get("instanceArrival"),
            "contract": a.get("contract"),
            "minTrust": int(a.get("minTrust", "-1")),
            "maxInstances": int(a.get("maxInstances", "-1")),
            "kindFirst": int(a.get("kindFirst", "-1")),
            "kindLast": int(a.get("kindLast", "-1")),
            "requiresAuthorizer": 1 if a.get("requiresAuthorizer", "false") in ("true", "1") else 0,
            "digestPolicy": a.get("digestPolicy", "none"),
            "status": a.get("status"),
            "gates": set((a.get("gates") or "").split()),
            "where": "classTypes/classType[%d](%s)" % (i + 1, a.get("id")),
        })
    return out


def links_of(root):
    out = []
    for i, ln in enumerate(root.findall("%slinks/%slink" % (NS, NS))):
        a = ln.attrib
        out.append({"from": a.get("from"), "to": a.get("to"), "mode": a.get("mode"),
                    "maxFanout": int(a.get("maxFanout", "-1")),
                    "where": "links/link[%d](%s->%s)" % (i + 1, a.get("from"), a.get("to"))})
    return out


def cross_field(rows, links, errors):
    by_id = {}
    for r in rows:
        if r["id"] in by_id:
            fail(errors, r["where"], "duplicate class type id")
        by_id[r["id"]] = r
    covered = {}
    for r in rows:
        w = r["where"]
        has_first = r["kindFirst"] >= 0
        has_last = r["kindLast"] >= 0
        if r["contract"] == "compartment":
            if not (has_first and has_last and r["kindFirst"] <= r["kindLast"]):
                fail(errors, w, "compartment type needs kindFirst <= kindLast")
            else:
                for k in range(r["kindFirst"], r["kindLast"] + 1):
                    if k in covered:
                        fail(errors, w, "kind %d already covered by %s" % (k, covered[k]))
                    covered[k] = r["id"]
        elif has_first or has_last:
            fail(errors, w, "only compartment types may carry kindFirst/kindLast")
        if r["codeArrival"] == "native":
            if r["contract"] != "native-abi":
                fail(errors, w, "native code must use the native-abi contract")
            if not r["requiresAuthorizer"]:
                fail(errors, w, "native code must set requiresAuthorizer=true")
            if r["digestPolicy"] != "host-pinned":
                fail(errors, w, "native code must use digestPolicy=host-pinned")
            if not {"authorizer", "digest-pin"} <= r["gates"]:
                fail(errors, w, "native code must declare the authorizer and digest-pin gates")
            if r["status"] == "verified":
                fail(errors, w, "native code cannot be 'verified' without a toolchain that runs it")
        if r["contract"] == "native-abi" and r["codeArrival"] != "native":
            fail(errors, w, "native-abi contract requires codeArrival=native")
        if r["instanceArrival"] == "hot" and not BASELINE_GATES <= r["gates"]:
            fail(errors, w, "hot type must declare gates %s" % sorted(BASELINE_GATES))
        if r["contract"] == "compartment" and r["instanceArrival"] == "hot":
            if not {"fanout-cap", "cycle-free"} <= r["gates"]:
                fail(errors, w, "hot compartment type must declare fanout-cap and cycle-free")
    pairs = set()
    for ln in links:
        w = ln["where"]
        for end in ("from", "to"):
            r = by_id.get(ln[end])
            if r is None:
                fail(errors, w, "'%s' names an unknown class type" % end)
            elif r["instanceArrival"] != "hot":
                fail(errors, w, "cold type %s cannot be linked" % r["id"])
        if (ln["from"], ln["to"]) in pairs:
            fail(errors, w, "duplicate link")
        pairs.add((ln["from"], ln["to"]))
        if ln["from"] == ln["to"] and ln["mode"] != "contain":
            fail(errors, w, "only a contain link may be a self-link")
    # acyclic apart from self-links
    adj = {}
    for ln in links:
        if ln["from"] != ln["to"]:
            adj.setdefault(ln["from"], []).append(ln["to"])
    state = {}

    def visit(n, path):
        state[n] = 1
        for m in adj.get(n, []):
            if state.get(m) == 1:
                fail(errors, "links", "type cycle: %s" % " -> ".join(path + [n, m]))
            elif m not in state:
                visit(m, path + [n])
        state[n] = 2
    for n in list(adj):
        if n not in state:
            visit(n, [])


ROW_RE = re.compile(r'addRow\(\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,'
                    r'\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,'
                    r'\s*"([^"]*)"\s*,\s*"([^"]*)"\s*\)\s*;')
EDGE_RE = re.compile(r'addEdge\(\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*,\s*(\d+)\s*\)\s*;')


def drift(rows, links, source, errors):
    try:
        text = open(source, encoding="utf-8").read()
    except OSError as exc:
        fail(errors, source, "cannot read: %s" % exc)
        return
    start = text.find("int standard()")
    if start < 0:
        fail(errors, source, "no standard() method found")
        return
    body = text[start:]
    src_rows = {}
    for m in ROW_RE.finditer(body):
        g = m.groups()
        src_rows[g[0]] = (g[1], g[2], g[3], int(g[4]), int(g[5]), int(g[6]), int(g[7]),
                          int(g[8]), g[9], g[10])
    src_links = {}
    for m in EDGE_RE.finditer(body):
        g = m.groups()
        src_links[(g[0], g[1])] = (g[2], int(g[3]))
    xml_rows = {}
    for r in rows:
        xml_rows[r["id"]] = (r["codeArrival"], r["instanceArrival"], r["contract"], r["minTrust"],
                             r["maxInstances"], r["kindFirst"], r["kindLast"],
                             r["requiresAuthorizer"], r["digestPolicy"], r["status"])
    xml_links = {(l["from"], l["to"]): (l["mode"], l["maxFanout"]) for l in links}
    for k in sorted(set(src_rows) | set(xml_rows)):
        if k not in xml_rows:
            fail(errors, "drift", "row %s is in %s but not in the XML" % (k, source))
        elif k not in src_rows:
            fail(errors, "drift", "row %s is in the XML but not in standard()" % k)
        elif src_rows[k] != xml_rows[k]:
            fail(errors, "drift", "row %s differs: source=%s xml=%s" % (k, src_rows[k], xml_rows[k]))
    for k in sorted(set(src_links) | set(xml_links)):
        if k not in xml_links:
            fail(errors, "drift", "link %s->%s is in standard() but not in the XML" % k)
        elif k not in src_links:
            fail(errors, "drift", "link %s->%s is in the XML but not in standard()" % k)
        elif src_links[k] != xml_links[k]:
            fail(errors, "drift", "link %s->%s differs: source=%s xml=%s" % (k + (src_links[k], xml_links[k])))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--xml", default="lib/hotlink/hotlink-matrix.xml")
    ap.add_argument("--xsd", default="lib/hotlink/hotlink-matrix.xsd")
    ap.add_argument("--source", default="lib/hotlink/SLHotLinkMatrix.sleela")
    args = ap.parse_args()
    for p in (args.xml, args.xsd, args.source):
        try:
            open(p).close()
        except OSError as exc:
            print("error: %s" % exc, file=sys.stderr)
            return 2
    errors = []
    run_xmllint(args.xml, args.xsd, errors)
    root = load_xml(args.xml, errors)
    if root is not None:
        rows, links = rows_of(root), links_of(root)
        cross_field(rows, links, errors)
        drift(rows, links, args.source, errors)
    if errors:
        for e in errors:
            print("INVALID  " + e)
        print("%d problem(s)" % len(errors))
        return 1
    print("OK  %d class types, %d links; schema, cross-field rules and standard() agree"
          % (len(rows), len(links)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
