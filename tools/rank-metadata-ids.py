#!/usr/bin/env python3
"""Assign metaId as a global sorted rank across all stamped .sleela files, and
set metaScienceRating by importance tier.

Sort key (ascending): date, then class/file name, then author, then rank of
importance (executive+ before worker). metaId = 1-based position in that order.
metaScienceRating: executive+ = 9, worker = 7.
"""
import os, re

ROOT = os.path.abspath(os.path.dirname(__file__) + "/..")
LIB = os.path.join(ROOT, "lib")

HEADER_TAG = "Descriptive metadata (version 1.11"

RE_DATE = re.compile(r'String metaDate\s*=\s*"([^"]*)"')
RE_BIRTH = re.compile(r'String metaBirthDate\s*=\s*"([^"]*)"')
RE_AUTHOR = re.compile(r'String metaAuthor\s*=\s*"([^"]*)"')
RE_CLASS = re.compile(r'^[ \t]*(?:class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)',
                      re.MULTILINE)
RE_ID = re.compile(r'(int metaId\s*=\s*)\d+(\s*;)')
RE_RATING = re.compile(r'(int metaScienceRating\s*=\s*)\d+(\s*;)')


def is_worker(relpath):
    p = relpath.lower()
    for mark in ("/example", "/tutorial", "/test", "/demo", "/sample"):
        if mark in p:
            return True
    return False


def collect():
    recs = []
    for dp, dn, fn in os.walk(LIB):
        for name in fn:
            if not name.endswith(".sleela"):
                continue
            path = os.path.join(dp, name)
            with open(path, "r", encoding="utf-8") as f:
                text = f.read()
            if HEADER_TAG not in text:
                continue  # not a stamped file (the 8 DSL files, etc.)
            # Only re-rank files carrying the placeholder metaId = 0 that this
            # pass introduced. Files with a real, non-zero id (e.g. codec id 17)
            # keep their honest registry value untouched.
            idm = RE_ID.search(text)
            if not idm or re.search(r'int metaId\s*=\s*0\s*;', text) is None:
                continue
            rel = os.path.relpath(path, ROOT)
            date = (RE_DATE.search(text) or [None, "9999-99-99"])
            date = date.group(1) if hasattr(date, "group") else "9999-99-99"
            birth_m = RE_BIRTH.search(text)
            birth = birth_m.group(1) if birth_m else "9999-99-99"
            author_m = RE_AUTHOR.search(text)
            author = author_m.group(1) if author_m else ""
            cls_m = RE_CLASS.search(text)
            cname = cls_m.group(1) if cls_m else name
            worker = is_worker(rel)
            # importance: executive+ (0) sorts before worker (1)
            imp = 1 if worker else 0
            recs.append({
                "path": path, "rel": rel, "text": text,
                "date": date, "birth": birth, "author": author,
                "name": cname, "imp": imp, "worker": worker,
            })
    return recs


def main():
    recs = collect()
    # composite sort: date, name, author, importance, then path for total order
    recs.sort(key=lambda r: (r["date"], r["birth"], r["name"],
                             r["author"], r["imp"], r["rel"]))
    changed = 0
    for i, r in enumerate(recs, start=1):
        rating = 7 if r["worker"] else 9
        text = r["text"]
        new = RE_ID.sub(rf'\g<1>{i}\g<2>', text, count=1)
        new = RE_RATING.sub(rf'\g<1>{rating}\g<2>', new, count=1)
        if new != text:
            with open(r["path"], "w", encoding="utf-8") as f:
                f.write(new)
            changed += 1
    print(f"stamped files ranked: {len(recs)}  files rewritten: {changed}")
    # sanity: id range
    print(f"metaId range: 1..{len(recs)}")


if __name__ == "__main__":
    main()
