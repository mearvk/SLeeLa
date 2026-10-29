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

def _selected_lib_packages(repo, modules, exclude):
    """Resolve the set of lib/ package names to KEEP given the MODULES/EXCLUDE
    selection, mirroring stage_lib. Returns None when every package is selected
    (MODULES=all with no excludes) so callers can skip filtering entirely."""
    lib_root = os.path.join(repo, "lib")
    available = sorted(
        name for name in os.listdir(lib_root)
        if os.path.isdir(os.path.join(lib_root, name))
    ) if os.path.isdir(lib_root) else []
    drop = set(exclude.split()) if exclude else set()
    if (not modules or modules.strip().lower() == "all") and not drop:
        return None  # everything; no filtering needed
    want = modules.split() if modules and modules.strip().lower() != "all" else list(available)
    return {n for n in want if n in available and n not in drop}

def copy_manifest(repo, runtime, modules="all", exclude=""):
    """Restore the trusted, SHA-256-manifested base files into the runtime tree.

    When a module selection is in effect, manifest entries under lib/<pkg>/ are
    only restored for packages in the selected set, so copy-manifest does not
    re-populate lib packages the install deliberately omitted. Non-lib manifest
    entries (core, tools, security, etc.) are always restored."""
    repo, runtime = map(os.path.abspath, (repo, runtime))
    manifest_path = os.path.join(repo, "security", "sha256-manifest.json")
    with open(manifest_path, encoding="utf-8") as stream:
        entries = json.load(stream).get("files", [])
    keep = _selected_lib_packages(repo, modules, exclude)
    skipped = 0
    for entry in entries:
        rel = entry["path"]
        # Honor the module selection for library paths.
        if keep is not None:
            parts = rel.replace("\\", "/").split("/")
            if len(parts) >= 2 and parts[0] == "lib":
                if parts[1] not in keep:
                    skipped += 1
                    continue
        src = safe_child(repo, rel)
        if not os.path.isfile(src):
            raise SystemExit("Invalid or missing manifest source: " + rel)
        dst = safe_child(runtime, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)
    if keep is not None and skipped:
        print("copy-manifest honored module selection: skipped %d file(s) in omitted lib packages."
              % skipped)

def stage_lib(repo, runtime_lib, modules, exclude):
    """Stage selected lib/ packages into the runtime's lib/ directory.

    modules: either the literal string "all" (every top-level lib package) or a
             whitespace-separated list of package names to include.
    exclude: whitespace-separated list of package names to drop from the
             selection (applied after the include set is resolved).

    This is the data-driven equivalent of the installer's module options: the
    caller (the Makefile / a package build) decides the set up front; there is
    no interactive prompting here. Unknown names are reported and skipped. The
    top-level lib/*.md inventory files always travel with the library. Returns
    the number of packages actually staged.
    """
    repo = os.path.abspath(repo)
    runtime_lib = os.path.abspath(runtime_lib)
    lib_root = os.path.join(repo, "lib")
    if not os.path.isdir(lib_root):
        raise SystemExit("No lib/ directory under repo root: " + lib_root)

    available = sorted(
        name for name in os.listdir(lib_root)
        if os.path.isdir(os.path.join(lib_root, name))
    )
    want = modules.split() if modules and modules.strip().lower() != "all" else list(available)
    drop = set(exclude.split()) if exclude else set()

    selected = []
    seen = set()
    for name in want:
        if name in drop or name in seen:
            continue
        if name not in available:
            print("  skip (not a lib package): " + name)
            continue
        seen.add(name)
        selected.append(name)

    os.makedirs(runtime_lib, exist_ok=True)
    for name in selected:
        src = safe_child(lib_root, name)
        dst = safe_child(runtime_lib, name)
        if os.path.isdir(dst):
            shutil.rmtree(dst)
        shutil.copytree(src, dst, symlinks=True)

    # The lib inventory markdown always accompanies the library.
    for entry in os.listdir(lib_root):
        if entry.endswith(".md"):
            shutil.copy2(os.path.join(lib_root, entry), os.path.join(runtime_lib, entry))

    kept = len(selected)
    dropped = len(available) - kept
    print("Staged %d lib package(s) into %s (%d of the tree omitted)."
          % (kept, runtime_lib, dropped))
    return kept

def _timestamp():
    import datetime
    return datetime.datetime.now().strftime("%Y%m%d-%H%M%S")

def backup(runtime, data, bindir, backup_root):
    """Snapshot the currently installed binaries and runtime/data trees before a
    reinstall overwrites them, so older files are retained (not clobbered) until
    the user removes them deliberately. Only runs when a prior tracked install is
    present; otherwise it is a no-op. Returns quietly if there is nothing to back
    up."""
    runtime, data, bindir = map(os.path.abspath, (runtime, data, bindir))
    marker = os.path.join(runtime, MARKER)
    # Nothing tracked yet -> first install -> nothing to preserve.
    have_marker = os.path.isfile(marker)
    have_bins = any(os.path.exists(os.path.join(bindir, b)) for b in ("sleela", "sleelvac", "nordshrift"))
    if not have_marker and not have_bins:
        print("No previous SLeeLa install detected; nothing to back up.")
        return
    backup_root = os.path.abspath(backup_root)
    # Guard against a snapshot living inside the trees it copies (which would
    # recurse). The Makefile keeps backups beside, not under, the runtime tree.
    for tree in (runtime, data):
        if os.path.commonpath((backup_root, tree)) == os.path.abspath(tree):
            raise SystemExit("Refusing backup root inside the install tree: " + backup_root)
    dest = os.path.join(backup_root, _timestamp())
    os.makedirs(dest, exist_ok=True)
    # Preserve the previous binaries.
    bin_dest = os.path.join(dest, "bin")
    for name in ("sleela", "sleelvac", "nordshrift"):
        src = os.path.join(bindir, name)
        if os.path.isfile(src) and not os.path.islink(src):
            os.makedirs(bin_dest, exist_ok=True)
            shutil.copy2(src, os.path.join(bin_dest, name))
    # Preserve the previous runtime and data trees.
    for label, root in (("runtime", runtime), ("data", data)):
        if os.path.isdir(root):
            shutil.copytree(root, os.path.join(dest, label), symlinks=True, dirs_exist_ok=True)
    print("Backed up the existing SLeeLa install to: " + dest)
    print("Older files are retained there and are not removed automatically;")
    print("delete that directory yourself when you no longer need the rollback.")

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
        os.path.join(bindir, "sleela"), os.path.join(bindir, "sleelvac"), os.path.join(bindir, "nordshrift")
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
        if os.path.dirname(os.path.abspath(binary)) != bindir or os.path.basename(binary) not in ("sleela", "sleelvac", "nordshrift"):
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
        raise SystemExit("usage: install-runtime.py copy-manifest REPO RUNTIME [MODULES EXCLUDE] | stage-lib REPO RUNTIME_LIB MODULES EXCLUDE | backup RUNTIME DATA BINDIR BACKUP_ROOT | record RUNTIME DATA BINDIR | uninstall RUNTIME DATA BINDIR")
    command = sys.argv[1]
    if command == "copy-manifest" and len(sys.argv) in (4, 6):
        if len(sys.argv) == 6:
            copy_manifest(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
        else:
            copy_manifest(sys.argv[2], sys.argv[3])
    elif command == "stage-lib" and len(sys.argv) == 6:
        stage_lib(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
    elif command == "backup" and len(sys.argv) == 6:
        backup(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
    elif command == "record" and len(sys.argv) == 5:
        record(sys.argv[2], sys.argv[3], sys.argv[4])
    elif command == "uninstall" and len(sys.argv) == 5:
        uninstall(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        raise SystemExit("invalid command or arguments")
