#!/usr/bin/env python3
"""Safe runtime-file staging and uninstall bookkeeping for SLeeLa."""
import json
import os
import shutil
import sys

MARKER = ".sleela-install-manifest.json"

def safe_child(root, relative):
    root = os.path.abspath(root)
    if not relative or os.path.isabs(relative) or ".." in relative.replace("\\", "/").split("/"):
        raise SystemExit("Refusing unsafe relative path: " + repr(relative))
    path = os.path.abspath(os.path.join(root, relative))
    if os.path.commonpath((root, path)) != root:
        raise SystemExit("Refusing path outside installation root: " + path)
    return path

def copy_manifest(repo, runtime):
    repo, runtime = map(os.path.abspath, (repo, runtime))
    manifest_path = os.path.join(repo, "security", "sha256-manifest.json")
    with open(manifest_path, encoding="utf-8") as stream:
        entries = json.load(stream).get("files", [])
    for entry in entries:
        rel = entry["path"]
        src = safe_child(repo, rel)
        if not os.path.isfile(src):
            raise SystemExit("Invalid or missing manifest source: " + rel)
        dst = safe_child(runtime, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)

def record(runtime, data, bindir):
    runtime, data, bindir = map(os.path.abspath, (runtime, data, bindir))
    marker = os.path.join(runtime, MARKER)
    files = []
    for label, root in (("runtime", runtime), ("data", data)):
        if not os.path.isdir(root):
            continue
        for current, dirs, names in os.walk(root, followlinks=False):
            dirs[:] = [d for d in dirs if not os.path.islink(os.path.join(current, d))]
            for name in names:
                path = os.path.join(current, name)
                if path == marker:
                    continue
                files.append({"root": label, "path": os.path.relpath(path, root)})
    payload = {"files": files, "binaries": [
        os.path.join(bindir, "sleela"), os.path.join(bindir, "nordshrift")
    ], "bindir": bindir}
    with open(marker, "w", encoding="utf-8") as stream:
        json.dump(payload, stream, indent=2)
        stream.write("\n")

def uninstall(runtime, data, bindir):
    runtime, data, bindir = map(os.path.abspath, (runtime, data, bindir))
    marker = os.path.join(runtime, MARKER)
    if not os.path.isfile(marker):
        print("No SLeeLa install manifest found; refusing to remove untracked binaries or runtime files.")
        return
    with open(marker, encoding="utf-8") as stream:
        manifest = json.load(stream)
    if os.path.abspath(manifest.get("bindir", "")) != bindir:
        raise SystemExit("Install BINDIR differs from recorded install; pass the original BINDIR.")
    roots = {"runtime": runtime, "data": data}
    for item in manifest.get("files", []):
        root = roots.get(item.get("root"))
        if root is None:
            raise SystemExit("Refusing unknown uninstall root: " + repr(item))
        path = safe_child(root, item.get("path", ""))
        if os.path.isfile(path) or os.path.islink(path):
            os.remove(path)
    for binary in manifest.get("binaries", []):
        if os.path.dirname(os.path.abspath(binary)) != bindir or os.path.basename(binary) not in ("sleela", "nordshrift"):
            raise SystemExit("Refusing unsafe binary uninstall path: " + repr(binary))
        if os.path.isfile(binary) or os.path.islink(binary):
            os.remove(binary)
    os.remove(marker)
    for root in (runtime, data):
        if not os.path.isdir(root):
            continue
        for current, dirs, _ in os.walk(root, topdown=False, followlinks=False):
            for name in dirs:
                path = os.path.join(current, name)
                try:
                    if not os.path.islink(path):
                        os.rmdir(path)
                except OSError:
                    pass
        try:
            os.rmdir(root)
        except OSError:
            pass
    print("Removed tracked SLeeLa files; untracked files and configuration were preserved.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise SystemExit("usage: install-runtime.py copy-manifest REPO RUNTIME | record RUNTIME DATA BINDIR | uninstall RUNTIME DATA BINDIR")
    command = sys.argv[1]
    if command == "copy-manifest" and len(sys.argv) == 4:
        copy_manifest(sys.argv[2], sys.argv[3])
    elif command == "record" and len(sys.argv) == 5:
        record(sys.argv[2], sys.argv[3], sys.argv[4])
    elif command == "uninstall" and len(sys.argv) == 5:
        uninstall(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        raise SystemExit("invalid command or arguments")
