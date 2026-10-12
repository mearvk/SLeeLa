#!/usr/bin/env python3
"""Native-first Sleelavirin coordinator with bounded, explicit ClamAV workers."""
import argparse
import fnmatch
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed


def size_arg(value):
    value = value.strip().lower()
    scale = 1
    if value.endswith("k"):
        scale, value = 1024, value[:-1]
    elif value.endswith("m"):
        scale, value = 1024 ** 2, value[:-1]
    elif value.endswith("g"):
        scale, value = 1024 ** 3, value[:-1]
    try:
        number = int(value)
        if number < 0:
            raise ValueError()
        return number * scale
    except ValueError as exc:
        raise argparse.ArgumentTypeError("use a non-negative byte count or K/M/G suffix") from exc


def stamp_from_stat(stat):
    return (
        stat.st_size,
        getattr(stat, "st_mtime_ns", int(stat.st_mtime * 1e9)),
        getattr(stat, "st_ctime_ns", int(stat.st_ctime * 1e9)),
        getattr(stat, "st_ino", 0),
        stat.st_mode,
    )


def file_stamp(path):
    # Include ctime and file mode as extra mutation signals. These do not eliminate
    # filesystem races, but catch more changes than size/mtime/inode alone.
    return stamp_from_stat(path.stat())


def enumerate_files(inputs, depth_limit, follow, excludes, extensions, size_limit):
    output, seen_files, seen_dirs = [], set(), set()
    incomplete = False
    for item in inputs:
        root = Path(item).expanduser()
        try:
            if not root.exists():
                print(f"ERROR path does not exist: {root}", file=sys.stderr)
                incomplete = True
                continue
            if root.is_file():
                candidates = [root]
            elif root.is_dir():
                candidates = []
                root_depth = len(root.resolve().parts)
                def walk_error(exc):
                    nonlocal incomplete
                    print(f"ERROR directory walk: {exc}", file=sys.stderr)
                    incomplete = True
                for current, dirs, names in os.walk(root, followlinks=follow, onerror=walk_error):
                    here = Path(current)
                    try:
                        real_dir = str(here.resolve())
                        if real_dir in seen_dirs:
                            dirs[:] = []
                            continue
                        seen_dirs.add(real_dir)
                        depth = len(here.resolve().parts) - root_depth
                    except OSError as exc:
                        print(f"ERROR directory {here}: {exc}", file=sys.stderr)
                        incomplete = True
                        dirs[:] = []
                        continue
                    dirs[:] = sorted(
                        d for d in dirs
                        if (follow or not (here / d).is_symlink())
                        and not any(fnmatch.fnmatch(str(here / d), pattern) or fnmatch.fnmatch(d, pattern) for pattern in excludes)
                    )
                    if depth_limit and depth >= depth_limit:
                        dirs[:] = []
                    candidates.extend(here / name for name in sorted(names))
            else:
                print(f"ERROR unsupported path: {root}", file=sys.stderr)
                incomplete = True
                continue
            for path in candidates:
                try:
                    if path.is_symlink() and not follow:
                        continue
                    if not path.is_file():
                        continue
                    resolved = str(path.resolve())
                    if resolved in seen_files:
                        continue
                    if any(fnmatch.fnmatch(str(path), pattern) or fnmatch.fnmatch(path.name, pattern) for pattern in excludes):
                        continue
                    if extensions and path.suffix.lower() not in extensions:
                        continue
                    if path.stat().st_size > size_limit:
                        print(f"SKIPPED_TOO_LARGE {path}")
                        incomplete = True
                        continue
                    seen_files.add(resolved)
                    output.append(path)
                except OSError as exc:
                    print(f"ERROR file {path}: {exc}", file=sys.stderr)
                    incomplete = True
        except OSError as exc:
            print(f"ERROR path {root}: {exc}", file=sys.stderr)
            incomplete = True
    return output, incomplete


def native_scan(executable, database, path, initial_stamp=None, timeout=120):
    try:
        before = file_stamp(path)
        if initial_stamp is not None and before != initial_stamp:
            return "ERROR", path, f"ERROR_CHANGED_BEFORE_SCAN {path}"
        result = subprocess.run(
            [str(executable), "--database", str(database), str(path)],
            capture_output=True, text=True, check=False, timeout=timeout
        )
        output = (result.stdout + result.stderr).strip()
        try:
            after = file_stamp(path)
        except OSError as exc:
            return "ERROR", path, f"ERROR_AFTER_SCAN {path}: {exc}"
        if before != after:
            return "ERROR", path, f"ERROR_CHANGED_DURING_SCAN {path}"
        if result.returncode == 0:
            return "NO_MATCH", path, output or f"NO_MATCH {path}"
        if result.returncode == 1 and "DETECTED " in output:
            return "DETECTED", path, output
        return "ERROR", path, output or f"ERROR {path} exit={result.returncode}"
    except subprocess.TimeoutExpired:
        return "ERROR", path, f"ERROR_SCAN_TIMEOUT {path} after {timeout}s"
    except OSError as exc:
        return "ERROR", path, f"ERROR {path}: {exc}"


def clamav_scan_one(executable, path, size_limit, scan_limit, recursion_limit, initial_stamp=None, timeout=120):
    try:
        before = file_stamp(path)
        if initial_stamp is not None and before != initial_stamp:
            return "ERROR", path, f"ERROR_CHANGED_BEFORE_SCAN {path}"
        command = [
            executable, "--infected", "--no-summary",
            f"--max-filesize={size_limit}", f"--max-scansize={scan_limit}",
            f"--max-recursion={recursion_limit}", "--", str(path)
        ]
        result = subprocess.run(command, capture_output=True, text=True, check=False, timeout=timeout)
        output = (result.stdout + result.stderr).strip()
        try:
            after = file_stamp(path)
        except OSError as exc:
            return "ERROR", path, f"ERROR_AFTER_SCAN {path}: {exc}"
        if before != after:
            return "ERROR", path, f"ERROR_CHANGED_DURING_SCAN {path}"
        if result.returncode == 0:
            return "NO_MATCH", path, output or f"CLAMAV_NO_MATCH {path}"
        if result.returncode == 1:
            detected_lines = [line for line in result.stdout.splitlines() if " FOUND" in line]
            if detected_lines:
                return "DETECTED", path, "\n".join(detected_lines)
            return "ERROR", path, f"CLAMAV_ERROR detection exit without a parseable file verdict: {path}"
        return "ERROR", path, output or f"CLAMAV_ERROR {path} exit={result.returncode}"
    except subprocess.TimeoutExpired:
        return "ERROR", path, f"ERROR_CLAMAV_TIMEOUT {path} after {timeout}s"
    except OSError as exc:
        return "ERROR", path, f"ERROR starting ClamAV for {path}: {exc}"


def safe_quarantine(path, destination, scanned_roots, expected_stamp):
    """Copy detections without overwriting a destination, then remove the unchanged source.

    O_NOFOLLOW is used where supported. Exclusive destination creation prevents a
    pre-existing or racing filename from being overwritten. This narrows filesystem
    races but is not a substitute for snapshot-based scanning on hostile filesystems.
    """
    if path.is_symlink():
        raise ValueError("refusing to quarantine a symbolic link; scan and quarantine its target explicitly")
    source = path.resolve(strict=True)
    dest = destination.resolve()
    if source == dest or dest in source.parents:
        raise ValueError("quarantine destination is the scanned file or an ancestor of it")
    for root in scanned_roots:
        root_path = Path(root).expanduser()
        try:
            if root_path.is_dir() and (dest == root_path.resolve() or root_path.resolve() in dest.parents):
                raise ValueError("quarantine directory must not be inside a scanned directory")
        except OSError as exc:
            raise ValueError(f"cannot validate scan root {root_path}: {exc}") from exc
    if file_stamp(path) != expected_stamp:
        raise ValueError("file changed after detection; refusing to quarantine a changed file")

    destination.mkdir(parents=True, exist_ok=True)
    source_flags = os.O_RDONLY | getattr(os, "O_NOFOLLOW", 0)
    source_fd = os.open(path, source_flags)
    target = None
    target_fd = None
    try:
        if stamp_from_stat(os.fstat(source_fd)) != expected_stamp:
            raise ValueError("opened file differs from the detected file; refusing quarantine")
        index = 0
        while True:
            name = path.name if index == 0 else f"{path.stem}.{index}{path.suffix}"
            candidate = destination / name
            try:
                target_fd = os.open(candidate, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
                target = candidate
                break
            except FileExistsError:
                index += 1
        source_hash = hashlib.sha256()
        with os.fdopen(source_fd, "rb") as source_handle:
            source_fd = -1
            with os.fdopen(target_fd, "wb") as target_handle:
                target_fd = -1
                while True:
                    chunk = source_handle.read(1024 * 1024)
                    if not chunk:
                        break
                    source_hash.update(chunk)
                    target_handle.write(chunk)
                target_handle.flush()
                os.fsync(target_handle.fileno())
            if stamp_from_stat(os.fstat(source_handle.fileno())) != expected_stamp:
                raise ValueError("opened source changed during quarantine copy")
        if file_stamp(path) != expected_stamp:
            raise ValueError("source path changed during quarantine copy; original was retained")
        target_hash = hashlib.sha256()
        with target.open("rb") as check_handle:
            for chunk in iter(lambda: check_handle.read(1024 * 1024), b""):
                target_hash.update(chunk)
        if target_hash.digest() != source_hash.digest():
            raise OSError("quarantine copy integrity check failed")
        # Recheck before removal. A tiny path replacement race can remain on filesystems
        # without stronger directory-handle primitives, so high-assurance deployments
        # should quiesce the source tree or scan a snapshot.
        if file_stamp(path) != expected_stamp:
            raise ValueError("source changed before removal; original was retained")
        path.unlink()
        return target
    except Exception:
        if target is not None:
            try:
                target.unlink()
            except OSError:
                pass
        raise
    finally:
        if source_fd >= 0:
            os.close(source_fd)
        if target_fd is not None:
            os.close(target_fd)


def unique_paths(paths):
    """Keep first occurrence of each path while preserving order."""
    return list(dict.fromkeys(paths))


def main():
    package = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description="Native-first Sleelavirin scanner with optional ClamAV secondary engine.")
    parser.add_argument("--engine", choices=("native", "clamav", "both"), default="native")
    parser.add_argument("--native-scanner", default=str(package / "build" / ("sleelavirin-scan.exe" if os.name == "nt" else "sleelavirin-scan")))
    parser.add_argument("--database", default=str(package / "signatures" / "native-sha256.txt"))
    parser.add_argument("--clamscan", default="clamscan")
    parser.add_argument("--max-depth", type=int, default=8, help="directory depth; 0 means unlimited")
    parser.add_argument("--jobs", type=int, default=1, help="concurrent native workers")
    parser.add_argument("--clamav-jobs", type=int, default=1, help="concurrent ClamAV processes (CPU/resource limit)")
    parser.add_argument("--max-file-size", type=size_arg, default=100 * 1024 * 1024, help="bytes or K/M/G; default 100M")
    parser.add_argument("--clamav-max-scansize", type=size_arg, default=400 * 1024 * 1024, help="ClamAV total bytes scanned per file; default 400M")
    parser.add_argument("--clamav-max-recursion", type=int, default=8, help="maximum nested archive recursion for ClamAV")
    parser.add_argument("--scan-timeout", type=int, default=120, help="per-file engine timeout in seconds; 0 disables timeout")
    parser.add_argument("--exclude", action="append", default=[], metavar="GLOB")
    parser.add_argument("--extension", action="append", default=[], metavar=".EXT")
    parser.add_argument("--follow-symlinks", action="store_true")
    parser.add_argument("--action", choices=("report", "quarantine", "stop"), default="report")
    parser.add_argument("--quarantine-dir", default=str(package / "quarantine"))
    parser.add_argument("paths", nargs="+")
    args = parser.parse_args()
    if args.max_depth < 0 or args.jobs < 1 or args.clamav_jobs < 1 or args.clamav_max_recursion < 0 or args.scan_timeout < 0:
        parser.error("depth/recursion must be >= 0 and worker counts must be >= 1")
    if args.clamav_max_scansize < args.max_file_size:
        parser.error("--clamav-max-scansize should be >= --max-file-size")
    extensions = {x.lower() if x.startswith(".") else "." + x.lower() for x in args.extension}
    paths, incomplete = enumerate_files(args.paths, args.max_depth, args.follow_symlinks, args.exclude, extensions, args.max_file_size)
    if not paths:
        print("ERROR: no eligible files found; scan is incomplete", file=sys.stderr)
        return 2
    initial_stamps = {}
    for path in paths:
        try:
            initial_stamps[path] = file_stamp(path)
        except OSError as exc:
            print(f"ERROR stat {path}: {exc}", file=sys.stderr)
            incomplete = True
    # Prevent a quarantine directory from being scanned and then recursively
    # receiving files while the same run is in progress.
    if args.action == "quarantine":
        destination = Path(args.quarantine_dir).expanduser()
        try:
            resolved_dest = destination.resolve()
            for root in args.paths:
                candidate = Path(root).expanduser()
                if candidate.is_dir() and (resolved_dest == candidate.resolve() or candidate.resolve() in resolved_dest.parents):
                    print("ERROR quarantine directory must be outside every scanned directory", file=sys.stderr)
                    return 2
        except OSError as exc:
            print(f"ERROR validating quarantine directory: {exc}", file=sys.stderr)
            return 2

    native_exe = Path(args.native_scanner).expanduser()
    found, errors = [], 0
    if args.engine in ("native", "both"):
        database = Path(args.database).expanduser()
        if not native_exe.is_file() or not database.is_file():
            print("ERROR: native scanner or database missing; build with make -C antivirus and check --database/--native-scanner", file=sys.stderr)
            return 2
        workers = 1 if args.action == "stop" else args.jobs
        with ThreadPoolExecutor(max_workers=workers) as pool:
            futures = {pool.submit(native_scan, native_exe, database, path, initial_stamps.get(path), args.scan_timeout or None): path for path in paths}
            for future in as_completed(futures):
                verdict, path, message = future.result()
                print(message)
                if verdict == "DETECTED":
                    found.append(path)
                    if args.action == "stop":
                        for pending in futures:
                            pending.cancel()
                        break
                elif verdict == "ERROR":
                    errors += 1
    if args.engine in ("clamav", "both") and not (args.action == "stop" and found):
        clam = shutil.which(args.clamscan)
        if not clam:
            print(f"ERROR: ClamAV not found: {args.clamscan}", file=sys.stderr)
            return 2
        workers = 1 if args.action == "stop" else args.clamav_jobs
        with ThreadPoolExecutor(max_workers=workers) as pool:
            futures = {pool.submit(clamav_scan_one, clam, path, args.max_file_size, args.clamav_max_scansize, args.clamav_max_recursion, initial_stamps.get(path), args.scan_timeout or None): path for path in paths}
            for future in as_completed(futures):
                verdict, path, message = future.result()
                print(message)
                if verdict == "DETECTED":
                    found.append(path)
                    if args.action == "stop":
                        for pending in futures:
                            pending.cancel()
                        break
                elif verdict == "ERROR":
                    errors += 1
    unique = unique_paths(found)
    if args.action == "quarantine" and unique:
        destination = Path(args.quarantine_dir).expanduser()
        for path in unique:
            try:
                target = safe_quarantine(path, destination, args.paths, initial_stamps[path])
                print(f"QUARANTINED {path} -> {target}")
            except (OSError, ValueError) as exc:
                print(f"ERROR quarantine failed for {path}: {exc}", file=sys.stderr)
                errors += 1
    if incomplete:
        print("INCOMPLETE_SCAN: one or more paths/files were skipped or could not be enumerated", file=sys.stderr)
    return 1 if unique else (2 if errors or incomplete else 0)


if __name__ == "__main__":
    raise SystemExit(main())
