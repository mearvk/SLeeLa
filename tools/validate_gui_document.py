#!/usr/bin/env python3
"""Validate the SLeeLa GUI document (lib/guidoc/gui-document.xml).

Three layers, all fail-closed with exact locations:
  1. XML Schema validation against gui-document.xsd (via xmllint if present;
     otherwise the built-in checks below stand in and a notice is printed).
  2. Cross-field rules XSD 1.0 cannot express: menu items name declared
     commands; accelerators are unique; no declared command is orphaned; every
     command that can discard work (closing a tab, quitting) asks for
     confirmation; each handler exists as a method of SLGuiDocApp; the window
     theme equals --require-theme (default: white).
  3. Drift check: the window, commands, menus, items, tabs, editor, file dialog,
     listener and readiness must match SLGuiDocSpec.standard() exactly.
  4. Readiness gate (--gate): run guidoc-demo with the sleela binary and require
     a clean run (no FAIL, "failed 0"), a READY lifecycle with no failed check,
     and COVERAGE >= 1 for every command and every menu item in the XML. A
     document that claims <readiness status="ready"> is INVALID unless the gate
     passes; --require-ready additionally demands the claim itself.

Stdlib only. Exit status 0 = valid, 1 = invalid, 2 = usage / missing file.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET

NS = "{urn:sleela:guidoc:1.0}"
DISCARDING = {"file.closeTab", "app.quit"}
WHEN = {"always", "hasTab", "dirty", "editable", "selectionEditable", "multipleTabs"}
ACCEL = re.compile(r"^$|^Ctrl\+(Shift\+)?[A-Za-z0-9]$|^Ctrl\+(Shift\+)?Tab$|^F([1-9]|1[0-2])$")


def fail(errors, where, message):
    errors.append("%s: %s" % (where, message))


def run_xmllint(xml, xsd, errors):
    exe = shutil.which("xmllint")
    if not exe:
        print("note: xmllint not found; XSD validation skipped (built-in checks only)")
        return
    proc = subprocess.run([exe, "--noout", "--schema", xsd, xml], capture_output=True, text=True)
    if proc.returncode != 0:
        for line in (proc.stderr or proc.stdout).splitlines():
            if line.strip() and not line.endswith("validates"):
                fail(errors, "xsd", line.strip())


def model_of(root):
    m = {"commands": [], "menus": [], "items": []}
    w = root.find(NS + "window")
    m["window"] = (w.get("title"), w.get("theme")) if w is not None else None
    for i, c in enumerate(root.findall("%scommands/%scommand" % (NS, NS))):
        m["commands"].append((c.get("id"), c.get("handler"),
                              1 if c.get("confirmsDiscard", "false") in ("true", "1") else 0,
                              "commands/command[%d](%s)" % (i + 1, c.get("id"))))
    for mi, menu in enumerate(root.findall("%smenus/%smenu" % (NS, NS))):
        m["menus"].append((menu.get("id"), menu.get("label")))
        for ii, it in enumerate(menu.findall(NS + "item")):
            m["items"].append((menu.get("id"), it.get("id"), it.get("label"), it.get("command"),
                               it.get("accelerator", ""), it.get("enabledWhen"),
                               "menus/menu[%d]/item[%d](%s)" % (mi + 1, ii + 1, it.get("id"))))
    t = root.find(NS + "tabs")
    m["tabs"] = (int(t.get("maxTabs")), t.get("dirtyMarker"), t.get("newTabTitle")) if t is not None else None
    e = root.find(NS + "editor")
    tf = lambda v: 1 if v in ("true", "1") else 0
    m["editor"] = (int(e.get("rows")), tf(e.get("lineNumbers")), tf(e.get("readOnlyWhenUnsplit"))) if e is not None else None
    f = root.find(NS + "fileDialog")
    m["file"] = (f.get("sidecarSuffix"), int(f.get("maxLines")), int(f.get("maxBytes"))) if f is not None else None
    r = root.find(NS + "readiness")
    m["readiness"] = (r.get("status"), r.get("release")) if r is not None else None
    l = root.find(NS + "listener")
    m["listener"] = (l.get("port"), tf(l.get("flagExternal"))) if l is not None else None
    return m


def cross_field(m, app_source, require_theme, errors):
    cmd_ids = {}
    for cid, handler, confirms, where in m["commands"]:
        if cid in cmd_ids:
            fail(errors, where, "duplicate command id")
        cmd_ids[cid] = (handler, confirms, where)
        if re.search(r"boolean\s+%s\s*\(" % re.escape(handler), app_source) is None:
            fail(errors, where, "handler %s is not a method of SLGuiDocApp" % handler)
        if cid in DISCARDING and not confirms:
            fail(errors, where, "%s can discard work and must set confirmsDiscard=true" % cid)
    used, accels = set(), {}
    for menu, iid, label, command, accel, when, where in m["items"]:
        if command not in cmd_ids:
            fail(errors, where, "command %s is not declared" % command)
        used.add(command)
        if when not in WHEN:
            fail(errors, where, "unknown enabledWhen %s" % when)
        if not ACCEL.match(accel):
            fail(errors, where, "malformed accelerator %r" % accel)
        if accel:
            if accel in accels:
                fail(errors, where, "accelerator %s already used by %s" % (accel, accels[accel]))
            accels[accel] = iid
    for cid, (_, _, where) in cmd_ids.items():
        if cid not in used:
            fail(errors, where, "command %s is not reachable from any menu item" % cid)
    ids = [i[1] for i in m["items"]]
    for x in set(ids):
        if ids.count(x) > 1:
            fail(errors, "menus", "duplicate item id %s" % x)
    for mid, _ in m["menus"]:
        if not any(i[0] == mid for i in m["items"]):
            fail(errors, "menus", "menu %s has no items" % mid)
    if m["window"] and m["window"][1] != require_theme:
        fail(errors, "window", "theme is %r, required %r" % (m["window"][1], require_theme))
    if m["tabs"] and not (1 <= m["tabs"][0] <= 14):
        fail(errors, "tabs", "maxTabs must be 1..14")


S = r'"((?:[^"\\]|\\.)*)"'
RE_WINDOW = re.compile(r"setWindow\(\s*%s\s*,\s*%s\s*\)\s*;" % (S, S))
RE_TABS = re.compile(r"setTabs\(\s*(\d+)\s*,\s*%s\s*,\s*%s\s*\)\s*;" % (S, S))
RE_EDITOR = re.compile(r"setEditor\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\)\s*;")
RE_FILE = re.compile(r"setFileDialog\(\s*%s\s*,\s*(\d+)\s*,\s*(\d+)\s*\)\s*;" % S)
RE_LIST = re.compile(r"setListener\(\s*%s\s*,\s*(\d+)\s*\)\s*;" % S)
RE_READY = re.compile(r"setReadiness\(\s*%s\s*,\s*%s\s*\)\s*;" % (S, S))
RE_CMD = re.compile(r"addCommand\(\s*%s\s*,\s*%s\s*,\s*(\d+)\s*\)\s*;" % (S, S))
RE_MENU = re.compile(r"addMenu\(\s*%s\s*,\s*%s\s*\)\s*;" % (S, S))
RE_ITEM = re.compile(r"addItem\(\s*%s\s*,\s*%s\s*,\s*%s\s*,\s*%s\s*,\s*%s\s*,\s*%s\s*\)\s*;" % ((S,) * 6))


def drift(m, spec_source, errors):
    start = spec_source.find("int standard()")
    if start < 0:
        fail(errors, "SLGuiDocSpec", "no standard() method found")
        return
    body = spec_source[start:]

    def one(rx, want, label):
        found = rx.findall(body)
        if len(found) != 1:
            fail(errors, "drift", "standard() must call %s exactly once (found %d)" % (label, len(found)))
            return
        got = tuple(int(x) if x.isdigit() else x for x in found[0])
        if got != want:
            fail(errors, "drift", "%s differs: source=%s xml=%s" % (label, got, want))
    if m["window"]:
        one(RE_WINDOW, m["window"], "setWindow")
    if m["tabs"]:
        one(RE_TABS, m["tabs"], "setTabs")
    if m["editor"]:
        one(RE_EDITOR, m["editor"], "setEditor")
    if m["file"]:
        one(RE_FILE, m["file"], "setFileDialog")
    if m["listener"]:
        one(RE_LIST, m["listener"], "setListener")
    if m["readiness"]:
        one(RE_READY, m["readiness"], "setReadiness")
    src_cmds = [(a, b, int(c)) for a, b, c in RE_CMD.findall(body)]
    xml_cmds = [(a, b, c) for a, b, c, _ in m["commands"]]
    if src_cmds != xml_cmds:
        for x in xml_cmds:
            if x not in src_cmds:
                fail(errors, "drift", "command %s is in the XML but not (identically) in standard()" % (x,))
        for x in src_cmds:
            if x not in xml_cmds:
                fail(errors, "drift", "command %s is in standard() but not (identically) in the XML" % (x,))
    src_menus = RE_MENU.findall(body)
    if src_menus != m["menus"]:
        fail(errors, "drift", "menus differ: source=%s xml=%s" % (src_menus, m["menus"]))
    src_items = RE_ITEM.findall(body)
    xml_items = [tuple(x[:6]) for x in m["items"]]
    if src_items != xml_items:
        for x in xml_items:
            if x not in src_items:
                fail(errors, "drift", "item %s is in the XML but not (identically) in standard()" % (x,))
        for x in src_items:
            if x not in xml_items:
                fail(errors, "drift", "item %s is in standard() but not (identically) in the XML" % (x,))
        if set(src_items) == set(xml_items):
            fail(errors, "drift", "items are the same but in a different order")


def gate(m, sleela, demo, errors):
    """Run the demo and verify the readiness claim. Returns (ok, summary)."""
    if not os.path.exists(sleela):
        fail(errors, "gate", "sleela binary not found: %s (use --sleela)" % sleela)
        return False, ""
    try:
        proc = subprocess.run([sleela, "run", demo], capture_output=True, text=True, timeout=600)
    except (OSError, subprocess.TimeoutExpired) as exc:
        fail(errors, "gate", "could not run the demo: %s" % exc)
        return False, ""
    out = proc.stdout
    problems = len(errors)
    if proc.returncode != 0:
        fail(errors, "gate", "demo exited with status %d" % proc.returncode)
    fails = [ln.strip() for ln in out.splitlines() if ln.startswith("  FAIL")]
    for ln in fails:
        fail(errors, "gate", "demo check failed: %s" % ln)
    res = re.search(r"^RESULT passed (\d+) failed (\d+)$", out, re.M)
    if not res:
        fail(errors, "gate", "demo printed no RESULT line (did it crash?)")
    elif int(res.group(2)) != 0:
        fail(errors, "gate", "demo reports %s failed check(s)" % res.group(2))
    rd = re.search(r"^READY state (\w+) checks (\d+) failures (\d+)", out, re.M)
    if not rd or rd.group(1) != "ready" or rd.group(3) != "0":
        fail(errors, "gate", "the GUI lifecycle did not reach ready with zero failures")
    cov = {"command": {}, "item": {}}
    for kind, ident, n in re.findall(r"^COVERAGE (command|item) (\S+) (\d+)$", out, re.M):
        cov[kind][ident] = int(n)
    want_cmds = [c[0] for c in m["commands"]]
    want_items = [i[1] for i in m["items"]]
    for c in want_cmds:
        if cov["command"].get(c, 0) < 1:
            fail(errors, "gate", "command %s was never exercised by the demo" % c)
    for i in want_items:
        if cov["item"].get(i, 0) < 1:
            fail(errors, "gate", "menu item %s was never clicked by the demo" % i)
    for kind, known in (("command", want_cmds), ("item", want_items)):
        for ident in cov[kind]:
            if ident not in known:
                fail(errors, "gate", "coverage names %s %s which is not in the XML" % (kind, ident))
    ok = len(errors) == problems
    summary = ""
    if res and rd:
        summary = "demo %s checks, lifecycle %s checks, coverage commands %d/%d items %d/%d" % (
            res.group(1), rd.group(2),
            sum(1 for c in want_cmds if cov["command"].get(c, 0) >= 1), len(want_cmds),
            sum(1 for i in want_items if cov["item"].get(i, 0) >= 1), len(want_items))
    return ok, summary


def read(path, errors):
    try:
        return open(path, encoding="utf-8").read()
    except OSError as exc:
        fail(errors, path, "cannot read: %s" % exc)
        return ""


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--xml", default="lib/guidoc/gui-document.xml")
    ap.add_argument("--xsd", default="lib/guidoc/gui-document.xsd")
    ap.add_argument("--spec", default="lib/guidoc/SLGuiDocSpec.sleela")
    ap.add_argument("--app", default="lib/guidoc/SLGuiDocApp.sleela")
    ap.add_argument("--require-theme", default="white")
    ap.add_argument("--gate", action="store_true", help="run the demo and verify the readiness claim")
    ap.add_argument("--require-ready", action="store_true", help="demand status=ready and a passing gate")
    ap.add_argument("--sleela", default="working-binaries/sleela")
    ap.add_argument("--demo", default="lib/guidoc/guidoc-demo.sleela")
    args = ap.parse_args()
    for p in (args.xml, args.xsd, args.spec, args.app):
        try:
            open(p).close()
        except OSError as exc:
            print("error: %s" % exc, file=sys.stderr)
            return 2
    errors = []
    run_xmllint(args.xml, args.xsd, errors)
    try:
        root = ET.parse(args.xml).getroot()
    except ET.ParseError as exc:
        print("INVALID  %s: cannot parse XML: %s" % (args.xml, exc))
        return 1
    m = model_of(root)
    cross_field(m, read(args.app, errors), args.require_theme, errors)
    drift(m, read(args.spec, errors), errors)
    claim = m["readiness"][0] if m["readiness"] else "draft"
    summary = ""
    gate_ok = None
    if args.gate or args.require_ready:
        gate_ok, summary = gate(m, args.sleela, args.demo, errors)
        if claim == "ready" and not gate_ok:
            fail(errors, "readiness", "the document claims status=ready but the gate fails")
    if args.require_ready:
        if claim != "ready":
            fail(errors, "readiness", "status is %r, required 'ready'" % claim)
    if errors:
        for e in errors:
            print("INVALID  " + e)
        print("%d problem(s)" % len(errors))
        return 1
    print("OK  %d commands, %d menus, %d items; schema, cross-field rules and standard() agree"
          % (len(m["commands"]), len(m["menus"]), len(m["items"])))
    if gate_ok:
        print("GATE OK  %s" % summary)
        if claim == "draft":
            print("note: the gate passes, so this document may be marked status=\"ready\"")
    elif claim == "ready":
        print("note: status=ready is not verified here; run with --gate")
    print("readiness: %s %s" % (claim, m["readiness"][1] if m["readiness"] else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
